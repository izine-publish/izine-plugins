//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/WS/Requests/CZExpUploadExportRequest.cpp $
//	$Revision: 3738 $
//	$Date: 2012-01-16 15:23:41 +0100 (Mon, 16 Jan 2012) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 14-6-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes

//OS / ThirdParty includes

#include "IZExpID.h"

//IZP Interfaces

//IZP General includes

#include "CZExpUploadExportRequest.h"
#include "CZExpUploadExportResponse.h"
#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZExpUploadExportRequest::CZExpUploadExportRequest()
: AZExpWSRequest( eReq_UploadExport )
, mContent( nil )
, mBuffSize ( 0 )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZExpUploadExportRequest::~CZExpUploadExportRequest()
{
	//This class is not owner of pointer.
}

#pragma mark -
#ifdef Debug_WS
//----------------------------------------------------------------------------------------
// GetName
//----------------------------------------------------------------------------------------
const char *
CZExpUploadExportRequest::GetName() const
{
	return "CZExpUploadExportRequest";
}
#endif

//----------------------------------------------------------------------------------------
// CreateResponseObj
//----------------------------------------------------------------------------------------
AZExpWSResponse *
CZExpUploadExportRequest::CreateResponseObj()
{
	return new CZExpUploadExportResponse;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetTitleId
//----------------------------------------------------------------------------------------
const PMString &
CZExpUploadExportRequest::GetTitleId()const
{
	return mTitleId;
}

//----------------------------------------------------------------------------------------
// SetTitleId
//----------------------------------------------------------------------------------------
void
CZExpUploadExportRequest::SetTitleId(
	const PMString &			inTitleId)
{
	mTitleId = inTitleId;
}

//----------------------------------------------------------------------------------------
// GetEditionId
//----------------------------------------------------------------------------------------
const PMString &
CZExpUploadExportRequest::GetEditionId()const
{
	return mEditionId;
}

//----------------------------------------------------------------------------------------
// SetEditionId
//----------------------------------------------------------------------------------------
void
CZExpUploadExportRequest::SetEditionId(
	const PMString &			inEditionId)
{
	mEditionId = inEditionId;
}

//----------------------------------------------------------------------------------------
// GetArticleId
//----------------------------------------------------------------------------------------
const PMString &
CZExpUploadExportRequest::GetArticleId()const
{
	return mArticleId;
}

//----------------------------------------------------------------------------------------
// SetArticleId
//----------------------------------------------------------------------------------------
void
CZExpUploadExportRequest::SetArticleId(
	const PMString &			inArticleId)
{
	mArticleId = inArticleId;
}

//----------------------------------------------------------------------------------------
// GetDocName
//----------------------------------------------------------------------------------------
const PMString &
CZExpUploadExportRequest::GetDocName()const
{
	return mDocName;
}

//----------------------------------------------------------------------------------------
// SetDocName
//----------------------------------------------------------------------------------------
void
CZExpUploadExportRequest::SetDocName(
	const PMString &			inDocName)
{
	mDocName = inDocName;
}

//----------------------------------------------------------------------------------------
// GetImageFileName
//----------------------------------------------------------------------------------------
const PMString &
CZExpUploadExportRequest::GetImageFileName()const
{
	return mImageFileName;
}

//----------------------------------------------------------------------------------------
// SetImageFileName
//----------------------------------------------------------------------------------------
void
CZExpUploadExportRequest::SetImageFileName(
	const PMString &			inImageFileName)
{
	mImageFileName = inImageFileName;
}

//----------------------------------------------------------------------------------------
// OtherParams
//----------------------------------------------------------------------------------------
const PMString &
CZExpUploadExportRequest::GetOtherParamStr()const
{
	return mOtherParamStr;
}

//----------------------------------------------------------------------------------------
// SetOtherParams
//----------------------------------------------------------------------------------------
void
CZExpUploadExportRequest::SetOtherParamStr(
	const PMString &			inOtherParamStr)
{
	mOtherParamStr = inOtherParamStr;
}

//----------------------------------------------------------------------------------------
// GetFilePath
//----------------------------------------------------------------------------------------
const PMString &
CZExpUploadExportRequest::GetFilePath()const
{
	return mFilePath;
}

//----------------------------------------------------------------------------------------
// SetFilePath
//----------------------------------------------------------------------------------------
void
CZExpUploadExportRequest::SetFilePath(
	const PMString &			inFilePath)
{
	mFilePath = inFilePath;
}

//----------------------------------------------------------------------------------------
// GetContent
//----------------------------------------------------------------------------------------
const uchar *
CZExpUploadExportRequest::GetContent()const
{
	return mContent;
}

//----------------------------------------------------------------------------------------
// SetContent
//----------------------------------------------------------------------------------------
void
CZExpUploadExportRequest::SetContent(
	const uchar *				inContent)
{
	mContent = inContent;
}

//----------------------------------------------------------------------------------------
// GetBuffSize
//----------------------------------------------------------------------------------------
const int
CZExpUploadExportRequest::GetBuffSize()const
{
	return mBuffSize;
}

//----------------------------------------------------------------------------------------
// SetBuffSize
//----------------------------------------------------------------------------------------
void
CZExpUploadExportRequest::SetBuffSize(
	const int					inBuffSize)
{
	mBuffSize = inBuffSize;
}
