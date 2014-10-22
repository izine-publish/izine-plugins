//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/Utils/IZPWSUtils.h $
//	$Revision: 2382 $
//	$Date: 2010-10-26 08:29:19 +0200 (Tue, 26 Oct 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 29-9-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPWSUtils_
#define _h_IZPWSUtils_
#pragma once

class AZPSoapRequest;

class IZPWSUtils : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPWSUTILS };

#pragma mark -
// sync updater functions.
	virtual ErrorCode			CreateNewTask(
							PMString &					oTaskID,
							const PMString &			inTitleID,
							const PMString &			inEditionID,
							const PMString &			inSubject,
							const PMString &			inDescription,
							const PMString &			inDocumentID,
							const PMString &			inAssignedToUserID,
							const PMString &			inStatusID,
							const PMString &			inCategory,
							const PMString &			inSpread,
							const PMString &			inPage ) const = 0;
#pragma mark -
// async updater functions.
	virtual void				UpdateTask(
							const PMString &			inTaskID,
							const PMString &			inTitleID,
							const PMString &			inEditionID,
							const PMString &			inSubject,
							const PMString &			inDescription,
							const PMString &			inDocumentID,
							const PMString &			inAssignedToUserID,
							const PMString &			inStatusID,
							const PMString &			inCategory,
							const PMString &			inSpread,
							const PMString &			inPage,
							int							inTaskOldVersion,
							const PMString &			inComments ) const = 0;

	virtual void				DeleteTask(
							const PMString &			inTaskID ) const = 0;
#pragma mark -
// sync Getter functions, no changes made on WS state.

#pragma mark -
// async Getter functions, no changes made on WS state.
	virtual void				FetchTaskCategoryList_async() const = 0;
	virtual void				FetchTaskStatusList_async() const = 0;
	virtual void				FetchTaskHistory_async(
							const PMString &			inTaskID ) const = 0;
	virtual void				FetchTitleUsersList_async(
							const PMString &			inTitleID ) const = 0;
	virtual void				FetchEditionTaskList_async(
							const PMString &			inEditionID ) const = 0;
#pragma mark -
//Create requests - WS state mofiying
	virtual AZPSoapRequest *	CreateCreateTaskRequest(
							const PMString &			inTitleID,
							const PMString &			inEditionID,
							const PMString &			inSubject,
							const PMString &			inDescription,
							const PMString &			inDocumentID,
							const PMString &			inAssignedToUserID,
							const PMString &			inStatusID,
							const PMString &			inCategory,
							const PMString &			inSpread,
							const PMString &			inPage ) const = 0;
	virtual AZPSoapRequest *	CreateUpdateTaskRequest(
							const PMString &			inTaskID,
							const PMString &			inTitleID,
							const PMString &			inEditionID,
							const PMString &			inSubject,
							const PMString &			inDescription,
							const PMString &			inDocumentID,
							const PMString &			inAssignedToUserID,
							const PMString &			inStatusID,
							const PMString &			inCategory,
							const PMString &			inSpread,
							const PMString &			inPage,
							int							inTaskOldVersion,
							const PMString &			inComments ) const = 0;
	virtual AZPSoapRequest *	CreateDeleteTaskRequest(
							const PMString &			inTaskID ) const = 0;
#pragma mark -
//Create requests - WS state not mofiying
	virtual AZPSoapRequest *	CreateFetchTaskCategoryListRequest( ) const = 0;
	virtual AZPSoapRequest *	CreateFetchTaskStatusListRequest( ) const = 0;
	virtual AZPSoapRequest *	CreateFetchTaskHistoryRequest(
							const PMString &			inTaskID ) const = 0;
	virtual AZPSoapRequest *	CreateFetchTitleUsersRequest(
							const PMString &			inTitleID ) const = 0;
	virtual AZPSoapRequest *	CreateFetchTasksListRequest(
							const PMString &			inEditionID ) const = 0;
};


#endif //_h_IZPWSUtils_
