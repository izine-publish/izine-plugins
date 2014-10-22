//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/Services/CZPAMDocumentCheckOutService.cpp $
//	$Revision: 2193 $
//	$Date: 2010-09-29 11:36:01 +0200 (Wed, 29 Sep 2010) $
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
#include "IDocument.h"
#include "IManageableAsset.h"

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

/** Checks out an asset for editing. If locking is supported, the asset is also locked to indicate
	to others that it is in use.
	
	Input IAMServiceData parameters:
	
		IAMService Data Identifier		 | Data Type | Notes
		---------------------------------+-----------+--------------------------------------
		IAMService::enCheckOutToUserName | PMString	 | Empty PMString signifies a blank comment.
		IAMService::enUIFlags			 | UIFlags	 | UI permitted for use by the service.
	
	Output IAMServiceData parameters: None.			
*/
#define inheritClass AZPAMService
class CZPAMDocumentCheckOutService : public inheritClass
{
public:
						CZPAMDocumentCheckOutService(IPMUnknown * boss);
	virtual				~CZPAMDocumentCheckOutService();

	virtual ErrorCode	Do(
							IManageableAsset * pAsset);
	virtual bool16		VerifyContext(
							IManageableAsset * pAsset);
protected:
	bool				GetAssetFile(
							const IManageableAsset *	inManageableAsset,
							IDFile &					oAssetFile) const;
private:
};

#define inheritClass_ICCheckOut CZPAMDocumentCheckOutService
class CZPAMInCopyDocumentCheckOutService : public inheritClass_ICCheckOut
{
public:
						CZPAMInCopyDocumentCheckOutService(IPMUnknown * boss);
	virtual				~CZPAMInCopyDocumentCheckOutService();

	bool16				VerifyContext(
							IManageableAsset * pAsset);
protected:
private:
};

CREATE_PMINTERFACE(CZPAMDocumentCheckOutService, kZPAMDocumentCheckOutServiceImpl)
CREATE_PMINTERFACE(CZPAMInCopyDocumentCheckOutService, kZPAMICDocumentCheckOutServiceImpl)

#pragma mark -
CZPAMDocumentCheckOutService::CZPAMDocumentCheckOutService(IPMUnknown * boss)
: inheritClass(boss)
{
	IZPLog_ObjC( thisFileLA );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMDocumentCheckOutService::~CZPAMDocumentCheckOutService()
{
	IZPLog_ObjD( thisFileLA );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// Do
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMDocumentCheckOutService::Do(
	IManageableAsset * pAsset)
{
	LogFunctionEnterExit;
	this->LogManageableAssetInfo( pAsset );

	ErrorCode toReturn = kFailure;

	const IStringData *	dbAsset = this->GetDBAsset_FromDoc( pAsset );
	if( dbAsset && !this->IsAssetLocked( dbAsset ) )
		toReturn = this->DoAcquireLock( dbAsset );

	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " service DO status = %d", toReturn);
	
	return toReturn;
}

//----------------------------------------------------------------------------------------
// VerifyContext
//----------------------------------------------------------------------------------------
bool16
CZPAMDocumentCheckOutService::VerifyContext(
	IManageableAsset * pAsset)
{
	LogFunctionEnterExit;
	this->LogManageableAssetInfo( pAsset );

	bool16 toReturn = kFalse;
	const IStringData *	dbAsset = this->GetDBAsset_FromDoc( pAsset );
	if( dbAsset && !this->IsAssetLocked( dbAsset ) )
		toReturn = kTrue;

	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " VerifyContext = %hd", toReturn);
	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetAssetFile
//----------------------------------------------------------------------------------------
bool
CZPAMDocumentCheckOutService::GetAssetFile(
	const IManageableAsset *	inManageableAsset,
	IDFile &					oAssetFile) const
{
	return inheritClass::GetAssetFile( inManageableAsset, oAssetFile );
}

#pragma mark -
CZPAMInCopyDocumentCheckOutService::CZPAMInCopyDocumentCheckOutService(IPMUnknown * boss)
: inheritClass_ICCheckOut(boss)
{
	IZPLog_ObjC( thisFileLA );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMInCopyDocumentCheckOutService::~CZPAMInCopyDocumentCheckOutService()
{
	IZPLog_ObjD( thisFileLA );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// VerifyContext
//----------------------------------------------------------------------------------------
bool16
CZPAMInCopyDocumentCheckOutService::VerifyContext(
	IManageableAsset * pAsset)
{
	LogFunctionEnterExit;
	this->LogManageableAssetInfo( pAsset );

	bool16 toReturn = kFalse;
	do
	{
		InterfacePtr<IDocument> assetDoc( pAsset, UseDefaultIID() );
		if ( !pAsset || !assetDoc )	//Also comes for DataLink if not handled by Linked Story services.
			break;

		bool16 isNewDocument = !assetDoc->IsSaved();
		if( isNewDocument )
			break;

		const IStringData *	dbAsset = this->GetDBAsset_FromDoc( pAsset );
		if( dbAsset && !this->IsAssetLocked( dbAsset ) )
			toReturn = kTrue;

	}while(kFalse);

	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " VerifyContext = %hd", toReturn);
	return toReturn;
}
