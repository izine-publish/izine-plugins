//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/Request/AZPUploadFileRequest.cpp $
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
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPCompressionUtils.h"
#include "IZPFileUtils.h"

//IZP General includes
#include "AZPUploadFileRequest.h"
#include "StUtils.h"

#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
AZPUploadFileRequest::AZPUploadFileRequest(
	enRequestType				inReqType)
: AZPSoapRequest( inReqType )
, mCompressData( false )
, mDataBuff( nil )
, mDataLen( nil )
, mDataBufferOwner( true )
, mDeleteBuffer( true )
{
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
AZPUploadFileRequest::~AZPUploadFileRequest()
{
	if( mDataBuff && mDataBufferOwner)
	{
		if( mDeleteBuffer )
			delete [] mDataBuff;
		else
			free( mDataBuff );
	}
}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetAssetID
//----------------------------------------------------------------------------------------
const PMString &
AZPUploadFileRequest::GetAssetID()const
{
	return mAssetID;
}

//----------------------------------------------------------------------------------------
// SetAssetID
//----------------------------------------------------------------------------------------
void
AZPUploadFileRequest::SetAssetID(
	const PMString &			inAssetID)
{
	mAssetID = inAssetID;
}

//----------------------------------------------------------------------------------------
// GetVersionID
//----------------------------------------------------------------------------------------
const PMString &
AZPUploadFileRequest::GetVersionID()const
{
	return mVersionID;
}

//----------------------------------------------------------------------------------------
// SetVersionID
//----------------------------------------------------------------------------------------
void
AZPUploadFileRequest::SetVersionID(
	const PMString &			inVersionID)
{
	mVersionID = inVersionID;
}

//----------------------------------------------------------------------------------------
// GetCompressData
//----------------------------------------------------------------------------------------
const bool
AZPUploadFileRequest::GetCompressData()const
{
	return mCompressData;
}

//----------------------------------------------------------------------------------------
// SetCompressData
//----------------------------------------------------------------------------------------
void
AZPUploadFileRequest::SetCompressData(
	const bool					inCompressData)
{
	mCompressData = inCompressData;
}

//----------------------------------------------------------------------------------------
// GetDataBuff
//----------------------------------------------------------------------------------------
const uchar *
AZPUploadFileRequest::GetDataBuff(
	bool						inReadFileIfNil )const
{
	if( !mDataBuff && inReadFileIfNil )
	{
		(const_cast<AZPUploadFileRequest*>(this))->ReadFileContents();
	}
	return mDataBuff;
}

//----------------------------------------------------------------------------------------
// SetDataBuff
//----------------------------------------------------------------------------------------
void
AZPUploadFileRequest::SetDataBuff(
	//It will become owner of this pointer
	uchar *						inDataBuff,
	int							inDataLen,
	bool						inOwnsDataBuff,		//owns the data pointer, will delete/free when used.
	bool						inDeleteBuffer)
{
	mDataBuff = inDataBuff;
	mDataLen = inDataLen;
	mDataBufferOwner = inOwnsDataBuff;
	mDeleteBuffer = inDeleteBuffer;
}

//----------------------------------------------------------------------------------------
// GetDataLen
//----------------------------------------------------------------------------------------
int
AZPUploadFileRequest::GetDataLen()const
{
	return mDataLen;
}

//----------------------------------------------------------------------------------------
// SetFile
//----------------------------------------------------------------------------------------
void
AZPUploadFileRequest::SetFile(
	const IDFile &				inFile)
{
	mFilePath = inFile;
}

//----------------------------------------------------------------------------------------
// CopyFileContents
//----------------------------------------------------------------------------------------
void
AZPUploadFileRequest::CopyFileContents(
	const IDFile &					inFile)
{
	this->SetFile( inFile );
	this->ReadFileContents();
}

//----------------------------------------------------------------------------------------
// ReadFileContents
//----------------------------------------------------------------------------------------
void
AZPUploadFileRequest::ReadFileContents()
{
	uchar * dataBuff = nil;
	int dataLen = 0;
	bool fileRead = Utils<IZPFileUtils>()->ReadFile( mFilePath, dataBuff, dataLen );
	if( !fileRead )
		return;
	if( this->GetCompressData() == false )
	{
		this->SetDataBuff( dataBuff, dataLen );
	}
	else
	{
		StPtrDeleter<uchar>			autoDelBuff( dataBuff );
		uchar *		compressedDataBuff = nil;
		unsigned int			compressedDataLen = dataLen;

		Utils<IZPCompressionUtils>()->Compress( dataBuff, dataLen, &compressedDataBuff, &compressedDataLen );

		if( compressedDataBuff && compressedDataLen > 0)
		{
			this->SetDataBuff( compressedDataBuff, compressedDataLen, true, false );
		}
		else
		{	//Data was not compressed. Use the original data, and set compression to false.
			autoDelBuff.Forget();
			this->SetDataBuff( dataBuff, dataLen );
			this->SetCompressData( false );
		}
	}

}
