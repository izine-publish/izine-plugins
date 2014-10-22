//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/ServiceProviders/CZPAMLinkedStoryServiceProvider.cpp $
//	$Revision: 4096 $
//	$Date: 2013-04-01 09:20:00 +0200 (Mon, 01 Apr 2013) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 28-6-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IAMService.h"
#include "IAMServiceProvider.h"
#include "IAMUIServiceProvider.h"
#include "IAssignment.h"
#include "IAssignmentMgr.h"
#include "IDataLink.h"
#include "IStringData.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IURIUtils.h"
#include "FileTypeInfo.h"
#endif
#include "IWorkgroupStoryFacade.h"

//SDK General includes
#include "CAlert.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPAssetInfo.h"
#include "IZPAssetUtils.h"
#include "IZPAMDebugHelper.h"
#include "IZPFileUtils.h"
#include "IZPInDnUtils.h"

//IZP General includes
#include "AZPAMServiceProvider.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AMSrvP
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass AZPAMServiceProvider
class CZPAMLinkedStoryServiceProvider : public inheritClass
{
public:
						CZPAMLinkedStoryServiceProvider(IPMUnknown * boss);
	virtual				~CZPAMLinkedStoryServiceProvider();

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


CREATE_PMINTERFACE(CZPAMLinkedStoryServiceProvider, kZPAMLinkedStoryServiceProviderImpl)

CZPAMLinkedStoryServiceProvider::CZPAMLinkedStoryServiceProvider(IPMUnknown * boss)
: inheritClass(boss)
{
	IZPLog_ObjC( thisFileLA );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMLinkedStoryServiceProvider::~CZPAMLinkedStoryServiceProvider()
{
	IZPLog_ObjD( thisFileLA );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// CanService
//----------------------------------------------------------------------------------------
bool16
CZPAMLinkedStoryServiceProvider::CanService(
	const IDFile & file, const IAMServiceProvider::AssetHints * hints)
{
	LogFunctionEnterExit;
	
	bool16 toReturn = kFalse;

	// Note: we don't require the file to exist locally in order to determine if we can service it.
	
	if ( hints && (hints->kFileType != 0 || !hints->kFileExtension.IsEmpty()) )
	{
		this->LogFileHints( hints );
		if ( hints->kFileType != 0 )
		{
			if (( hints->kFileType == kICFileType ) || (hints->kFileType == FileTypeRegistry::GetCurrentFileType(kInCopyXMLInterchangeFileTypeInfoID))
			||	( hints->kFileType == kInCopyBinType )
			||	( hints->kFileType == kICTemplateFileType ) )
			{
				toReturn = kTrue;
			}
		}

		if ( !toReturn 
		&& ( ( kICFileExt.Compare( kFalse, hints->kFileExtension ) == 0) || (FileTypeRegistry::GetCurrentFileExtension(kInCopyXMLInterchangeFileTypeInfoID).Compare( kFalse, hints->kFileExtension ) == 0)
			|| (kInCopyBinExt.Compare(kFalse, hints->kFileExtension ) == 0 )
			|| (kICTemplateFileExt.Compare( kFalse, hints->kFileExtension ) == 0) ) )
		{
			toReturn = kTrue;
			IZPLog_Str_( thisFileLA, enLT_DebugInfo, "InCopy story or document extension");
		}
	}
	
	if( toReturn )
	{
		//check if this story is linked story or not. Standalone IC doc without save is not handled here.
		const IStringData * dbAssetID = this->GetDBAsset( file );

		toReturn = this->CanServiceDBAsset( dbAssetID );
	}

	if( toReturn ) //Base will check if file is from our replica folder or not.
		toReturn = inheritClass::CanService( file, hints );
		
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "canService file : %hd", toReturn );

	return toReturn;
}

//----------------------------------------------------------------------------------------
// CanService
//----------------------------------------------------------------------------------------
bool16
CZPAMLinkedStoryServiceProvider::CanService(
	const IManageableAsset * asset)
{
	LogFunctionEnterExit;
	
	bool16 toReturn = kFalse;

#if defined(InDnCS5) || defined(InDnCS5_5)
	InterfacePtr<ILinkResource> assetLinkResource( asset, UseDefaultIID() );
	if( assetLinkResource )
#else
	InterfacePtr<IDataLink> assetDataLink( asset, UseDefaultIID() );
	if( assetDataLink )
#endif
	{
		UIDRef assetRef = ::GetUIDRef( asset );

		//Check if this data link is of a story
		IDFile linkedFilePath;
		UIDRef storyRef = Utils<Facade::IWorkgroupStoryFacade>()->GetStoryUIDRef( assetRef );

#if defined(InDnCS5) || defined(InDnCS5_5)
		if(!storyRef)
		{
			//storyRef may be nil as the link may not have yet been created. So checking for resource type if storyRef is nil.
			//PMString ICExt("icml");
			//FileTypeInfo typeInfo = assetLinkResource->GetDataType();
			FileTypeInfoID fileType = assetLinkResource->GetDataType().GetFileTypeInfoID();
			if(fileType == kInCopyXMLMarkupFileTypeInfoID || fileType == kInCopyXMLInterchangeFileTypeInfoID)
			//if( fileType == kInCopyXMLMarkupFileTypeInfoID )
				toReturn = kTrue;
			//SysOSType s = typeInfo.GetFileType();  
			//if(typeInfo.GetFileExtension().IsEqual(ICExt, kFalse)) // kInCopyMarkupFileType(icml)
			//	toReturn = kTrue;			
		}
#endif
//#else
		if( storyRef != UIDRef::gNull )
		{
			toReturn = kTrue;
#if 0		//Following does not work in all cases. Just after initialize assignment is comming nil
			InterfacePtr<IAssignmentMgr> assMgr(gSession, UseDefaultIID());
			ASSERT(assMgr);
			InterfacePtr<IAssignment>	iAssignment;
			InterfacePtr<IAssignedStory> iAssignedStory;

			assMgr->QueryAssignmentAndAssignedStory( storyRef , iAssignment, iAssignedStory );
			if( !iAssignedStory )
				toReturn = false;
#endif
		}
//#endif
	}

#ifdef DEBUG
	this->LogManageableAssetInfo( asset );
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " CanService asset: %hd", toReturn );
#endif
	return toReturn;
}

//----------------------------------------------------------------------------------------
// CanServiceDBAsset
//----------------------------------------------------------------------------------------
bool16
CZPAMLinkedStoryServiceProvider::CanServiceDBAsset(
	const IStringData *			inDbAssetID) const
{
	//LogFunctionEnterExit;

	bool16 toReturn = kTrue;
	InterfacePtr<const IZPAssetInfo> dbAssetInfo( inDbAssetID, UseDefaultIID());	//Can be null for IC document, but we should not handle that here, how?
	if( dbAssetInfo && (dbAssetInfo->GetType() != eAssetType_IDDocument && dbAssetInfo->GetType() != eAssetType_IDTemplate ) )
	{
		//bIsIDStoryAsset = ( dbAssetInfo && dbAssetInfo->GetType() == eAssetType_IDStory );
		bool bIsIDStoryAsset = ( dbAssetInfo->GetType() == eAssetType_IDStory	//Story type allowed
								||( ( dbAssetInfo->GetType() == eAssetType_ICDocument || dbAssetInfo->GetType() == eAssetType_ICTemplate )
									&& dbAssetInfo->GetVersionNumber() >= 1 ) ) ;	//IC document/template after save is treated as linked story
	
		if( !bIsIDStoryAsset )
			toReturn = kFalse;
	}

#ifdef DEBUG
//	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " CanServiceDBAsset asset: %hd", toReturn );
#endif

	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetService
//----------------------------------------------------------------------------------------
IAMService*
CZPAMLinkedStoryServiceProvider::GetService(
	int32 assetServiceID)
{
	LogFunctionEnterExit;
	IAMService * toReturn = nil;

	ClassID serviceBoss = kInvalidClass;

	switch ( assetServiceID )
	{
		case IAMServiceProvider::enInitialize: // need this to initialize asset management on the link
			serviceBoss = kZPAMLinkedStoryInitializeServiceBoss;
			break;
		case IAMServiceProvider::enEnsureLatestVersion :
			serviceBoss =	kZPAMLinkedStoryEnsureLatestVersionServiceBoss;
			break;
		case IAMServiceProvider::enCheckOut :
			serviceBoss =	kZPAMLinkedStoryCheckOutServiceBoss;
			break;
		case IAMServiceProvider::enCancelCheckOut :
			serviceBoss =	kZPAMLinkedStoryCancelCheckOutServiceBoss;
			break;
		case IAMServiceProvider::enRevert :
			serviceBoss =	kZPAMLinkedStoryRevertServiceBoss;
			break;
		case IAMServiceProvider::enCheckIn :
			serviceBoss =	kZPAMLinkedStoryCheckInServiceBoss;
			break;
		case IAMServiceProvider::enAfterSave:
		case IAMServiceProvider::enAfterSaveAs:
			serviceBoss =	kZPAMLinkedStoryAfterSaveServiceBoss;
			break;
		case IAMServiceProvider::enRelink:
			// service = ::CreateObject2<IAMService>( kIZPAMRelinkServiceBoss );
			break;
		case IAMServiceProvider::enUpdateLinkLocation :
			// service = ::CreateObject2<IAMService>( kIZPAMUpdateLinkLocationServiceBoss );
			break;
		case IAMServiceProvider::enBeforeCheckOut :
			//			service = ::CreateObject2<IAMService>( kSETICBeforeCheckOutServiceBoss );
			break;
		case IAMServiceProvider::enAfterCancelCheckOut :
			//			service = ::CreateObject2<IAMService>( kSETICAfterCancelCheckOutServiceBoss );
			break;
		case IAMServiceProvider::enAfterCheckIn :
			serviceBoss = kZPAMLinkedStoryAfterCheckInServiceBoss;
			break;
		case IAMServiceProvider::enUnembed :
			CAlert::InformationAlert("enUnembed");
			// service = ::CreateObject2<IAMService>( kIZPAMUnembedServiceBoss );
			break;
		case IAMServiceProvider::enTerminate :
			serviceBoss = kZPAMLinkedStoryTerminateServiceBoss;
			break;
		case IAMServiceProvider::enUpdateProxyVersionNumber :
			serviceBoss =	kZPAMLinkedStoryUpdateProxyVersionNumberServiceBoss;
			break;
		case IAMServiceProvider::enUpdateStoredAssetReference :
			serviceBoss =	kZPAMLinkedStoryUpdateStoredAssetReferenceServiceBoss;
			break;
		case IAMServiceProvider::enInvalidateProxyVersionNumber :
//			service = ::CreateObject2<IAMService>( kIZPAMInvalidateProxyVersionNumberServiceBoss );
			break;
		case IAMServiceProvider::enGetUniqueAssetLocation :
			// service = ::CreateObject2<IAMService>( kIZPAMGetUniqueAssetLocationServiceBoss );
			break;
		case IAMServiceProvider::enBatchCheckIn :
			CAlert::InformationAlert("enBatchCheckIn");
			//			service = ::CreateObject2<IAMService>( kSETBatchCheckInServiceBoss ); // used for packaging stories
			break;
		case IAMServiceProvider::enInvalidService :
			ASSERT_FAIL( "**IZPAMServiceProvider::GetService - called with enInvalidService assetServiceID**" );
			break;
	}

	if( !toReturn && serviceBoss.Get() != kInvalidClass.Get() )
	{
		toReturn = ::CreateObject2<IAMService>( serviceBoss );
	}
#ifdef DEBUG
	IZPLog_Str_( thisFileLA, enLT_Details, "%s : %08X : service impl instance : %08X", Utils<IZPAMDebugHelper>()->GetAMServiceName((const IAMServiceProvider::AssetService)assetServiceID), assetServiceID, toReturn );
#endif	

	return toReturn;
}

//----------------------------------------------------------------------------------------
// OpenFlags
//----------------------------------------------------------------------------------------

IOpenFileCmdData::OpenFlags
CZPAMLinkedStoryServiceProvider::OverrideOpenFlags(
	const IDFile & file, IOpenFileCmdData::OpenFlags inOpenFlags)
{
	LogFunctionEnterExit;

	IOpenFileCmdData::OpenFlags openFlags = inOpenFlags;
	//Must never come here for ID story. Only InCopy document & templates types come here in InCopy.
	//No need to check for read only as story is check-out separately.
	
	return openFlags;
}

//----------------------------------------------------------------------------------------
// GetProviderName
//----------------------------------------------------------------------------------------
const char *
CZPAMLinkedStoryServiceProvider::GetProviderName()const
{
	return "Linked story service provider";
}

#if defined(InDnCS5) || defined(InDnCS5_5)
//----------------------------------------------------------------------------------------
// GetLinkResourceId
//----------------------------------------------------------------------------------------
bool16
CZPAMLinkedStoryServiceProvider::GetLinkResourceId(
	const IDFile & inFile, ILinkResource::ResourceId & outResourceId )
{
	LogFunctionEnterExit;
	//TODO: CS5: Test functionality
	bool result = Utils<IURIUtils>()->IDFileToURI(inFile, outResourceId);
	return result;
}

#endif