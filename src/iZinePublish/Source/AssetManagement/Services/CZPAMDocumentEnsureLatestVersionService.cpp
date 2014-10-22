//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/Services/CZPAMDocumentEnsureLatestVersionService.cpp $
//	$Revision: 2193 $
//	$Date: 2010-09-29 11:36:01 +0200 (Wed, 29 Sep 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 5-7-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IAMServiceData.h"
#include "IManageableAsset.h"

//SDK General includes
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPAssetUtils.h"
#include "IZPFileUtils.h"

//IZP General includes
#include "AZPAMService.h"
#include "ZPDataHelper.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AMService
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

/** This service ensures that the local (cached) file for an asset is the latest version by downloading
	the file from the asset store in cases where the cached file is missing or a newer version is
	available. If the cached file has been modified, the download should not be done without user
	authorization even if a newer version of the asset is available; i.e., local changes are never
	clobbered automatically.
	
	Provided that a local copy of the asset exists, implementations of this service should report
	kSuccess for this service even if the asset management system reports a download error. This
	policy assures that the existing local copy of the asset at least can be opened so that status
	alerts can be displayed with more infomation.
	
	Input IAMServiceData parameters:
		
		IAMService Data Identifier		  | Data Type	  |	Notes
		----------------------------------+---------------+--------------------------------------
		IAMService::enTargetAsset		  | IDFile		  | When specified, the service should use
										  |				  | the IDFile rather than the IManageableAsset *
										  |				  | argument on the IAMService::Do() method.
		
	Output IAMServiceData parameters: None.
	
	@see IAMService.
	@see IAMServiceData.		
*/

#define inheritClass AZPAMService
class CZPAMDocumentEnsureLatestVersionService : public inheritClass
{
public:
						CZPAMDocumentEnsureLatestVersionService(IPMUnknown * boss);
	virtual				~CZPAMDocumentEnsureLatestVersionService();

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


CREATE_PMINTERFACE(CZPAMDocumentEnsureLatestVersionService, kZPAMDocumentEnsureLatestVersionServiceImpl)

CZPAMDocumentEnsureLatestVersionService::CZPAMDocumentEnsureLatestVersionService(IPMUnknown * boss)
: inheritClass(boss)
{
	IZPLog_ObjC( thisFileLA );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMDocumentEnsureLatestVersionService::~CZPAMDocumentEnsureLatestVersionService()
{
	IZPLog_ObjD( thisFileLA );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// Do
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMDocumentEnsureLatestVersionService::Do(
	IManageableAsset * pAsset)
{
	LogFunctionEnterExit;
	this->LogManageableAssetInfo( pAsset );

	ErrorCode toReturn = kSuccess;

	toReturn = this->EnsureLatestVersion( pAsset );
	
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " service DO status = %d", toReturn);
	
	return toReturn;
}

//----------------------------------------------------------------------------------------
// VerifyContext
//----------------------------------------------------------------------------------------
bool16
CZPAMDocumentEnsureLatestVersionService::VerifyContext(
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
CZPAMDocumentEnsureLatestVersionService::GetAssetFile(
	const IManageableAsset *	inManageableAsset,
	IDFile &					oAssetFile) const
{
	return inheritClass::GetAssetFile( inManageableAsset, oAssetFile );
}
