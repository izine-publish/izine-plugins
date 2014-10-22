//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/Data/IZExpServerData.h $
//	$Revision: 3682 $
//	$Date: 2011-11-28 12:05:31 +0100 (Mon, 28 Nov 2011) $
//	$Author: aman.alam $
//	
//	Creator: Aman Alam
//	Created: 6-6-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZExpServerData_
#define _h_IZExpServerData_
#pragma once

#include "ZExpWSTypeDefs.h"

class IZExpServerData : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZEXPSERVERDATA };

	virtual void				FetchTitles() = 0;
	virtual void				FetchEditions(
									const PMString &		inTitleId) = 0;

	virtual const ZExpIDPairList &	GetTitles() const = 0;
	virtual const ZExpIDPairList &	GetEditions() const = 0;

	virtual const PMString &	GetTitleIDByIndex(
									const int				inIndex) const = 0;
	virtual const PMString &	GetEditionIDByIndex(
									const int				inIndex) const = 0;

	virtual void				GetConfig() = 0;

};


#endif //_h_IZExpServerData_
