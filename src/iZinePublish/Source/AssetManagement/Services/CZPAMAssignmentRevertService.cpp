//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/Services/CZPAMAssignmentRevertService.cpp $
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
class CZPAMAssignmentRevertService : public inheritClass
{
public:
						CZPAMAssignmentRevertService(IPMUnknown * boss);
	virtual				~CZPAMAssignmentRevertService();

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


CREATE_PMINTERFACE(CZPAMAssignmentRevertService, kZPAMAssignmentRevertServiceImpl)

CZPAMAssignmentRevertService::CZPAMAssignmentRevertService(IPMUnknown * boss)
: inheritClass(boss)
{
	IZPLog_ObjC( thisFileLA );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMAssignmentRevertService::~CZPAMAssignmentRevertService()
{
	IZPLog_ObjD( thisFileLA );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// Do
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMAssignmentRevertService::Do(
	IManageableAsset * pAsset)
{
	LogFunctionEnterExit;
	this->LogManageableAssetInfo( pAsset );

	ErrorCode toReturn = kFailure;

	const IStringData *	dbAsset = this->GetDBAsset_FromAsgmt( pAsset );
	if( dbAsset )
	{
		//Set the local file to out-of-date, so that it can be downloaded again.
		this->UpdateLocalInfo_LocallyModified( dbAsset, false );
		this->UpdateLocalInfo_VersionNumber( dbAsset, 0 );

		//RJK: commented following as the file writing fails if called from here.
		//Download last check-in version over current file.
		//IDFile assetFile;
		//Utils<IZPAMHelper>()->GetAssetFilePath( dbAsset, assetFile );
		//toReturn = Utils<IZPAMHelper>()->DoDownloadAsset( dbAsset, assetFile );

		toReturn = kSuccess;
	}

	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " service DO status = %d", toReturn);
	
	return toReturn;
}

//----------------------------------------------------------------------------------------
// VerifyContext
//----------------------------------------------------------------------------------------
bool16
CZPAMAssignmentRevertService::VerifyContext(
	IManageableAsset * pAsset)
{
	LogFunctionEnterExit;
	this->LogManageableAssetInfo( pAsset );

	bool16 toReturn = kFalse;
	const IStringData *	dbAsset = this->GetDBAsset_FromAsgmt( pAsset );
	if( dbAsset )
	{
		IDFile assetFile;
		Utils<IZPAMHelper>()->GetAssetFilePath( dbAsset, assetFile );
		//check if this file locally modified or not.
		//Don't release lock if file is locally modified.
		IManagedStatus::VersionState fileState = Utils<IZPAMHelper>()->GetAssetFileVersionState( dbAsset, assetFile );
		if( fileState == IManagedStatus::enLocalIsNewer || fileState == IManagedStatus::enVersionConflict )
			toReturn = kTrue;
	}

	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " VerifyContext = %hd", toReturn);
	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetAssetFile
//----------------------------------------------------------------------------------------
bool
CZPAMAssignmentRevertService::GetAssetFile(
	const IManageableAsset *	inManageableAsset,
	IDFile &					oAssetFile) const
{
	return inheritClass::GetAssetFile( inManageableAsset, oAssetFile );
}
