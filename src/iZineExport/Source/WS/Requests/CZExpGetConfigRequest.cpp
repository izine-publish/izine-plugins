//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/WS/Requests/CZExpGetConfigRequest.cpp $
//	$Revision: 3677 $
//	$Date: 2011-11-28 11:55:00 +0100 (Mon, 28 Nov 2011) $
//	$Author: aman.alam $
//	
//	Creator: Aman Alam
//	Created: 28-11-2011
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

#include "CZExpGetConfigRequest.h"
#include "CZExpGetConfigResponse.h"
#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZExpGetConfigRequest::CZExpGetConfigRequest()
: AZExpWSRequest( eReq_GetConfig )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZExpGetConfigRequest::~CZExpGetConfigRequest()
{

}

#pragma mark -
#ifdef Debug_WS
//----------------------------------------------------------------------------------------
// GetName
//----------------------------------------------------------------------------------------
const char *
CZExpGetConfigRequest::GetName() const
{
	return "CZExpGetConfigRequest";
}
#endif

//----------------------------------------------------------------------------------------
// CreateResponseObj
//----------------------------------------------------------------------------------------
AZExpWSResponse *
CZExpGetConfigRequest::CreateResponseObj()
{
	return new CZExpGetConfigResponse;
}

#pragma mark -
