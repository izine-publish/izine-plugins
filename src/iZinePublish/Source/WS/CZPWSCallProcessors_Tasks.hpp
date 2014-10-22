//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/CZPWSCallProcessors_Tasks.hpp $
//	$Revision: 2405 $
//	$Date: 2010-10-26 16:41:30 +0200 (Tue, 26 Oct 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 29-9-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:	contains functions related to processing Task related calls,
//					This file must be included in CZPWSCallProcessors.cpp
//========================================================================================
#ifndef _h_CZPWSCallProcessors_Tasks_
#define _h_CZPWSCallProcessors_Tasks_
#pragma once

//IZP General includes
#include "CZPCreateTaskRequest.h"
#include "CZPCreateTaskResponse.h"
#include "CZPDeleteTaskRequest.h"
#include "CZPDeleteTaskResponse.h"
#include "CZPGetTaskListRequest.h"
#include "CZPGetTaskListResponse.h"
#include "CZPGetTaskCategoryListResponse.h"
#include "CZPGetTaskHistoryRequest.h"
#include "CZPGetTaskHistoryResponse.h"
#include "CZPGetTaskStatusListResponse.h"
#include "CZPGetUsersForTitleRequest.h"
#include "CZPGetUsersForTitleResponse.h"
#include "CZPUpdateTaskRequest.h"
#include "CZPUpdateTaskResponse.h"

#pragma mark -
void	ExtactValues_TaskDTO(
			const WSSt_TaskDTO &			inTaskDTO,
			ZPIDPair *						oTaskIDPair,
			ZPTaskInfo &					oTaskInfo );

void	MemsetZero(
			WSSt_TaskDTO &					inTaskDTO );

void	InitStruct_TaskDTO(
			WSSt_TaskDTO &					oTaskDTO,
			const ZPTaskInfo &				inTaskInfo,
			const PMString &				inTitleID,
			const PMString &				inEditionID);


void	DeInitStruct_TaskDTO(
			WSSt_TaskDTO &					ioTaskDTO );
#pragma mark -
//----------------------------------------------------------------------------------------
// DoFetchTaskCategoryList
//----------------------------------------------------------------------------------------
ZPWSError
CZPWSCallProcessors::DoFetchTaskCategoryList(
	const AZPSoapRequest *		inRequest,
	AZPSoapResponse *			oResponse )
{
	DefineAndInitServiceVar(inRequest);

	_ns3__GetTaskCategoryList theReqData;
	_ns3__GetTaskCategoryListResponse theRespData;

	ZPWSError toReturn = _service.__ns1__GetTaskCategoryList( &theReqData, &theRespData);
	CopyErrorDetailIfAny(oResponse);

	if( toReturn == eWSErr_Success )
	{
		CZPGetTaskCategoryListResponse * theResponse = dynamic_cast<CZPGetTaskCategoryListResponse*>(oResponse);
		ASSERT(theResponse);
		
		ZPIDPairSList & statusList = theResponse->GetTaskCategoryList();
		
		WSSt_ArrayOfIdNameDTO *getTaskCategoryListResult = theRespData.GetTaskCategoryListResult;
		statusList.reserve(getTaskCategoryListResult->__sizeIdNameDTO );
		
		for (int i = 0 ; i < getTaskCategoryListResult->__sizeIdNameDTO ; ++i)
		{
			const ns3__IdNameDTO * currVal = getTaskCategoryListResult->IdNameDTO[i];
			
			ZPIDPair * newVal = new ZPIDPair;

			newVal->mID.AppendNumber( currVal->id );
			newVal->mName = WideString(currVal->name);
			
			statusList.push_back( newVal );
		}
	}

	return toReturn;
}

//----------------------------------------------------------------------------------------
// DoFetchTaskStatusList
//----------------------------------------------------------------------------------------
ZPWSError
CZPWSCallProcessors::DoFetchTaskStatusList(
	const AZPSoapRequest *		inRequest,
	AZPSoapResponse *			oResponse )
{
	DefineAndInitServiceVar(inRequest);

	_ns3__GetTaskStatusList theReqData;
	_ns3__GetTaskStatusListResponse theRespData;

	ZPWSError toReturn = _service.__ns1__GetTaskStatusList( &theReqData, &theRespData);
	CopyErrorDetailIfAny(oResponse);

	if( toReturn == eWSErr_Success )
	{
		CZPGetTaskStatusListResponse * theResponse = dynamic_cast<CZPGetTaskStatusListResponse*>(oResponse);
		ASSERT(theResponse);
		
		ZPIDPairSList & statusList = theResponse->GetTaskStatusList();
		
		WSSt_ArrayOfIdNameDTO *getTaskStatusListResult = theRespData.GetTaskStatusListResult;
		statusList.reserve(getTaskStatusListResult->__sizeIdNameDTO );
		
		for (int i = 0 ; i < getTaskStatusListResult->__sizeIdNameDTO ; ++i)
		{
			const ns3__IdNameDTO * currVal = getTaskStatusListResult->IdNameDTO[i];
			
			ZPIDPair * newVal = new ZPIDPair;

			newVal->mID.AppendNumber( currVal->id );
			newVal->mName = WideString(currVal->name);
			
			statusList.push_back( newVal );
		}
	}

	return toReturn;
}

//----------------------------------------------------------------------------------------
// DoFetchTitleUsersList
//----------------------------------------------------------------------------------------
ZPWSError
CZPWSCallProcessors::DoFetchTitleUsersList(
	const AZPSoapRequest *		inRequest,
	AZPSoapResponse *			oResponse )
{
	DefineAndInitServiceVar(inRequest);

	_ns3__GetUsersForTitle theReqData;
	_ns3__GetUsersForTitleResponse theRespData;

	StSmartBuff<wchar_t>	titleid(MaxSize_TitleID);
	theReqData.titleId = titleid.Get();
	
	const CZPGetUsersForTitleRequest * theRequest = dynamic_cast<const CZPGetUsersForTitleRequest*> ( inRequest );
	ASSERT( theRequest );

	theRequest->GetTitleID().GetWChar_tString(theReqData.titleId, MaxSize_TitleID);

	ZPWSError toReturn = _service.__ns1__GetUsersForTitle( &theReqData, &theRespData);
	CopyErrorDetailIfAny(oResponse);

	if( toReturn == eWSErr_Success )
	{
		CZPGetUsersForTitleResponse * theResponse = dynamic_cast<CZPGetUsersForTitleResponse*>(oResponse);
		ASSERT(theResponse);
		
		ZPIDPairSList & usersList = theResponse->GetTitleUsersList();
		
		WSSt_ArrayOfUserDTO *getUsersListResult = theRespData.GetUsersForTitleResult;
		usersList.reserve(getUsersListResult->__sizeUserDTO );
		
		for (int i = 0 ; i < getUsersListResult->__sizeUserDTO ; ++i)
		{
			const WSSt_UserDTO * currVal = getUsersListResult->UserDTO[i];
			
			ZPIDPair * newVal = new ZPIDPair;

			newVal->mID = WideString( currVal->userid );
			newVal->mName = WideString( currVal->name );
			
			usersList.push_back( newVal );
		}
	}

	return toReturn;
}

//----------------------------------------------------------------------------------------
// DoCreateTask
//----------------------------------------------------------------------------------------
ZPWSError
CZPWSCallProcessors::DoCreateTask(
	const AZPSoapRequest *		inRequest,
	AZPSoapResponse *			oResponse )
{
	DefineAndInitServiceVar(inRequest);

	_ns3__CreateTask theReqData;
	_ns3__CreateTaskResponse theRespData;

	const CZPCreateTaskRequest * theRequest = dynamic_cast<const CZPCreateTaskRequest*> ( inRequest );
	ASSERT( theRequest );

	WSSt_TaskDTO taskDTO;
	//::memset( &taskDTO, 0, sizeof( taskDTO ) );
	theReqData.task = &taskDTO;

	MemsetZero( taskDTO );
	InitStruct_TaskDTO( taskDTO, theRequest->GetTaskInfo(), theRequest->GetTitleID(), theRequest->GetEditionID() );

	const PMString & theSubject = theRequest->GetSubject();
	int theSubjectLength = theSubject.ByteLength()+1;
	CreateZeroInitWChar_tArray( taskDTO.subject, theSubjectLength );
	theSubject.GetWChar_tString( taskDTO.subject, theSubjectLength );

	const PMString & theEdition = theRequest->GetEditionID();
	CreateZeroInitWChar_tArray( taskDTO.editionId, MaxSize_EditionID );
	theEdition.GetWChar_tString( taskDTO.editionId, MaxSize_EditionID );


	// Create a new task
	ZPWSError toReturn = _service.__ns1__CreateTask( &theReqData, &theRespData);
	CopyErrorDetailIfAny(oResponse);

	if( toReturn == eWSErr_Success )
	{
		CZPCreateTaskResponse * theResponse = dynamic_cast<CZPCreateTaskResponse*>(oResponse);
		ASSERT(theResponse);
#if 0
		PMString taskID;
		CopyWCharPtrToPMString (taskID, theRespData.CreateTaskResult);
		theResponse->SetTaskID( taskID );
#else
		WSSt_TaskDTO* currTaskDTO = theRespData.CreateTaskResult;
		
		if (currTaskDTO)
		{
			ZPIDPair taskIDInfo;
			ZPTaskInfo & taskInfo = theResponse->GetTaskInfo();

			ExtactValues_TaskDTO( *currTaskDTO, &taskIDInfo, taskInfo );
			theResponse->SetTaskID( taskIDInfo.mID );
			theResponse->SetSubject( taskIDInfo.mName );

			//PMString editionID;
			//CopyWCharPtrToPMString (editionID, currTaskDTO->editionId);
			//theResponse->SetEditionID( editionID );
		}
#endif
	}

	DeInitStruct_TaskDTO( taskDTO );
	return toReturn;
}

//----------------------------------------------------------------------------------------
// DoUpdateTask
//----------------------------------------------------------------------------------------
ZPWSError
CZPWSCallProcessors::DoUpdateTask(
	const AZPSoapRequest *		inRequest,
	AZPSoapResponse *			oResponse )
{
	DefineAndInitServiceVar(inRequest);

	_ns3__UpdateTask theReqData;
	_ns3__UpdateTaskResponse theRespData;

	const CZPUpdateTaskRequest * theRequest = dynamic_cast<const CZPUpdateTaskRequest*> ( inRequest );
	ASSERT( theRequest );

	WSSt_TaskDTO taskDTO;
	//::memset( &taskDTO, 0, sizeof( taskDTO ) );
	theReqData.task = &taskDTO;

	MemsetZero( taskDTO );
	InitStruct_TaskDTO( taskDTO, theRequest->GetTaskInfo(), theRequest->GetTitleID(), theRequest->GetEditionID() );

	const PMString & theSubject = theRequest->GetSubject();
	int theSubjectLength = theSubject.ByteLength()+1;
	CreateZeroInitWChar_tArray( taskDTO.subject, theSubjectLength );
	theSubject.GetWChar_tString( taskDTO.subject, theSubjectLength );

	const PMString & theEdition = theRequest->GetEditionID();
	CreateZeroInitWChar_tArray( taskDTO.editionId, MaxSize_EditionID );
	theEdition.GetWChar_tString( taskDTO.editionId, MaxSize_EditionID );

	//ID
	const PMString & theTaskID = theRequest->GetTaskID();
	CreateZeroInitWChar_tArray( taskDTO.taskid, MaxSize_TaskID );
	theTaskID.GetWChar_tString( taskDTO.taskid, MaxSize_TaskID );

	//Comments
	if (!theRequest->GetComments().IsEmpty()) {
		int theCommentsLength = theRequest->GetComments().ByteLength()+1;
		CreateZeroInitWChar_tArray( theReqData.comment, theCommentsLength );
		theRequest->GetComments().GetWChar_tString(theReqData.comment, theCommentsLength);
	}

	// Update a new task
	ZPWSError toReturn = _service.__ns1__UpdateTask( &theReqData, &theRespData);
	CopyErrorDetailIfAny(oResponse);

	if( toReturn == eWSErr_Success )
	{
		CZPUpdateTaskResponse * theResponse = dynamic_cast<CZPUpdateTaskResponse*>(oResponse);
		ASSERT(theResponse);

		WSSt_TaskDTO* currTaskDTO = theRespData.UpdateTaskResult;
		
		if (currTaskDTO)
		{
			ZPIDPair taskIDInfo;
			ZPTaskInfo & taskInfo = theResponse->GetTaskInfo();

			ExtactValues_TaskDTO( *currTaskDTO, &taskIDInfo, taskInfo );
			theResponse->SetTaskID( taskIDInfo.mID );
			theResponse->SetSubject( taskIDInfo.mName );

			//PMString editionID;
			//CopyWCharPtrToPMString (editionID, currTaskDTO->editionId);
			//theResponse->SetEditionID( editionID );
		}
	}

	DeInitStruct_TaskDTO( taskDTO );
	if( theReqData.comment )
		delete [] theReqData.comment;

	return toReturn;
}

//----------------------------------------------------------------------------------------
// DoDeleteTask
//----------------------------------------------------------------------------------------
ZPWSError
CZPWSCallProcessors::DoDeleteTask(
	const AZPSoapRequest *		inRequest,
	AZPSoapResponse *			oResponse )
{
	DefineAndInitServiceVar(inRequest);

	_ns3__DeleteTask theReqData;
	_ns3__DeleteTaskResponse theRespData;

	const CZPDeleteTaskRequest * theRequest = dynamic_cast<const CZPDeleteTaskRequest*> ( inRequest );
	ASSERT( theRequest );

	StSmartBuff<wchar_t>	taskID(MaxSize_TaskID);
	theReqData.taskId = taskID.Get();

	theRequest->GetTaskID().GetWChar_tString(theReqData.taskId, MaxSize_TaskID);

	// Delete an task.
	ZPWSError toReturn = _service.__ns1__DeleteTask( &theReqData, &theRespData);
	CopyErrorDetailIfAny(oResponse);

	if( toReturn == eWSErr_Success )
	{
		CZPDeleteTaskResponse * theResponse = dynamic_cast<CZPDeleteTaskResponse*>(oResponse);
		ASSERT(theResponse);
	}

	return toReturn;

}

//----------------------------------------------------------------------------------------
// DoFetchEditionTasksList
//----------------------------------------------------------------------------------------
ZPWSError
CZPWSCallProcessors::DoFetchEditionTasksList(
	const AZPSoapRequest *		inRequest,
	AZPSoapResponse *			oResponse )
{
	DefineAndInitServiceVar(inRequest);

	_ns3__GetEditionTaskList theReqData;
	_ns3__GetEditionTaskListResponse theRespData;

	const CZPGetTaskListRequest * theRequest = dynamic_cast<const CZPGetTaskListRequest*> ( inRequest );
	ASSERT( theRequest );

	StSmartBuff<wchar_t>	editionIDBuff(MaxSize_EditionID);
	theReqData.editionId = editionIDBuff.Get();
	theRequest->GetEditionID().GetWChar_tString(theReqData.editionId, MaxSize_EditionID);

	// get tasks for edition
	ZPWSError toReturn = _service.__ns1__GetEditionTaskList( &theReqData, &theRespData);
	CopyErrorDetailIfAny(oResponse);

	if( toReturn == eWSErr_Success )
	{
		CZPGetTaskListResponse * theResponse = dynamic_cast<CZPGetTaskListResponse*>(oResponse);
		ASSERT(theResponse);
	
		ZPIDPairSList &		theTaskList = theResponse->GetTaskIDPairList();
		ZPTaskInfoSList &	theTaskInfoList = theResponse->GetTaskInfoList();

		WSSt_ArrayOfTaskDTO * tasksdto = theRespData.GetEditionTaskListResult;
		
		//TODO: move this to a function
		ZPIDPair * currTaskIDPair = nil;
		ZPTaskInfo * currTaskInfo = nil;

		for (int i=0; i<tasksdto->__sizeTaskDTO; i++)
		{
			WSSt_TaskDTO* task = tasksdto->TaskDTO[i];
			currTaskIDPair = new ZPIDPair;
			currTaskInfo = new ZPTaskInfo;

			if( task )
				ExtactValues_TaskDTO( *task, currTaskIDPair, *currTaskInfo );

			theTaskList.push_back( currTaskIDPair );
			theTaskInfoList.push_back( currTaskInfo );
		}
	}

	return toReturn;
}

//----------------------------------------------------------------------------------------
// DoGetTasksHistory
//----------------------------------------------------------------------------------------
ZPWSError
CZPWSCallProcessors::DoGetTasksHistory(
	const AZPSoapRequest *		inRequest,
	AZPSoapResponse *			oResponse )
{
	DefineAndInitServiceVar(inRequest);

	_ns3__GetTaskHistory theReqData;
	_ns3__GetTaskHistoryResponse theRespData;

	const CZPGetTaskHistoryRequest * theRequest = dynamic_cast<const CZPGetTaskHistoryRequest*> ( inRequest );
	ASSERT( theRequest );

	//ID
	const PMString & theTaskID = theRequest->GetTaskID();
	StSmartBuff<wchar_t>	buffTaskID(MaxSize_TaskID);
	theReqData.taskid = buffTaskID.Get();

	theTaskID.GetWChar_tString( theReqData.taskid, MaxSize_TaskID );

	soap_set_imode( _service.soap, SOAP_ENC_MTOM );	//Required for soap 1.2

	// Get task history
	ZPWSError toReturn = _service.__ns1__GetTaskHistory( &theReqData, &theRespData);
	CopyErrorDetailIfAny(oResponse);

	if( toReturn == eWSErr_Success )
	{
		CZPGetTaskHistoryResponse * theResponse = dynamic_cast<CZPGetTaskHistoryResponse*>(oResponse);
		ASSERT(theResponse);

		ASSERT(theRespData.GetTaskHistoryResult);
		if( theRespData.GetTaskHistoryResult )
		{
			xsd__base64Binary* historyContent = theRespData.GetTaskHistoryResult;
		
			unsigned char * ptrCopy = new unsigned char[historyContent->__size + 1];
			::memcpy( ptrCopy, historyContent->__ptr, historyContent->__size );
			ptrCopy[historyContent->__size] = 0;
			theResponse->SetDataPtr( ptrCopy );
		}
	}
	return toReturn;
}


#pragma mark -
//----------------------------------------------------------------------------------------
// ExtactValues_TaskDTO
//----------------------------------------------------------------------------------------
void
ExtactValues_TaskDTO(
	const WSSt_TaskDTO &			inTaskDTO,
	ZPIDPair *						oTaskIDPair,
	ZPTaskInfo &					oTaskInfo)
{
	CopyWCharPtrToPMString ( oTaskIDPair->mID, inTaskDTO.taskid );
	CopyWCharPtrToPMString ( oTaskIDPair->mName, inTaskDTO.subject );

	CopyWCharPtrToPMString ( oTaskInfo.mDocumentID, inTaskDTO.assetid );
	
	CopyWCharPtrToPMString ( oTaskInfo.mDescription, inTaskDTO.description );
	oTaskInfo.mStatusID.AppendNumber( inTaskDTO.taskStatusId );
	CopyWCharPtrToPMString ( oTaskInfo.mAssignedToUserID, inTaskDTO.assignToUserId );
	if( inTaskDTO.categoryId )
		oTaskInfo.mCategoryID.AppendNumber( *inTaskDTO.categoryId );
	CopyWCharPtrToPMString ( oTaskInfo.mSpread, inTaskDTO.spread );
	CopyWCharPtrToPMString ( oTaskInfo.mPage, inTaskDTO.page );

	oTaskInfo.mCurrVersionNumber = inTaskDTO.versionNumber;
	if( inTaskDTO.TaskUpdateInfo )
	{
		if( inTaskDTO.TaskUpdateInfo->lastModifiedDate )
			oTaskInfo.mLastUpdatedDate = *(inTaskDTO.TaskUpdateInfo->lastModifiedDate);
		CopyWCharPtrToPMString( oTaskInfo.mLastUpdatingUser.mID, inTaskDTO.TaskUpdateInfo->lastModifiedUserId );
		CopyWCharPtrToPMString( oTaskInfo.mLastUpdatingUser.mName, inTaskDTO.TaskUpdateInfo->lastModifiedUserName );
	}
}

//----------------------------------------------------------------------------------------
// MemsetZero
//----------------------------------------------------------------------------------------
void
MemsetZero(
WSSt_TaskDTO &				inTaskDTO)
{
	inTaskDTO.taskid = nil;
	inTaskDTO.assetid = nil;
	inTaskDTO.subject = nil;
	inTaskDTO.description = nil;
	inTaskDTO.taskStatusId = 0;
	inTaskDTO.assignToUserId = nil;
	inTaskDTO.categoryId = nil;
	inTaskDTO.spread = nil;
	inTaskDTO.page = nil;
	inTaskDTO.editionId = nil;
	inTaskDTO.TaskUpdateInfo = nil;
}

void InitStruct_TaskDTO(
		WSSt_TaskDTO &					oTaskDTO,
		const ZPTaskInfo &				inTaskInfo,
		const PMString &				inTitleID,
		const PMString &				inEditionID)
{
	//Description
	if (!inTaskInfo.mDescription.IsEmpty()) {
		int theDescriptionLength = inTaskInfo.mDescription.ByteLength()+1;
		CreateZeroInitWChar_tArray( oTaskDTO.description, theDescriptionLength );
		inTaskInfo.mDescription.GetWChar_tString(oTaskDTO.description, theDescriptionLength);
	}
	
	//DocumentID
	if( !inTaskInfo.mDocumentID.IsEmpty() ) {
		int theDocumentIDLength = inTaskInfo.mDocumentID.ByteLength()+1;
		CreateZeroInitWChar_tArray( oTaskDTO.assetid, theDocumentIDLength );
		inTaskInfo.mDocumentID.GetWChar_tString( oTaskDTO.assetid, theDocumentIDLength );
	}

	//AssignedToUserID
	if( !inTaskInfo.mAssignedToUserID.IsEmpty() ) {
		int theAssignedToUserIDLength = inTaskInfo.mAssignedToUserID.ByteLength()+1;
		CreateZeroInitWChar_tArray( oTaskDTO.assignToUserId, theAssignedToUserIDLength );
		inTaskInfo.mAssignedToUserID.GetWChar_tString( oTaskDTO.assignToUserId, theAssignedToUserIDLength );
	}

	//StatusID
	if( !inTaskInfo.mStatusID.IsEmpty() )
		oTaskDTO.taskStatusId = inTaskInfo.mStatusID.GetAsNumber();
	
	//Category
	if( !inTaskInfo.mCategoryID.IsEmpty() ) {
		CreateZeroInitIntArray( oTaskDTO.categoryId, 1 );
		*oTaskDTO.categoryId = inTaskInfo.mCategoryID.GetAsNumber();
	}

	//Spread
	if( !inTaskInfo.mSpread.IsEmpty() ) {
		int theSpreadLength = inTaskInfo.mSpread.ByteLength()+1;
		CreateZeroInitWChar_tArray( oTaskDTO.spread, theSpreadLength );
		inTaskInfo.mSpread.GetWChar_tString( oTaskDTO.spread, theSpreadLength );
	}

	//Page
	if( !inTaskInfo.mPage.IsEmpty() ) {
		int thePageLength = inTaskInfo.mPage.ByteLength()+1;
		CreateZeroInitWChar_tArray( oTaskDTO.page, thePageLength );
		inTaskInfo.mPage.GetWChar_tString( oTaskDTO.page, thePageLength );
	}

	//Version Number
	oTaskDTO.versionNumber = inTaskInfo.mCurrVersionNumber;
}

//----------------------------------------------------------------------------------------
// DeInitStruct_TaskDTO
//----------------------------------------------------------------------------------------
void DeInitStruct_TaskDTO(
	WSSt_TaskDTO &					ioTaskDTO )
{
	if( ioTaskDTO.taskid )
		delete [] ioTaskDTO.taskid;
	ioTaskDTO.taskid = nil;
	
	if( ioTaskDTO.assetid )
		delete [] ioTaskDTO.assetid;
	ioTaskDTO.assetid = nil;
	
	if( ioTaskDTO.subject )
		delete [] ioTaskDTO.subject;
	ioTaskDTO.subject = nil;
	
	if( ioTaskDTO.description )
		delete [] ioTaskDTO.description;
	ioTaskDTO.description = nil;
	
	//ioTaskDTO.taskStatusId = 0;
	
	if( ioTaskDTO.assignToUserId )
		delete [] ioTaskDTO.assignToUserId;
	ioTaskDTO.assignToUserId = nil;
	
	if( ioTaskDTO.categoryId )
		delete [] ioTaskDTO.categoryId;
	ioTaskDTO.categoryId = nil;
	
	if( ioTaskDTO.spread )
		delete [] ioTaskDTO.spread;
	ioTaskDTO.spread = nil;
	
	if( ioTaskDTO.page )
		delete [] ioTaskDTO.page;
	ioTaskDTO.page = nil;
	
	if( ioTaskDTO.editionId )
		delete [] ioTaskDTO.editionId;
	ioTaskDTO.editionId = nil;
	
	if( ioTaskDTO.TaskUpdateInfo )
		delete ioTaskDTO.TaskUpdateInfo;
	ioTaskDTO.TaskUpdateInfo = nil;
}
#endif //_h_CZPWSCallProcessors_Tasks_
