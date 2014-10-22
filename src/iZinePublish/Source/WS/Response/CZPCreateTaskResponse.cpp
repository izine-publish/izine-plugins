//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/Response/CZPCreateTaskResponse.cpp $
//	$Revision: 2405 $
//	$Date: 2010-10-26 16:41:30 +0200 (Tue, 26 Oct 2010) $
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
#include "CZPCreateTaskResponse.h"

#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPCreateTaskResponse::CZPCreateTaskResponse(
	enResponseType						inRespType )
: AZPSoapResponse( inRespType )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPCreateTaskResponse::~CZPCreateTaskResponse()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetTaskInfo
//----------------------------------------------------------------------------------------
const ZPTaskInfo &
CZPCreateTaskResponse::GetTaskInfo()const
{
	return mTaskInfo;
}

//----------------------------------------------------------------------------------------
// GetTaskInfo
//----------------------------------------------------------------------------------------
ZPTaskInfo &
CZPCreateTaskResponse::GetTaskInfo()
{
	return mTaskInfo;
}

//----------------------------------------------------------------------------------------
// GetSubject
//----------------------------------------------------------------------------------------
const PMString &
CZPCreateTaskResponse::GetSubject()const
{
	return mIDInfo.mName;
}

//----------------------------------------------------------------------------------------
// SetSubject
//----------------------------------------------------------------------------------------
void
CZPCreateTaskResponse::SetSubject(
	const PMString &			inSubject)
{
	mIDInfo.mName = inSubject;
}

//----------------------------------------------------------------------------------------
// GetTaskID
//----------------------------------------------------------------------------------------
const PMString &
CZPCreateTaskResponse::GetTaskID()const
{
	return mIDInfo.mID;
}

//----------------------------------------------------------------------------------------
// SetTaskID
//----------------------------------------------------------------------------------------
void
CZPCreateTaskResponse::SetTaskID(
	const PMString &			inTaskID)
{
	mIDInfo.mID = inTaskID;
}
