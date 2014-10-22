//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Utils/CZPFileUtils.cpp $
//	$Revision: 4093 $
//	$Date: 2012-12-19 12:18:48 +0100 (Wed, 19 Dec 2012) $
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
#include "ICoreFilename.h"
#include "IPMStream.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "CoreFileUtils.h"
#endif

//SDK General includes
#include "FileUtils.h"
#include "NameInfo.h"
#include "PlatformFileSystemIterator.h"
#include "StreamUtil.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPFileUtils.h"

//IZP General includes
#include "StInDesignUtils.h"

#if defined(InDnCS5) || defined(InDnCS5_5)
#include <string>
#include <iostream>
#endif

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_Utils
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

const static PlatformChar SDKFileHelper_UNIXDELCHAR =('/');
const static PlatformChar  SDKFileHelper_MACDELCHAR =(':');
const static PlatformChar  SDKFileHelper_WINDELCHAR =('\\');

#define inheritClass CPMUnknown<IZPFileUtils>
class CZPFileUtils : public inheritClass
{
public:
						CZPFileUtils(IPMUnknown * boss);
	virtual				~CZPFileUtils();

	int32				WriteFile(
							const IDFile &				inOutputFile,
							uchar*						inBuffer,
							int32						inBufferLen) const;

	bool				ReadFile(
							const IDFile &				inFileToRead,
							uchar * &					oReadBuffer,
							int		&					oFileLen ) const;

	bool				ReadStream(
							IPMStream *					inReadStream,
							uchar * &					oReadBuffer,	//Caller must delete this
							int		&					oFileLen ) const;

	bool				CreateFolderIfNeeded(
							const IDFile &				inFolderFileSpec,
							bool						inCreateParent = kFalse) const;

	bool				DeleteFolder(
							const IDFile				&inFolder) const;

	IPMStream *			CreateReadStream(				//Caller must release
							const IDFile &				inFileToRead ) const;

	int					GetFileSize(
							const IPMStream *			inReadStream ) const;
	
	bool				GetLocalStoreFolder(
							IDFile &					oFolderPath ) const;

	bool				GetParentDirectoryName(
							const PMString &			inFile,
							PMString &					oParentDirectory ) const;
	bool				GetParentDirectoryName(
							const IDFile &				inFile,
							PMString &					oParentDirectory ) const;

	bool				GetParentDirectoryPath(
							const IDFile &				inFile,
							IDFile&						oParentDirectory) const;

	void				AppendPath(
							IDFile &					ioPath,
							const PMString &			inFilePath,
							const PMString *			inFileExtension) const;

	void				ConvertIDFileToStr(
							const IDFile &				inPath,
							PMString &					oPathStr ) const;

	void				ConvertStrToIDFile(
							IDFile &					oFile,
							const PMString &			inPathStr ) const;

	IDFile *			ConvertNameInfoToIDFile(
							const NameInfo &			inNameInfo,
							IDFile &					oFile ) const;
	NameInfo *			ConvertIDFileToNameInfo(
							const IDFile &				inFile,
							NameInfo &					oNameInfo) const;

	void				GetFileName(
							const IDFile &				inPath,
							PMString &					oFileName,
							bool						inIncludeExtension ) const;
	void				GetFileName(
							const PMString &			inPath,
							PMString &					oFileName,
							bool						inIncludeExtension ) const;

	void				GetExtension(
							const IDFile &				inPath,
							PMString &					oFileExtension) const;

	bool				DoesFileExist(
							const IDFile &				inFilePath ) const;
	
	bool				IsSubPath(
							const IDFile &				inParentFolderPath,
							const IDFile &				inSubPath ) const;
	bool				IsSubPath(
							const PMString &			inParentFolderPath,
							const PMString &			inSubPath ) const;

	void				TouchFile(						//Changes file modification date time
							const IDFile &				inFilePath ) const;
	void				RemoveAllFiles(
							const PMString &			inFolderPath,
							const PMString &			inFileExtension ) const;
	void				RemoveFiles(
							const PMString &			inFolderPath,
							const PMString &			inFileName,
							const PMString &			inFileExtension = kNullString) const;
	void				RemoveFiles(
							const PMString &			inFolderPath,
							const K2Vector<PMString> &	inFilesWithoutExtension ) const;
	bool16				ValidPath(
							const PMString&				path) const;
#if defined(InDnCS5) || defined(InDnCS5_5)
	uint32				GetAssetFileType(
							const IDFile& assetFile) const;
	void				RenameFile(
							IDFile& sourceFile,
							const PMString& ext) const;
#endif
protected:
private:
};


CREATE_PMINTERFACE(CZPFileUtils, kZPFileUtilsImpl)

CZPFileUtils::CZPFileUtils(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPFileUtils::~CZPFileUtils()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// WriteFile
//----------------------------------------------------------------------------------------
int32
CZPFileUtils::WriteFile(
	const IDFile &				inOutputFile,
	uchar*						inBuffer,
	int32						inBufferLen) const
{
	LogFunctionEnterExit;
	
	int32 result = kFailure;
	
	do {
		InterfacePtr<IPMStream> outStream( StreamUtil::CreateFileStreamWriteLazy( inOutputFile, kOpenOut|kOpenTrunc ) );
		ASSERT(outStream);
		
		if ( !outStream )
		{
#if DEBUG
			PMString pathStr;	// debug - show the replica file path
			this->ConvertIDFileToStr( inOutputFile, pathStr );
			IZPLog_Str_( thisFileLA, enLT_Error,"WriteFile (%s) failed.", pathStr.GrabCString());
#endif
			break;
		}
			
		if( outStream->Open() )
		{
			outStream->XferByte(inBuffer, inBufferLen);
			outStream->SetEndOfStream();
	//		stream->Flush();
			outStream->Close();
			result = kSuccess;
		}
		else
		{
#if DEBUG
			PMString pathStr;	// debug - show the replica file path
			this->ConvertIDFileToStr( inOutputFile, pathStr );
			IZPLog_Str_( thisFileLA, enLT_Error,"WriteFile (%s) failed.", pathStr.GrabCString());
#endif
		}
	} while (kFalse);
	
	return (result);
}

//----------------------------------------------------------------------------------------
// ReadFile
//----------------------------------------------------------------------------------------
bool
CZPFileUtils::ReadFile(
	const IDFile &				inFileToRead,
	uchar * &					oReadBuffer,
	int		&					oFileLen) const
{
	LogFunctionEnterExit;
	do
	{
		oReadBuffer = nil; oFileLen = 0;

		InterfacePtr<IPMStream> readStream( this->CreateReadStream( inFileToRead ) );
		ASSERT( readStream );
		
		StStreamCloser			autoCloseStream( readStream );

		return this->ReadStream( readStream, oReadBuffer, oFileLen );
	}while( false );

	return false;
}

//----------------------------------------------------------------------------------------
// ReadStream
//----------------------------------------------------------------------------------------
bool
CZPFileUtils::ReadStream(
	IPMStream *					inReadStream,
	uchar * &					oReadBuffer,	//Caller must delete this
	int		&					oFileLen) const
{

	do
	{
		oReadBuffer = nil; oFileLen = 0;

		int32 fileSize = this->GetFileSize( inReadStream );
		if( fileSize <= 0 )	//No bytes to read.
			break;
		
		uchar * dataBuffer = new uchar[fileSize];
		if( dataBuffer == nil )
			break;

		inReadStream->Seek(0, kSeekFromStart);
		inReadStream->XferByte( dataBuffer, fileSize );
		//Someday: check if read size is same as requested size.

		oReadBuffer = dataBuffer;
		oFileLen = fileSize;
		return true;
	}while( false );
	return false;
}

//----------------------------------------------------------------------------------------
// CreateFolderIfNeeded
//----------------------------------------------------------------------------------------
bool
CZPFileUtils::CreateFolderIfNeeded(
	const IDFile				&inFolderFileSpec,
	bool						inCreateParent) const
{
	return FileUtils::CreateFolderIfNeeded(inFolderFileSpec, inCreateParent);
}

//----------------------------------------------------------------------------------------
// DeleteFolder
//----------------------------------------------------------------------------------------
bool
CZPFileUtils::DeleteFolder(
	const IDFile				&inFolder) const
{
	PMString folderPath;
	this->ConvertIDFileToStr(inFolder, folderPath);
	
	this->RemoveAllFiles(folderPath, "*");		//Remove files to make it empty

	PlatformFileSystemIterator iter;
	if(!iter.IsDirectory(inFolder))
		return true;	//Should be a directory. Assue file as deleted

	iter.SetStartingPath(inFolder);

	K2Vector<PMString> folderFullPathList;
	IDFile tempIdFile;
	PMString filter("*.*");
	filter.InsertW(FileUtils::GetDirectorySeperator(), 0);
	IZPLog_Str( thisFileLA, enLT_DebugInfo, "File filter : %s", filter.GrabCString());

	PMString tempFileName;
	bool16 hasNext = iter.FindFirstFile(tempIdFile, filter);
	while(hasNext) {
		if(!iter.IsDirectory(tempIdFile))		//Must be a directory
		{
			hasNext= iter.FindNextFile(tempIdFile);
			continue;
		}

		tempFileName = tempIdFile.GetFileName();
		if(ValidPath(tempFileName))							//No macintosh code here as no filter is required
			folderFullPathList.push_back(tempFileName);

		hasNext= iter.FindNextFile(tempIdFile);
	}

	IDFile directoryToDelete;

	K2Vector<PMString>::iterator iterFolder = folderFullPathList.begin();
	K2Vector<PMString>::iterator endIter = folderFullPathList.end();
	while( iterFolder != endIter )
	{
		const PMString & folderToDeletePath = *iterFolder;
		IZPLog_Str( thisFileLA, enLT_DebugInfo, "Folder path : %s", folderToDeletePath.GrabCString());
	
		this->ConvertStrToIDFile(directoryToDelete, folderPath);
		this->AppendPath(directoryToDelete, folderToDeletePath, 0);

		if (this->DeleteFolder(directoryToDelete) == false)
			return false;

		++iterFolder;
	}

	return inFolder.Delete();
}

//----------------------------------------------------------------------------------------
// CreateReadStream
//----------------------------------------------------------------------------------------
IPMStream *
CZPFileUtils::CreateReadStream(
	const IDFile &				inFileToRead) const
{
	return StreamUtil::CreateFileStreamReadLazy(inFileToRead, kOpenIn);
}

//----------------------------------------------------------------------------------------
// GetFileSize
//----------------------------------------------------------------------------------------
int
CZPFileUtils::GetFileSize(
	const IPMStream *			inReadStream) const
{
	ASSERT( inReadStream );
	if( !inReadStream )
		return 0;
	IPMStream * theReadStream = const_cast<IPMStream*>(inReadStream);
	int32 currPos = theReadStream->Seek( 0, kSeekFromCurrent );
	int32 fileSize = theReadStream->Seek(0, kSeekFromEnd);
	theReadStream->Seek(currPos, kSeekFromStart);
	return fileSize;
}

//----------------------------------------------------------------------------------------
// GetLocalStoreFolder
//----------------------------------------------------------------------------------------
bool
CZPFileUtils::GetLocalStoreFolder(
	IDFile &					oFolderPath) const
{
	LogFunctionEnterExit;
	
#if defined(InDnCS5) || defined(InDnCS5_5)
	const	PMString	replicaFolderName("iZINE", PMString::kDontTranslateDuringCall);
#else
	const	PMString	replicaFolderName("iZINE", -1, PMString::kNoTranslate);
#endif
	
	FileUtils::GetAppLocalCachedDataFolder( &oFolderPath, replicaFolderName );
	
	FileUtils::CreateFolderIfNeeded( oFolderPath );
	
	return true;
}

//----------------------------------------------------------------------------------------
// GetParentDirectoryName
//----------------------------------------------------------------------------------------
bool
CZPFileUtils::GetParentDirectoryName(
	const IDFile &				inFile,
	PMString &					oParentDirectory) const
{
#if 0	//Commented as it fails on MAC when the path does not exists till last directory
	IDFile parent;
	bool result = FileUtils::GetParentDirectory(inFile, parent);
	if (result)
		FileUtils::GetBaseFileName( parent, oParentDirectory );
	else
	{
		PMString filePath = FileUtils::SysFileToPMString(inFile);
		IZPLog_Str( thisFileLA, enLT_DebugInfo, "Insert File : %s", filePath.GrabCString());
	}
#else
	PMString thePath;
	this->ConvertIDFileToStr( inFile, thePath );
	bool result = this->GetParentDirectoryName( thePath, oParentDirectory );
#endif
	return result;
}

//----------------------------------------------------------------------------------------
// GetParentDirectoryName
//----------------------------------------------------------------------------------------
bool
CZPFileUtils::GetParentDirectoryName(
	const PMString &			inFile,
	PMString &					oParentDirectory) const
{
	oParentDirectory = inFile;
	// takes off the last element from the path. We search for the last delimeter and knock off the
	// stuff that comes after. We lose this data so beware!
	int lastpos = 0;
	for (int i = 0 ; i< inFile.CharCount();i++) {
		if ((inFile[i] == SDKFileHelper_MACDELCHAR) || (inFile[i] == SDKFileHelper_UNIXDELCHAR) || (inFile[i] == SDKFileHelper_WINDELCHAR)) {
			lastpos = i;
		}
	}
	// At this point lastpos should point to the last delimeter, knock off the rest of the string.
	oParentDirectory.Truncate(inFile.CharCount() - lastpos);
	FileUtils::GetBaseFileName( oParentDirectory, oParentDirectory );
	return oParentDirectory.IsEmpty() == kFalse;
}

//----------------------------------------------------------------------------------------
// GetParentDirectoryPath
//----------------------------------------------------------------------------------------
bool
CZPFileUtils::GetParentDirectoryPath(
	const IDFile &				inFile,
	IDFile&						oParentDirectory) const
{
	return FileUtils::GetParentDirectory(inFile, oParentDirectory);
}

//----------------------------------------------------------------------------------------
// AppendPath
//----------------------------------------------------------------------------------------
void
CZPFileUtils::AppendPath(
	IDFile &					ioPath,
	const PMString &			inFilePath,
	const PMString *			inFileExtension) const
{
	//TODO: trim path and extension.
	if( inFileExtension && inFileExtension->IsEmpty() == kFalse )
	{
		PMString fileName;
		fileName += inFilePath;
		fileName += ".";
		fileName += *inFileExtension;
		
		FileUtils::AppendPath( &ioPath, fileName );
	}
	else
	{
		FileUtils::AppendPath( &ioPath, inFilePath );
	}
}

//----------------------------------------------------------------------------------------
// ConvertIDFileToStr
//----------------------------------------------------------------------------------------
void
CZPFileUtils::ConvertIDFileToStr(
	const IDFile &				inPath,
	PMString &					oPathStr) const
{
	FileUtils::IDFileToPMString( inPath, oPathStr );
}

//----------------------------------------------------------------------------------------
// ConvertStrToIDFile
//----------------------------------------------------------------------------------------
void
CZPFileUtils::ConvertStrToIDFile(
	IDFile &					oFile,
	const PMString &			inPathStr) const
{
	FileUtils::PMStringToIDFile( inPathStr, oFile );
}

//----------------------------------------------------------------------------------------
// ConvertNameInfoToIDFile
//----------------------------------------------------------------------------------------
IDFile *
CZPFileUtils::ConvertNameInfoToIDFile(
	const NameInfo &			inNameInfo,
	IDFile &					oFile) const
{
	
	InterfacePtr<ICoreFilename> cfn((ICoreFilename *)::CreateObject(kCoreFilenameBoss, IID_ICOREFILENAME));
	ASSERT(cfn);
	if(cfn)
	{
		cfn->Initialize( const_cast<NameInfo*>(& inNameInfo) );
		if(!cfn->GetSysFile( &oFile))
			return &oFile;
	}
	return nil;
}

//----------------------------------------------------------------------------------------
// ConvertIDFileToNameInfo
//----------------------------------------------------------------------------------------
NameInfo *
CZPFileUtils::ConvertIDFileToNameInfo(
	const IDFile &				inFile,
	NameInfo &					oNameInfo) const
{
	InterfacePtr<ICoreFilename> cfn((ICoreFilename *)::CreateObject(kCoreFilenameBoss, IID_ICOREFILENAME));
	ASSERT(cfn);
	if(cfn)
	{
		cfn->Initialize( &inFile );
		if(!cfn->GetNameInfo( &oNameInfo))
			return &oNameInfo;
	}
	return nil;
}

//----------------------------------------------------------------------------------------
// GetFileName
//----------------------------------------------------------------------------------------
void
CZPFileUtils::GetFileName(
	const IDFile &				inPath,
	PMString &					oFileName,
	bool						inIncludeExtension) const
{
	if( inIncludeExtension )
		FileUtils::GetFileName( inPath, oFileName );
	else
		FileUtils::GetBaseFileName( inPath, oFileName );
}
//----------------------------------------------------------------------------------------
// GetFileName
//----------------------------------------------------------------------------------------
void
CZPFileUtils::GetFileName(
	const PMString &			inPath,
	PMString &					oFileName,
	bool						inIncludeExtension) const
{
	if( inIncludeExtension )
		FileUtils::GetFileName( inPath, oFileName );
	else
		FileUtils::GetBaseFileName( inPath, oFileName );
}

//----------------------------------------------------------------------------------------
// GetExtension
//----------------------------------------------------------------------------------------
void
CZPFileUtils::GetExtension(
	const IDFile &				inPath,
	PMString &					oFileExtension) const
{
	FileUtils::GetExtension( inPath, oFileExtension );
}

//----------------------------------------------------------------------------------------
// DoesFileExist
//----------------------------------------------------------------------------------------
bool
CZPFileUtils::DoesFileExist(
	const IDFile &				inFilePath) const
{
	return FileUtils::DoesFileExist( inFilePath ) == kTrue;
}

//----------------------------------------------------------------------------------------
// IsSubPath
//----------------------------------------------------------------------------------------
bool
CZPFileUtils::IsSubPath(
	const IDFile &				inParentFolderPath,
	const IDFile &				inSubPath) const
{
	PMString parentFolderPathStr;
	PMString subPathStr;
	this->ConvertIDFileToStr( inParentFolderPath, parentFolderPathStr );
	this->ConvertIDFileToStr( inSubPath, subPathStr );
	
	return this->IsSubPath( parentFolderPathStr, subPathStr );
}

//----------------------------------------------------------------------------------------
// IsSubPath
//----------------------------------------------------------------------------------------
bool
CZPFileUtils::IsSubPath(
	const PMString &			inParentFolderPath,
	const PMString &			inSubPath) const
{
	bool toReturn = ( inSubPath.IsEmpty() == kFalse && inSubPath.IndexOfString( inParentFolderPath ) == 0 );
	return toReturn;
}

#include <sys/stat.h>
//----------------------------------------------------------------------------------------
// TouchFile
//	Changes file modification date time
//----------------------------------------------------------------------------------------
void
CZPFileUtils::TouchFile(
	const IDFile &			inFilePath) const
{
	//struct stat filestatus;
	//stat( inFilePath.GrabCString(), &filestatus );
	InterfacePtr<ICoreFilename> cfn((ICoreFilename *)::CreateObject(kCoreFilenameBoss, IID_ICOREFILENAME));
	ASSERT(cfn);
	if(cfn)
	{
		cfn->Initialize( &inFilePath );
		cfn->Touch();
	}
}

//----------------------------------------------------------------------------------------
// RemoveAllFiles
//----------------------------------------------------------------------------------------
void
CZPFileUtils::RemoveAllFiles(
	const PMString &			inFolderPath,
	const PMString &			inFileExtension) const
{
	LogFunctionEnterExit;
	IDFile fileId = FileUtils::PMStringToSysFile(inFolderPath);
	PlatformFileSystemIterator iter;
	if(!iter.IsDirectory(fileId))
		return;

	iter.SetStartingPath(fileId);

	K2Vector<PMString> fileFullPathList;
	IDFile tempIdFile;
	PMString filter("*.");
	filter.Append(inFileExtension);
	filter.InsertW(FileUtils::GetDirectorySeperator(), 0);
	IZPLog_Str( thisFileLA, enLT_DebugInfo, "File filter : %s", filter.GrabCString());

	bool16 hasNext = iter.FindFirstFile(tempIdFile, filter);
	
	const PMString astric("*");

	while(hasNext) {
		//This function is only for deleting files. Directory deletion(fails when not empty) will take extra time to complete this process.
		if(iter.IsDirectory(tempIdFile))
		{
			hasNext= iter.FindNextFile(tempIdFile);
			continue;
		}

		PMString tempFileName = tempIdFile.GetFileName();
		if(ValidPath(tempFileName))
		{
#ifdef MACINTOSH
			PMString tempExtension;
			FileUtils::GetExtension(tempIdFile, tempExtension);
			if (tempExtension != inFileExtension && inFileExtension != astric)
			{
				hasNext= iter.FindNextFile(tempIdFile);
				continue;
			}
#endif

			fileFullPathList.push_back(tempFileName);
		}

		hasNext= iter.FindNextFile(tempIdFile);
	}

	K2Vector<PMString>::iterator iterFile = fileFullPathList.begin();
	K2Vector<PMString>::iterator endIter = fileFullPathList.end();
	while( iterFile != endIter )
	{
		const PMString & filePath = *iterFile;
		IZPLog_Str( thisFileLA, enLT_DebugInfo, "File path : %s", filePath.GrabCString());
	
		InterfacePtr<ICoreFilename> cfn((ICoreFilename *)::CreateObject(kCoreFilenameBoss, IID_ICOREFILENAME));
		ASSERT(cfn);
		if(cfn)
		{
			PMString fileFullPath (inFolderPath);
			fileFullPath.Append(FileUtils::GetDirectorySeperator());
			fileFullPath.Append(filePath);
			cfn->Initialize( &fileFullPath );
			if (cfn->Delete() != 0)
				IZPLog_Str( thisFileLA, enLT_DebugInfo, "Delete failed for file path : %s", fileFullPath.GrabCString());
		}
		++iterFile;
	}
}

//----------------------------------------------------------------------------------------
// RemoveFiles
//----------------------------------------------------------------------------------------
void
CZPFileUtils::RemoveFiles(
	const PMString &			inFolderPath,
	const PMString &			inFileName,
	const PMString &			inFileExtension) const
{
	LogFunctionEnterExit;
	if (inFileExtension != kNullString)
	{
		InterfacePtr<ICoreFilename> cfn((ICoreFilename *)::CreateObject(kCoreFilenameBoss, IID_ICOREFILENAME));
		ASSERT(cfn);
		if(cfn)
		{
			PMString fileFullPath (inFolderPath);
			fileFullPath.Append(FileUtils::GetDirectorySeperator());
			fileFullPath.Append(inFileName);
			fileFullPath.Append(".");
			fileFullPath.Append(inFileExtension);
			cfn->Initialize( &fileFullPath );
			if (cfn->Delete() != 0)
				IZPLog_Str( thisFileLA, enLT_DebugInfo, "Delete failed for file path : %s", fileFullPath.GrabCString());
		}
	}
	else
	{
		IDFile fileId = FileUtils::PMStringToSysFile(inFolderPath);
		PlatformFileSystemIterator iter;
		if(!iter.IsDirectory(fileId))
			return;

		iter.SetStartingPath(fileId);

		K2Vector<PMString> fileFullPathList;
		IDFile tempIdFile;
		PMString filter(inFileName);
		filter.Append(".*");
		filter.InsertW(FileUtils::GetDirectorySeperator(), 0);
		IZPLog_Str( thisFileLA, enLT_DebugInfo, "File filter : %s", filter.GrabCString());

		bool16 hasNext = iter.FindFirstFile(tempIdFile, filter);

		while(hasNext) {
			PMString tempFileName = tempIdFile.GetFileName();
			if(ValidPath(tempFileName))
			{
#ifdef MACINTOSH
				PMString tempBaseFileName;
				FileUtils::GetBaseFileName(tempIdFile, tempBaseFileName);
				if (tempBaseFileName != inFileName)
				{
					hasNext= iter.FindNextFile(tempIdFile);
					continue;
				}
#endif

				fileFullPathList.push_back(tempFileName);
			}

			hasNext= iter.FindNextFile(tempIdFile);
		}

		K2Vector<PMString>::iterator iterFile = fileFullPathList.begin();
		K2Vector<PMString>::iterator endIter = fileFullPathList.end();
		while( iterFile != endIter )
		{
			const PMString & filePath = *iterFile;
			IZPLog_Str( thisFileLA, enLT_DebugInfo, "File path : %s", filePath.GrabCString());
		
			InterfacePtr<ICoreFilename> cfn((ICoreFilename *)::CreateObject(kCoreFilenameBoss, IID_ICOREFILENAME));
			ASSERT(cfn);
			if(cfn)
			{
				PMString fileFullPath (inFolderPath);
				fileFullPath.Append(FileUtils::GetDirectorySeperator());
				fileFullPath.Append(filePath);
				cfn->Initialize( &fileFullPath );
				if (cfn->Delete() != 0)
					IZPLog_Str( thisFileLA, enLT_DebugInfo, "Delete failed for file path : %s", fileFullPath.GrabCString());
			}
			++iterFile;
		}
	}
}

//----------------------------------------------------------------------------------------
// RemoveFiles
//----------------------------------------------------------------------------------------
void
CZPFileUtils::RemoveFiles(
	const PMString &			inFolderPath,
	const K2Vector<PMString> &	inFilesWithoutExtension) const
{
	LogFunctionEnterExit;

	IDFile fileId = FileUtils::PMStringToSysFile(inFolderPath);
	PlatformFileSystemIterator fileIter;
	if(!fileIter.IsDirectory(fileId))
		return;

	fileIter.SetStartingPath(fileId);

	K2Vector<PMString> fileFullNamesToDelete;
	IDFile currIDFile;
	PMString filter;
	filter.Append("*.*");
	filter.InsertW(FileUtils::GetDirectorySeperator(), 0);
	IZPLog_Str( thisFileLA, enLT_DebugInfo, "File filter : %s", filter.GrabCString());

	PMString currFileName;
	PMString currFileBaseFileName;
	K2Vector<PMString>::const_iterator inFileNamesIter;
	K2Vector<PMString>::const_iterator inFileNamesEndIter;
	
	bool16 hasNext = fileIter.FindFirstFile(currIDFile, filter);
	while(hasNext)
	{
		currFileName = currIDFile.GetFileName();
		if(ValidPath(currFileName))
		{
			FileUtils::GetBaseFileName(currIDFile, currFileBaseFileName);
			//Check if this name is in file list to remove.
			inFileNamesIter = inFilesWithoutExtension.begin();
			inFileNamesEndIter = inFilesWithoutExtension.end();
			while( inFileNamesIter != inFileNamesEndIter )
			{
				const PMString&  fileName = *inFileNamesIter;
				if( currFileBaseFileName.Compare( kTrue, fileName ) )
				{
					fileFullNamesToDelete.push_back(currFileName);
					break;
				}
				++inFileNamesIter;
			}
		}
		hasNext= fileIter.FindNextFile(currIDFile);
	}

	K2Vector<PMString>::iterator iterFile = fileFullNamesToDelete.begin();
	K2Vector<PMString>::iterator endIter = fileFullNamesToDelete.end();
	while( iterFile != endIter )
	{
		const PMString & filePath = *iterFile;
		IZPLog_Str( thisFileLA, enLT_DebugInfo, "Deleting file : %s", filePath.GrabCString());
	
		InterfacePtr<ICoreFilename> cfn((ICoreFilename *)::CreateObject(kCoreFilenameBoss, IID_ICOREFILENAME));
		ASSERT(cfn);
		if(cfn)
		{
			PMString fileFullPath (inFolderPath);
			fileFullPath.Append(FileUtils::GetDirectorySeperator());
			fileFullPath.Append(filePath);
			cfn->Initialize( &fileFullPath );
			if (cfn->Delete() != 0)
				IZPLog_Str( thisFileLA, enLT_DebugInfo, "Delete failed for file path : %s", fileFullPath.GrabCString());
		}
		++iterFile;
	}
}

//----------------------------------------------------------------------------------------
// ValidPath
//----------------------------------------------------------------------------------------
bool16
CZPFileUtils::ValidPath(
	const PMString&				path) const
{
	const PMString thisDir(".");
	const PMString parentDir("..");
	return path != thisDir && path != parentDir;
}

//----------------------------------------------------------------------------------------
// Get asset file type (incx, inca, icml, icma, etc.)
//----------------------------------------------------------------------------------------
#if defined(InDnCS5) || defined(InDnCS5_5)
uint32
CZPFileUtils::GetAssetFileType(
	const IDFile&				assetFile) const
{
	uint32 nResult		= 0;
	uchar* readBuffer	= nil;
	int nDataLen		= 0;	
	uint32 nType		= 0;
	int32 nFoundVer			= 0;
	std::string strFoundType("");
	size_t posVer		= std::string::npos, posType = std::string::npos;	
	std::string strLine;
		
	if( this->ReadFile( assetFile, readBuffer, nDataLen ) )
	{		
		std::stringstream sstream;
		sstream << readBuffer;
		while( getline( sstream, strLine ) )
		{
			if( !nFoundVer )
			{
				std::string strFindDOM = "DOMVersion=";
				posVer = strLine.find( strFindDOM );
				if( posVer != std::string::npos )
				{
					posVer += strFindDOM.length() + 1;
					std::string strDomVer = strLine.substr( posVer, ( strLine.find( "\"", posVer ) - posVer) );
					//strDomVer.erase( 0, strFindDOM.length() );
					strDomVer.erase( strDomVer.size(), 1 );
					nFoundVer = atoi( strDomVer.substr( 0, 1 ).c_str() );
				}
			}
			if( !strFoundType.length() )
			{
				std::string strFindType = "type=";
				posType = strLine.find( strFindType );
				if( posType != std::string::npos )
				{
					posType += strFindType.length() + 1;
					std::string strType = strLine.substr( posType, ( strLine.find( "\"", posType ) - posType) );
					//strType.erase( 0, strFindType.length() );
					strType.erase( strType.size(), 1 );

					strFoundType = strType;					
				}
			}
			if( nFoundVer && strFoundType.length() )
				break;
		}

		switch( nFoundVer )
		{
			case 5:	//CS3
			{
				if( !strFoundType.compare("assignment") )
					nResult = kAssignmentFileTypeInfoID;
				else if ( !strFoundType.compare("snippet") )
					nResult = kInCopyXMLInterchangeFileTypeInfoID;
				break;
			}
			case 6:	//CS4			
			case 7:	//CS5
			{
				if( !strFoundType.compare("assignment") )
					nResult = kAssignmentMarkupFileTypeInfoID;
				else if ( !strFoundType.compare("snippet") )
					nResult = kInCopyXMLMarkupFileTypeInfoID;
				break;
			}
			default:
				break;
		};
	}
	
	return nResult;
}

void
CZPFileUtils::RenameFile(
	IDFile& sourceFile,
	const PMString& ext) const
{
	InterfacePtr<ICoreFilename> cfn((ICoreFilename *)::CreateObject(kCoreFilenameBoss, IID_ICOREFILENAME));
	ASSERT(cfn);
	if(cfn)
	{
		cfn->Initialize( &sourceFile );
		if ( cfn->FileExists() )
		{
			PMString fileName;
			this->GetFileName( sourceFile, fileName, false );						
			if( !fileName.Contains( PMString( "." ) ) )
				fileName.Append(".");
			fileName.Append(ext);

			if (fileName.IsEmpty() == false)
			{
				AString aFileName(fileName.GrabCString());
				CoreFileUtils::RenameFile( sourceFile, aFileName);				
			}
		}
	}
}
#endif