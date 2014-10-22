//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/IZPUITVNodeWidgetMgr.h $
//	$Revision: 1221 $
//	$Date: 2010-05-25 11:44:40 +0200 (Tue, 25 May 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 25-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPUITVNodeWidgetMgr_
#define _h_IZPUITVNodeWidgetMgr_
#pragma once


class IZPUITVNodeWidgetMgr : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPUITVNODEWIDGETMGR };

						//Updates the UI controls of this node.
	virtual void		Refresh() const = 0;
};



#endif //_h_IZPUITVNodeWidgetMgr_
