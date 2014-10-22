//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/Request/CZPGetTaskHistoryRequest.cpp $
//	$Revision: 2344 $
//	$Date: 2010-10-18 15:43:44 +0200 (Mon, 18 Oct 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 18-10-2010
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

#include "CZPGetTaskHistoryRequest.h"
#include "CZPGetTaskHistoryResponse.h"
#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPGetTaskHistoryRequest::CZPGetTaskHistoryRequest()
: AZPSoapRequest( eReq_GetTaskHistory )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPGetTaskHistoryRequest::~CZPGetTaskHistoryRequest()
{

}

#pragma mark -
#ifdef Debug_WS
//----------------------------------------------------------------------------------------
// GetName
//----------------------------------------------------------------------------------------
const char *
CZPGetTaskHistoryRequest::GetName() const
{
	return "";
}
#endif

//----------------------------------------------------------------------------------------
// CreateResponseObj
//----------------------------------------------------------------------------------------
AZPSoapResponse *
CZPGetTaskHistoryRequest::CreateResponseObj()
{
	return new CZPGetTaskHistoryResponse;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetTaskID
//----------------------------------------------------------------------------------------
const PMString &
CZPGetTaskHistoryRequest::GetTaskID()const
{
	return mTaskID;
}

//----------------------------------------------------------------------------------------
// SetTaskID
//----------------------------------------------------------------------------------------
void
CZPGetTaskHistoryRequest::SetTaskID(
	const PMString &			inTaskID)
{
	mTaskID = inTaskID;
}