//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/SDK/Public/ZMData/IZMEditionList.h $
//	$Revision: 2738 $
//	$Date: 2011-04-07 13:08:10 +0200 (Thu, 07 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 1-5-2010
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZMEditionList_
#define _h_IZMEditionList_
#pragma once

#include "IStringListData.h"

class IZMEditionList : public IStringListData
{
public:
	enum { kDefaultIID = IID_IZMEDITIONLIST };

	virtual void		SetStringList(
							const K2Vector<PMString>&	newList) = 0;
	virtual const K2Vector<PMString>& GetStringList(
							const bool					inFetchIfNotHave = true) = 0;

	virtual bool		HasData() const = 0;
	virtual void		SetHasData(
							bool						inHasData) = 0;

	virtual bool		ContainsEditionID(
							const PMString &			inEditionID ) const = 0;
};

#endif //_h_IZMEditionList_
