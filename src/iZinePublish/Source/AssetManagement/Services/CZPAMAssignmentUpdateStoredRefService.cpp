//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/Services/CZPAMAssignmentUpdateStoredRefService.cpp $
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
class CZPAMAssignmentUpdateStoredRefService : public inheritClass
{
public:
						CZPAMAssignmentUpdateStoredRefService(IPMUnknown * boss);
	virtual				~CZPAMAssignmentUpdateStoredRefService();

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


CREATE_PMINTERFACE(CZPAMAssignmentUpdateStoredRefService, kZPAMAssignmentUpdateStoredRefServiceImpl)

CZPAMAssignmentUpdateStoredRefService::CZPAMAssignmentUpdateStoredRefService(IPMUnknown * boss)
: inheritClass(boss)
{
	IZPLog_ObjC( thisFileLA );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMAssignmentUpdateStoredRefService::~CZPAMAssignmentUpdateStoredRefService()
{
	IZPLog_ObjD( thisFileLA );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// Do
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMAssignmentUpdateStoredRefService::Do(
	IManageableAsset * pAsset)
{
	LogFunctionEnterExit;
	this->LogManageableAssetInfo( pAsset );

	ErrorCode toReturn = kFailure;

	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " service DO status = %d", toReturn);
	
	return toReturn;
}

//----------------------------------------------------------------------------------------
// VerifyContext
//----------------------------------------------------------------------------------------
bool16
CZPAMAssignmentUpdateStoredRefService::VerifyContext(
	IManageableAsset * pAsset)
{
	LogFunctionEnterExit;
	this->LogManageableAssetInfo( pAsset );

	bool16 toReturn = kFalse;

	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " VerifyContext = %hd", toReturn);
	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetAssetFile
//----------------------------------------------------------------------------------------
bool
CZPAMAssignmentUpdateStoredRefService::GetAssetFile(
	const IManageableAsset *	inManageableAsset,
	IDFile &					oAssetFile) const
{
	return inheritClass::GetAssetFile( inManageableAsset, oAssetFile );
}
