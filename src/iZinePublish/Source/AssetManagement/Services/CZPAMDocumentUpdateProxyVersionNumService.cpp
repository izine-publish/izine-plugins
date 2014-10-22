//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/Services/CZPAMDocumentUpdateProxyVersionNumService.cpp $
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
#include "IStringData.h"

//SDK General includes

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPAssetInfo.h"

//IZP General includes
#include "AZPAMService.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AMService
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

/** Upates the proxy version number for a managed link. The proxy version number is the version number
	of the asset used to create the proxy stored in an InDesign document. For example, the proxy may be
	the low-res version of a placed image, or the latest text in a placed InCopy story. The proxy version
	number is used to determine when the proxy itself (not the link) is out of date. 

	Input IAMServiceData parameters: None.
	Output IAMServiceData parameters: None.
*/
#define inheritClass AZPAMService
class CZPAMDocumentUpdateProxyVersionNumService : public inheritClass
{
public:
						CZPAMDocumentUpdateProxyVersionNumService(IPMUnknown * boss);
	virtual				~CZPAMDocumentUpdateProxyVersionNumService();

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


CREATE_PMINTERFACE(CZPAMDocumentUpdateProxyVersionNumService, kZPAMDocumentUpdateProxyVersionNumServiceImpl)

CZPAMDocumentUpdateProxyVersionNumService::CZPAMDocumentUpdateProxyVersionNumService(IPMUnknown * boss)
: inheritClass(boss)
{
	IZPLog_ObjC( thisFileLA );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMDocumentUpdateProxyVersionNumService::~CZPAMDocumentUpdateProxyVersionNumService()
{
	IZPLog_ObjD( thisFileLA );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// Do
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMDocumentUpdateProxyVersionNumService::Do(
	IManageableAsset * pAsset)
{
	LogFunctionEnterExit;
	this->LogManageableAssetInfo( pAsset );

	ErrorCode toReturn = kFailure;

	//const IStringData *	dbAssetID = this->GetDBAsset_FromDoc( pAsset );
	//ASSERT( dbAssetID );
	//
	//InterfacePtr<const IZPAssetInfo> dbAssetInfo( dbAssetID, UseDefaultIID() );
	//ASSERT( dbAssetInfo );
	//this->UpdateLocalInfo_VersionNumber( dbAssetID, dbAssetInfo->GetVersionNumber() );

	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " service DO status = %d", toReturn);
	
	return toReturn;
}

//----------------------------------------------------------------------------------------
// VerifyContext
//----------------------------------------------------------------------------------------
bool16
CZPAMDocumentUpdateProxyVersionNumService::VerifyContext(
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
CZPAMDocumentUpdateProxyVersionNumService::GetAssetFile(
	const IManageableAsset *	inManageableAsset,
	IDFile &					oAssetFile) const
{
	return inheritClass::GetAssetFile( inManageableAsset, oAssetFile );
}
