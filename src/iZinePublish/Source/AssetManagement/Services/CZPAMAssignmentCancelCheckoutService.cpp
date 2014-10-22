//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/Services/CZPAMAssignmentCancelCheckoutService.cpp $
//	$Revision: 1762 $
//	$Date: 2010-07-16 13:28:38 +0200 (Fri, 16 Jul 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 2-7-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes
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

#define inheritClass AZPAMService
class CZPAMAssignmentCancelCheckoutService : public inheritClass
{
public:
						CZPAMAssignmentCancelCheckoutService(IPMUnknown * boss);
	virtual				~CZPAMAssignmentCancelCheckoutService();

	ErrorCode			Do(
							IManageableAsset * pAsset);
	bool16				VerifyContext(
							IManageableAsset * pAsset);
protected:
	bool				GetAssetFile(
							const IManageableAsset *	inManageableAsset,
							IDFile &					oAssetFile) const;
private:
};


CREATE_PMINTERFACE(CZPAMAssignmentCancelCheckoutService, kZPAMAssignmentCancelCheckoutServiceImpl)

CZPAMAssignmentCancelCheckoutService::CZPAMAssignmentCancelCheckoutService(IPMUnknown * boss)
: inheritClass(boss)
{
	IZPLog_ObjC( thisFileLA );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMAssignmentCancelCheckoutService::~CZPAMAssignmentCancelCheckoutService()
{
	IZPLog_ObjD( thisFileLA );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// Do
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMAssignmentCancelCheckoutService::Do(
	IManageableAsset * pAsset)
{
	LogFunctionEnterExit;
	this->LogManageableAssetInfo( pAsset );

	ErrorCode toReturn = kFailure;
	do {
		const IStringData *	dbAsset = this->GetDBAsset_FromAsgmt( pAsset );
		if( !dbAsset )
			break;
		IDFile assetFile;
		Utils<IZPAMHelper>()->GetAssetFilePath( dbAsset, assetFile );
		//check if this file locally modified or not.
		//Don't release lock if file is locally modified.
		IManagedStatus::VersionState fileState = Utils<IZPAMHelper>()->GetAssetFileVersionState( dbAsset, assetFile );
		if( fileState != IManagedStatus::enLocalIsNewer && fileState != IManagedStatus::enVersionConflict )
			this->DoReleaseLock( dbAsset );

		toReturn = kSuccess;
		
	} while (false);

	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " service DO status = %d", toReturn);
	
	return toReturn;
}

//----------------------------------------------------------------------------------------
// VerifyContext
//----------------------------------------------------------------------------------------
bool16
CZPAMAssignmentCancelCheckoutService::VerifyContext(
	IManageableAsset * pAsset)
{
	LogFunctionEnterExit;
	this->LogManageableAssetInfo( pAsset );

	bool16 toReturn = kFalse;	

	const IStringData *	dbAsset = this->GetDBAsset_FromAsgmt( pAsset );

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
CZPAMAssignmentCancelCheckoutService::GetAssetFile(
	const IManageableAsset *	inManageableAsset,
	IDFile &					oAssetFile) const
{
	return inheritClass::GetAssetFile( inManageableAsset, oAssetFile );
}
