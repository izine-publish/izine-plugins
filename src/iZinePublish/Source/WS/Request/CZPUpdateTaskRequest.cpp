//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/Request/CZPUpdateTaskRequest.cpp $
//	$Revision: 2318 $
//	$Date: 2010-10-15 13:35:17 +0200 (Fri, 15 Oct 2010) $
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

#include "CZPUpdateTaskRequest.h"
#include "CZPUpdateTaskResponse.h"
#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPUpdateTaskRequest::CZPUpdateTaskRequest()
: CZPCreateTaskRequest( eReq_UpdateTask )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPUpdateTaskRequest::~CZPUpdateTaskRequest()
{

}

#pragma mark -
#ifdef Debug_WS
//----------------------------------------------------------------------------------------
// GetName
//----------------------------------------------------------------------------------------
const char *
CZPUpdateTaskRequest::GetName() const
{
	return "";
}
#endif

//----------------------------------------------------------------------------------------
// CreateResponseObj
//----------------------------------------------------------------------------------------
AZPSoapResponse *
CZPUpdateTaskRequest::CreateResponseObj()
{
	return new CZPUpdateTaskResponse;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetTaskID
//----------------------------------------------------------------------------------------
const PMString &
CZPUpdateTaskRequest::GetTaskID()const
{
	return mTaskID;
}

//----------------------------------------------------------------------------------------
// SetTaskID
//----------------------------------------------------------------------------------------
void
CZPUpdateTaskRequest::SetTaskID(
	const PMString &			inTaskID)
{
	mTaskID = inTaskID;
}

//----------------------------------------------------------------------------------------
// GetComments
//----------------------------------------------------------------------------------------
const PMString &
CZPUpdateTaskRequest::GetComments()const
{
	return mComments;
}

//----------------------------------------------------------------------------------------
// SetComments
//----------------------------------------------------------------------------------------
void
CZPUpdateTaskRequest::SetComments(
	const PMString &			inComments)
{
	mComments = inComments;
}

//----------------------------------------------------------------------------------------
// SetTaskOldVersion
//----------------------------------------------------------------------------------------
void
CZPUpdateTaskRequest::SetTaskOldVersion(
	int							inOldVersion)
{
	mTaskInfo.mCurrVersionNumber = inOldVersion;
}


