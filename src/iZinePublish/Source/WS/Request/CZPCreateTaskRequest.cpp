//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/Request/CZPCreateTaskRequest.cpp $
//	$Revision: 2314 $
//	$Date: 2010-10-15 11:35:38 +0200 (Fri, 15 Oct 2010) $
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

#include "CZPCreateTaskRequest.h"
#include "CZPCreateTaskResponse.h"
#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPCreateTaskRequest::CZPCreateTaskRequest(
	enRequestType				inReqType)
: AZPSoapRequest( inReqType )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPCreateTaskRequest::~CZPCreateTaskRequest()
{

}

#pragma mark -
#ifdef Debug_WS
//----------------------------------------------------------------------------------------
// GetName
//----------------------------------------------------------------------------------------
const char *
CZPCreateTaskRequest::GetName() const
{
	return "";
}
#endif

//----------------------------------------------------------------------------------------
// CreateResponseObj
//----------------------------------------------------------------------------------------
AZPSoapResponse *
CZPCreateTaskRequest::CreateResponseObj()
{
	return new CZPCreateTaskResponse;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetTaskInfo
//----------------------------------------------------------------------------------------
const ZPTaskInfo &
CZPCreateTaskRequest::GetTaskInfo()const
{
	return mTaskInfo;
}

//----------------------------------------------------------------------------------------
// GetTitleID
//----------------------------------------------------------------------------------------
const PMString &
CZPCreateTaskRequest::GetTitleID()const
{
	return mTitleID;
}

//----------------------------------------------------------------------------------------
// SetTitleID
//----------------------------------------------------------------------------------------
void
CZPCreateTaskRequest::SetTitleID(
	const PMString &			inTitleID)
{
	mTitleID = inTitleID;
}

//----------------------------------------------------------------------------------------
// GetEditionID
//----------------------------------------------------------------------------------------
const PMString &
CZPCreateTaskRequest::GetEditionID()const
{
	return mEditionID;
}

//----------------------------------------------------------------------------------------
// SetEditionID
//----------------------------------------------------------------------------------------
void
CZPCreateTaskRequest::SetEditionID(
	const PMString &			inEditionID)
{
	mEditionID = inEditionID;
}

//----------------------------------------------------------------------------------------
// GetSubject
//----------------------------------------------------------------------------------------
const PMString &
CZPCreateTaskRequest::GetSubject()const
{
	return mSubject;
}

//----------------------------------------------------------------------------------------
// SetSubject
//----------------------------------------------------------------------------------------
void
CZPCreateTaskRequest::SetSubject(
	const PMString &			inSubject)
{
	mSubject = inSubject;
}

//----------------------------------------------------------------------------------------
// GetDescription
//----------------------------------------------------------------------------------------
const PMString &
CZPCreateTaskRequest::GetDescription()const
{
	return mTaskInfo.mDescription;
}

//----------------------------------------------------------------------------------------
// SetDescription
//----------------------------------------------------------------------------------------
void
CZPCreateTaskRequest::SetDescription(
	const PMString &			inDescription)
{
	mTaskInfo.mDescription = inDescription;
}

//----------------------------------------------------------------------------------------
// GetDocumentID
//----------------------------------------------------------------------------------------
const PMString &
CZPCreateTaskRequest::GetDocumentID()const
{
	return mTaskInfo.mDocumentID;
}

//----------------------------------------------------------------------------------------
// SetDocumentID
//----------------------------------------------------------------------------------------
void
CZPCreateTaskRequest::SetDocumentID(
	const PMString &			inDocumentID)
{
	mTaskInfo.mDocumentID = inDocumentID;
}

//----------------------------------------------------------------------------------------
// GetAssignedToUserID
//----------------------------------------------------------------------------------------
const PMString &
CZPCreateTaskRequest::GetAssignedToUserID()const
{
	return mTaskInfo.mAssignedToUserID;
}

//----------------------------------------------------------------------------------------
// SetAssignedToUserID
//----------------------------------------------------------------------------------------
void
CZPCreateTaskRequest::SetAssignedToUserID(
	const PMString &			inAssignedToUserID)
{
	mTaskInfo.mAssignedToUserID = inAssignedToUserID;
}

//----------------------------------------------------------------------------------------
// GetStatusID
//----------------------------------------------------------------------------------------
const PMString &
CZPCreateTaskRequest::GetStatusID()const
{
	return mTaskInfo.mStatusID;
}

//----------------------------------------------------------------------------------------
// SetStatusID
//----------------------------------------------------------------------------------------
void
CZPCreateTaskRequest::SetStatusID(
	const PMString &			inStatusID)
{
	mTaskInfo.mStatusID = inStatusID;
}

//----------------------------------------------------------------------------------------
// GetCategory
//----------------------------------------------------------------------------------------
const PMString &
CZPCreateTaskRequest::GetCategory()const
{
	return mTaskInfo.mCategoryID;
}

//----------------------------------------------------------------------------------------
// SetCategory
//----------------------------------------------------------------------------------------
void
CZPCreateTaskRequest::SetCategory(
	const PMString &			inCategory)
{
	mTaskInfo.mCategoryID = inCategory;
}

//----------------------------------------------------------------------------------------
// GetSpread
//----------------------------------------------------------------------------------------
const PMString &
CZPCreateTaskRequest::GetSpread()const
{
	return mTaskInfo.mSpread;
}

//----------------------------------------------------------------------------------------
// SetSpread
//----------------------------------------------------------------------------------------
void
CZPCreateTaskRequest::SetSpread(
	const PMString &			inSpread)
{
	mTaskInfo.mSpread = inSpread;
}

//----------------------------------------------------------------------------------------
// GetPage
//----------------------------------------------------------------------------------------
const PMString &
CZPCreateTaskRequest::GetPage()const
{
	return mTaskInfo.mPage;
}

//----------------------------------------------------------------------------------------
// SetPage
//----------------------------------------------------------------------------------------
void
CZPCreateTaskRequest::SetPage(
	const PMString &			inPage)
{
	mTaskInfo.mPage = inPage;
}
