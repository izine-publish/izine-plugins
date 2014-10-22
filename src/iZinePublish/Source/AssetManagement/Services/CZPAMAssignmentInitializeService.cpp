//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/Services/CZPAMAssignmentInitializeService.cpp $
//	$Revision: 3568 $
//	$Date: 2011-08-04 12:29:35 +0200 (Thu, 04 Aug 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 16-8-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IAssignment.h"
#include "IAssetMonitor.h"
#include "IDocument.h"
#include "IManageableAsset.h"
#include "IStringData.h"

//SDK General includes
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPAMHelper.h"
#include "IZPAssetData.h"
#include "IZPAssetInfo.h"
#include "IZPDocUtils.h"

//IZP General includes
#include "AZPAMService.h"
#include "StInDesignUtils.h"
#include "ZPDataHelper.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AMService
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass AZPAMService
class CZPAMAssignmentInitializeService : public inheritClass
{
public:
						CZPAMAssignmentInitializeService(IPMUnknown * boss);
	virtual				~CZPAMAssignmentInitializeService();

	ErrorCode			Do(
							IManageableAsset * pAsset = nil );
	bool16				VerifyContext(
							IManageableAsset * pAsset = nil );
protected:
	void				InitAssignmentDoc(
							IDocument *					assetDoc,
							IManageableAsset *			pAsset );

	bool				GetAssetFile(
							const IManageableAsset *	inManageableAsset,
							IDFile &					oAssetFile) const;
private:
};


CREATE_PMINTERFACE(CZPAMAssignmentInitializeService, kZPAMAssignmentInitializeServiceImpl)

CZPAMAssignmentInitializeService::CZPAMAssignmentInitializeService(IPMUnknown * boss)
: inheritClass(boss)
{
	IZPLog_ObjC( thisFileLA );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMAssignmentInitializeService::~CZPAMAssignmentInitializeService()
{
	IZPLog_ObjD( thisFileLA );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// Do
//----------------------------------------------------------------------------------------
ErrorCode 
CZPAMAssignmentInitializeService::Do( IManageableAsset * pAsset)
{
	LogFunctionEnterExit;

	ErrorCode status = kFailure;

	this->LogManageableAssetInfo( pAsset );
	do {
		
		InterfacePtr<IDocument> assetDoc( pAsset, UseDefaultIID() );
		if( assetDoc )	//Must be called only from InCopy
			this->InitAssignmentDoc( assetDoc , pAsset );

		status = kSuccess;
	
	} while(kFalse);
	
	return status;
}

//----------------------------------------------------------------------------------------
// VerifyContext
//----------------------------------------------------------------------------------------
bool16
CZPAMAssignmentInitializeService::VerifyContext( IManageableAsset * pAsset)
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
// InitAssignmentDoc
//----------------------------------------------------------------------------------------
void
CZPAMAssignmentInitializeService::InitAssignmentDoc(
	IDocument *					inAssetDoc,
	IManageableAsset *			pAsset)
{
	LogFunctionEnterExit;
	do
	{
		if ( !pAsset || !inAssetDoc )
			break;
		
		if( !inAssetDoc->IsSaved() )
			break;			//In InCopy assignment can be opened, not created so it must be saved asset docs

		StRetainModifiedFlag dontModifyDB( pAsset );	//Init service must not mark DB to be dirty.

		//Patch: for InCopy, it is doc boss.
		//We don't have manageable asset interface for Assignment in InCopy, so we will be monitoring the doc as it is one to one relation.
		InterfacePtr<IAssignment> iAssignment( Utils<IZPDocUtils>()->QueryAssignmentFromAssignmentDoc( inAssetDoc ) );
		if(!iAssignment)	//Don't handle IC document/templates there. Only Assignment document must be handled.
			break;
		
		InterfacePtr<const IZPAssetData> assignmentAssetData(iAssignment, UseDefaultIID());
		//InterfacePtr<const IZPAssetData> assetData( pAsset, UseDefaultIID());
		const PMString & theAssetIDStr = assignmentAssetData->GetAssetID();
		if( theAssetIDStr.IsEmpty() )		//Handle only if our Asset Data is set.
			break;
		
		ClassID currentServiceProviderClassID = pAsset->GetServiceProviderClass();
		if( currentServiceProviderClassID != kZPAMSPAssignmentBoss )
			pAsset->SetServiceProviderClass( kZPAMSPAssignmentBoss );

		InterfacePtr<IAssetMonitor> iAssetMonitor( this->QueryAssetMonitor(kZPAMSPAssignmentBoss) );
		pAsset->SetAssetMonitor( iAssetMonitor );
		
		InterfacePtr<IManagedStatus> iManagedStatus(::CreateObject2<IManagedStatus>(kZPAMDocManagedStatusBoss));
		pAsset->SetStatus( iManagedStatus );		//Set ManagedStatus object.

		InterfacePtr<IStringData> managedStatusAssetID( iManagedStatus, IID_IZPDATAID );
		ASSERT( managedStatusAssetID );
		managedStatusAssetID->Set( assignmentAssetData->GetAssetID() );	//Note: we are setting assignment asset id with doc asset. This is used by asset monitor.

		UIDRef	docRef(::GetUIDRef(pAsset));
		iManagedStatus->SetAsset( docRef );

		InterfacePtr<IAMLockable> iLockable( iManagedStatus, UseDefaultIID() );
		pAsset->SetLockable( iLockable );			//set lockable object, don't know if this is used by Adobe or not.
		
		pAsset->SetHasVersionedContent( kTrue );	//Used by UI enablement rules.

		const IStringData * dbAssetID = ZPDataHelper::GetAssetByID( theAssetIDStr );
		if( !dbAssetID )
			break;
		InterfacePtr<const IZPAssetInfo> dbAssetInfo( dbAssetID, UseDefaultIID() );
		ASSERT( dbAssetInfo );

		IDFile writeFile ;
		Utils<IZPAMHelper>()->GetAssetFilePath( dbAssetID, writeFile );

		pAsset->SetWriteFile( writeFile );	//Set for easy access in other services.

		pAsset->SetIsVersioned( kTrue );

	}while(kFalse);
}


//----------------------------------------------------------------------------------------
// GetAssetFile
//----------------------------------------------------------------------------------------
bool
CZPAMAssignmentInitializeService::GetAssetFile(
	const IManageableAsset *	inManageableAsset,
	IDFile &					oAssetFile) const
{
	return inheritClass::GetAssetFile( inManageableAsset, oAssetFile );
}
