//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/ZPData/IZPEditionList.h $
//	$Revision: 4036 $
//	$Date: 2012-06-14 09:17:28 +0200 (Thu, 14 Jun 2012) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 1-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPEditionList_
#define _h_IZPEditionList_
#pragma once

#include "IStringListData.h"

class IZPEditionList : public IStringListData
{
public:
	enum { kDefaultIID = IID_IZPEDITIONLIST };

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

struct PLUGIN_DECL CompareEditionIDByName
{
	bool operator()( const PMString & inFirstVal, const PMString & inSecondVal );
};

#endif //_h_IZPEditionList_
