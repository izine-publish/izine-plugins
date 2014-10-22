//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/App/CZPAppIdleTask.cpp $
//	$Revision: 2193 $
//	$Date: 2010-09-29 11:36:01 +0200 (Wed, 29 Sep 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 11-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes
#include "CIdleTask.h"

#include "IZPID.h"

//IZP Interfaces

//IZP General includes
#include "CZPIdleTaskMgr.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_Idle
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

class CZPAppIdleTask : public CIdleTask
{
public:
				CZPAppIdleTask(IPMUnknown * boss);
	virtual		~CZPAppIdleTask();

	/** Will perform its task and then relinquish control until it is called again.
		See CIdleTaskThread.h if this API is inconvenient for the task at hand.
		@param appFlags - Informs this method of the current state of the app see 
		IIdleTaskMgr::ApplicationStateFlags
		@param timeCheck - A callback method that should be called periodically 
		during long tasks to see whether RunTask has run out of time and needs to return.
		@return - The number of milliseconds to sleep before running again.

		  o  Returning kEndOfTime means that this task will be removed
			from the IdleTaskMgr--without calling its UninstallTask method.
			Hence, you should do whatever other chores you do in UninstallTask 
			before returning this value.

		  o  If this task's UninstallTask method is called before RunTask
			returns, then the return value for RunTask will be ignored.

		  o  Returning kOnFlagChange suspends this task until appFlags
			changes.  At which point it will be called again ASAP.  RunTask
			can continue to return kOnFlagChange as long as it doesn't like
			the current appFlags.
		@see IdleTaskMgr
		@see CIdleTaskThread
		@see IIdleTaskMgr::ApplicationStateFlags
	*/
	uint32 RunTask( uint32 appFlags, IdleTimer* timeCheck);
protected:
private:
};


CREATE_PMINTERFACE(CZPAppIdleTask, kZPAppIdleTaskImpl)

CZPAppIdleTask::CZPAppIdleTask(IPMUnknown * boss) :
CIdleTask(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAppIdleTask::~CZPAppIdleTask()
{

}

//----------------------------------------------------------------------------------------
// RunTask
//----------------------------------------------------------------------------------------
uint32
CZPAppIdleTask::RunTask(
	uint32 appFlags, IdleTimer* timeCheck)
{
	LogFunctionEnterExit;

	CZPIdleTaskMgr * taskMgr = CZPIdleTaskMgr::GetInstance();
	uint32 toReturn = taskMgr->HandleIdleEvent( appFlags );

	return toReturn ;//IIdleTask::kOnFlagChange;
}


