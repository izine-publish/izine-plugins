//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/Request/CZPGetTaskListRequest.cpp $
//	$Revision: 2237 $
//	$Date: 2010-10-05 16:07:26 +0200 (Tue, 05 Oct 2010) $
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

#include "CZPGetTaskListRequest.h"
#include "CZPGetTaskListResponse.h"
#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPGetTaskListRequest::CZPGetTaskListRequest()
: AZPSoapRequest( eReq_GetTaskList )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPGetTaskListRequest::~CZPGetTaskListRequest()
{

}

#pragma mark -
#ifdef Debug_WS
//----------------------------------------------------------------------------------------
// GetName
//----------------------------------------------------------------------------------------
const char *
CZPGetTaskListRequest::GetName() const
{
	return "";
}
#endif

//----------------------------------------------------------------------------------------
// CreateResponseObj
//----------------------------------------------------------------------------------------
AZPSoapResponse *
CZPGetTaskListRequest::CreateResponseObj()
{
	return new CZPGetTaskListResponse;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetTitleID
//----------------------------------------------------------------------------------------
const PMString &
CZPGetTaskListRequest::GetEditionID()const
{
	return mEditionID;
}

//----------------------------------------------------------------------------------------
// SetTitleID
//----------------------------------------------------------------------------------------
void
CZPGetTaskListRequest::SetEditionID(
	const PMString &			inEditionID)
{
	mEditionID = inEditionID;
}
