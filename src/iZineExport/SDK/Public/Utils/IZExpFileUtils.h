//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/SDK/Public/Utils/IZExpFileUtils.h $
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
#ifndef _h_IZExpFileUtils_
#define _h_IZExpFileUtils_
#pragma once

class IPMStream;

class IZExpFileUtils : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZEXPFILEUTILS };

	virtual	bool		GetFileParDir(
							const IDFile &				inFilePath,
							IDFile &					oFolderPath ) const = 0;

	virtual	PMString	GetFileName(
							const IDFile &				inFile,
							bool						inWithExtension = false ) const = 0;

	virtual	IPMStream *	CreateWriteStreamForImage(
							const IDFile &				inFile ) const = 0;

#pragma mark -
	//Export specific utils

	virtual	void		GetImageExportFileFolders(
							const IDFile &				inXMLFilePath,
							IDFile &					oLowResFilePath,
							IDFile &					oHighResFilePath ) const = 0;

	virtual void		GetTemporaryFolder(
							IDFile &					oFolder) const = 0;

	virtual void		AppendPath(
							IDFile &					ioPath,
							const PMString &			inFilePath,
							const PMString *			inFileExtension = 0 ) const = 0;

	virtual bool		ReadFile(
							const IDFile &				inFileToRead,
							uchar * &					oReadBuffer,	//Caller must delete this
							int		&					oFileLen ) const = 0;
	virtual bool		ReadStream(
							IPMStream *					inReadStream,
							uchar * &					oReadBuffer,	//Caller must delete this
							int		&					oFileLen ) const = 0;

	virtual IPMStream *	CreateReadStream(				//Caller must release
							const IDFile &				inFileToRead ) const = 0;

	virtual int			GetFileSize(
							const IPMStream *			inReadStream ) const = 0;
	virtual void		RemoveAllFiles(
							const PMString &			inFolderPath,
							const PMString &			inFileExtension ) const = 0;
};


#endif //_h_IZExpFileUtils_
