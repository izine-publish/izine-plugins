//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Data/CZPAssetsLocalInfoMgr.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 2-7-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IDateTimeData.h"
#include "IDTime.h"
#include "IStringData.h"
#include "IWorkspace.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes
#include "FileUtils.h"
#include "PlatformFileSystemIterator.h"
#include "UIDList.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPAssetLocalInfo.h"
#include "IZPAssetsLocalInfoMgr.h"
#include "IZPAssetUtils.h"
#include "IZPCommandFacade.h"
#include "IZPFileUtils.h"
#include "IZPUIDListData.h"

//IZP General includes
#include "ZPConstants.h"
#include "ZPTypes.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_None
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZPAssetsLocalInfoMgr>
class CZPAssetsLocalInfoMgr : public inheritClass
{
public:
						CZPAssetsLocalInfoMgr(IPMUnknown * boss);
	virtual				~CZPAssetsLocalInfoMgr();

	const IZPAssetLocalInfo *	QueryInfoForAsset(
							const PMString &			inAssetID,
							bool						inCreateIfNotFound ) const;
	
	UIDRef				AddInfoForAsset(
							const PMString &			inAssetID );
	void				RemoveAssetInfo(
							const PMString &			inAssetID );

	void				RemoveAssetInfo(
							K2Vector<UID> &				inAssetUIDList );

	UIDRef				FindInfoForAsset(
							const PMString &			inAssetID ) const;

	void				RemoveMissingLocalAssetsInfo();		//Local info not exist
	void				ClearCache_OldAssets();
	void				RemoveMissingCacheLocalInfo();		//File not exist

	bool16				ValidPath(						//false for . & ..
							const PMString&				path) const;
protected:
	IZPUIDListData *	GetUIDListData() const;
private:
};


CREATE_PMINTERFACE(CZPAssetsLocalInfoMgr, kZPAssetsLocalInfoMgrImpl)

CZPAssetsLocalInfoMgr::CZPAssetsLocalInfoMgr(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAssetsLocalInfoMgr::~CZPAssetsLocalInfoMgr()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// QueryInfoForAsset
//----------------------------------------------------------------------------------------
const IZPAssetLocalInfo *
CZPAssetsLocalInfoMgr::QueryInfoForAsset(
	const PMString &			inAssetID,
	bool						inCreateIfNotFound ) const
{
	UIDRef assetLocalInfoRef = this->FindInfoForAsset( inAssetID );

	InterfacePtr<const IZPAssetLocalInfo> assetLocalInfo( assetLocalInfoRef, UseDefaultIID() );
	if( !assetLocalInfo && inCreateIfNotFound )
	{
		UIDRef localInfoRef = (const_cast<CZPAssetsLocalInfoMgr*>(this))->AddInfoForAsset( inAssetID );
		InterfacePtr<const IZPAssetLocalInfo> assetLocalInfo1( localInfoRef, UseDefaultIID() );
		assetLocalInfo.reset( assetLocalInfo1.forget() );
	}
	
	return assetLocalInfo.forget();
}

//----------------------------------------------------------------------------------------
// AddInfoForAsset
//----------------------------------------------------------------------------------------
UIDRef
CZPAssetsLocalInfoMgr::AddInfoForAsset(
	const PMString &			inAssetID)
{
	LogFunctionEnterExit;
	//First check if the asset is alread present or not.
#ifdef DEBUG
	InterfacePtr<const IZPAssetLocalInfo> assetLocalInfo( this->QueryInfoForAsset( inAssetID, false ) );
#else
	InterfacePtr<const IZPAssetLocalInfo> assetLocalInfo;
#endif
	ASSERT( !assetLocalInfo );
	if( !assetLocalInfo )
	{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
		InterfacePtr<const IWorkspace> iWorkspace(gSession->QueryWorkspace());
		
		//Create a new UID in asset Local info list.
		InterfacePtr<ICommand> addCmd( Utils<IZPCommandFacade>()->CreateCommand_AddAssetLocalInfo() );
		ASSERT( addCmd );

		UIDList cmdTargetList( ::GetDataBase( iWorkspace ) );
		cmdTargetList.Append( ::GetUID( iWorkspace ) );
		addCmd->SetItemList( cmdTargetList );
		
		ErrorCode err = CmdUtils::ProcessCommand( addCmd );

		const UIDList * cmdItemList = addCmd->GetItemList();

		InterfacePtr<const IStringData> assetID( cmdItemList->GetRef(1), IID_IZPDATAID );
		ASSERT( assetID );
		
		InterfacePtr<const IZPAssetLocalInfo> newAssetLocalInfo( assetID, UseDefaultIID() );

		//Set the asset ID with newly created object
		Utils<IZPCommandFacade>()->UpdateAssetLocalInfo( newAssetLocalInfo, &inAssetID, nil , nil );

		assetLocalInfo.reset( newAssetLocalInfo.forget() );
	}

	return ::GetUIDRef( assetLocalInfo );
}



//----------------------------------------------------------------------------------------
// RemoveAssetInfo
//----------------------------------------------------------------------------------------
void
CZPAssetsLocalInfoMgr::RemoveAssetInfo(
	const PMString &			inAssetID)
{
	LogFunctionEnterExit;
	//First check if the asset is alread present or not.
	InterfacePtr<const IZPAssetLocalInfo> assetLocalInfo( this->QueryInfoForAsset( inAssetID, false ));
	if( assetLocalInfo )
	{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
		InterfacePtr<const IWorkspace> iWorkspace(gSession->QueryWorkspace());
		
		//Create a new UID in asset Local info list.
		InterfacePtr<ICommand> addCmd( Utils<IZPCommandFacade>()->CreateCommand_DeleteAssetLocalInfo() );
		ASSERT( addCmd );

		UIDList cmdTargetList( ::GetDataBase( iWorkspace ) );
		cmdTargetList.Append( ::GetUID( iWorkspace ) );
		cmdTargetList.Append( ::GetUID( assetLocalInfo ) );
		addCmd->SetItemList( cmdTargetList );
		
		ErrorCode err = CmdUtils::ProcessCommand( addCmd );

		ASSERT( err == kSuccess );
	}
}

//----------------------------------------------------------------------------------------
// RemoveAssetInfo
//----------------------------------------------------------------------------------------
void
CZPAssetsLocalInfoMgr::RemoveAssetInfo(
	K2Vector<UID> &			inAssetUIDList)
{
	LogFunctionEnterExit;
	if (inAssetUIDList.Length() == 0)
		return;

	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Removing %d assets info", inAssetUIDList.Length());

#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IWorkspace> iWorkspace(gSession->QueryWorkspace());
	
	InterfacePtr<ICommand> removeCmd( Utils<IZPCommandFacade>()->CreateCommand_DeleteAssetLocalInfo() );
	ASSERT( removeCmd );

	UIDList cmdTargetList( ::GetDataBase( iWorkspace ) );
	cmdTargetList.Append( ::GetUID( iWorkspace ) );

	K2Vector<UID>::iterator iterUID = inAssetUIDList.begin();
	K2Vector<UID>::iterator endIterUID = inAssetUIDList.end();
	while( iterUID != endIterUID )
	{
		UID uid = *iterUID;
		cmdTargetList.Append( uid );

		++iterUID;
	}

	removeCmd->SetItemList( cmdTargetList );

	ErrorCode err = CmdUtils::ProcessCommand( removeCmd );

	ASSERT( err == kSuccess );
}

//----------------------------------------------------------------------------------------
// GetUIDListData
//----------------------------------------------------------------------------------------
IZPUIDListData *
CZPAssetsLocalInfoMgr::GetUIDListData() const
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IWorkspace> iWorkspace(gSession->QueryWorkspace());
	ASSERT( iWorkspace );

	InterfacePtr<IZPUIDListData> localInfoUIDList( iWorkspace, IID_IZPASSETLOCALINFOUIDLIST );
	ASSERT( localInfoUIDList );

	return localInfoUIDList;
}

//----------------------------------------------------------------------------------------
// FindInfoForAsset
//----------------------------------------------------------------------------------------
UIDRef
CZPAssetsLocalInfoMgr::FindInfoForAsset(
	const PMString &			inAssetID) const
{
	UIDRef toReturn = UIDRef::gNull;
	IZPUIDListData * localInfoUIDList = this->GetUIDListData();
	if( localInfoUIDList )
	{
		const UIDList * uidList = localInfoUIDList->GetUIDList();
		ASSERT( uidList );
		if( !uidList )
			return toReturn;

		int listLen = uidList->Length();
		for (int32 i = 0 ; i < listLen; ++i)
		{
			InterfacePtr<const IStringData> assetID( uidList->GetRef(i), IID_IZPDATAID );
			ASSERT( assetID );

			if( assetID && assetID->Get().Compare( kFalse, inAssetID ) == 0 )
			{
				toReturn = uidList->GetRef(i);
				break;
			}
		}
	}
	return toReturn;
}

//----------------------------------------------------------------------------------------
// RemoveMissingCacheLocalInfo
//----------------------------------------------------------------------------------------
void
CZPAssetsLocalInfoMgr::RemoveMissingCacheLocalInfo()
{
	LogFunctionEnterExit;
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Removing missing local assets info");
	IZPUIDListData * localInfoUIDList = this->GetUIDListData();
	if( localInfoUIDList )
	{
		const UIDList * uidList = localInfoUIDList->GetUIDList();
		ASSERT( uidList );

		const UIDList tempUidList(*uidList);	//Make copy as actual list get change on removal

		Utils<IZPFileUtils> zpFileUtils;
		IDFile rootPath;
		if (! zpFileUtils->GetLocalStoreFolder( rootPath ) )
			return;

		
		int listLen = tempUidList.Length();
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Current local info object count : %d", listLen);
		for (int32 i = 0 ; i < listLen; ++i)
		{
			InterfacePtr<const IStringData> assetID( tempUidList.GetRef(i), IID_IZPDATAID );
			ASSERT( assetID );

			if (assetID)
			{
				PMString assetIdStr = assetID->Get();
				do
				{
#ifdef CacheWithFolder
					IDFile filePathIcDoc(rootPath);
					zpFileUtils->AppendPath( filePathIcDoc, assetIdStr);
					if (zpFileUtils->DoesFileExist(filePathIcDoc))
						break;
#else
					//Check for file
					//TODO: Currently checking all type of extentions, someday store the type with local info.
					IDFile filePathIdDoc(rootPath);
					const PMString & strExtIdDoc = Utils<IZPAssetUtils>()->GetAssetFileExtenstion( eAssetType_IDDocument );
					zpFileUtils->AppendPath( filePathIdDoc, assetIdStr, &strExtIdDoc );
					if (zpFileUtils->DoesFileExist(filePathIdDoc))
						break;

					IDFile filePathIdTemp(rootPath);
					const PMString & strExtIdTemp = Utils<IZPAssetUtils>()->GetAssetFileExtenstion( eAssetType_IDTemplate );
					zpFileUtils->AppendPath( filePathIdTemp, assetIdStr, &strExtIdTemp );
					if (zpFileUtils->DoesFileExist(filePathIdTemp))
						break;

					IDFile filePathIcTemp(rootPath);
					const PMString & strExtIcTemp = Utils<IZPAssetUtils>()->GetAssetFileExtenstion( eAssetType_ICTemplate );
					zpFileUtils->AppendPath( filePathIcTemp, assetIdStr, &strExtIcTemp );
					if (zpFileUtils->DoesFileExist(filePathIcTemp))
						break;

					IDFile filePathIcAss(rootPath);
					const PMString & strExtIcAss = Utils<IZPAssetUtils>()->GetAssetFileExtenstion( eAssetType_ICAssignment );
					zpFileUtils->AppendPath( filePathIcAss, assetIdStr, &strExtIcAss );
					if (zpFileUtils->DoesFileExist(filePathIcAss))
						break;

					IDFile filePathIcDoc(rootPath);
					const PMString & strExtIcDoc = Utils<IZPAssetUtils>()->GetAssetFileExtenstion( eAssetType_ICDocument );	//same for eAssetType_IDStory
					zpFileUtils->AppendPath( filePathIcDoc, assetIdStr, &strExtIcDoc );
					if (zpFileUtils->DoesFileExist(filePathIcDoc))
						break;
#endif
					IZPLog_Str_( thisFileLA, enLT_DebugInfo, "No file found for asset in cache, assetID : %s", assetIdStr.GrabCString());
					//Remove local info
					RemoveAssetInfo(assetIdStr);

				} while(kFalse);
			}
		}
	}
}

#ifdef CacheWithFolder
//----------------------------------------------------------------------------------------
// ClearCache_OldAssets
// Old means not used for a long time and still unmodified.
//----------------------------------------------------------------------------------------
void
CZPAssetsLocalInfoMgr::ClearCache_OldAssets()
{
	LogFunctionEnterExit;
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Removing all unmodified assets that are older than %d days", kKeepCacheForDays);

	K2Vector<UID> assetLocalInfoUIDList;	//List to remove from persistence

	time_t currentTime = time(nil);
	time_t keepCacheAfterDate = currentTime - (86400 * kKeepCacheForDays);	// 1 day = 86400 sec

#ifdef MACINTOSH
	//On mac the modified time comes from 1904, but current time comes from 1970, so shift the current time to 1904
	LongDateRec lDate;
	memset(&lDate, 0, sizeof(lDate));
	lDate.ld.year = 1970;
	lDate.ld.month = 1;
	lDate.ld.day = 1;
	
	LongDateTime lSecs;
	LongDateToSeconds(&lDate, &lSecs);

	keepCacheAfterDate = keepCacheAfterDate + lSecs;
#endif	

	IZPUIDListData * localInfoUIDList = this->GetUIDListData();
	if( localInfoUIDList )
	{
		const UIDList * uidList = localInfoUIDList->GetUIDList();
		ASSERT( uidList );
		if( !uidList )
			return;

		int listLen = uidList->Length();
		for (int32 i = 0 ; i < listLen; ++i)
		{
			InterfacePtr<const IStringData> localInfoAssetID( uidList->GetRef(i), IID_IZPDATAID );
			ASSERT( localInfoAssetID );

			InterfacePtr<const IZPAssetLocalInfo> assetLocalInfo(localInfoAssetID, UseDefaultIID());
			ASSERT(assetLocalInfo);

			bool16 locallyModified = assetLocalInfo->GetFileIsLocallyModified();
			if( locallyModified != kTrue )
			{	//It is not modified, safe to delete if old
				InterfacePtr<const IDateTimeData> modifiedDateTime(assetLocalInfo, UseDefaultIID());
				ASSERT( modifiedDateTime );
				uint64 modifiedOn = modifiedDateTime->Get();
				if (modifiedOn < keepCacheAfterDate)
					assetLocalInfoUIDList.push_back( ::GetUID( assetLocalInfo ) );
				
				//TODO: remove asset directory here.
			}
		}
	}

	RemoveAssetInfo(assetLocalInfoUIDList);
}
#else
//----------------------------------------------------------------------------------------
// ClearCache_OldAssets
// Old means not used for a long time and still unmodified.
//----------------------------------------------------------------------------------------
void
CZPAssetsLocalInfoMgr::ClearCache_OldAssets()
{
	LogFunctionEnterExit;
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Removing all unmodified assets that are older than %d days", kKeepCacheForDays);
		
	Utils<IZPFileUtils> zpFileUtils;
	IDFile rootPath;
	if (! zpFileUtils->GetLocalStoreFolder( rootPath ) )
		return;

	PlatformFileSystemIterator iter;
	if(!iter.IsDirectory(rootPath))
		return;

	iter.SetStartingPath(rootPath);

	K2Vector<PMString> fileNameList;
	K2Vector<UID> assetLocalInfoUIDList;	//List to remove from persistence

	IDFile tempIdFile;
	PMString filter("*.*");
	filter.InsertW(FileUtils::GetDirectorySeperator(), 0);
	IZPLog_Str( thisFileLA, enLT_DebugInfo, "File filter : %s", filter.GrabCString());

	time_t currentTime = time(nil);
	time_t keepCacheAfterDate = currentTime - (86400 * kKeepCacheForDays);	// 1 day = 86400 sec

#ifdef MACINTOSH
	//On mac the modified time comes from 1904, but current time comes from 1970, so shift the current time to 1904
	LongDateRec lDate;
	memset(&lDate, 0, sizeof(lDate));
	lDate.ld.year = 1970;
	lDate.ld.month = 1;
	lDate.ld.day = 1;
	
	LongDateTime lSecs;
	LongDateToSeconds(&lDate, &lSecs);

	keepCacheAfterDate = keepCacheAfterDate + lSecs;
#endif	
	
	InterfacePtr<const IZPAssetsLocalInfoMgr> assetsLocalInfoMgr( gSession, UseDefaultIID() );
	ASSERT( assetsLocalInfoMgr );

	PMString fileBaseName;
	PMString tempFileName;
	
	bool16 hasNext = iter.FindFirstFile(tempIdFile, filter);

	while(hasNext) {
		tempFileName = tempIdFile.GetFileName();
		if(ValidPath(tempFileName))
		{
			uint32 modDate;
			FileUtils::GetModificationDate(tempIdFile, &modDate);
			if (modDate < keepCacheAfterDate)
			{
				FileUtils::GetBaseFileName(tempFileName, fileBaseName);

				InterfacePtr<const IZPAssetLocalInfo> assetLocalInfo( assetsLocalInfoMgr->QueryInfoForAsset( fileBaseName ) );
				if( !assetLocalInfo )
				{
					IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Local info not found for file :%s", fileBaseName.GrabCString());
					fileNameList.push_back(fileBaseName);	//Local info not found. Add it in remove list
				}
				else
				{
					bool16 locallyModified = assetLocalInfo->GetFileIsLocallyModified();
					if( locallyModified != kTrue )
					{
						IZPLog_Str_( thisFileLA, enLT_DebugInfo, "File is not modified but old: %s", fileBaseName.GrabCString());
						fileNameList.push_back(fileBaseName);		//Add unchanged old file to delete
						assetLocalInfoUIDList.push_back( ::GetUID( assetLocalInfo ) );
					}
					else
					{
						IZPLog_Str_( thisFileLA, enLT_DebugInfo, "File is locally modified: %s", fileBaseName.GrabCString());
					}
				}
			}
			else	//Check for local info
			{
				FileUtils::GetBaseFileName(tempFileName, fileBaseName);

				InterfacePtr<const IZPAssetLocalInfo> assetLocalInfo( assetsLocalInfoMgr->QueryInfoForAsset( fileBaseName ) );
				if( !assetLocalInfo )
				{
					IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Local info not found for file :%s", fileBaseName.GrabCString());
					fileNameList.push_back(fileBaseName);	//Local info not found. Add in remove list
				}
			}
		}

		hasNext= iter.FindNextFile(tempIdFile);
	}

	PMString parentPath =  FileUtils::SysFileToPMString(rootPath);
	
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Deleting files that are not locally modified but old");
#ifdef MACINTOSH
	zpFileUtils->RemoveFiles(parentPath, fileNameList);
#else
	K2Vector<PMString>::iterator iterFile = fileNameList.begin();
	K2Vector<PMString>::iterator endIter = fileNameList.end();
	while( iterFile != endIter )
	{
		const PMString & fileName = *iterFile;
		IZPLog_Str( thisFileLA, enLT_DebugInfo, "File name : %s", fileName.GrabCString());
	
		zpFileUtils->RemoveFiles(parentPath, fileName);

		++iterFile;
	}
#endif

	RemoveAssetInfo(assetLocalInfoUIDList);
}
#endif

//----------------------------------------------------------------------------------------
// RemoveMissingLocalAssetsInfo
//----------------------------------------------------------------------------------------
void
CZPAssetsLocalInfoMgr::RemoveMissingLocalAssetsInfo()
{
#ifdef CacheWithFolder
	LogFunctionEnterExit;

	Utils<IZPFileUtils> zpFileUtils;
	IDFile rootPath;
	if (! zpFileUtils->GetLocalStoreFolder( rootPath ) )
		return;

	PlatformFileSystemIterator iter;
	if(!iter.IsDirectory(rootPath))
		return;

	iter.SetStartingPath(rootPath);

	K2Vector<PMString> directoryNameList;	//List of directory to remove, stored so that iterator does not misbehave

	IDFile tempIdFile;
	PMString filter("*.*");
	filter.InsertW(FileUtils::GetDirectorySeperator(), 0);
	IZPLog_Str( thisFileLA, enLT_DebugInfo, "File filter : %s", filter.GrabCString());

	PMString currDirName;		//It is asset id
	PMString currDirPath;
	
	bool16 hasNext = iter.FindFirstFile(tempIdFile, filter);
	while(hasNext) {
		if(!iter.IsDirectory(tempIdFile))		//Must be a directory
		{
			hasNext= iter.FindNextFile(tempIdFile);
			continue;
		}

		currDirPath = tempIdFile.GetFileName();
		if(ValidPath(currDirPath))
		{
			FileUtils::GetBaseFileName(currDirPath, currDirName);

			InterfacePtr<const IZPAssetLocalInfo> assetLocalInfo( this->QueryInfoForAsset( currDirName, false ) );
			if( !assetLocalInfo )
			{
				IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Local info not found for directory :%s", currDirName.GrabCString());
				directoryNameList.push_back(currDirName);	//Local info not found. Add it in remove list
			}
		}

		hasNext= iter.FindNextFile(tempIdFile);
	}

	PMString parentPath;
	zpFileUtils->ConvertIDFileToStr(rootPath, parentPath);
	IDFile directoryToDelete;
	
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Deleting directories");
	K2Vector<PMString>::iterator iterDirectory = directoryNameList.begin();
	K2Vector<PMString>::iterator endIter = directoryNameList.end();
	while( iterDirectory != endIter )
	{
		const PMString & directoryName = *iterDirectory;
		IZPLog_Str( thisFileLA, enLT_DebugInfo, "Directory name : %s", directoryName.GrabCString());

		zpFileUtils->ConvertStrToIDFile(directoryToDelete, parentPath);
		zpFileUtils->AppendPath(directoryToDelete, directoryName);

		zpFileUtils->DeleteFolder(directoryToDelete);		//Delete the directory

		++iterDirectory;
	}
#else
	//This is already done in ClearCache_OldAssets
#endif
}

//----------------------------------------------------------------------------------------
// ValidPath
//----------------------------------------------------------------------------------------
bool16
CZPAssetsLocalInfoMgr::ValidPath(
	const PMString&				path) const
{
	const PMString thisDir(".");
	const PMString parentDir("..");
	return path != thisDir && path != parentDir;
}
