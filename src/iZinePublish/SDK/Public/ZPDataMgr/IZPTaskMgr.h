//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/ZPDataMgr/IZPTaskMgr.h $
//	$Revision: 2240 $
//	$Date: 2010-10-06 12:33:06 +0200 (Wed, 06 Oct 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 23-9-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPTaskMgr_
#define _h_IZPTaskMgr_
#pragma once

#include "ZPTypes.h"
#include "ZPWSTypeDefs.h"

class IStringData;
class IZPSoapRequest;

class IZPTaskMgr : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPTASKMGR };

	virtual void		UpdateTaskList(
							const IStringData *			inEditionID,
							const ZPIDPairSList &		inTaskList,
							const ZPTaskInfoSList &		inTaskInfoList ) = 0;

	virtual void		UpdateTask(
							const IStringData *			inTaskID,
							const PMString *			inTaskSubject,
							const ZPTaskInfo *			inTaskInfo ) const = 0;

	virtual void		AddTask(
							const IStringData *			inEditionID,
							const PMString &			inTaskID,			//New TaskID
							const PMString *			inTaskSubject,
							const ZPTaskInfo *			inTaskInfo ) = 0;

	virtual const IStringData *
						GetTaskByID(
							const PMString &			inTaskID ) const = 0;

	virtual void		FetchTasksForEdition(
							const PMString &			inEditionID ) const = 0;

	virtual void		DeleteTask(					//Sends WS request async
							const PMString &			inTaskID ) const = 0;

	virtual void		HandleWSResponse(
							const IZPSoapRequest *		inSoapRequest,
							const AZPSoapResponse *		inResponse,
							ZPWSError					inStatus) = 0;

	virtual void		DecrementReferenceForTasks(
							const ZPTaskIDsList &		inTaskIDs ) = 0;
};


#endif //_h_IZPTaskMgr_
