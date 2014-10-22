//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Requests/AZPUploadFileRequest.h $
//	$Revision: 1413 $
//	$Date: 2010-06-11 08:44:29 +0200 (Fri, 11 Jun 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 10-6-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_AZPUploadFileRequest_
#define _h_AZPUploadFileRequest_
#pragma once

#include "AZPSoapRequest.h"

class AZPUploadFileRequest : public AZPSoapRequest
{
public:
	virtual				~AZPUploadFileRequest();

	const PMString &	GetAssetID() const;
	void				SetAssetID(
							const PMString &			inAssetID );

	const PMString &	GetVersionID() const;
	void				SetVersionID(
							const PMString &			inVersionID );

	const bool			GetCompressData() const;
	void				SetCompressData(				//Must be called before setting data buffer.
							const bool					inCompressData );

	const uchar *		GetDataBuff(
							bool						inReadFileIfNil = true ) const;
	int					GetDataLen() const;
	void				SetDataBuff(	//It will become owner of this pointer
							uchar *						inDataBuff,
							int							inDataLen,
							bool						inOwnsDataBuff = true,		//owns the data pointer, will delete/free when used.
							bool						inDeleteBuffer = true);	//false if malloc used.

	void				SetFile(						//Use this where file can be read later on.
							const IDFile &				inFile );
	void				CopyFileContents(				//Use this where file can be read only during its call.
							const IDFile &				inFile );
	//Read the file set with it and store its contents.
	//Compression flag must be set before it's call, as it will be used by this function.
	void				ReadFileContents();


protected:
						AZPUploadFileRequest(
							enRequestType				inReqType);
protected:
	PMString		mAssetID;
	PMString		mVersionID;
	bool			mCompressData;

	IDFile			mFilePath;	//File path will be used if data buff is nil.
	uchar *			mDataBuff;	//TODO: Should we read complete file in memory, or create a temp file, and read while sending.
	int				mDataLen;
	bool			mDataBufferOwner;	//this object is owner of data buffer pointer.
	bool			mDeleteBuffer;	//use free if false.
private:
};

#endif //_h_AZPUploadFileRequest_
