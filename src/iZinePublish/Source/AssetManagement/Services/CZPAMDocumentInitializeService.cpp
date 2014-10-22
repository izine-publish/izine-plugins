//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/Services/CZPAMDocumentInitializeService.cpp $
//	$Revision: 4097 $
//	$Date: 2013-08-14 11:17:30 +0200 (Wed, 14 Aug 2013) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 28-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IAMLockable.h"
#include "IAMService.h"
#include "IAMServiceData.h"
//#include "IAMSPManager.h"
#include "IApplication.h"
#include "IAssetMonitor.h"
#include "IAssetStateMachine.h"
#include "IBoolData.h"
#include "ICommandInterceptor.h"
#include "IControlView.h"
#include "IDocument.h"
#include "IManageableAsset.h"
#include "IMetaDataAccess.h"
#include "InCopyBridgeID.h"
#include "IPanelMgr.h"
#include "IStandAloneDoc.h"
#include "IStringData.h"

//SDK General includes
#include "CPMUnknown.h"
#include "CAlert.h"
#include "Utils.h"
#include "FileUtils.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPAssetData.h"
#include "IZPAssetUtils.h"
#include "IZPCommandFacade.h"
#include "IZPInDnUtils.h"

//IZP General includes
#include "AZPAMService.h"
#include "StInDesignUtils.h"
#include "ZPDataHelper.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AMService
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

/** Executes any initialization code to make an IManageableAsset instance ready for use.
	
	Input IAMServiceData parameters:
	
		IAMService Data Identifier	| Data Type		| Notes
		----------------------------+---------------+--------------------------------------
		IAMService::enUseStoredData	| boolean flag	| If set, init using asset management data
													| stored with the link. This parameter applies
													| only to managed linked assets.
	
	Output IAMServiceData parameters: None.			
*/
#define inheritClass AZPAMService
class CZPAMDocumentInitializeService : public inheritClass
{
public:
						CZPAMDocumentInitializeService(IPMUnknown * boss);
	virtual				~CZPAMDocumentInitializeService();

	virtual ErrorCode	Do(
							IManageableAsset *			pAsset = nil );
	
	virtual bool16		VerifyContext(
							IManageableAsset *			pAsset = nil );
protected:
	bool				GetAssetFile(
							const IManageableAsset *	inManageableAsset,
							IDFile &					oAssetFile) const;
private:
};

#define inheritClass_ICDocInit CZPAMDocumentInitializeService
class CZPAMInCopyDocumentInitializeService : public inheritClass_ICDocInit
{
public:
						CZPAMInCopyDocumentInitializeService(IPMUnknown * boss);
	virtual				~CZPAMInCopyDocumentInitializeService();

	ErrorCode			Do(
							IManageableAsset *			pAsset = nil );
	
	bool16				VerifyContext(
							IManageableAsset *			pAsset = nil );
protected:
	void				SetAssetData(
							IManageableAsset *			pAsset );
private:
};


CREATE_PMINTERFACE(CZPAMDocumentInitializeService, kZPAMDocumentInitializeServiceImpl)
CREATE_PMINTERFACE(CZPAMInCopyDocumentInitializeService, kZPAMICDocumentInitializeServiceImpl)

#pragma mark -
CZPAMDocumentInitializeService::CZPAMDocumentInitializeService(IPMUnknown * boss)
: inheritClass(boss)
{
	IZPLog_ObjC( thisFileLA );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMDocumentInitializeService::~CZPAMDocumentInitializeService()
{
	IZPLog_ObjD( thisFileLA );
}

#pragma mark -

//----------------------------------------------------------------------------------------
// Do
//----------------------------------------------------------------------------------------
ErrorCode 
CZPAMDocumentInitializeService::Do( IManageableAsset * pAsset)
{
	LogFunctionEnterExit;

	ErrorCode status = kFailure;

	this->LogManageableAssetInfo( pAsset );
	do {
		InterfacePtr<IDocument> assetDoc( pAsset, UseDefaultIID() );
#ifdef InDnCS3
		ASSERT( assetDoc );
#endif
		if ( !pAsset || !assetDoc )
			break;
		
		if( !assetDoc->IsSaved() )
			break;			//Handle saved asset docs only

		StRetainModifiedFlag dontModifyDB( pAsset );	//Init service must not mark DB to be dirty.

		ClassID currentServiceProviderClassID = pAsset->GetServiceProviderClass();
		if( currentServiceProviderClassID != kZPAMSPDocumentBoss )
			pAsset->SetServiceProviderClass( kZPAMSPDocumentBoss );

		InterfacePtr<const IZPAssetData> assetData( pAsset, UseDefaultIID());
		const PMString & theAssetIDStr = assetData->GetAssetID();
		if( theAssetIDStr.IsEmpty() )		//Handle only if our Asset Data is set.
			break;
		
		InterfacePtr<IAssetMonitor> iAssetMonitor( this->QueryAssetMonitor(kZPAMSPDocumentBoss) );
		pAsset->SetAssetMonitor( iAssetMonitor );
		
		InterfacePtr<IManagedStatus> iManagedStatus(::CreateObject2<IManagedStatus>(kZPAMDocManagedStatusBoss));
		pAsset->SetStatus( iManagedStatus );		//Set ManagedStatus object.

		InterfacePtr<IStringData> assetID( iManagedStatus, IID_IZPDATAID );
		ASSERT( assetID );
		assetID->Set( assetData->GetAssetID() );

		UIDRef	docRef(::GetUIDRef(pAsset));
		iManagedStatus->SetAsset( docRef );

		InterfacePtr<IAMLockable> iLockable( iManagedStatus, UseDefaultIID() );
		pAsset->SetLockable( iLockable );			//set lockable object, don't know if this is used by Adobe or not.
		
		pAsset->SetHasVersionedContent( kTrue );	//Used by UI enablement rules.

		//Install the assignment command interceptor so that we can modify the assignment file paths.
		InterfacePtr<ICommandInterceptor> docAssignCmdInterceptor( pAsset, IID_IZPASSIGNMENTCMDINTERCEPTOR );
		ASSERT( docAssignCmdInterceptor );
		docAssignCmdInterceptor->InstallSelf();	//TODO: do it only once for a document

		status = kSuccess;
	
	} while(kFalse);
	
	return status;
}

//----------------------------------------------------------------------------------------
// VerifyContext
//----------------------------------------------------------------------------------------
bool16
CZPAMDocumentInitializeService::VerifyContext( IManageableAsset * pAsset)
{
	LogFunctionEnterExit;
	this->LogManageableAssetInfo( pAsset );

	bool16 isBound = kFalse;
	
	if ( pAsset && pAsset->IsBound() )
		isBound = kTrue;
	else
		isBound = kFalse;
	
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "(UID=0x%08X) = %s\n",::GetUID(pAsset).Get(),(isBound?"true":"false"));
	return isBound;
}

//----------------------------------------------------------------------------------------
// GetAssetFile
//----------------------------------------------------------------------------------------
bool
CZPAMDocumentInitializeService::GetAssetFile(
	const IManageableAsset *	inManageableAsset,
	IDFile &					oAssetFile) const
{
	return inheritClass::GetAssetFile( inManageableAsset, oAssetFile );
}

#pragma mark -
CZPAMInCopyDocumentInitializeService::CZPAMInCopyDocumentInitializeService(IPMUnknown * boss)
: inheritClass_ICDocInit(boss)
{
	IZPLog_ObjC( thisFileLA );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMInCopyDocumentInitializeService::~CZPAMInCopyDocumentInitializeService()
{
	IZPLog_ObjD( thisFileLA );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// Do
//----------------------------------------------------------------------------------------
ErrorCode 
CZPAMInCopyDocumentInitializeService::Do( IManageableAsset * pAsset)
{
	LogFunctionEnterExit;

	ErrorCode status = kFailure;

	this->LogManageableAssetInfo( pAsset );
	do {
		InterfacePtr<IDocument> assetDoc( pAsset, UseDefaultIID() );
		if ( !pAsset || !assetDoc )	//Also comes for DataLink if not handled by Linked Story services.
			break;
		
		bool16 isHostInCopy = Utils<IZPInDnUtils>()->IsHostAppInCopy();
		ASSERT( isHostInCopy );

		bool16 isNewDocument = !assetDoc->IsSaved();

		InterfacePtr<const IBoolData> isTemplateCopy( assetDoc, IID_IZPISTEMPLATECOPY );
		ASSERT(isTemplateCopy);
		if( !isNewDocument )
			isNewDocument = isTemplateCopy->Get();

		if( isNewDocument && !isHostInCopy )
			break;			//Handle saved asset docs only in InDesign

		StRetainModifiedFlag dontModifyDB( pAsset );	//Init service must not mark DB to be dirty.

		//We are not able to store asset data with IC document, so fill it here.
		if( isHostInCopy && !isNewDocument )
			this->SetAssetData( pAsset );	//TODO: it should not come here. Test.

		ClassID currentServiceProviderClassID = pAsset->GetServiceProviderClass();
		if( currentServiceProviderClassID != kZPAMSPICDocumentBoss )
			pAsset->SetServiceProviderClass( kZPAMSPICDocumentBoss );

		InterfacePtr<const IZPAssetData> assetData( pAsset, UseDefaultIID());
		const PMString & theAssetIDStr = assetData->GetAssetID();
		if( !isNewDocument && theAssetIDStr.IsEmpty() )		//Handle only if our Asset Data is set.
			break;

		InterfacePtr<IStandAloneDoc> standAlone( pAsset, IID_ISTANDALONEDOC);
		//if( isNewDocument && standAlone->IsStandAloneDoc() )	//Commented as now we are handling Save & SaveAs in action filter.
		//	standAlone->SetIsStandAloneDoc( kFalse );
		
		//InterfacePtr<IAssetMonitor> iAssetMonitor( this->QueryAssetMonitor(kZPAMSPDocumentBoss) );
		//pAsset->SetAssetMonitor( iAssetMonitor );
		
		InterfacePtr<IManagedStatus> iManagedStatus(::CreateObject2<IManagedStatus>(kZPAMDocManagedStatusBoss));

		InterfacePtr<IStringData> assetID( iManagedStatus, IID_IZPDATAID );
		ASSERT( assetID );
		assetID->Set( theAssetIDStr );

		UIDRef	docRef(::GetUIDRef(pAsset));
		iManagedStatus->SetAsset( docRef );

		pAsset->SetStatus( iManagedStatus );

		InterfacePtr<IAMLockable> iLockable( iManagedStatus, UseDefaultIID() );
		pAsset->SetLockable( iLockable );

		pAsset->SetHasVersionedContent( kTrue );	//Used by UI enablement rules.

		//Set write file for new InCopy Document asset
		if( isNewDocument && isHostInCopy )
		{
			PMString theNewICDocIDStr = Utils<IZPAssetUtils>()->NewGUID();
			const PMString & strExt = Utils<IZPAssetUtils>()->GetAssetFileExtenstion( eAssetType_ICDocument );
			//TODO: is asset write file used anywhere?
			IDFile writeFile = Utils<IZPAssetUtils>()->CreateLocalFilePathFromAssetID( theNewICDocIDStr, strExt, kNullString);
			pAsset->SetWriteFile( writeFile );	//Set for easy access in other services.
		}

		status = kSuccess;
	
	} while(kFalse);
	
	return status;
}

//----------------------------------------------------------------------------------------
// VerifyContext
//----------------------------------------------------------------------------------------
bool16
CZPAMInCopyDocumentInitializeService::VerifyContext( IManageableAsset * pAsset)
{
	LogFunctionEnterExit;
	this->LogManageableAssetInfo( pAsset );
	
	bool16 isBound = kFalse;

	InterfacePtr<IDocument> assetDoc( pAsset, UseDefaultIID() );	
	
	if ( pAsset && pAsset->IsBound() && assetDoc )
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
CZPAMInCopyDocumentInitializeService::SetAssetData(
	IManageableAsset *			pAsset)
{
	LogFunctionEnterExit;

	InterfacePtr<IAMServiceData> serviceData( this, UseDefaultIID() );
	ASSERT(serviceData);
	
	IDFile theDocAssetFile;
	
	bool hasFilePath = this->GetAssetFile( pAsset, theDocAssetFile );	// get the local file
	ASSERT( hasFilePath );

	PMString assetID = Utils<IZPAssetUtils>()->GetAssetIDFromLocalFile( theDocAssetFile );
	ASSERT( assetID.IsEmpty() == kFalse );

	const IStringData * dbAsset = ZPDataHelper::GetAssetByID( assetID );

	if( dbAsset )
		Utils<IZPCommandFacade>()->SaveAssetData( dbAsset, pAsset );
}

// End, CZPAMDocumentInitializeService.cpp.
