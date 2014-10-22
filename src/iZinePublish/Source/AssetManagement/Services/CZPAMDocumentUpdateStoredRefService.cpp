//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/Services/CZPAMDocumentUpdateStoredRefService.cpp $
//	$Revision: 1762 $
//	$Date: 2010-07-16 13:28:38 +0200 (Fri, 16 Jul 2010) $
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

/** Updates stored asset reference data for a managed link. The stored data (if any) depends on
	the requirements of the asset management system. For example, an opaque data byte sequence supplied
	by the Version Cue API is stored with the IDataLink when the link references a Version Cue asset.
	
	Input IAMServiceData parameters: None.
	Output IAMServiceData parameters: None.				
*/
#define inheritClass AZPAMService
class CZPAMDocumentUpdateStoredRefService : public inheritClass
{
public:
						CZPAMDocumentUpdateStoredRefService(IPMUnknown * boss);
	virtual				~CZPAMDocumentUpdateStoredRefService();

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


CREATE_PMINTERFACE(CZPAMDocumentUpdateStoredRefService, kZPAMDocumentUpdateStoredRefServiceImpl)

CZPAMDocumentUpdateStoredRefService::CZPAMDocumentUpdateStoredRefService(IPMUnknown * boss)
: inheritClass(boss)
{
	IZPLog_ObjC( thisFileLA );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMDocumentUpdateStoredRefService::~CZPAMDocumentUpdateStoredRefService()
{
	IZPLog_ObjD( thisFileLA );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// Do
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMDocumentUpdateStoredRefService::Do(
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
CZPAMDocumentUpdateStoredRefService::VerifyContext(
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
CZPAMDocumentUpdateStoredRefService::GetAssetFile(
	const IManageableAsset *	inManageableAsset,
	IDFile &					oAssetFile) const
{
	return inheritClass::GetAssetFile( inManageableAsset, oAssetFile );
}
