//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/Common/IdleTask/CZPIdleTaskMgr.h $
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
#ifndef _h_CZPIdleTaskMgr_
#define _h_CZPIdleTaskMgr_
#pragma once

#include <vector>

struct AZPIdleTask;

class CZPIdleTaskMgr
{
public:
	typedef std::vector<AZPIdleTask*>	IdleTaskArr;
private:
							CZPIdleTaskMgr();	//Singleton

public:
	virtual					~CZPIdleTaskMgr();

	static CZPIdleTaskMgr*	GetInstance();
	static void				RemoveInstance();

	uint32					HandleIdleEvent(
								uint32							inAppFlags);	//Flag defined in IIdleTask
	void					AddIdleTask(
								AZPIdleTask *					inNewIdleTask );

protected:
	AZPIdleTask *			GetNextTaskToPerform() const;
	bool					CanPerformIdleTask(
								const AZPIdleTask *				inIdleTask ) const;
	uint32					PerformIdleTask(
								AZPIdleTask *					inIdleTask,
								uint32							inAppFlags);	//Flag defined in IIdleTask
	void					RemoveIdleTask(
								AZPIdleTask *					inIdleTask );

private:
	static CZPIdleTaskMgr * gInstance_IdleTaskMgr;

	IdleTaskArr				mIdleTaskList;
};

#endif //_h_CZPIdleTaskMgr_
