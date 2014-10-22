//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/SDK/Public/ZMData/IZMAdInfo.h $
//	$Revision: 2747 $
//	$Date: 2011-04-08 09:40:09 +0200 (Fri, 08 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 8-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZMAdInfo_
#define _h_IZMAdInfo_
#pragma once

#include "ZMTypes.h"

class IZMAdInfo : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZMADINFO };

	virtual void				SetAdDetail(
									const stAd &						inDetail ) = 0;

	virtual const PMReal &		GetAdHeight() const = 0;
	virtual const PMReal &		GetAdWidth() const = 0;
	virtual const PMString &	GetComments() const = 0;
	virtual const PMString &	GetUrl() const = 0;
};


#endif //_h_IZMAdInfo_
