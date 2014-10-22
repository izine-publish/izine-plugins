//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/Request/CZPUploadExportRequest.cpp $
//	$Revision: 3335 $
//	$Date: 2011-06-22 10:43:51 +0200 (Wed, 22 Jun 2011) $
//	$Author: aman.alam $
//	
//	Creator: Aman Alam
//	Created: 22-6-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
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

#include "CZPUploadExportRequest.h"
#include "CZPUploadExportResponse.h"
#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPUploadExportRequest::CZPUploadExportRequest()
: AZPUploadFileRequest( eReq_UploadExport )
, mIsXML ( true )
, mIsHighRes ( true )
{
	this->SetCompressData( true );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPUploadExportRequest::~CZPUploadExportRequest()
{
}

#pragma mark -
#ifdef Debug_WS
//----------------------------------------------------------------------------------------
// GetName
//----------------------------------------------------------------------------------------
const char *
CZPUploadExportRequest::GetName() const
{
	return "Upload Export";
}
#endif

//----------------------------------------------------------------------------------------
// CreateResponseObj
//----------------------------------------------------------------------------------------
AZPSoapResponse *
CZPUploadExportRequest::CreateResponseObj()
{
	return new CZPUploadExportResponse;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetImageFileName
//----------------------------------------------------------------------------------------
const PMString &
CZPUploadExportRequest::GetImageFileName()const
{
	return mImageFileName;
}

//----------------------------------------------------------------------------------------
// SetImageFileName
//----------------------------------------------------------------------------------------
void
CZPUploadExportRequest::SetImageFileName(
	const PMString &			inImageFileName)
{
	mImageFileName = inImageFileName;
}

//----------------------------------------------------------------------------------------
// GetIsXML
//----------------------------------------------------------------------------------------
const bool
CZPUploadExportRequest::GetIsXML()const
{
	return mIsXML;
}

//----------------------------------------------------------------------------------------
// SetIsXML
//----------------------------------------------------------------------------------------
void
CZPUploadExportRequest::SetIsXML(
	const bool					inIsXML)
{
	mIsXML = inIsXML;
}

//----------------------------------------------------------------------------------------
// GetIsHighRes
//----------------------------------------------------------------------------------------
const bool
CZPUploadExportRequest::GetIsHighRes()const
{
	return mIsHighRes;
}

//----------------------------------------------------------------------------------------
// SetIsHighRes
//----------------------------------------------------------------------------------------
void
CZPUploadExportRequest::SetIsHighRes(
	const bool					inIsHighRes)
{
	mIsHighRes = inIsHighRes;
}
