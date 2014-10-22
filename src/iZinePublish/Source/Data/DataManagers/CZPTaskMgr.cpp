//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Data/DataManagers/CZPTaskMgr.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 23-9-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IIntData.h"
#include "IStringData.h"
#include "ISubject.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPApp.h"
#include "IZPEditionMgr.h"
#include "IZPSoapRequest.h"
#include "IZPTaskInfo.h"
#include "IZPTaskList.h"
#include "IZPTaskMgr.h"
#include "IZPWSController.h"

//IZP General includes
#include "CZPDeleteTaskRequest.h"
#include "CZPGetTaskListRequest.h"
#include "CZPGetTaskListResponse.h"
#include "CZPUpdateTaskRequest.h"
#include "CZPUpdateTaskResponse.h"
#include "ZPDataHelper.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_DBObjects
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZPTaskMgr>
class CZPTaskMgr : public inheritClass
{
public:
						CZPTaskMgr(IPMUnknown * boss);
	virtual				~CZPTaskMgr();

	void				Shutdown();

	void				UpdateTaskList(
							const IStringData *			inEditionID,
							const ZPIDPairSList &		inTaskList,
							const ZPTaskInfoSList &		inTaskInfoList );

	void				UpdateTask(
							const IStringData *			inTaskID,
							const PMString *			inTaskSubject,
							const ZPTaskInfo *			inTaskInfo ) const;

	void				AddTask(
							const IStringData *			inEditionID,
							const PMString &			inTaskID,			//New TaskID
							const PMString *			inTaskSubject,
							const ZPTaskInfo *			inTaskInfo );

	const IStringData *
						GetTaskByID(
							const PMString &			inTaskID ) const;

	void				FetchTasksForEdition(
							const PMString &			inEditionID ) const;

	void				DeleteTask(					//Sends WS request async
							const PMString &			inTaskID ) const;

	void				HandleWSResponse(
							const IZPSoapRequest *		inSoapRequest,
							const AZPSoapResponse *		inResponse,
							ZPWSError					inStatus);

	void				DecrementReferenceForTasks(
							const ZPTaskIDsList &		inTaskIDs );

protected:
	static const IStringData * CreateTask(
							const PMString &			inTaskID );

	bool				AddUpdateTask(
							const PMString &			inTaskID,
							const PMString &			inTaskSubject,
							const ZPTaskInfo *			inTaskInfo,
							const IStringData **		oTaskID );

	void				HandleTaskDeletion(
							const PMString &			inTaskID );
	void				HandleTaskUpdation(
							const PMString &			inTaskID,
							const PMString &			inTaskSubject,
							const ZPTaskInfo &			inTaskInfo );

	void				RemoveFromCache(
							const IStringData *			inTaskID );

	void				BroadcastMessage(
							const ClassID &				inTheChange,
							void*						inChangedBy = NULL );

private:
	ZPTaskIDMap		mTaskMap;
};


CREATE_PMINTERFACE(CZPTaskMgr, kZPTaskMgrImpl)

CZPTaskMgr::CZPTaskMgr(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPTaskMgr::~CZPTaskMgr()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// CreateTask
//----------------------------------------------------------------------------------------
const IStringData *
CZPTaskMgr::CreateTask(
	const PMString &			inTaskID)
{
	IStringData * theTaskID = ::CreateObject2<IStringData>( kZPDataTaskBoss , IID_IZPDATAID );
	ASSERT(theTaskID);
#ifdef DEBUG
	InterfacePtr<const IControllingUnknown> controlling( theTaskID, UseDefaultIID() );
	IZPLog_Str( thisFileLA, enLT_ObjectCreations, "Task boss created = 0x%08X", controlling.get() );
	IZPLog_Str_( thisFileLA, enLT_ObjectCreations, "Object ID = %s", inTaskID.GrabCString() );
#endif
	theTaskID->Set( inTaskID );

	return theTaskID;
}

//----------------------------------------------------------------------------------------
// FetchTasksForEdition
//----------------------------------------------------------------------------------------
void
CZPTaskMgr::FetchTasksForEdition(
	const PMString &			inEditionID) const
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IZPWSController> wsController( gSession , UseDefaultIID() );
	ASSERT(wsController);

	NewWSRequest( CZPGetTaskListRequest )

	ASSERT( ZPDataHelper::IsValidGUID(inEditionID ) );
	
	theRequest->SetEditionID( inEditionID );

	//TODO: Fetch task without closed status. Closed task should be fetched only if required.
	
	wsController->AddRequest( soapRequest );
}

//----------------------------------------------------------------------------------------
// DeleteTask
//----------------------------------------------------------------------------------------
void
CZPTaskMgr::DeleteTask(
	const PMString &			inTaskID) const
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IZPWSController> wsController( gSession , UseDefaultIID() );
	ASSERT(wsController);

	NewWSRequest( CZPDeleteTaskRequest )

	//Someday: save the task to delete with this request, so that anyone that needs info from old can be fetched till its handling is over.
	theRequest->SetTaskID( inTaskID );

	wsController->AddRequest( soapRequest );
}


//----------------------------------------------------------------------------------------
// HandleWSResponse
//----------------------------------------------------------------------------------------
void
CZPTaskMgr::HandleWSResponse(
	const IZPSoapRequest *			inSoapRequest,
	const AZPSoapResponse *			inResponse,
	ZPWSError						inStatus)
{
	LogFunctionEnterExit;
	
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	const AZPSoapRequest * theRequest = inSoapRequest->GetRequestData();
	switch( theRequest->GetRequestType() )
	{
	case eReq_GetTaskList:
		//this->HandleResponse_FetchTaskList( theRequest, inResponse );
		{
			InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
			ASSERT( zpApp );

			InterfacePtr<IZPEditionMgr> editionMgr( zpApp->GetEditionMgr(), UseDefaultIID() );
			ASSERT(editionMgr);

			const CZPGetTaskListRequest * theRequest = dynamic_cast<const CZPGetTaskListRequest*>(inSoapRequest->GetRequestData());
			ASSERT(theRequest);
			
			const CZPGetTaskListResponse * theResponse = dynamic_cast<const CZPGetTaskListResponse*>(inResponse);
			ASSERT(theResponse);
			

			const IStringData * editionID = editionMgr->GetEditionForID( theRequest->GetEditionID() );
			ASSERT( editionID );

			if( !editionID )
				return;

			this->UpdateTaskList( editionID, theResponse->GetTaskIDPairList(), theResponse->GetTaskInfoList() );
		}
		break;
	case eReq_CreateTask:
		//TODO:
		break;
	case eReq_UpdateTask:
		{
			const CZPUpdateTaskRequest * theRequest = dynamic_cast<const CZPUpdateTaskRequest*>(inSoapRequest->GetRequestData());
			ASSERT(theRequest);
			const CZPUpdateTaskResponse * theResponse = dynamic_cast<const CZPUpdateTaskResponse*>(inResponse);
			ASSERT(theResponse);
			this->HandleTaskUpdation( theResponse->GetTaskID(), theResponse->GetSubject(), theResponse->GetTaskInfo() );
		}
		break;
	case eReq_DeleteTask:
		{
			const CZPDeleteTaskRequest * theRequest = dynamic_cast<const CZPDeleteTaskRequest*>(inSoapRequest->GetRequestData());
			ASSERT(theRequest);
			this->HandleTaskDeletion( theRequest->GetTaskID() );
		}
		break;
	}
}

#pragma mark -
//----------------------------------------------------------------------------------------
// UpdateTaskList
//----------------------------------------------------------------------------------------
void
CZPTaskMgr::UpdateTaskList(
	const IStringData *			inEditionID,
	const ZPIDPairSList &		inTaskList,
	const ZPTaskInfoSList &		inTaskInfoList)
{
	ZPTaskIDsList taskIDsToRemove;
	ZPTaskIDsList taskIDsNewList;

	InterfacePtr<IZPTaskList> editionTaskList( inEditionID, UseDefaultIID() );

	ASSERT( editionTaskList != nil );	//Updating an edition that is not cached.
	if ( editionTaskList == nil )
		return;

	taskIDsToRemove = editionTaskList->GetStringList( false );
	
	InterfacePtr<ISubject> editionSubject( editionTaskList, UseDefaultIID() );
	ASSERT(editionSubject);
	//Send list will change message so that UI does not update with intermediate data.
	editionSubject->Change( kZPTaskListWillChangeMsg, IZPTaskList::kDefaultIID, nil );

	//Add/Update the Task for edition
	ZPIDPairSList::const_iterator	currIter = inTaskList.begin();
	ZPIDPairSList::const_iterator	endIter = inTaskList.end();
	ZPTaskInfoSList::const_iterator	currDetailsIter = inTaskInfoList.begin();

	while( currIter != endIter )
	{
		const PMString & taskID = (*currIter)->mID;

		//Update the Task
		const IStringData * theTask = NULL;
		//(*currIter)->mName.SetTranslatable( kFalse );
		bool newTaskCreated = this->AddUpdateTask( taskID, (*currIter)->mName, *currDetailsIter, &theTask );
		
		//Add to new list.
		taskIDsNewList.push_back(taskID);
		
		++currIter;
		++currDetailsIter;

		if( newTaskCreated == false )
			theTask->AddRef();	//Retain till this ID is referenced.

		//Increament ref count maintained by this manager.
		InterfacePtr<IIntData> cachedCount( theTask, IID_IZPCACHEMGRCOUNT );
		ASSERT( cachedCount );
		cachedCount->Set( cachedCount->Get() + 1 );
	}

	//Update the task id list
	editionTaskList->SetStringList( taskIDsNewList );

	//Remove from cached list. Should we? what if same Edition is shared by multiple titles.
	this->DecrementReferenceForTasks( taskIDsToRemove );
}

//----------------------------------------------------------------------------------------
// AddTask
//----------------------------------------------------------------------------------------
void
CZPTaskMgr::AddTask(
	const IStringData *			inEditionID,
	const PMString &			inTaskID,
	const PMString *			inTaskSubject,
	const ZPTaskInfo *			inTaskInfo)
{
	InterfacePtr<IZPTaskList> editionTaskList( inEditionID, UseDefaultIID() );

	ASSERT( editionTaskList != nil );	//Updating an edition that is not cached.
	if ( editionTaskList == nil )
		return;

	ZPTaskIDsList taskIDsNewList = editionTaskList->GetStringList( false );
	//Create the Task object
	const IStringData * theTask = NULL;
	//(*currIter)->mName.SetTranslatable( kFalse );
	ASSERT(inTaskSubject);

	bool newTaskCreated = this->AddUpdateTask( inTaskID, *inTaskSubject, inTaskInfo, &theTask );
	
	if( !theTask )
		return;

	//Add to new list.
	taskIDsNewList.push_back( inTaskID );

	//Increament ref count maintained by this manager.
	InterfacePtr<IIntData> cachedCount( theTask, IID_IZPCACHEMGRCOUNT );
	ASSERT( cachedCount );
	cachedCount->Set( cachedCount->Get() + 1 );

	//Update the task id list
	editionTaskList->SetStringList( taskIDsNewList, false );
}

//----------------------------------------------------------------------------------------
// AddUpdateTask
//----------------------------------------------------------------------------------------
bool
CZPTaskMgr::AddUpdateTask(
	const PMString &			inTaskID,
	const PMString &			inTaskSubject,
	const ZPTaskInfo *			inTaskDetails,
	const IStringData **		oTaskID)
{
	const IStringData * taskID = this->GetTaskByID( inTaskID );
	bool toReturn = taskID == nil;
	if( toReturn )
	{
		taskID = mTaskMap[inTaskID] = CreateTask( inTaskID );
	}

	this->UpdateTask( taskID, &inTaskSubject, inTaskDetails );

	if( oTaskID )
		*oTaskID = taskID;

	return toReturn;
}

//----------------------------------------------------------------------------------------
// HandleTaskDeletion
//----------------------------------------------------------------------------------------
void
CZPTaskMgr::HandleTaskDeletion(
	const PMString &			inTaskID)
{
	const IStringData * dbTaskID = this->GetTaskByID( inTaskID );
	if( !dbTaskID )
		return;

#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	//Remove from task list.
	InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<IZPEditionMgr> editionMgr( zpApp->GetEditionMgr(), UseDefaultIID() );
	ASSERT(editionMgr);
	const IStringData * dbEditionID = editionMgr->GetEditionForTaskID( inTaskID );
	ASSERT( dbEditionID );
	if( !dbEditionID )
		return;

	InterfacePtr<IZPTaskList> editionTaskList( dbEditionID, UseDefaultIID() );
	ASSERT( editionTaskList );
	editionTaskList->RemoveTaskID( inTaskID );

	//Remove from cache.
	//As task can be listed with only one edition, it is safe to remove from cache.
	//without checking the mgr reference count.
	this->RemoveFromCache( dbTaskID );
}

//----------------------------------------------------------------------------------------
// HandleTaskUpdation
//----------------------------------------------------------------------------------------
void
CZPTaskMgr::HandleTaskUpdation(
	const PMString &			inTaskID,
	const PMString &			inTaskSubject,
	const ZPTaskInfo &			inTaskInfo)
{
	const IStringData * dbTaskID = this->GetTaskByID( inTaskID );
	if( !dbTaskID )
		return;

	this->UpdateTask( dbTaskID, &inTaskSubject, &inTaskInfo );
	this->BroadcastMessage( kZPTaskInfoChangedMsg, (void*)dbTaskID );
}

//----------------------------------------------------------------------------------------
// UpdateTask
//----------------------------------------------------------------------------------------
void
CZPTaskMgr::UpdateTask(
	const IStringData *			inTaskID,
	const PMString *			inTaskSubject,
	const ZPTaskInfo *			inTaskDetails) const
{
	if( inTaskSubject && inTaskID )
	{
		InterfacePtr<IStringData> taskSubject( inTaskID, IID_IZPDATANAME );
		ASSERT( taskSubject );
		taskSubject->Set( *inTaskSubject );
	}
	
	if( inTaskDetails && inTaskID )
	{
		InterfacePtr<IZPTaskInfo> taskInfo( inTaskID, UseDefaultIID() );
		ASSERT( taskInfo );
		taskInfo->SetTaskInfo( *inTaskDetails );
	}
	//else?
}

//----------------------------------------------------------------------------------------
// RemoveFromCache
//----------------------------------------------------------------------------------------
void
CZPTaskMgr::RemoveFromCache(
	const IStringData *				inTaskID)
{
	if( !inTaskID )
		return;

	const PMString & theTaskIDStr = inTaskID->Get();
#ifdef DEBUG
	InterfacePtr<const IControllingUnknown> controlling( inTaskID, UseDefaultIID() );
	IZPLog_Str( thisFileLA, enLT_ObjectCreations, "Task boss removed from cache = 0x%08X", controlling.get() );
	IZPLog_Str_( thisFileLA, enLT_ObjectCreations, "Object ID = %s", theTaskIDStr.GrabCString() );
#endif

	mTaskMap.erase( inTaskID->Get() );
}

//----------------------------------------------------------------------------------------
// DecrementReferenceForTasks
//----------------------------------------------------------------------------------------
void
CZPTaskMgr::DecrementReferenceForTasks(
	const ZPTaskIDsList &		inTaskIDs)
{
	ZPTaskIDsList::const_iterator idListCurrIter = inTaskIDs.begin();
	ZPTaskIDsList::const_iterator idListEndIter = inTaskIDs.end();
	
	while( idListCurrIter != idListEndIter )
	{
		const IStringData * theTaskID = this->GetTaskByID( *idListCurrIter );

		//Update reference count maintained by this manager
		InterfacePtr<IIntData> cachedCount( theTaskID, IID_IZPCACHEMGRCOUNT );
		ASSERT( cachedCount );
		if( cachedCount->Get() == 1 )
			this->RemoveFromCache( theTaskID );
		else
			cachedCount->Set( cachedCount->Get() -1 );

		if( theTaskID )
			theTaskID->Release();
		++idListCurrIter;
	}
}

//----------------------------------------------------------------------------------------
// GetTaskByID
//----------------------------------------------------------------------------------------
const IStringData *
CZPTaskMgr::GetTaskByID(
	const PMString &			inTaskID) const

{
	ZPTaskIDMapConstIter foundIter = mTaskMap.find( inTaskID );
	ZPTaskIDMapConstIter endIter = mTaskMap.end();
	if( foundIter != endIter )
		return foundIter->second;
	else
		return nil;
}

//----------------------------------------------------------------------------------------
// BroadcastMessage
//----------------------------------------------------------------------------------------
void
CZPTaskMgr::BroadcastMessage(
	const ClassID &				inTheChange,
	void*						inChangedBy)
{
	InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
	ASSERT(selfSubject);
	selfSubject->Change( inTheChange, IZPTaskMgr::kDefaultIID, inChangedBy );
}

//----------------------------------------------------------------------------------------
// Shutdown
//----------------------------------------------------------------------------------------
void
CZPTaskMgr::Shutdown()
{
	//TODO: 
}

#pragma mark -
