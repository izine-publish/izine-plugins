//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/Request/CZPDownloadVersionRequest.cpp $
//	$Revision: 1835 $
//	$Date: 2010-07-26 10:35:29 +0200 (Mon, 26 Jul 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 26-5-2010
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

#include "CZPDownloadVersionRequest.h"
#include "CZPDownloadVersionResponse.h"
#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPDownloadVersionRequest::CZPDownloadVersionRequest()
: AZPDownloadFileRequest( eReq_DownloadVersion )
{
	mNeedDecompression = false;
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPDownloadVersionRequest::~CZPDownloadVersionRequest()
{

}

#pragma mark -
#ifdef Debug_WS
//----------------------------------------------------------------------------------------
// GetName
//----------------------------------------------------------------------------------------
const char *
CZPDownloadVersionRequest::GetName() const
{
	return "Download Version";
}
#endif

//----------------------------------------------------------------------------------------
// CreateResponseObj
//----------------------------------------------------------------------------------------
AZPSoapResponse *
CZPDownloadVersionRequest::CreateResponseObj()
{
	return new CZPDownloadVersionResponse;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetVersionID
//----------------------------------------------------------------------------------------
const PMString &
CZPDownloadVersionRequest::GetVersionID()const
{
	return mVersionID;
}

//----------------------------------------------------------------------------------------
// SetVersionID
//----------------------------------------------------------------------------------------
void
CZPDownloadVersionRequest::SetVersionID(
	const PMString &			inVersionID)
{
	mVersionID = inVersionID;
}
