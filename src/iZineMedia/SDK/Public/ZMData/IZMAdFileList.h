//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/SDK/Public/ZMData/IZMAdFileList.h $
//	$Revision: 2687 $
//	$Date: 2011-04-05 14:52:14 +0200 (Tue, 05 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 5-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZMAdFileList_
#define _h_IZMAdFileList_
#pragma once
#include "IStringListData.h"

class IZMAdFileList : public IStringListData
{
public:
	enum { kDefaultIID = IID_IZMADFILELIST };

	virtual void		SetStringList(
							const K2Vector<PMString>&	newList,
							const bool					inHasCompleteList = true) = 0;
	virtual const K2Vector<PMString>& GetStringList(
							const bool					inFetchIfNotHave = true) = 0;

	virtual bool		HasData() const = 0;
	virtual void		SetHasData(
							bool						inHasData) = 0;

	virtual void		RequestUpdateFromServer() = 0;	//Will be asyn

	virtual bool		ContainsAdFileID(
							const PMString &			inAdFileID ) const = 0;
};

#endif //_h_IZMAdFileList_
