//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/Request/CZPGetTaskStatusListRequest.cpp $
//	$Revision: 2154 $
//	$Date: 2010-09-23 13:53:54 +0200 (Thu, 23 Sep 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 23-9-2010
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

#include "CZPGetTaskStatusListRequest.h"
#include "CZPGetTaskStatusListResponse.h"
#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPGetTaskStatusListRequest::CZPGetTaskStatusListRequest()
: AZPSoapRequest( eReq_GetTaskStatusList )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPGetTaskStatusListRequest::~CZPGetTaskStatusListRequest()
{

}

#pragma mark -
#ifdef Debug_WS
//----------------------------------------------------------------------------------------
// GetName
//----------------------------------------------------------------------------------------
const char *
CZPGetTaskStatusListRequest::GetName() const
{
	return "";
}
#endif

//----------------------------------------------------------------------------------------
// CreateResponseObj
//----------------------------------------------------------------------------------------
AZPSoapResponse *
CZPGetTaskStatusListRequest::CreateResponseObj()
{
	return new CZPGetTaskStatusListResponse;
}

#pragma mark -
