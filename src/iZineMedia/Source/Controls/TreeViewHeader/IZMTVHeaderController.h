//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Controls/TreeViewHeader/IZMTVHeaderController.h $
//	$Revision: 2724 $
//	$Date: 2011-04-07 09:30:18 +0200 (Thu, 07 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 7-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZMTVHeaderController_
#define _h_IZMTVHeaderController_
#pragma once


class IZMTVHeaderController : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZMTVHEADERCONTROLLER };

	virtual void		InitHeaderControlsInfo() = 0;

	virtual void		CreateHeaderWidgets() = 0;

	virtual void		HandleTreeViewScrolled(
							const IPMUnknown *					inTreeView ) = 0;
	virtual void		HandleTreeViewResized(
							const IPMUnknown *					inTreeView ) = 0;

	virtual void		ToggleColumnVisibility(
							const int							inColID ) = 0;

	virtual void		BroadcastMessage(
							const ClassID&						inTheChange,
							void *								inChangedBy = nil) const = 0;
};


#endif //_h_IZMTVHeaderController_
