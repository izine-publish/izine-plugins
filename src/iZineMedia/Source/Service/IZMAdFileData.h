//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Service/IZMAdFileData.h $
//	$Revision: 2681 $
//	$Date: 2011-04-05 11:35:14 +0200 (Tue, 05 Apr 2011) $
//	$Author: aman.alam $
//	
//	Creator: Aman Alam
//	Created: 5-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZMAdFileData_
#define _h_IZMAdFileData_
#pragma once

#include "ZMTypes.h"

class IZMAdFileData : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZMADFILEDATA };

	virtual const ZMstAdSList &		GetAdsList() const = 0;
	virtual ZMstAdSList &		GetAdsList() = 0;
};


#endif //_h_IZMAdFileData_
