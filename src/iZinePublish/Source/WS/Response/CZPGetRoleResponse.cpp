//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/Response/CZPGetRoleResponse.cpp $
//	$Revision: 1045 $
//	$Date: 2010-04-23 13:29:03 +0200 (Fri, 23 Apr 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 22-4-2010
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
#include "CZPGetRoleResponse.h"

#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPGetRoleResponse::CZPGetRoleResponse()
:AZPSoapResponse(eResp_GetRole)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPGetRoleResponse::~CZPGetRoleResponse()
{

}

//----------------------------------------------------------------------------------------
// GetRole
//----------------------------------------------------------------------------------------
const PMString &
CZPGetRoleResponse::GetRole()const
{
	return mRole;
}



//----------------------------------------------------------------------------------------
// SetRole
//----------------------------------------------------------------------------------------
void
CZPGetRoleResponse::SetRole(
	const PMString &				inRole)
{
	mRole = inRole;
}
