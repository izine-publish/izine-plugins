//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/Services/CZPAMDocumentRevertService.cpp $
//	$Revision: 1900 $
//	$Date: 2010-08-05 14:24:02 +0200 (Thu, 05 Aug 2010) $
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
#include "IManagedStatus.h"

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

/** Reverts local changes to an asset.
	
	Input IAMServiceData parameters: None.
	Output IAMServiceData parameters: None.
*/

#define inheritClass AZPAMService
class CZPAMDocumentRevertService : public inheritClass
{
public:
						CZPAMDocumentRevertService(IPMUnknown * boss);
	virtual				~CZPAMDocumentRevertService();

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


CREATE_PMINTERFACE(CZPAMDocumentRevertService, kZPAMDocumentRevertServiceImpl)

CZPAMDocumentRevertService::CZPAMDocumentRevertService(IPMUnknown * boss)
: inheritClass(boss)
{
	IZPLog_ObjC( thisFileLA );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMDocumentRevertService::~CZPAMDocumentRevertService()
{
	IZPLog_ObjD( thisFileLA );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// Do
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMDocumentRevertService::Do(
	IManageableAsset * pAsset)
{
	LogFunctionEnterExit;
	this->LogManageableAssetInfo( pAsset );

	ErrorCode toReturn = kFailure;
	
	const IStringData *	dbAsset = this->GetDBAsset_FromDoc( pAsset );
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
CZPAMDocumentRevertService::VerifyContext(
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
	}while(kFalse);

	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " VerifyContext = %hd", toReturn);
	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetAssetFile
//----------------------------------------------------------------------------------------
bool
CZPAMDocumentRevertService::GetAssetFile(
	const IManageableAsset *	inManageableAsset,
	IDFile &					oAssetFile) const
{
	return inheritClass::GetAssetFile( inManageableAsset, oAssetFile );
}
