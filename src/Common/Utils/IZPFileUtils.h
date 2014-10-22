//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/Common/Utils/IZPFileUtils.h $
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
#ifndef _h_IZPFileUtils_
#define _h_IZPFileUtils_
#pragma once

class NameInfo;

class IZPFileUtils : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPFILEUTILS };

	virtual int32		WriteFile(
							const IDFile &				inOutputFile,
							uchar*						inBuffer,
							int32						inBufferLen) const = 0;

	virtual bool		ReadFile(
							const IDFile &				inFileToRead,
							uchar * &					oReadBuffer,	//Caller must delete this
							int		&					oFileLen ) const = 0;
	virtual bool		ReadStream(
							IPMStream *					inReadStream,
							uchar * &					oReadBuffer,	//Caller must delete this
							int		&					oFileLen ) const = 0;

	virtual bool		CreateFolderIfNeeded(
							const IDFile				&inFolderFileSpec,
							bool						inCreateParent = kFalse) const = 0;

	virtual bool		DeleteFolder(
							const IDFile				&inFolder) const = 0;

	virtual IPMStream *	CreateReadStream(				//Caller must release
							const IDFile &				inFileToRead ) const = 0;

	virtual int			GetFileSize(
							const IPMStream *			inReadStream ) const = 0;
							
	
	virtual bool		GetLocalStoreFolder(
							IDFile &					oFolderPath ) const = 0;

	virtual bool		GetParentDirectoryName(
							const PMString &			inFile,
							PMString &					oParentDirectory) const = 0;
	virtual bool		GetParentDirectoryName(
							const IDFile &				inFile,
							PMString &					oParentDirectory) const = 0;

	virtual bool		GetParentDirectoryPath(
							const IDFile &				inFile,
							IDFile&						oParentDirectory) const = 0;

	virtual void		AppendPath(
							IDFile &					ioPath,
							const PMString &			inFilePath,
							const PMString *			inFileExtension = 0 ) const = 0;

	virtual void		ConvertIDFileToStr(
							const IDFile &				inPath,
							PMString &					oPathStr ) const = 0;

	virtual void		ConvertStrToIDFile(
							IDFile &					oFile,
							const PMString &			inPathStr ) const = 0;

	virtual IDFile *	ConvertNameInfoToIDFile(
							const NameInfo &			inNameInfo,
							IDFile &					oFile ) const = 0;

	virtual NameInfo *	ConvertIDFileToNameInfo(
							const IDFile &				inFile,
							NameInfo &					oNameInfo) const = 0;

	virtual void		GetFileName(
							const IDFile &				inPath,
							PMString &					oFileName,
							bool						inIncludeExtension = false ) const = 0;
	virtual void		GetFileName(
							const PMString &			inPath,
							PMString &					oFileName,
							bool						inIncludeExtension = false ) const = 0;

	virtual void		GetExtension(
							const IDFile &				inPath,
							PMString &					oFileExtension) const = 0;

	virtual bool		DoesFileExist(
							const IDFile &				inFilePath ) const = 0;

	virtual bool		IsSubPath(
							const IDFile &				inParentFolderPath,
							const IDFile &				inSubPath ) const = 0;
	virtual bool		IsSubPath(
							const PMString &			inParentFolderPath,
							const PMString &			inSubPath ) const = 0;

	virtual void		TouchFile(						//Changes file modification date time
							const IDFile &				inFilePath ) const = 0;
	virtual void		RemoveAllFiles(
							const PMString &			inFolderPath,
							const PMString &			inFileExtension ) const = 0;
	virtual void		RemoveFiles(
							const PMString &			inFolderPath,
							const PMString &			inFileName,
							const PMString &			inFileExtension = kNullString) const = 0;
	virtual void		RemoveFiles(
							const PMString &			inFolderPath,
							const K2Vector<PMString> &	inFilesWithoutExtension ) const = 0;
#if defined(InDnCS5) || defined(InDnCS5_5)
	virtual uint32		GetAssetFileType(
							const IDFile& assetFile) const = 0;
	virtual void		RenameFile(
							IDFile& sourceFile,
							const PMString& ext) const = 0;
#endif
};


#endif //_h_IZPFileUtils_
