//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/ServiceProviders/CZPAMICDocumentServiceProvider.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 4-8-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IAMService.h"
#include "IAMServiceProvider.h"
#include "IAMUIServiceProvider.h"
#include "IStandAloneDoc.h"
#include "IStringData.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IURIUtils.h"
#endif

//SDK General includes
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPAMDebugHelper.h"
#include "IZPAssetInfo.h"
#include "IZPFileUtils.h"
#include "IZPInDnUtils.h"

//IZP General includes
#include "AZPAMServiceProvider.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AMSrvP
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass AZPAMServiceProvider
class CZPAMICDocumentServiceProvider : public inheritClass
{
public:
						CZPAMICDocumentServiceProvider(IPMUnknown * boss);
	virtual				~CZPAMICDocumentServiceProvider();
	
	ErrorCode			Init();
						//{	return kSuccess;	}
	bool16				CanService(
							const IDFile & file, const IAMServiceProvider::AssetHints * hints );
	bool16				CanService(
							const IManageableAsset * asset );
	//bool16				CanUpdateScriptedAssetData(
	//						UIDRef assetRef, const PMString & scriptString, int32 scriptID );
	//bool16				GetLockData(
	//						const IDFile & assetFile, IManagedStatus::StatusStringTable & outLockData, UIFlags uiFlags );
	IAMService*			GetService(
							int32 assetServiceID );
	IOpenFileCmdData::OpenFlags OverrideOpenFlags(
							const IDFile & file, IOpenFileCmdData::OpenFlags inOpenFlags );
	
	//bool16				IsTemporaryVersion(
	//						const IDFile & file );
	//bool16				IsVersioned(
	//						const IDFile & file );
	//bool16				IsWriteable(
	//						const IDFile & storyFile );
	//ErrorCode			UpdateScriptedAssetData(
	//						UIDRef assetRef, const PMString & scriptString, int32 scriptID );

#if defined(InDnCS5) || defined(InDnCS5_5)
	bool16				GetLinkResourceId(
							const IDFile & inFile, ILinkResource::ResourceId & outResourceId );
#endif
protected:
	bool16				CanServiceDBAsset(
							const IStringData *			inDbAssetID ) const;
	const char *		GetProviderName() const;
private:
};


CREATE_PMINTERFACE(CZPAMICDocumentServiceProvider, kZPAMICDocumentServiceProviderImpl)

CZPAMICDocumentServiceProvider::CZPAMICDocumentServiceProvider(IPMUnknown * boss)
: inheritClass(boss)
{
	IZPLog_ObjC( thisFileLA );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMICDocumentServiceProvider::~CZPAMICDocumentServiceProvider()
{
	IZPLog_ObjD( thisFileLA );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// Init
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMICDocumentServiceProvider::Init()
{
	LogFunctionEnterExit;
	ErrorCode toReturn = kSuccess;
	if( Utils<IZPInDnUtils>()->IsHostAppInCopy() != kTrue )
		toReturn = kFailure;
	
	return toReturn;
}


//----------------------------------------------------------------------------------------
// CanService
//----------------------------------------------------------------------------------------
bool16
CZPAMICDocumentServiceProvider::CanService(
	const IDFile& file, const IAMServiceProvider::AssetHints * hints )
{
	LogFunctionEnterExit;
	
	bool16 canService = kFalse;
	bool needsToCheckForAssetType = false;
	
	// Note: we don't require the file to exist locally in order to determine if we can service it.
	if ( hints && (hints->kFileType != 0 || !hints->kFileExtension.IsEmpty()) )
	{
		this->LogFileHints( hints );
		if ( hints->kFileType != 0 )
		{
			if ( hints->kFileType == kICFileType || hints->kFileType == kICTemplateFileType )
			{
				canService = kTrue;
				needsToCheckForAssetType = kTrue;//hints->kFileType == kICFileType;
				IZPLog_Str_( thisFileLA, enLT_DebugInfo, "InCopy Document or Template");
			}
		}

		if ( !canService )
		{
			if ( kICFileExt.Compare( kFalse, hints->kFileExtension ) == 0 || kICTemplateFileExt.Compare( kFalse, hints->kFileExtension ) == 0)
			{
				canService = kTrue;
				needsToCheckForAssetType = kTrue;//kICFileExt.Compare( kFalse, hints->kFileExtension ) == 0;
			}
		}
	}
	
	if ( needsToCheckForAssetType )
	{
		const IStringData * dbAssetID = this->GetDBAsset( file );

		InterfacePtr<const IZPAssetInfo> dbAssetInfo( dbAssetID, UseDefaultIID());
		//ASSERT( dbAssetInfo );
		if( dbAssetInfo && (dbAssetInfo->GetType() == eAssetType_IDStory || dbAssetInfo->GetVersionNumber() >= 1) )
		{
			IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Can service this file type but not this one as it is ID story");
			canService = kFalse;
		}
	}

	if( canService ) //Base will check if file is from our replica folder or not.
		canService = inheritClass::CanService( file, hints );

	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "canService file : %hd", canService);
	return canService;
}

//----------------------------------------------------------------------------------------
// CanService
//----------------------------------------------------------------------------------------
bool16
CZPAMICDocumentServiceProvider::CanService(
	const IManageableAsset * asset )
{
	LogFunctionEnterExit;
	bool16 canService = kFalse;
	
	if ( asset )
	{
		// if it's one of our links, then we service it
		if( ::GetUID( asset ).Get() == ::GetDataBase(asset)->GetRootUID() )
		//if ( ::GetClass( asset ) == kDocBoss )	//Someday: Better use UID of asset, for doc, it is always 1.
		{
			canService = kTrue;
			if( Utils<IZPInDnUtils>()->IsHostAppInCopy() != kTrue )
			{
				canService = kFalse;
			}
			//TODO: check if this document has assetID set or it is new document.
			//If it has assetID then check the assetType, only doc/template types are handled by this provider.
		}
	}

	if( canService )
	{
		const IStringData * dbAssetID = this->GetDBAsset( asset );
		//Don't handle the Assignment documents here. dbAssetID will come nil for such case.
		//Get the dbAssetID from file in such case.
		if( !dbAssetID )
		{
			InterfacePtr<IDocument> document( asset, UseDefaultIID());
			if( document && document->IsSaved() )
			{
				IDataBase * db = ::GetDataBase( asset );
				const IDFile * documentFile = db->GetSysFile();
				if( documentFile )
					dbAssetID = this->GetDBAsset( *documentFile );
			}
		}

		canService = this->CanServiceDBAsset( dbAssetID );
	}
	
	//TODO: it must be new document or from our replica folder... i.e having valid asset ID.
#ifdef DEBUG
	this->LogManageableAssetInfo( asset );
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " CanService asset: %hd", canService );
#endif
	return canService;
}

//----------------------------------------------------------------------------------------
// CanServiceDBAsset
//----------------------------------------------------------------------------------------
bool16
CZPAMICDocumentServiceProvider::CanServiceDBAsset(
	const IStringData *			inDbAssetID) const
{
	//LogFunctionEnterExit;
	bool16 toReturn = kTrue;
	InterfacePtr<const IZPAssetInfo> dbAssetInfo( inDbAssetID, UseDefaultIID());
	//ASSERT( dbAssetInfo );
	if( dbAssetInfo && (dbAssetInfo->GetType() == eAssetType_IDStory || dbAssetInfo->GetVersionNumber() >= 1) )
	{
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Can service this file type but not this asset.");
		toReturn = kFalse;
	}
	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetService
// This service provider is used only for InCopy document and template till the first check-in is made.
// InCopy document/template handling is different than InDesign Document/Template handling
// as these are treated as Linked Story and handled by Linked Story AM after first check-in.
//----------------------------------------------------------------------------------------
IAMService*
CZPAMICDocumentServiceProvider::GetService(
	int32 assetServiceID )
{
	LogFunctionEnterExit;
	IAMService * service = nil;
	
	ClassID serviceBoss = kInvalidClass;

	bool16 isHostInCopy = Utils<IZPInDnUtils>()->IsHostAppInCopy();

	switch ( assetServiceID )
	{
		case IAMServiceProvider::enInitialize: // need this to initialize asset management on the link
			serviceBoss =	kZPAMICDocumentInitializeServiceBoss;
			break;
		case IAMServiceProvider::enEnsureLatestVersion :
			serviceBoss =	kZPAMDocumentEnsureLatestVersionServiceBoss;	//Should not be used
			break;
		case IAMServiceProvider::enCheckOut :
			serviceBoss =	kZPAMICDocumentCheckOutServiceBoss;		//Should not be used
			break;
		case IAMServiceProvider::enCancelCheckOut :
			serviceBoss =	kZPAMDocumentCancelCheckOutServiceBoss;	//Should not be used
			break;
		case IAMServiceProvider::enRevert :
			serviceBoss =	kZPAMDocumentRevertServiceBoss;			//Should not be used
			break;
		case IAMServiceProvider::enCheckIn :
			serviceBoss =	kZPAMICDocumentCheckInServiceBoss;
			break;
		case IAMServiceProvider::enAfterSave:
		case IAMServiceProvider::enAfterSaveAs:
			serviceBoss =	kZPAMICDocumentAfterSaveServiceBoss;	//Check-in is initiated from this service.
			break;
		case IAMServiceProvider::enRelink:
			break;
		case IAMServiceProvider::enUpdateLinkLocation :
			break;
		case IAMServiceProvider::enBeforeCheckOut :
			break;
		case IAMServiceProvider::enAfterCancelCheckOut :
			break;
		case IAMServiceProvider::enAfterCheckIn :
			break;
		case IAMServiceProvider::enUnembed :
			break;
		case IAMServiceProvider::enTerminate :
			serviceBoss =	kZPAMDocumentTerminateServiceBoss;
			break;
		case IAMServiceProvider::enUpdateProxyVersionNumber :
			serviceBoss =	kZPAMDocumentUpdateProxyVersionNumServiceBoss;	//Should not be used
			break;
		case IAMServiceProvider::enUpdateStoredAssetReference :
			serviceBoss =	kZPAMDocumentUpdateStoredAssetRefServiceBoss;	//Should not be used
			break;
		case IAMServiceProvider::enInvalidateProxyVersionNumber :
			break;
		case IAMServiceProvider::enGetUniqueAssetLocation :
			break;
		case IAMServiceProvider::enBatchCheckIn :
			break;
		case IAMServiceProvider::enInvalidService :
			ASSERT_FAIL( "**IZPAMDocumentServiceProvider::GetService - called with enInvalidService assetServiceID**" );
			break;
	}
	
	if( !service && serviceBoss.Get() != kInvalidClass.Get() )
	{
		service = ::CreateObject2<IAMService>( serviceBoss );
	}
#ifdef DEBUG
	IZPLog_Str_( thisFileLA, enLT_Details, "%s : %08X: service impl instance : %08X", Utils<IZPAMDebugHelper>()->GetAMServiceName((const IAMServiceProvider::AssetService)assetServiceID), assetServiceID, service );
#endif

	return service;
}

//----------------------------------------------------------------------------------------
// OverrideOpenFlags
//----------------------------------------------------------------------------------------
IOpenFileCmdData::OpenFlags
CZPAMICDocumentServiceProvider::OverrideOpenFlags(
	const IDFile & file, IOpenFileCmdData::OpenFlags inOpenFlags )
{
	LogFunctionEnterExit;

	IOpenFileCmdData::OpenFlags openFlags = inOpenFlags;
	//This function must be called only for ID documents, where old version must be opned with IOpenFileCmdData::kOpenCopy flag.
	//Template document as copy opening is controlled by UI, based on key pressed by user
	
	//No need to check if this is old version or not, as assets have proper status are shown in asset panel and status changes during check-in.
	//No need to check if the file path is old asset, from our replica folder. As this is called after CanService.
	
	return openFlags;
}

//----------------------------------------------------------------------------------------
// GetProviderName
//----------------------------------------------------------------------------------------
const char *
CZPAMICDocumentServiceProvider::GetProviderName()const
{
	return "IC Document service provider";
}

#if defined(InDnCS5) || defined(InDnCS5_5)
//----------------------------------------------------------------------------------------
// GetLinkResourceId
//----------------------------------------------------------------------------------------
bool16
CZPAMICDocumentServiceProvider::GetLinkResourceId(
	const IDFile & inFile, ILinkResource::ResourceId & outResourceId )
{
	LogFunctionEnterExit;
	//TODO: CS5: Test functionality
	bool result = Utils<IURIUtils>()->IDFileToURI(inFile, outResourceId);
	return result;
}

#endif
