//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/Response/CZPLoginResponse.cpp $
//	$Revision: 1362 $
//	$Date: 2010-06-08 09:33:11 +0200 (Tue, 08 Jun 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 19-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes

//#include "IZPID.h"

//IZP Interfaces

//IZP General includes
#include "AZPSoapResponse.h"
#include "CZPLoginResponse.h"

#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPLoginResponse::CZPLoginResponse()
:AZPSoapResponse(eResp_Login)
{
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPLoginResponse::~CZPLoginResponse()
{

}
#pragma mark -
//----------------------------------------------------------------------------------------
// GetUserID
//----------------------------------------------------------------------------------------
const PMString &
CZPLoginResponse::GetUserID()const
{
	return mUserID;
}

//----------------------------------------------------------------------------------------
// SetUserID
//----------------------------------------------------------------------------------------
void
CZPLoginResponse::SetUserID(
	const PMString &			inUserID)
{
	mUserID = inUserID;
}

//----------------------------------------------------------------------------------------
// GetDisplayName
//----------------------------------------------------------------------------------------
const PMString &
CZPLoginResponse::GetDisplayName()const
{
	return mDisplayName;
}

//----------------------------------------------------------------------------------------
// SetDisplayName
//----------------------------------------------------------------------------------------
void
CZPLoginResponse::SetDisplayName(
	const PMString &			inDisplayName)
{
	mDisplayName = inDisplayName;
}
