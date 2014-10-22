//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/Request/CZPUploadPagePreviewRequest.cpp $
//	$Revision: 1777 $
//	$Date: 2010-07-20 08:29:01 +0200 (Tue, 20 Jul 2010) $
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

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces

//IZP General includes

#include "CZPUploadPagePreviewRequest.h"
#include "CZPUploadPagePreviewResponse.h"
#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPUploadPagePreviewRequest::CZPUploadPagePreviewRequest()
: AZPUploadFileRequest( eReq_UploadPagePreview )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPUploadPagePreviewRequest::~CZPUploadPagePreviewRequest()
{

}

#pragma mark -
#ifdef Debug_WS
//----------------------------------------------------------------------------------------
// GetName
//----------------------------------------------------------------------------------------
const char *
CZPUploadPagePreviewRequest::GetName() const
{
	return "Upload Page Preview";
}
#endif

//----------------------------------------------------------------------------------------
// CreateResponseObj
//----------------------------------------------------------------------------------------
AZPSoapResponse *
CZPUploadPagePreviewRequest::CreateResponseObj()
{
	return new CZPUploadPagePreviewResponse;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetPageID
//----------------------------------------------------------------------------------------
const PMString &
CZPUploadPagePreviewRequest::GetPageID()const
{
	return mPageID;
}

//----------------------------------------------------------------------------------------
// SetPageID
//----------------------------------------------------------------------------------------
void
CZPUploadPagePreviewRequest::SetPageID(
	const PMString &			inPageID)
{
	mPageID = inPageID;
}

//----------------------------------------------------------------------------------------
// GetPageTypeID
//----------------------------------------------------------------------------------------
const PMString &
CZPUploadPagePreviewRequest::GetPageTypeID()const
{
	return mPageTypeID;
}

//----------------------------------------------------------------------------------------
// SetPageTypeID
//----------------------------------------------------------------------------------------
void
CZPUploadPagePreviewRequest::SetPageTypeID(
	const PMString &			inPageTypeID)
{
	mPageTypeID = inPageTypeID;
}

//----------------------------------------------------------------------------------------
// GetPageNumber
//----------------------------------------------------------------------------------------
const int
CZPUploadPagePreviewRequest::GetPageNumber()const
{
	return mPageNumber;
}

//----------------------------------------------------------------------------------------
// SetPageNumber
//----------------------------------------------------------------------------------------
void
CZPUploadPagePreviewRequest::SetPageNumber(
	const int					inPageNumber)
{
	mPageNumber = inPageNumber;
}
