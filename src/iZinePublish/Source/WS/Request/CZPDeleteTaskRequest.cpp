//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/Request/CZPDeleteTaskRequest.cpp $
//	$Revision: 2238 $
//	$Date: 2010-10-06 10:40:56 +0200 (Wed, 06 Oct 2010) $
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

#include "CZPDeleteTaskRequest.h"
#include "CZPDeleteTaskResponse.h"
#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPDeleteTaskRequest::CZPDeleteTaskRequest()
: AZPSoapRequest( eReq_DeleteTask )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPDeleteTaskRequest::~CZPDeleteTaskRequest()
{

}

#pragma mark -
#ifdef Debug_WS
//----------------------------------------------------------------------------------------
// GetName
//----------------------------------------------------------------------------------------
const char *
CZPDeleteTaskRequest::GetName() const
{
	return "";
}
#endif

//----------------------------------------------------------------------------------------
// CreateResponseObj
//----------------------------------------------------------------------------------------
AZPSoapResponse *
CZPDeleteTaskRequest::CreateResponseObj()
{
	return new CZPDeleteTaskResponse;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetTaskID
//----------------------------------------------------------------------------------------
const PMString &
CZPDeleteTaskRequest::GetTaskID()const
{
	return mTaskID;
}

//----------------------------------------------------------------------------------------
// SetTaskID
//----------------------------------------------------------------------------------------
void
CZPDeleteTaskRequest::SetTaskID(
	const PMString &			inTaskID)
{
	mTaskID = inTaskID;
}


