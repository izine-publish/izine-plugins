//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/ZPData/IZPStatusList.h $
//	$Revision: 1192 $
//	$Date: 2010-05-21 09:33:32 +0200 (Fri, 21 May 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 1-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPStatusList_
#define _h_IZPStatusList_
#pragma once

#include "IStringListData.h"

class IZPStatusList : public IStringListData
{
public:
	enum { kDefaultIID = IID_IZPSTATUSLIST };

	virtual void		SetStringList(
							const K2Vector<PMString>&	newList) = 0;
	virtual const K2Vector<PMString>& GetStringList(
							const bool					inFetchIfNotHave = true) = 0;

	virtual bool		HasData() const = 0;
	virtual void		SetHasData(
							bool						inHasData) = 0;

};


#endif //_h_IZPStatusList_
