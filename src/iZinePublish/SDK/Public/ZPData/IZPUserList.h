//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/ZPData/IZPUserList.h $
//	$Revision: 2213 $
//	$Date: 2010-10-01 12:44:26 +0200 (Fri, 01 Oct 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 1-10-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPUserList_
#define _h_IZPUserList_
#pragma once

#include "IStringListData.h"

class IZPUserList : public IStringListData
{
public:
	enum { kDefaultIID = IID_IZPUSERLIST };

	virtual void		SetStringList(
							const K2Vector<PMString>&	newList,
							const bool					inHasCompleteList = true) = 0;
	virtual const K2Vector<PMString>& GetStringList(
							const bool					inFetchIfNotHave = true) = 0;

	virtual bool		HasData() const = 0;
	virtual void		SetHasData(
							bool						inHasData) = 0;

	virtual void		RequestUpdateFromServer() = 0;	//Will be asyn

	virtual bool		ContainsUserID(
							const PMString &			inUserID ) const = 0;

};


#endif //_h_IZPUserList_
