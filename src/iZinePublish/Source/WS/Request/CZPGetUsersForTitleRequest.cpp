//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/Request/CZPGetUsersForTitleRequest.cpp $
//	$Revision: 2213 $
//	$Date: 2010-10-01 12:44:26 +0200 (Fri, 01 Oct 2010) $
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

#include "CZPGetUsersForTitleRequest.h"
#include "CZPGetUsersForTitleResponse.h"
#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPGetUsersForTitleRequest::CZPGetUsersForTitleRequest()
: AZPSoapRequest( eReq_GetUsersForTitle )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPGetUsersForTitleRequest::~CZPGetUsersForTitleRequest()
{

}

#pragma mark -
#ifdef Debug_WS
//----------------------------------------------------------------------------------------
// GetName
//----------------------------------------------------------------------------------------
const char *
CZPGetUsersForTitleRequest::GetName() const
{
	return "";
}
#endif

//----------------------------------------------------------------------------------------
// CreateResponseObj
//----------------------------------------------------------------------------------------
AZPSoapResponse *
CZPGetUsersForTitleRequest::CreateResponseObj()
{
	return new CZPGetUsersForTitleResponse;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetTitleID
//----------------------------------------------------------------------------------------
const PMString &
CZPGetUsersForTitleRequest::GetTitleID()const
{
	return mTitleID;
}

//----------------------------------------------------------------------------------------
// SetTitleID
//----------------------------------------------------------------------------------------
void
CZPGetUsersForTitleRequest::SetTitleID(
	const PMString &			inTitleID)
{
	mTitleID = inTitleID;
}
