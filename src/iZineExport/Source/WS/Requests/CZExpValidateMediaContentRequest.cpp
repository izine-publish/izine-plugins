//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/WS/Requests/CZExpValidateMediaContentRequest.cpp $
//	$Revision: 3941 $
//	$Date: 2012-02-24 07:36:48 +0100 (Fri, 24 Feb 2012) $
//	$Author: aman.alam $
//	
//	Creator: Aman Alam
//	Created: 22-2-2012
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

#include "CZExpValidateMediaContentRequest.h"
#include "CZExpValidateMediaContentResponse.h"
#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZExpValidateMediaContentRequest::CZExpValidateMediaContentRequest()
: AZExpWSRequest( eReq_ValidateMediaContent )
, mContent( nil )
, mBuffSize ( 0 )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZExpValidateMediaContentRequest::~CZExpValidateMediaContentRequest()
{
	//This class is not owner of pointer.
}

#pragma mark -
#ifdef Debug_WS
//----------------------------------------------------------------------------------------
// GetName
//----------------------------------------------------------------------------------------
const char *
CZExpValidateMediaContentRequest::GetName() const
{
	return "CZExpValidateMediaContentRequest";
}
#endif

//----------------------------------------------------------------------------------------
// CreateResponseObj
//----------------------------------------------------------------------------------------
AZExpWSResponse *
CZExpValidateMediaContentRequest::CreateResponseObj()
{
	return new CZExpValidateMediaContentResponse;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetTitleId
//----------------------------------------------------------------------------------------
const PMString &
CZExpValidateMediaContentRequest::GetTitleId()const
{
	return mTitleId;
}

//----------------------------------------------------------------------------------------
// SetTitleId
//----------------------------------------------------------------------------------------
void
CZExpValidateMediaContentRequest::SetTitleId(
	const PMString &			inTitleId)
{
	mTitleId = inTitleId;
}

//----------------------------------------------------------------------------------------
// GetEditionId
//----------------------------------------------------------------------------------------
const PMString &
CZExpValidateMediaContentRequest::GetEditionId()const
{
	return mEditionId;
}

//----------------------------------------------------------------------------------------
// SetEditionId
//----------------------------------------------------------------------------------------
void
CZExpValidateMediaContentRequest::SetEditionId(
	const PMString &			inEditionId)
{
	mEditionId = inEditionId;
}

//----------------------------------------------------------------------------------------
// GetArticleId
//----------------------------------------------------------------------------------------
const PMString &
CZExpValidateMediaContentRequest::GetArticleId()const
{
	return mArticleId;
}

//----------------------------------------------------------------------------------------
// SetArticleId
//----------------------------------------------------------------------------------------
void
CZExpValidateMediaContentRequest::SetArticleId(
	const PMString &			inArticleId)
{
	mArticleId = inArticleId;
}

//----------------------------------------------------------------------------------------
// GetDocName
//----------------------------------------------------------------------------------------
const PMString &
CZExpValidateMediaContentRequest::GetDocName()const
{
	return mDocName;
}

//----------------------------------------------------------------------------------------
// SetDocName
//----------------------------------------------------------------------------------------
void
CZExpValidateMediaContentRequest::SetDocName(
	const PMString &			inDocName)
{
	mDocName = inDocName;
}

//----------------------------------------------------------------------------------------
// GetImageFileName
//----------------------------------------------------------------------------------------
const PMString &
CZExpValidateMediaContentRequest::GetImageFileName()const
{
	return mImageFileName;
}

//----------------------------------------------------------------------------------------
// SetImageFileName
//----------------------------------------------------------------------------------------
void
CZExpValidateMediaContentRequest::SetImageFileName(
	const PMString &			inImageFileName)
{
	mImageFileName = inImageFileName;
}

//----------------------------------------------------------------------------------------
// OtherParams
//----------------------------------------------------------------------------------------
const PMString &
CZExpValidateMediaContentRequest::GetOtherParamStr()const
{
	return mOtherParamStr;
}

//----------------------------------------------------------------------------------------
// SetOtherParams
//----------------------------------------------------------------------------------------
void
CZExpValidateMediaContentRequest::SetOtherParamStr(
	const PMString &			inOtherParamStr)
{
	mOtherParamStr = inOtherParamStr;
}

//----------------------------------------------------------------------------------------
// GetFilePath
//----------------------------------------------------------------------------------------
const PMString &
CZExpValidateMediaContentRequest::GetFilePath()const
{
	return mFilePath;
}

//----------------------------------------------------------------------------------------
// SetFilePath
//----------------------------------------------------------------------------------------
void
CZExpValidateMediaContentRequest::SetFilePath(
	const PMString &			inFilePath)
{
	mFilePath = inFilePath;
}

//----------------------------------------------------------------------------------------
// GetContent
//----------------------------------------------------------------------------------------
const uchar *
CZExpValidateMediaContentRequest::GetContent()const
{
	return mContent;
}

//----------------------------------------------------------------------------------------
// SetContent
//----------------------------------------------------------------------------------------
void
CZExpValidateMediaContentRequest::SetContent(
	const uchar *				inContent)
{
	mContent = inContent;
}

//----------------------------------------------------------------------------------------
// GetBuffSize
//----------------------------------------------------------------------------------------
const int
CZExpValidateMediaContentRequest::GetBuffSize()const
{
	return mBuffSize;
}

//----------------------------------------------------------------------------------------
// SetBuffSize
//----------------------------------------------------------------------------------------
void
CZExpValidateMediaContentRequest::SetBuffSize(
	const int					inBuffSize)
{
	mBuffSize = inBuffSize;
}
