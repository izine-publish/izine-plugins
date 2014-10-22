//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/TreeViewHeader/IZPTVHeaderController.h $
//	$Revision: 2336 $
//	$Date: 2010-10-18 12:58:13 +0200 (Mon, 18 Oct 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 11-10-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPTVHeaderController_
#define _h_IZPTVHeaderController_
#pragma once


class IZPTVHeaderController : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPTVHEADERCONTROLLER };

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


#endif //_h_IZPTVHeaderController_
