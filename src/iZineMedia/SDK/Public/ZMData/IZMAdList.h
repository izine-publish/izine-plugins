//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/SDK/Public/ZMData/IZMAdList.h $
//	$Revision: 2785 $
//	$Date: 2011-04-11 17:20:43 +0200 (Mon, 11 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 11-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZMAdList_
#define _h_IZMAdList_
#pragma once

#include "IStringListData.h"

class IZMAdList : public IStringListData
{
public:
	enum { kDefaultIID = IID_IZMADLIST };

	virtual void		SetStringList(
							const K2Vector<PMString>&	newList,
							const bool					inHasCompleteList = true) = 0;
	virtual const K2Vector<PMString>& GetStringList(
							const bool					inFetchIfNotHave = true) = 0;

	virtual bool		HasData() const = 0;
	virtual void		SetHasData(
							bool						inHasData) = 0;

	virtual void		RequestUpdateFromServer() = 0;	//Will be asyn

	virtual bool		ContainsAdID(
							const PMString &			inAdID ) const = 0;
};

#endif //_h_IZMAdList_
