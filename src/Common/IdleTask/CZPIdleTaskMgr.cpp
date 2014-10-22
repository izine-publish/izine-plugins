//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/Common/IdleTask/CZPIdleTaskMgr.cpp $
//	$Revision: 3565 $
//	$Date: 2011-08-04 12:09:14 +0200 (Thu, 04 Aug 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 9-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IIdleTask.h"

//SDK General includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPAppController.h"

//IZP General includes
#include "AZPIdleTask.h"
#include "CZPIdleTaskMgr.h"
#include "StUtils.h"

#include "IZPLog.h"

#define thisFileLA	enLMA_Idle
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

CZPIdleTaskMgr * CZPIdleTaskMgr::gInstance_IdleTaskMgr = NULL;

#pragma mark -
//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPIdleTaskMgr::CZPIdleTaskMgr()
{
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPIdleTaskMgr::~CZPIdleTaskMgr()
{
	//TODO: empty task list
}

//----------------------------------------------------------------------------------------
// GetInstance
//----------------------------------------------------------------------------------------
CZPIdleTaskMgr*
CZPIdleTaskMgr::GetInstance()
{
	if( !gInstance_IdleTaskMgr )
		gInstance_IdleTaskMgr = new CZPIdleTaskMgr;
	return gInstance_IdleTaskMgr;
}

//----------------------------------------------------------------------------------------
// RemoveInstance
//----------------------------------------------------------------------------------------
void
CZPIdleTaskMgr::RemoveInstance()
{
	if( gInstance_IdleTaskMgr )
		delete	gInstance_IdleTaskMgr;
	gInstance_IdleTaskMgr = NULL;
}


//----------------------------------------------------------------------------------------
// HandleIdleEvent
// Return value is same as IIdleTask::RunTask
//----------------------------------------------------------------------------------------
uint32
CZPIdleTaskMgr::HandleIdleEvent(
	uint32				inAppFlags)	//Flag defined in IIdleTask
{
	//LogFunctionEnterExit;
	AZPIdleTask * nextTask = this->GetNextTaskToPerform();
	if( !nextTask )
	{
		if( mIdleTaskList.size() > 0 )
			return IIdleTask::kNextEventCycle;
		else
			return IIdleTask::kEndOfTime;
	}

	uint32 toReturn = this->PerformIdleTask(nextTask, inAppFlags);

	//Remove task if completed or allowed tries are made.
	if( nextTask->mCompleted || nextTask->mTriesAllowed <= nextTask->mTriedCount )
		this->RemoveIdleTask( nextTask );

	if( toReturn == IIdleTask::kEndOfTime )
	{
		if( mIdleTaskList.size() > 0 )
			toReturn = IIdleTask::kNextEventCycle;
	}
	return toReturn;
}


//----------------------------------------------------------------------------------------
// AddIdleTask
//----------------------------------------------------------------------------------------
void
CZPIdleTaskMgr::AddIdleTask(
	AZPIdleTask *					inNewIdleTask)
{
	LogFunctionEnterExit;
	mIdleTaskList.push_back( inNewIdleTask );
	if( mIdleTaskList.size() == 1 )
	{
		ASSERT( gZPApp );
		InterfacePtr<IIdleTask> zpAppIdleTask(gZPApp, UseDefaultIID());
		if(zpAppIdleTask)
			zpAppIdleTask->InstallTask(0);
	}
}


//----------------------------------------------------------------------------------------
// GetNextTaskToPerform
//----------------------------------------------------------------------------------------
AZPIdleTask *
CZPIdleTaskMgr::GetNextTaskToPerform()const
{
	//LogFunctionEnterExit;
	AZPIdleTask * toReturn = NULL;
	if( mIdleTaskList.size() > 0 )
	{
		for( int i = 0; i < mIdleTaskList.size(); ++i )	//Don't take size out of this for loop
		{
			toReturn = mIdleTaskList[i];
			if( this->CanPerformIdleTask( toReturn ) )
				break;
			else if( toReturn->mSkipNextNIdleEvents > 0 )
			{
				toReturn->mSkipNextNIdleEvents -= 1;
			}

			toReturn = NULL;
		}
	}
	return toReturn;
}


//----------------------------------------------------------------------------------------
// CanPerformIdleTask
//----------------------------------------------------------------------------------------
bool
CZPIdleTaskMgr::CanPerformIdleTask(
	const AZPIdleTask *					inIdleTask) const
{
	//LogFunctionEnterExit;
	bool toReturn = false;
	if( inIdleTask->mIsExecuting )
		return toReturn;

	if( inIdleTask->mSkipNextNIdleEvents > 0 )
	{
		return toReturn;
	}

	toReturn = true;

	//Add case specific conditions here.

	return toReturn;
}


//----------------------------------------------------------------------------------------
// PerformIdleTask
//----------------------------------------------------------------------------------------
uint32
CZPIdleTaskMgr::PerformIdleTask(
	AZPIdleTask *		inIdleTask,
	uint32				inAppFlags)	//Flag defined in IIdleTask
{
	LogFunctionEnterExit;
	StRestoreValue<bool> autoRestore( inIdleTask->mIsExecuting, true );
	inIdleTask->mTriedCount += 1;

	uint32 toReturn = IIdleTask::kNextEventCycle;
	toReturn = inIdleTask->CanPerformTask( inAppFlags );
	if( toReturn == IIdleTask::kEndOfTime )
	{
		inIdleTask->PerformTask();
		inIdleTask->mCompleted = true;
	}
	return toReturn;
}


//----------------------------------------------------------------------------------------
// RemoveIdleTask
//----------------------------------------------------------------------------------------
void
CZPIdleTaskMgr::RemoveIdleTask(
	AZPIdleTask *					inIdleTask)
{
	LogFunctionEnterExit;
	IdleTaskArr::const_iterator endIter = mIdleTaskList.end();
	IdleTaskArr::iterator foundIter = std::find( mIdleTaskList.begin(), mIdleTaskList.end(), inIdleTask);
	if( foundIter != endIter )
	{
		mIdleTaskList.erase( foundIter );
		delete inIdleTask;
	}

	if( mIdleTaskList.size() == 0 )
	{
		InterfacePtr<IIdleTask> zpAppIdleTask(gZPApp, UseDefaultIID());
		if(zpAppIdleTask)
			zpAppIdleTask->UninstallTask();
	}
}
