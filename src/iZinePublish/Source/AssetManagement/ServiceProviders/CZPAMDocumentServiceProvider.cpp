//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/ServiceProviders/CZPAMDocumentServiceProvider.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 27-5-2010
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
#include "CAlert.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPAMDebugHelper.h"
#include "IZPAssetInfo.h"
#include "IZPAssetUtils.h"
#include "IZPFileUtils.h"
#include "IZPInDnUtils.h"

//IZP General includes
#include "AZPAMServiceProvider.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AMSrvP
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass AZPAMServiceProvider
class CZPAMDocumentServiceProvider : public inheritClass
{
public:
						CZPAMDocumentServiceProvider(IPMUnknown * boss);
	virtual				~CZPAMDocumentServiceProvider();

	ErrorCode			Init()
						{	return kSuccess;	}
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


CREATE_PMINTERFACE(CZPAMDocumentServiceProvider, kZPAMDocumentServiceProviderImpl)

CZPAMDocumentServiceProvider::CZPAMDocumentServiceProvider(IPMUnknown * boss)
: inheritClass(boss)
{
	IZPLog_ObjC( thisFileLA );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMDocumentServiceProvider::~CZPAMDocumentServiceProvider()
{
	IZPLog_ObjD( thisFileLA );
}

#pragma mark -

//----------------------------------------------------------------------------------------
// CanService
//----------------------------------------------------------------------------------------
bool16
CZPAMDocumentServiceProvider::CanService(
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
			if ( hints->kFileType == kInDesignType || hints->kFileType == kInDesignTemplateType)
			{
				canService = kTrue;
				IZPLog_Str_( thisFileLA, enLT_DebugInfo, "InDesign Document or Template");
			}
			//else if ( hints->kFileType == kICFileType || hints->kFileType == kICTemplateFileType )	//Commented as it is handled by ICDocServiceProvider
			//{
			//	//TODO: if assetID is present then check it's type with cached object.
			//	//Don't allow indesign story type here.
			//	canService = kTrue;
			//	needsToCheckForAssetType = hints->kFileType == kICFileType;
			//	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "InCopy Document or Template");
			//}
		}

		if ( !canService )
		{
			if ( kInDesignExt.Compare( kFalse, hints->kFileExtension ) == 0 || kInDesignTemplateExt.Compare( kFalse, hints->kFileExtension ) == 0)
				canService = kTrue;
			//Commented as it is handled by ICDocServiceProvider
			//else if ( kICFileExt.Compare( kFalse, hints->kFileExtension ) == 0 || kICTemplateFileExt.Compare( kFalse, hints->kFileExtension ) == 0)
			//{
			//	canService = kTrue;
			//	needsToCheckForAssetType = kICFileExt.Compare( kFalse, hints->kFileExtension ) == 0;
			//}
		}
	}
	
	if ( needsToCheckForAssetType )
	{
		const IStringData * dbAssetID = this->GetDBAsset( file );

		InterfacePtr<const IZPAssetInfo> dbAssetInfo( dbAssetID, UseDefaultIID());
		//ASSERT( dbAssetInfo );	//Nil for new documents
		if( dbAssetInfo && dbAssetInfo->GetType() == eAssetType_IDStory )
		{
			IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Can service this file type but not this one as it is ID story");
			canService = kFalse;
		}

		//TODO: check if this is an IC document then it is standonly document
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
CZPAMDocumentServiceProvider::CanService(
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
			if( Utils<IZPInDnUtils>()->IsHostAppInCopy() )
			{
				//InterfacePtr<IStandAloneDoc> standAlone(asset, IID_ISTANDALONEDOC);
				//canService = standAlone->IsStandAloneDoc();
				canService = kFalse;
			}
			//TODO: check if this document has assetID set or it is new document.
			//If it has assetID then check the assetType, only doc/template types are handled by this provider.
		}
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
CZPAMDocumentServiceProvider::CanServiceDBAsset(
	const IStringData *			inDbAssetID) const
{
	//LogFunctionEnterExit;
	bool16 toReturn = kTrue;
	InterfacePtr<const IZPAssetInfo> dbAssetInfo( inDbAssetID, UseDefaultIID());
	//ASSERT( dbAssetInfo );
	if( dbAssetInfo && (dbAssetInfo->GetType() != eAssetType_IDDocument && dbAssetInfo->GetType() != eAssetType_IDTemplate ) )
	{
		toReturn = kFalse;	//Must never come here.
	}
	return toReturn;
}

// this method returns a boss to handle each requested service that we support
//----------------------------------------------------------------------------------------
// GetService
//----------------------------------------------------------------------------------------
IAMService*
CZPAMDocumentServiceProvider::GetService(
	int32 assetServiceID )
{
	LogFunctionEnterExit;
	IAMService * service = nil;
	
	ClassID serviceBoss = kInvalidClass;

	//bool16 isHostInCopy = Utils<IZPInDnUtils>()->IsHostAppInCopy();

	switch ( assetServiceID )
	{
		case IAMServiceProvider::enInitialize: // need this to initialize asset management on the link
			//if( isHostInCopy )
			//	serviceBoss =	kZPAMICDocumentInitializeServiceBoss;
			//else
			serviceBoss =	kZPAMDocumentInitializeServiceBoss;
			break;
		case IAMServiceProvider::enEnsureLatestVersion :
			serviceBoss =	kZPAMDocumentEnsureLatestVersionServiceBoss;
			break;
		case IAMServiceProvider::enCheckOut :
			//if( isHostInCopy )
			//	serviceBoss =	kZPAMICDocumentCheckOutServiceBoss;
			//else
			serviceBoss =	kZPAMDocumentCheckOutServiceBoss;
			break;
		case IAMServiceProvider::enCancelCheckOut :
			serviceBoss =	kZPAMDocumentCancelCheckOutServiceBoss;
			break;
		case IAMServiceProvider::enRevert :
			serviceBoss =	kZPAMDocumentRevertServiceBoss;
			break;
		case IAMServiceProvider::enCheckIn :
			//if( isHostInCopy )
			//	serviceBoss =	kZPAMICDocumentCheckInServiceBoss;
			//else
			serviceBoss =	kZPAMDocumentCheckInServiceBoss;
			break;
		case IAMServiceProvider::enAfterSave:
		case IAMServiceProvider::enAfterSaveAs:
			//if( isHostInCopy )
			//	serviceBoss =	kZPAMICDocumentAfterSaveServiceBoss;
			//else
			serviceBoss =	kZPAMDocumentAfterSaveServiceBoss;
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
			serviceBoss =	kZPAMDocumentUpdateProxyVersionNumServiceBoss;
			break;
		case IAMServiceProvider::enUpdateStoredAssetReference :
			serviceBoss =	kZPAMDocumentUpdateStoredAssetRefServiceBoss;
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
CZPAMDocumentServiceProvider::OverrideOpenFlags(
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
CZPAMDocumentServiceProvider::GetProviderName()const
{
	return "Document service provider";
}

#if defined(InDnCS5) || defined(InDnCS5_5)
//----------------------------------------------------------------------------------------
// GetLinkResourceId
//----------------------------------------------------------------------------------------
bool16
CZPAMDocumentServiceProvider::GetLinkResourceId(
	const IDFile & inFile, ILinkResource::ResourceId & outResourceId )
{
	LogFunctionEnterExit;
	//TODO: CS5: Test functionality
	//TODO: try converting it to iZP://titleid/editionid/assetid
	bool result = Utils<IURIUtils>()->IDFileToURI(inFile, outResourceId);
	return result;
}

#endif
