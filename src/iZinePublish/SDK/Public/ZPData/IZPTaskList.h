//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/ZPData/IZPTaskList.h $
//	$Revision: 2391 $
//	$Date: 2010-10-26 11:09:32 +0200 (Tue, 26 Oct 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 6-10-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPTaskList_
#define _h_IZPTaskList_
#pragma once

#include "IStringListData.h"

class IZPTaskList : public IStringListData
{
public:
	enum { kDefaultIID = IID_IZPTASKLIST };

	virtual void		SetStringList(
							const K2Vector<PMString>&	newList,
							const bool					inHasCompleteList = true) = 0;
	virtual const K2Vector<PMString>& GetStringList(
							const bool					inFetchIfNotHave = true) = 0;

	virtual bool		HasData() const = 0;
	virtual void		SetHasData(
							bool						inHasData) = 0;

	virtual void		RequestUpdateFromServer() = 0;	//Will be asyn

	virtual bool		ContainsTaskID(
							const PMString &			inTaskID ) const = 0;

	virtual void		RemoveTaskID(
							const PMString &			inTaskID ) = 0;
};


#endif //_h_IZPTaskList_
