//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/Services/CZPAMDocumentCancelCheckOutService.cpp $
//	$Revision: 2193 $
//	$Date: 2010-09-29 11:36:01 +0200 (Wed, 29 Sep 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 28-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description: Release lock on document if not modified locally. Does not impact the local copy of asset.
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IAMService.h"
#include "IManageableAsset.h"

//SDK General includes
#include "CAlert.h"
#include "CPMUnknown.h"
#include "NameInfo.h"
#include "UIDList.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPAMHelper.h"

//IZP General includes
#include "AZPAMService.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AMService
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

/** Cancels a previous checkout without reverting the asset's contents. This service fails if the
	asset has been edited.
	
	Input IAMServiceData parameters:
	
		IAMService Data Identifier		 | Data Type | Notes
		---------------------------------+-----------+--------------------------------------
		IAMService::enUIFlags			 | UIFlags	 | UI permitted for use by the service.
	
	Output IAMServiceData parameters: None.			
*/
#define inheritClass AZPAMService
class CZPAMDocumentCancelCheckOutService : public inheritClass
{
public:
						CZPAMDocumentCancelCheckOutService(IPMUnknown * boss);
	virtual				~CZPAMDocumentCancelCheckOutService();

	ErrorCode			Do(
							IManageableAsset * pAsset = nil );
	bool16				VerifyContext(
							IManageableAsset * pAsset = nil );
protected:
	bool				GetAssetFile(
							const IManageableAsset *	inManageableAsset,
							IDFile &					oAssetFile) const;
private:
};


CREATE_PMINTERFACE(CZPAMDocumentCancelCheckOutService, kZPAMDocumentCancelCheckOutServiceImpl)

CZPAMDocumentCancelCheckOutService::CZPAMDocumentCancelCheckOutService(IPMUnknown * boss)
: inheritClass(boss)
{
	IZPLog_ObjC( thisFileLA );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMDocumentCancelCheckOutService::~CZPAMDocumentCancelCheckOutService()
{
	IZPLog_ObjD( thisFileLA );
}

#pragma mark -

//----------------------------------------------------------------------------------------
// Do
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMDocumentCancelCheckOutService::Do(
	IManageableAsset * pAsset)
{
	LogFunctionEnterExit;

	ErrorCode status = kFailure;

	this->LogManageableAssetInfo( pAsset );

	do {
		const IStringData *	dbAsset = this->GetDBAsset( pAsset );
		if( !dbAsset )
			break;
		IDFile assetFile;
		Utils<IZPAMHelper>()->GetAssetFilePath( dbAsset, assetFile );
		//check if this file locally modified or not.
		//Don't release lock if file is locally modified.
		IManagedStatus::VersionState fileState = Utils<IZPAMHelper>()->GetAssetFileVersionState( dbAsset, assetFile );
		if( fileState != IManagedStatus::enLocalIsNewer && fileState != IManagedStatus::enVersionConflict )
			this->DoReleaseLock( dbAsset );

		status = kSuccess;
		
	} while (false);
	
	return status;
}

//----------------------------------------------------------------------------------------
// VerifyContext
//----------------------------------------------------------------------------------------
bool16
CZPAMDocumentCancelCheckOutService::VerifyContext(
	IManageableAsset * pAsset)
{
	LogFunctionEnterExit;
	
	this->LogManageableAssetInfo( pAsset );
	
	bool16 toReturn = kFalse;	

	const IStringData *	dbAsset = this->GetDBAsset( pAsset );

	//check if this lock is acquired by this user & from this app.
	if( dbAsset && this->IsAssetLockedByCurrentUser( dbAsset ) )
	{
		IDFile assetFile;
		Utils<IZPAMHelper>()->GetAssetFilePath( dbAsset, assetFile );
		//check if this file locally modified or not.
		//Don't release lock if file is locally modified.
		IManagedStatus::VersionState fileState = Utils<IZPAMHelper>()->GetAssetFileVersionState( dbAsset, assetFile );
		if( fileState != IManagedStatus::enLocalIsNewer && fileState != IManagedStatus::enVersionConflict )
			toReturn = kTrue;
	}

	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " VerifyContext = %hd", toReturn);
	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetAssetFile
//----------------------------------------------------------------------------------------
bool
CZPAMDocumentCancelCheckOutService::GetAssetFile(
	const IManageableAsset *	inManageableAsset,
	IDFile &					oAssetFile) const
{
	return inheritClass::GetAssetFile( inManageableAsset, oAssetFile );
}
// End, CZPAMDocumentCancelCheckOutService.cpp.
