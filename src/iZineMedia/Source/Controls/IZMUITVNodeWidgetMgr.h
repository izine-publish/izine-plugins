//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Controls/IZMUITVNodeWidgetMgr.h $
//	$Revision: 2730 $
//	$Date: 2011-04-07 09:45:42 +0200 (Thu, 07 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 7-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZMUITVNodeWidgetMgr_
#define _h_IZMUITVNodeWidgetMgr_
#pragma once


class IZMUITVNodeWidgetMgr : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZMUITVNODEWIDGETMGR };

						//Updates the UI controls of this node.
	virtual void		Refresh() const = 0;
};



#endif //_h_IZMUITVNodeWidgetMgr_
