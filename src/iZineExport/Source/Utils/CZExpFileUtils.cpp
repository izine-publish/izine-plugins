//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/Utils/CZExpFileUtils.cpp $
//	$Revision: 3644 $
//	$Date: 2011-09-05 14:39:51 +0200 (Mon, 05 Sep 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 23-11-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "ICoreFilename.h"

//SDK General includes
#include "FileUtils.h"
#include "PlatformFileSystemIterator.h"
#include "StreamUtil.h"

//OS / ThirdParty includes

#include "IZExpID.h"

//IZP Interfaces
#include "IZExpFileUtils.h"

//IZP General includes
#include "StInDesignUtils.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_Utils
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZExpFileUtils>
class CZExpFileUtils : public inheritClass
{
public:
						CZExpFileUtils(IPMUnknown * boss);
	virtual				~CZExpFileUtils();

	bool				GetFileParDir(
							const IDFile &				inFilePath,
							IDFile &					oFolderPath ) const;

	PMString			GetFileName(
							const IDFile &				inFile,
							bool						inWithExtension ) const;

	IPMStream *			CreateWriteStreamForImage(
							const IDFile &				inFile ) const;
#pragma mark -
	void				GetImageExportFileFolders(
							const IDFile &				inXMLFilePath,
							IDFile &					oLowResFilePath,
							IDFile &					oHighResFilePath ) const;

	void				GetTemporaryFolder(
							IDFile &					oFolder) const;

	void				AppendPath(
							IDFile &					ioPath,
							const PMString &			inFilePath,
							const PMString *			inFileExtension) const;

	bool				ReadFile(
							const IDFile &				inFileToRead,
							uchar * &					oReadBuffer,
							int		&					oFileLen ) const;

	bool				ReadStream(
							IPMStream *					inReadStream,
							uchar * &					oReadBuffer,	//Caller must delete this
							int		&					oFileLen ) const;

	IPMStream *			CreateReadStream(				//Caller must release
							const IDFile &				inFileToRead ) const;

	int					GetFileSize(
							const IPMStream *			inReadStream ) const;

	void				RemoveAllFiles(
							const PMString &			inFolderPath,
							const PMString &			inFileExtension ) const;
	bool16				ValidPath(
							const PMString&				path) const;
protected:
private:
};


CREATE_PMINTERFACE(CZExpFileUtils, kZExpFileUtilsImpl)

CZExpFileUtils::CZExpFileUtils(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZExpFileUtils::~CZExpFileUtils()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetFileParDir
//----------------------------------------------------------------------------------------
bool
CZExpFileUtils::GetFileParDir(
	const IDFile &				inFilePath,
	IDFile &					oFolderPath) const
{
	return FileUtils::GetParentDirectory( inFilePath, oFolderPath ) == kTrue;
}

//----------------------------------------------------------------------------------------
// GetFileName
//----------------------------------------------------------------------------------------
PMString
CZExpFileUtils::GetFileName(
	const IDFile &				inFile,
	bool						inWithExtension ) const
{
	PMString toReturn;
	if( inWithExtension )
		FileUtils::GetFileName( inFile , toReturn );
	else
		FileUtils::GetBaseFileName( inFile, toReturn );
	return toReturn;
}

//----------------------------------------------------------------------------------------
// CreateWriteStreamForImage
//----------------------------------------------------------------------------------------
IPMStream *
CZExpFileUtils::CreateWriteStreamForImage(
	const IDFile &				inFile ) const
{
	return StreamUtil::CreateFileStreamWrite( inFile, kOpenOut|kOpenTrunc, 'JPEG' );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetImageExportFileFolders
//----------------------------------------------------------------------------------------
void
CZExpFileUtils::GetImageExportFileFolders(
	const IDFile &				inXMLFilePath,
	IDFile &					oLowResFilePath,
	IDFile &					oHighResFilePath) const
{
	IDFile parFolder;
	this->GetFileParDir( inXMLFilePath, parFolder );
	PMString fileName = this->GetFileName( inXMLFilePath, false );
	
	PMString imagesFolderName = fileName;
	imagesFolderName.Append( "_Images" );

	IDFile lowResFile(parFolder);
	FileUtils::AppendPath( &lowResFile, imagesFolderName );
	FileUtils::CreateFolderIfNeeded( lowResFile );

	IDFile highResFile(lowResFile);

	FileUtils::AppendPath( &lowResFile, PMString("LowRes") );
	FileUtils::CreateFolderIfNeeded( lowResFile );
	FileUtils::AppendPath( &highResFile, PMString("HighRes") );
	FileUtils::CreateFolderIfNeeded( highResFile );

	oLowResFilePath = lowResFile;
	oHighResFilePath = highResFile;
}

//----------------------------------------------------------------------------------------
// GetTemporaryFolder
//----------------------------------------------------------------------------------------
void
CZExpFileUtils::GetTemporaryFolder(
	IDFile &					oFolder) const
{
#ifdef InDnCS5
	const PMString temporaryFolderName("iZINE Export", PMString::kDontTranslateDuringCall );
#else
	const PMString temporaryFolderName("iZINE Export", -1, PMString::kNoTranslate);
#endif
	FileUtils::GetAppLocalCachedDataFolder( &oFolder, temporaryFolderName );
	FileUtils::CreateFolderIfNeeded( oFolder );
}

//----------------------------------------------------------------------------------------
// AppendPath
//----------------------------------------------------------------------------------------
void
CZExpFileUtils::AppendPath(
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
// ReadFile
//----------------------------------------------------------------------------------------
bool
CZExpFileUtils::ReadFile(
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
CZExpFileUtils::ReadStream(
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
// CreateReadStream
//----------------------------------------------------------------------------------------
IPMStream *
CZExpFileUtils::CreateReadStream(	//Caller must release
	const IDFile &				inFileToRead) const
{
	return StreamUtil::CreateFileStreamReadLazy(inFileToRead, kOpenIn);
}

//----------------------------------------------------------------------------------------
// GetFileSize
//----------------------------------------------------------------------------------------
int
CZExpFileUtils::GetFileSize(
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
// RemoveAllFiles
//----------------------------------------------------------------------------------------
void
CZExpFileUtils::RemoveAllFiles(
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
// ValidPath
//----------------------------------------------------------------------------------------
bool16
CZExpFileUtils::ValidPath(
	const PMString&				path) const
{
	const PMString thisDir(".");
	const PMString parentDir("..");
	return path != thisDir && path != parentDir;
}
