//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/Request/CZPLoginRequest.cpp $
//	$Revision: 1027 $
//	$Date: 2010-04-21 07:32:28 +0200 (Wed, 21 Apr 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 16-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes

#include "IZPID.h"

//IZP Interfaces

//IZP General includes
#include "CZPLoginRequest.h"
#include "CZPLoginResponse.h"

#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPLoginRequest::CZPLoginRequest()
: AZPSoapRequest( eReq_Login )
{
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPLoginRequest::~CZPLoginRequest()
{

}

//----------------------------------------------------------------------------------------
// CreateResponseObj
//----------------------------------------------------------------------------------------
AZPSoapResponse *
CZPLoginRequest::CreateResponseObj()
{
	return new CZPLoginResponse;
}


#ifdef Debug_WS
//----------------------------------------------------------------------------------------
// GetName
//----------------------------------------------------------------------------------------
const char *
CZPLoginRequest::GetName()const
{
	return "Login request";
}

//----------------------------------------------------------------------------------------
// PrintToLogs
//----------------------------------------------------------------------------------------
void
CZPLoginRequest::PrintToLogs()const
{
	AZPSoapRequest::PrintToLogs();
}

#endif