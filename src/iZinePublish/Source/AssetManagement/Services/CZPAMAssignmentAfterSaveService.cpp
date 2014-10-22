//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/Services/CZPAMAssignmentAfterSaveService.cpp $
//	$Revision: 2191 $
//	$Date: 2010-09-29 10:48:16 +0200 (Wed, 29 Sep 2010) $
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

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces

//IZP General includes
#include "AZPAMService.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AMService
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass AZPAMService
class CZPAMAssignmentAfterSaveService : public inheritClass
{
public:
						CZPAMAssignmentAfterSaveService(IPMUnknown * boss);
	virtual				~CZPAMAssignmentAfterSaveService();

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


CREATE_PMINTERFACE(CZPAMAssignmentAfterSaveService, kZPAMAssignmentAfterSaveServiceImpl)

CZPAMAssignmentAfterSaveService::CZPAMAssignmentAfterSaveService(IPMUnknown * boss)
: inheritClass(boss)
{
	IZPLog_ObjC( thisFileLA );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMAssignmentAfterSaveService::~CZPAMAssignmentAfterSaveService()
{
	IZPLog_ObjD( thisFileLA );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// Do
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMAssignmentAfterSaveService::Do(
	IManageableAsset * pAsset)
{
	LogFunctionEnterExit;
	this->LogManageableAssetInfo( pAsset );

	ErrorCode toReturn = kSuccess;

	const IStringData *	dbAsset = this->GetDBAsset_FromAsgmt( pAsset );
	if( dbAsset )
	{
		//Note: the assignment is never marked modified  Only its stories are.
		//Mark all check-out stories as modified.

		//TODO: Mark all check-out stories as modified.
	}

	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " service DO status = %d", toReturn);
	
	return toReturn;
}

//----------------------------------------------------------------------------------------
// VerifyContext
//----------------------------------------------------------------------------------------
bool16
CZPAMAssignmentAfterSaveService::VerifyContext(
	IManageableAsset * pAsset)
{
	LogFunctionEnterExit;
	this->LogManageableAssetInfo( pAsset );

	bool16 toReturn = kTrue;
	const IStringData *	dbAsset = this->GetDBAsset_FromAsgmt( pAsset );
	if( !dbAsset )
		toReturn = kFalse;

	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " VerifyContext = %hd", toReturn);
	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetAssetFile
//----------------------------------------------------------------------------------------
bool
CZPAMAssignmentAfterSaveService::GetAssetFile(
	const IManageableAsset *	inManageableAsset,
	IDFile &					oAssetFile) const
{
	return inheritClass::GetAssetFile( inManageableAsset, oAssetFile );
}
