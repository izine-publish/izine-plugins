//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/ServiceProviders/AZPAMServiceProvider.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
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
#include "IStringData.h"
#include "IAMServiceProvider.h"

//SDK General includes
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPAMHelper.h"
#include "IZPAssetData.h"
#include "IZPAssetInfo.h"
#include "IZPAssetLockInfo.h"
#include "IZPAssetUtils.h"
#include "IZPFileUtils.h"

//IZP General includes
#include "AZPAMServiceProvider.h"
#include "ZPDataHelper.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AMSrvP
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

const SysOSType	AZPAMServiceProvider::kInDesignType			= FileTypeRegistry::GetCurrentFileType(kInDesignFileTypeInfoID);
const SysOSType	AZPAMServiceProvider::kInDesignTemplateType	= FileTypeRegistry::GetCurrentFileType(kIDTemplateFileTypeInfoID);
#if defined(InDnCS5) || defined(InDnCS5_5)
const SysOSType	AZPAMServiceProvider::kICFileType			= FileTypeRegistry::GetCurrentFileType(kInCopyXMLMarkupFileTypeInfoID);
#else
const SysOSType	AZPAMServiceProvider::kICFileType			= FileTypeRegistry::GetCurrentFileType(kInCopyXMLInterchangeFileTypeInfoID);
#endif
const SysOSType	AZPAMServiceProvider::kICTemplateFileType	= FileTypeRegistry::GetCurrentFileType(kInCopyTemplateFileTypeInfoID);
#if defined(InDnCS5) || defined(InDnCS5_5)
const SysOSType	AZPAMServiceProvider::kAssignmentType		= FileTypeRegistry::GetCurrentFileType(kAssignmentMarkupFileTypeInfoID);
#else
const SysOSType	AZPAMServiceProvider::kAssignmentType		= FileTypeRegistry::GetCurrentFileType(kAssignmentFileTypeInfoID);
#endif
const SysOSType	AZPAMServiceProvider::kInCopyBinType		= FileTypeRegistry::GetCurrentFileType(kInCopyFileTypeInfoID);

const PMString	AZPAMServiceProvider::kInDesignExt			= FileTypeRegistry::GetCurrentFileExtension(kInDesignFileTypeInfoID);
const PMString	AZPAMServiceProvider::kInDesignTemplateExt	= FileTypeRegistry::GetCurrentFileExtension(kIDTemplateFileTypeInfoID);
#if defined(InDnCS5) || defined(InDnCS5_5)
const PMString	AZPAMServiceProvider::kICFileExt			= FileTypeRegistry::GetCurrentFileExtension(kInCopyXMLMarkupFileTypeInfoID);
#else
const PMString	AZPAMServiceProvider::kICFileExt			= FileTypeRegistry::GetCurrentFileExtension(kInCopyXMLInterchangeFileTypeInfoID);
#endif
const PMString	AZPAMServiceProvider::kICTemplateFileExt	= FileTypeRegistry::GetCurrentFileExtension(kInCopyTemplateFileTypeInfoID);
#if defined(InDnCS5) || defined(InDnCS5_5)
const PMString	AZPAMServiceProvider::kAssignmentExt		= FileTypeRegistry::GetCurrentFileExtension(kAssignmentMarkupFileTypeInfoID);
#else
const PMString	AZPAMServiceProvider::kAssignmentExt		= FileTypeRegistry::GetCurrentFileExtension(kAssignmentFileTypeInfoID);
#endif
const PMString	AZPAMServiceProvider::kInCopyBinExt			= FileTypeRegistry::GetCurrentFileExtension(kInCopyFileTypeInfoID);

#pragma mark -
//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
AZPAMServiceProvider::AZPAMServiceProvider(IPMUnknown * boss)
: inheritClass_ZPAMServiceP( boss )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
AZPAMServiceProvider::~AZPAMServiceProvider()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// Init
//----------------------------------------------------------------------------------------
ErrorCode
AZPAMServiceProvider::Init()
{
	IZPLog_Str_( thisFileLA, enLT_Details, " Provider : %s", this->GetProviderName() );

	return kSuccess;
}

//----------------------------------------------------------------------------------------
// CanService
//----------------------------------------------------------------------------------------
bool16
AZPAMServiceProvider::CanService(
	const IDFile& file, const IAMServiceProvider::AssetHints * hints )
{
	LogFunctionEnterExit;
	
	bool16 canService = kFalse;
	// Note: we don't require the file to exist locally in order to determine if we can service it.
	
	//check if this file is from our replica folder.
	if( this->IsSubPathOfOurReplicaFolder( file ) )
	{
		canService = kTrue;

#if defined(CacheWithFolder) && defined(MACINTOSH)
		//Patch: on Mac, AM does not provide file in services till the folder is present
		Utils<IZPAssetUtils> assetUtils;
		PMString assetId = assetUtils->GetAssetIDFromLocalFile(file);
		if (assetId.empty())
			ASSERT(assetId.empty());
		else
			assetUtils->CreateAssetCacheFolderPath(assetId);
#endif
	}

#if DEBUG
	this->LogFilePath( file );
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " canService : %hd", canService);
#endif

	return canService;
}

//----------------------------------------------------------------------------------------
// GetLockData
//	@return kTrue if the asset is locked and data is available; kFalse otherwise. 
//----------------------------------------------------------------------------------------
bool16
AZPAMServiceProvider::GetLockData(
	const IDFile & inAssetFile, IManagedStatus::StatusStringTable & outLockData, UIFlags uiFlags )
{
	LogFunctionEnterExit;
	IZPLog_Str_( thisFileLA, enLT_Details, " Provider : %s", this->GetProviderName() );
	// if document is locked, return user, doc and app name of person with the lock

	//outLockData.insert( std::pair<int32, PMString>( IManagedStatus::enCheckedOutBy, username ) );
	//outLockData.insert( std::pair<int32, PMString>( IManagedStatus::enCheckedOutDoc, documentName ) );
	//outLockData.insert( std::pair<int32, PMString>( IManagedStatus::enCheckedOutApp, applicationName ) );

	bool16 toReturn = kFalse;
	
	do
	{
		const IStringData * assetID = this->GetDBAsset( inAssetFile );
		ASSERT( assetID );
		if( !assetID )
			break;
		
		InterfacePtr<const IZPAssetLockInfo> assetLockInfo( assetID, UseDefaultIID());
		ASSERT( assetLockInfo );
		if( !assetLockInfo )
			break;

		//TODO: find if we have to check who has the lock, self or others or no-one.

		if( assetLockInfo->GetLockID().IsEmpty() )
			break;	//No one has the lock

		if( !assetLockInfo->GetUserName().IsEmpty() )
			outLockData.insert( std::pair<int32, PMString>( IManagedStatus::enCheckedOutBy, assetLockInfo->GetUserName() ) );

		if( !assetLockInfo->GetDocumentName().IsEmpty() )
			outLockData.insert( std::pair<int32, PMString>( IManagedStatus::enCheckedOutDoc, assetLockInfo->GetDocumentName() ) );
		
		if( !assetLockInfo->GetAppName().IsEmpty() )
			outLockData.insert( std::pair<int32, PMString>( IManagedStatus::enCheckedOutApp, assetLockInfo->GetAppName() ) );
		
		toReturn = kTrue;
	} while (kFalse);
	
#if DEBUG
	this->LogFilePath( inAssetFile );
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " has lock data : %hd", toReturn );
#endif

	return toReturn;
}

//----------------------------------------------------------------------------------------
// IsTemporaryVersion
//----------------------------------------------------------------------------------------
bool16
AZPAMServiceProvider::IsTemporaryVersion(
	const IDFile & file )
{
	LogFunctionEnterExit;

	bool16 isTemporaryVersion = kFalse;
	//TODO: Return true when the file is other than HEAD version.
	//easy way would be to set such file name as assetID#versionID
	
	//Currently we are not allowing the feature of viewing old versions.
#if DEBUG
	IZPLog_Str_( thisFileLA, enLT_Details, " Provider : %s", this->GetProviderName() );
	this->LogFilePath( file );
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " isTemporaryVersion = %hd", isTemporaryVersion );
#endif
	
	return isTemporaryVersion;
}

//----------------------------------------------------------------------------------------
// IsVersioned
//----------------------------------------------------------------------------------------
bool16
AZPAMServiceProvider::IsVersioned(
	const IDFile & file )
{
	LogFunctionEnterExit;

	bool16 isVersioned = kFalse;
	//No need to check if the file path is old asset, from our replica folder. As this is called after CanService.

	const IStringData * assetID = this->GetDBAsset( file );
	if( assetID )
	{
		InterfacePtr<const IZPAssetInfo> dbAssetInfo( assetID, UseDefaultIID());
		ASSERT( dbAssetInfo );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "db Asset version number = %d", dbAssetInfo->GetVersionNumber() );
		if( dbAssetInfo->GetVersionNumber() > 0 )
			isVersioned = kTrue;
	}

#if DEBUG
	IZPLog_Str_( thisFileLA, enLT_Details, " Provider : %s", this->GetProviderName() );
	this->LogFilePath( file );
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " isVersioned = %hd", isVersioned );
#endif
	
	return isVersioned;
}

//----------------------------------------------------------------------------------------
// IsWriteable
//----------------------------------------------------------------------------------------
bool16
AZPAMServiceProvider::IsWriteable(
	const IDFile & inAssetFile )
{
	LogFunctionEnterExit;

	bool16 toReturn = kFalse;
	do
	{
		const IStringData * assetID = this->GetDBAsset( inAssetFile );
		ASSERT( assetID );
		if( !assetID )
			break;
			
		//check if lock is acquired by the current user.
		if( Utils<IZPAMHelper>()->IsAssetLockedByCurrentUser( assetID, nil, false ) )
			toReturn = kTrue;
	}while( false );

#if DEBUG
	IZPLog_Str_( thisFileLA, enLT_Details, " Provider : %s", this->GetProviderName() );
	this->LogFilePath( inAssetFile );
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " isWriteable = %hd", toReturn );
#endif

	return toReturn;
}

//----------------------------------------------------------------------------------------
// CanUpdateScriptedAssetData
//----------------------------------------------------------------------------------------
bool16
AZPAMServiceProvider::CanUpdateScriptedAssetData(
	UIDRef assetRef, const PMString & scriptString, int32 scriptID )
{
	LogFunctionEnterExit;
	bool16 canUpdate = kFalse;

#ifdef DEBUG
	char scriptIDStr[4] = {0};
	*(int32*)(scriptIDStr) = scriptID;
	IZPLog_Str_( thisFileLA, enLT_Details, "ScriptID : %s", scriptIDStr );
#endif

	// don't currently provide scripting support
	return canUpdate;
}

//----------------------------------------------------------------------------------------
// UpdateScriptedAssetData
//----------------------------------------------------------------------------------------
ErrorCode
AZPAMServiceProvider::UpdateScriptedAssetData(
	UIDRef assetRef, const PMString & scriptString, int32 scriptID )
{
	LogFunctionEnterExit;

	ErrorCode status = kFailure;
	
#ifdef DEBUG
	char scriptIDStr[4] = {0};
	*(int32*)(scriptIDStr) = scriptID;
	IZPLog_Str_( thisFileLA, enLT_Details, "ScriptID : %s", scriptIDStr );
#endif

	return status;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// IsSubPathOfOurReplicaFolder
//----------------------------------------------------------------------------------------
bool
AZPAMServiceProvider::IsSubPathOfOurReplicaFolder(
	const IDFile &				inFilePath)
{
	LogFunctionEnterExit;
	bool toReturn = Utils<IZPAMHelper>()->IsSubPathOfOurReplicaFolder( inFilePath );
	if( !toReturn )
		toReturn = Utils<IZPAMHelper>()->IsPathOfAnyReplicaFolder( inFilePath );
#ifdef DEBUG
	if( !toReturn )
	{
		this->LogFilePath( inFilePath );
		IZPLog_Str_( thisFileLA, enLT_Warning, "path not from our replica folder" );
	}
#endif

	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetDBAsset
//----------------------------------------------------------------------------------------
const IStringData *
AZPAMServiceProvider::GetDBAsset(
	const IDFile &				inAssetFile)
{
	LogFunctionEnterExit;
	IZPLog_Str_( thisFileLA, enLT_Details, " Provider : %s", this->GetProviderName() );
	PMString assetIDStr = Utils<IZPAssetUtils>()->GetAssetIDFromLocalFile( inAssetFile );

	const IStringData * toReturn = nil;
	
	if( assetIDStr.IsEmpty() == kFalse )
		toReturn = ZPDataHelper::GetAssetByID( assetIDStr );
	//ASSERT( assetID );

	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "asset from file path : %08X", toReturn );
	return toReturn;
}


//----------------------------------------------------------------------------------------
// GetDBAsset
//----------------------------------------------------------------------------------------
const IStringData *
AZPAMServiceProvider::GetDBAsset(
	const IManageableAsset *	inManageableAsset)
{
	LogFunctionEnterExit;
	IZPLog_Str_( thisFileLA, enLT_Details, " Provider : %s", this->GetProviderName() );

	const IStringData * toReturn = nil;
	InterfacePtr<const IZPAssetData> assetData(inManageableAsset, UseDefaultIID());
	toReturn = this->GetDBAsset( assetData );

	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "asset from IManagableAsset : %08X", toReturn );
	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetDBAsset
//----------------------------------------------------------------------------------------
const IStringData *
AZPAMServiceProvider::GetDBAsset(
	const IZPAssetData *		inAssetData) const
{
	IZPLog_Str_( thisFileLA, enLT_Details, " Provider : %s", this->GetProviderName() );
	const IStringData * toReturn = nil;
	if( inAssetData )
	{
		const PMString & assetIDStr = inAssetData->GetAssetID();
		if( assetIDStr.IsEmpty() == kFalse )
			toReturn = ZPDataHelper::GetAssetByID( assetIDStr );
	}
	return toReturn;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// LogFilePath
//----------------------------------------------------------------------------------------
void
AZPAMServiceProvider::LogFilePath(
	const IDFile &				inFile) const
{
#ifdef DEBUG
	PMString pathStr;
	Utils<IZPFileUtils>()->ConvertIDFileToStr( inFile, pathStr );
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "File path : %s ", pathStr.GrabCString() );
#endif
}

//----------------------------------------------------------------------------------------
// LogManageableAssetInfo
//----------------------------------------------------------------------------------------
void
AZPAMServiceProvider::LogManageableAssetInfo(
	const IManageableAsset *	inAsset) const
{
#ifdef DEBUG
	if( inAsset )
	{
		DebugClassUtilsBuffer classBuf;
		const char* className = DebugClassUtils::GetIDName(&classBuf, ::GetClass( inAsset ));
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, " Asset UID = 0x%08X , asset class name = %s", ::GetUID( inAsset ).Get(), className );
	}
	else
	{
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, " Asset is nil");
	}
#endif
}

//----------------------------------------------------------------------------------------
// LogFileHints
//----------------------------------------------------------------------------------------
void
AZPAMServiceProvider::LogFileHints(
	const IAMServiceProvider::AssetHints * inHints) const
{
#ifdef DEBUG
	if ( inHints && (inHints->kFileType != 0 || !inHints->kFileExtension.IsEmpty()) )
	{
		if ( inHints->kFileType != 0 )
		{
			char fileType[sizeof(inHints->kFileType)+1] = {0};
			*((SysOSType*)fileType) = inHints->kFileType;
			IZPLog_Str_( thisFileLA, enLT_DebugInfo, "File type hint : %s", fileType);
		}
		else
		{
			IZPLog_Str_( thisFileLA, enLT_DebugInfo, "File extenstion hint : %s", inHints->kFileExtension.GrabCString() );
		}
	}
#endif
}
