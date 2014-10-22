//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/Request/CZPDownloadThumbnailRequest.cpp $
//	$Revision: 1835 $
//	$Date: 2010-07-26 10:35:29 +0200 (Mon, 26 Jul 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 26-7-2010
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

#include "CZPDownloadThumbnailRequest.h"
#include "CZPDownloadThumbnailResponse.h"
#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPDownloadThumbnailRequest::CZPDownloadThumbnailRequest()
: AZPDownloadFileRequest( eReq_DownloadThumbnail )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPDownloadThumbnailRequest::~CZPDownloadThumbnailRequest()
{

}

#pragma mark -
#ifdef Debug_WS
//----------------------------------------------------------------------------------------
// GetName
//----------------------------------------------------------------------------------------
const char *
CZPDownloadThumbnailRequest::GetName() const
{
	return "";
}
#endif

//----------------------------------------------------------------------------------------
// CreateResponseObj
//----------------------------------------------------------------------------------------
AZPSoapResponse *
CZPDownloadThumbnailRequest::CreateResponseObj()
{
	return new CZPDownloadThumbnailResponse;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetAssetID
//----------------------------------------------------------------------------------------
const PMString &
CZPDownloadThumbnailRequest::GetAssetID()const
{
	return mAssetID;
}

//----------------------------------------------------------------------------------------
// SetAssetID
//----------------------------------------------------------------------------------------
void
CZPDownloadThumbnailRequest::SetAssetID(
	const PMString &			inAssetID)
{
	mAssetID = inAssetID;
}
