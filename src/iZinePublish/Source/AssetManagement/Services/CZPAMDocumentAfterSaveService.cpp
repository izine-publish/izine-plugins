//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/Services/CZPAMDocumentAfterSaveService.cpp $
//	$Revision: 3415 $
//	$Date: 2011-06-29 14:07:13 +0200 (Wed, 29 Jun 2011) $
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
#include "IBoolData.h"

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

/** Executes any followup code required by the asset management system at the conclusion of
	a document Save operation. For example, as part of its asset status bookkeeping, Version Cue
	requires notification after a document has been saved locally.
	
	Input IAMServiceData parameters: None.
	Output IAMServiceData parameters: None.				
*/
#define inheritClass AZPAMService
class CZPAMDocumentAfterSaveService : public inheritClass
{
public:
						CZPAMDocumentAfterSaveService(IPMUnknown * boss);
	virtual				~CZPAMDocumentAfterSaveService();

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

#define inheritClass_ICAfterSave CZPAMDocumentAfterSaveService
class CZPAMInCopyDocumentAfterSaveService : public inheritClass_ICAfterSave
{
public:
						CZPAMInCopyDocumentAfterSaveService(IPMUnknown * boss);
	virtual				~CZPAMInCopyDocumentAfterSaveService();

	ErrorCode			Do(
							IManageableAsset * pAsset);
	bool16				VerifyContext(
							IManageableAsset * pAsset);
protected:
private:
};

CREATE_PMINTERFACE(CZPAMDocumentAfterSaveService, kZPAMDocumentAfterSaveServiceImpl)
CREATE_PMINTERFACE(CZPAMInCopyDocumentAfterSaveService, kZPAMICDocumentAfterSaveServiceImpl)

#pragma mark -
CZPAMDocumentAfterSaveService::CZPAMDocumentAfterSaveService(IPMUnknown * boss)
: inheritClass(boss)
{
	IZPLog_ObjC( thisFileLA );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMDocumentAfterSaveService::~CZPAMDocumentAfterSaveService()
{
	IZPLog_ObjD( thisFileLA );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// Do
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMDocumentAfterSaveService::Do(
	IManageableAsset * pAsset)
{
	LogFunctionEnterExit;
	this->LogManageableAssetInfo( pAsset );

	ErrorCode toReturn = kSuccess;

	const IStringData *	dbAsset = this->GetDBAsset_FromDoc( pAsset );
	if( dbAsset )
		this->UpdateLocalInfo_LocallyModified( dbAsset, true );

	
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " service DO status = %d", toReturn);
	
	return toReturn;
}

//----------------------------------------------------------------------------------------
// VerifyContext
//----------------------------------------------------------------------------------------
bool16
CZPAMDocumentAfterSaveService::VerifyContext(
	IManageableAsset * pAsset)
{
	LogFunctionEnterExit;
	this->LogManageableAssetInfo( pAsset );

	bool16 toReturn = kTrue;
	const IStringData *	dbAsset = this->GetDBAsset_FromDoc( pAsset );
	if( !dbAsset )
		toReturn = kFalse;

	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " VerifyContext = %hd", toReturn);
	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetAssetFile
//----------------------------------------------------------------------------------------
bool
CZPAMDocumentAfterSaveService::GetAssetFile(
	const IManageableAsset *	inManageableAsset,
	IDFile &					oAssetFile) const
{
	return inheritClass::GetAssetFile( inManageableAsset, oAssetFile );
}

#pragma mark -
CZPAMInCopyDocumentAfterSaveService::CZPAMInCopyDocumentAfterSaveService(IPMUnknown * boss)
: inheritClass_ICAfterSave(boss)
{
	IZPLog_ObjC( thisFileLA );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMInCopyDocumentAfterSaveService::~CZPAMInCopyDocumentAfterSaveService()
{
	IZPLog_ObjD( thisFileLA );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// Do
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMInCopyDocumentAfterSaveService::Do(
	IManageableAsset * pAsset)
{
	LogFunctionEnterExit;
	this->LogManageableAssetInfo( pAsset );

	ErrorCode toReturn = kSuccess;
	do
	{
		InterfacePtr<IDocument> assetDoc( pAsset, UseDefaultIID() );
		if ( !pAsset || !assetDoc )	//Also comes for DataLink if not handled by Linked Story services.
			break;

		bool16 isNewDocument = !assetDoc->IsSaved();
		InterfacePtr<const IBoolData> isTemplateCopy( pAsset, IID_IZPISTEMPLATECOPY );
		ASSERT(isTemplateCopy);
		if( !isNewDocument )
			isNewDocument = isTemplateCopy->Get();

		if( isNewDocument )
		{	//Patch: Save the document as new Asset And Check-In
			//toReturn = Utils<IZPAMHelper>()->ProcessAction_DocSave( assetDoc );	//Commented as it is not handled by action filter
			break;
		}
		inheritClass_ICAfterSave::Do( pAsset );

	}while(kFalse);

	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " service DO status = %d", toReturn);
	
	return toReturn;
}

//----------------------------------------------------------------------------------------
// VerifyContext
//----------------------------------------------------------------------------------------
bool16
CZPAMInCopyDocumentAfterSaveService::VerifyContext(
	IManageableAsset * pAsset)
{
	LogFunctionEnterExit;
	this->LogManageableAssetInfo( pAsset );

	bool16 toReturn = kTrue;
	do
	{
		InterfacePtr<IDocument> assetDoc( pAsset, UseDefaultIID() );
		if ( !pAsset || !assetDoc )	//Also comes for DataLink if not handled by Linked Story services.
			break;

		bool16 isNewDocument = !assetDoc->IsSaved();
		InterfacePtr<const IBoolData> isTemplateCopy( pAsset, IID_IZPISTEMPLATECOPY );
		ASSERT(isTemplateCopy);
		if( !isNewDocument )
			isNewDocument = isTemplateCopy->Get();

		toReturn = kTrue;
		if( isNewDocument )
			break;

		const IStringData *	dbAsset = this->GetDBAsset_FromDoc( pAsset );
		if( !dbAsset )
			toReturn = kFalse;
	}while(kFalse);

	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " VerifyContext = %hd", toReturn);
	return toReturn;
}
