//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/Services/CZPAMLinkedStoryInitializeService.cpp $
//	$Revision: 4097 $
//	$Date: 2013-08-14 11:17:30 +0200 (Wed, 14 Aug 2013) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 14-7-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IAMService.h"
#include "IAMServiceData.h"
#include "IAssetMonitor.h"
#include "ICommandInterceptor.h"
#include "IDataLink.h"
#include "IManageableAsset.h"
#include "IStandAloneDoc.h"
#include "IStringData.h"

//SDK General includes
#include "NameInfo.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPAMHelper.h"
#include "IZPAssetData.h"
#include "IZPAssetInfo.h"
#include "IZPAssetUtils.h"
#include "IZPCommandFacade.h"
#include "IZPFileUtils.h"
#include "IZPInDnUtils.h"

//IZP General includes
#include "AZPAMService.h"
#include "StInDesignUtils.h"
#include "ZPDataHelper.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AMService
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass AZPAMService
class CZPAMLinkedStoryInitializeService : public inheritClass
{
public:
						CZPAMLinkedStoryInitializeService(IPMUnknown * boss);
	virtual				~CZPAMLinkedStoryInitializeService();

	ErrorCode			Do(
							IManageableAsset *			pAsset = nil );
	
	bool16				VerifyContext(
							IManageableAsset *			pAsset = nil );

protected:
	bool				GetAssetFile(
							const IManageableAsset *	inManageableAsset,
							IDFile &					oAssetFile) const;
	void				SetAssetData(
							IManageableAsset *			pAsset );
private:
};


CREATE_PMINTERFACE(CZPAMLinkedStoryInitializeService, kZPAMLinkedStoryInitializeServiceImpl)

CZPAMLinkedStoryInitializeService::CZPAMLinkedStoryInitializeService(IPMUnknown * boss)
: inheritClass(boss)
{
	IZPLog_ObjC( thisFileLA );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMLinkedStoryInitializeService::~CZPAMLinkedStoryInitializeService()
{
	IZPLog_ObjD( thisFileLA );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// Do
//----------------------------------------------------------------------------------------
ErrorCode 
CZPAMLinkedStoryInitializeService::Do( IManageableAsset * pAsset)
{
	LogFunctionEnterExit;

	ErrorCode status = kFailure;

	this->LogManageableAssetInfo( pAsset );
	do {
#ifdef InDnCS3
		if ( !pAsset || ! this->IsLinkedStoryAsset( pAsset ) ) 
			break;
#else
		if ( !pAsset )	//Note: CS5, IsLinkedStoryAsset fails to get story from resource
			break;
#endif
		
		StRetainModifiedFlag dontModifyDB( pAsset );	//Init service must not mark DB to be dirty.

		this->SetAssetData( pAsset );	//Make sure the asset id is set.

		InterfacePtr<const IZPAssetData> assetData( pAsset, UseDefaultIID());
		const PMString & theAssetIDStr = assetData->GetAssetID();
		if( theAssetIDStr.IsEmpty() )		//Handle only if our Asset Data is set.
			break;
		
		ClassID currentServiceProviderClassID = pAsset->GetServiceProviderClass();
		if( currentServiceProviderClassID != kZPAMSPLinkedStoryBoss )
			pAsset->SetServiceProviderClass( kZPAMSPLinkedStoryBoss );

		InterfacePtr<IAssetMonitor> iAssetMonitor( this->QueryAssetMonitor( kZPAMSPLinkedStoryBoss ) );
		pAsset->SetAssetMonitor( iAssetMonitor );
		
		InterfacePtr<IManagedStatus> iManagedStatus(::CreateObject2<IManagedStatus>(kZPAMLinkedStoryManagedStatusBoss));

		InterfacePtr<IStringData> assetID( iManagedStatus, IID_IZPDATAID );
		ASSERT( assetID );
		assetID->Set( theAssetIDStr );

		UIDRef	dataLinkRef(::GetUIDRef(pAsset));
		iManagedStatus->SetAsset( dataLinkRef );

		pAsset->SetStatus( iManagedStatus );

		InterfacePtr<IAMLockable> iLockable( iManagedStatus, UseDefaultIID() );
		pAsset->SetLockable( iLockable );
		pAsset->SetHasVersionedContent( kTrue );	//Used by UI enablement rules.

		const IStringData * dbAssetID = ZPDataHelper::GetAssetByID( theAssetIDStr );
		if( !dbAssetID )	//Object not in cache, might be deleted or not logged in.
			break;
		InterfacePtr<const IZPAssetInfo> dbAssetInfo( dbAssetID, UseDefaultIID() );
		ASSERT( dbAssetInfo );

		//const PMString & strExt = Utils<IZPAssetUtils>()->GetAssetFileExtenstion( eAssetType_IDStory );
		IDFile writeFile ;//= Utils<IZPAssetUtils>()->CreateLocalFilePathFromAssetID( theAssetIDStr, strExt);
		Utils<IZPAMHelper>()->GetAssetFilePath( dbAssetID, writeFile );

		pAsset->SetWriteFile( writeFile );	//Set for easy access in other services.

		//Install cmd interceptor for template
		if( dbAssetInfo )
		{
			bool isInCopy = Utils<IZPInDnUtils>()->IsHostAppInCopy();
			IDataBase * db = ::GetDataBase( pAsset );
			UIDRef docRef( db, db->GetRootUID() );
			InterfacePtr<IStandAloneDoc> standAlone( docRef, IID_ISTANDALONEDOC);
			if (dbAssetInfo->GetType() == eAssetType_ICTemplate )
			{
				InterfacePtr<ICommandInterceptor> templateCmdInterceptor( docRef, IID_IZPTEMPLATECMDINTERCEPTOR );
				ASSERT( templateCmdInterceptor );
				templateCmdInterceptor->InstallSelf();
				if( standAlone && isInCopy && standAlone->IsStandAloneDoc() == kFalse) 
					standAlone->SetIsStandAloneDoc( kTrue );
			}
			else if( standAlone && isInCopy && standAlone->IsStandAloneDoc() == kTrue)
			{
				//Disable style editting for non templates.
				standAlone->SetIsStandAloneDoc( kFalse );
			}
		}

		if( dbAssetInfo && 0 )	//Commented as it does not have any effect
		{
			enAssetType assetType = dbAssetInfo->GetType();
			if( assetType == eAssetType_IDStory )
				pAsset->SetType( IManageableAsset::enLinked );
			else
			{
				pAsset->SetType( IManageableAsset::enDocument );
				if( dbAssetInfo->GetVersionNumber() > 0 )
					pAsset->SetIsVersioned( kTrue );	//IsVersioned is only for doc type
			}
		}

		status = kSuccess;

		//Ensure latest version call is not made in each case by AM,
		//So make one ourself.
		//this->CallEnsureLatestVersion( pAsset );	//Commented as at this place path may not be correct.

	} while(kFalse);
	
	return status;
}

//----------------------------------------------------------------------------------------
// VerifyContext
//----------------------------------------------------------------------------------------
bool16
CZPAMLinkedStoryInitializeService::VerifyContext( IManageableAsset * pAsset)
{
	LogFunctionEnterExit;
	this->LogManageableAssetInfo( pAsset );

	bool16 isBound = kFalse;	
	
	IDataBase* db = ::GetDataBase( pAsset );
	UIDRef docRef( db, db->GetRootUID() );
	InterfacePtr<IDocument> assetDoc( docRef, UseDefaultIID() );
	
	if ( pAsset && pAsset->IsBound() && assetDoc)
		isBound = kTrue;
	else
		isBound = kFalse;
	
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "(UID=0x%08X) = %s\n",::GetUID(pAsset).Get(),(isBound?"true":"false"));
	return isBound;
}

//----------------------------------------------------------------------------------------
// SetAssetData
//----------------------------------------------------------------------------------------
void
CZPAMLinkedStoryInitializeService::SetAssetData(
	IManageableAsset *			pAsset)
{
	LogFunctionEnterExit;

	InterfacePtr<const IZPAssetData> assetData( pAsset, UseDefaultIID() );
	ASSERT( assetData );
	if( assetData->GetAssetID().IsEmpty() == kFalse )	//Already set.
		return;

	InterfacePtr<IAMServiceData> serviceData( this, UseDefaultIID() );
	ASSERT(serviceData);
	
	IDFile theAssetFile;
	
	if( !this->GetIDStoryFile( pAsset, theAssetFile ) )
		return;

	if( !Utils<IZPAMHelper>()->IsPathOfAnyReplicaFolder( theAssetFile ) )
		return;

	PMString assetID = Utils<IZPAssetUtils>()->GetAssetIDFromLocalFile( theAssetFile );
	//ASSERT( assetID.IsEmpty() == kFalse );	//It may come empty when link is created but not yet restored from inca file.

	const IStringData * dbAsset = ZPDataHelper::GetAssetByID( assetID );

	if( dbAsset )
		Utils<IZPCommandFacade>()->SaveAssetData( dbAsset, pAsset );
}

//----------------------------------------------------------------------------------------
// GetAssetFile
//----------------------------------------------------------------------------------------
bool
CZPAMLinkedStoryInitializeService::GetAssetFile(
	const IManageableAsset *	inManageableAsset,
	IDFile &					oAssetFile) const
{
	//LogFunctionEnterExit;
	return inheritClass::GetIDStoryFile( inManageableAsset, oAssetFile );
}

