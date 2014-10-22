//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Panels/Ads/IZMAdsPanelHelper.h $
//	$Revision: 2756 $
//	$Date: 2011-04-08 13:50:19 +0200 (Fri, 08 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 6-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZMAdsPanelHelper_
#define _h_IZMAdsPanelHelper_
#pragma once

class IStringData;

class IZMAdsPanelHelper : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZMADSPANELHELPER };

	virtual const IStringData *	GetSelectedAd() const = 0;
};

#endif //_h_IZMAdsPanelHelper_
