//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/Common/IdleTask/AZPIdleTask.cpp $
//	$Revision: 984 $
//	$Date: 2010-04-12 13:07:14 +0200 (Mon, 12 Apr 2010) $
//	$Author: rajkumar.sehrawat $
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

//#include "IZPID.h"

//IZP Interfaces

//IZP General includes
#include "AZPIdleTask.h"

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
AZPIdleTask::AZPIdleTask(			//protected as only subclass is allowed.
	enIdleTaskType		inTaskType,
	int					inSkipNextNIdleEvents,
	int					inTriesAllowed,
	bool				inAllowOthers)
:
mTaskType( inTaskType )
,mSkipNextNIdleEvents( inSkipNextNIdleEvents )
,mTriesAllowed( inTriesAllowed )
,mTriedCount( 0 )
,mCompleted( false )
,mAllowOthers( inAllowOthers )
,mIsExecuting( false )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
AZPIdleTask::~AZPIdleTask()
{

}

//----------------------------------------------------------------------------------------
// CanPerformTask
//----------------------------------------------------------------------------------------
uint32
AZPIdleTask::CanPerformTask(
	uint32				inAppFlags)	//Flag defined in IIdleTask
{
	return IIdleTask::kEndOfTime;
}


//----------------------------------------------------------------------------------------
// PerformTask
//----------------------------------------------------------------------------------------
void
AZPIdleTask::PerformTask()
{
	//must never reach here.
}

