//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/ZPData/IZPAssetList.h $
//	$Revision: 2133 $
//	$Date: 2010-09-20 15:59:00 +0200 (Mon, 20 Sep 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 24-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPAssetList_
#define _h_IZPAssetList_
#pragma once


#include "IStringListData.h"

class IZPAssetList : public IStringListData
{
public:
	enum { kDefaultIID = IID_IZPASSETLIST };

	virtual void		SetStringList(
							const K2Vector<PMString>&	newList,
							const bool					inHasCompleteList = true) = 0;
	virtual const K2Vector<PMString>& GetStringList(
							const bool					inFetchIfNotHave = true) = 0;

	virtual bool		HasData() const = 0;
	virtual void		SetHasData(
							bool						inHasData) = 0;

	virtual void		RequestUpdateFromServer() = 0;	//Will be asyn

	virtual bool		ContainsAssetID(
							const PMString &			inAssetID ) const = 0;
};

#endif //_h_IZPAssetList_
