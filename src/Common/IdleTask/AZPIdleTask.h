//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/Common/IdleTask/AZPIdleTask.h $
//	$Revision: 2985 $
//	$Date: 2011-05-11 08:46:10 +0200 (Wed, 11 May 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 9-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_AZPIdleTask_
#define _h_AZPIdleTask_
#pragma once

#include <vector>

enum enIdleTaskType
{
	eIdleTaskNone,
	eIdleTask_CallWSRespHandler,
	eIdleTask_ShowLoginDialog
};

struct AZPIdleTask
{
public:
						AZPIdleTask();
		virtual			~AZPIdleTask();

		virtual uint32	CanPerformTask(
							uint32				inAppFlags);	//Flag defined in IIdleTask
		virtual	void	PerformTask() = 0;
protected:
						AZPIdleTask(			//protected as only subclass is allowed.
							enIdleTaskType		inTaskType,
							int					inSkipNextNIdleEvents,
							int					inTriesAllowed,
							bool				inAllowOthers = false);

public:
	enIdleTaskType		mTaskType;				//Kind of task
	int					mSkipNextNIdleEvents;	//Skip n events before performing this task
	int					mTriesAllowed;			//Number of tries that can be made till task is over
	int					mTriedCount;			//Number of tries already made
	bool				mCompleted;				//True if task is over
	bool				mAllowOthers;			//If true then other tasks can be done during skipping of events
	bool				mIsExecuting;			//True is task has been started.
};


#endif	//_h_AZPIdleTask_
