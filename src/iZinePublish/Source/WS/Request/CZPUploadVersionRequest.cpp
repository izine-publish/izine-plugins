//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/Request/CZPUploadVersionRequest.cpp $
//	$Revision: 1777 $
//	$Date: 2010-07-20 08:29:01 +0200 (Tue, 20 Jul 2010) $
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

#include "CZPUploadVersionRequest.h"
#include "CZPUploadVersionResponse.h"
#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPUploadVersionRequest::CZPUploadVersionRequest()
: AZPUploadFileRequest( eReq_UploadVersion )
{
	this->SetCompressData( true );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPUploadVersionRequest::~CZPUploadVersionRequest()
{
}

#pragma mark -
#ifdef Debug_WS
//----------------------------------------------------------------------------------------
// GetName
//----------------------------------------------------------------------------------------
const char *
CZPUploadVersionRequest::GetName() const
{
	return "Upload Version";
}
#endif

//----------------------------------------------------------------------------------------
// CreateResponseObj
//----------------------------------------------------------------------------------------
AZPSoapResponse *
CZPUploadVersionRequest::CreateResponseObj()
{
	return new CZPUploadVersionResponse;
}

#pragma mark -
