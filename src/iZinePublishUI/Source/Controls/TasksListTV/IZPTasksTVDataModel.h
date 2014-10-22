//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/TasksListTV/IZPTasksTVDataModel.h $
//	$Revision: 2396 $
//	$Date: 2010-10-26 13:07:02 +0200 (Tue, 26 Oct 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 27-9-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPTasksTVDataModel_
#define _h_IZPTasksTVDataModel_
#pragma once

#include "ZPTypes.h"

typedef bool	(*FilterTaskFnPtr)(const IStringData * inTaskID, void * inFilterFnData );

class IZPTasksTVDataModel : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPTASKSTVDATAMODEL };
	
	virtual const PMString &		GetEditionID() const = 0;
	//No setter for TitleID and EditionID. Use UpdateData instead.

	virtual const ZPTaskIDsList *	GetTaskIDList() const = 0;

	virtual const PMString *		GetNthTaskID(
										int							inIndex ) const = 0;
	virtual int32					GetTaskIDIndex(
										const PMString &			inTaskID ) const = 0;

	virtual void					UpdateData(
										const PMString &			inEditionID ) = 0;

	virtual void					UpdateTaskList() = 0;
	virtual void					HandleTaskInfoChange(
										const IStringData *			inTaskID ) = 0;

	virtual const FilterTaskFnPtr	GetTaskFilteringFunction() const = 0;
	virtual void					SetTaskFilteringFunction(
										FilterTaskFnPtr			inFunc,
										void *					inFilterFnData ) = 0;
};


#endif //_h_IZPTasksTVDataModel_
