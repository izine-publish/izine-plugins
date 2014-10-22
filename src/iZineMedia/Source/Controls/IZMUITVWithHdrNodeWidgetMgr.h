//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Controls/IZMUITVWithHdrNodeWidgetMgr.h $
//	$Revision: 2731 $
//	$Date: 2011-04-07 09:46:19 +0200 (Thu, 07 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 7-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZMUITVWithHdrNodeWidgetMgr_
#define _h_IZMUITVWithHdrNodeWidgetMgr_
#pragma once

#include "IZMUITVNodeWidgetMgr.h"

class IZMTVColumnsPref;

class IZMUITVWithHdrNodeWidgetMgr : public IZMUITVNodeWidgetMgr
{
public:
	virtual void	AddRemoveColumnWidgets(
						const IZMTVColumnsPref *		inColPrefs ) = 0;
	virtual void	RepositionColumnWidgets(
						const IZMTVColumnsPref *		inColPrefs ) const = 0;
};


#endif //_h_IZMUITVWithHdrNodeWidgetMgr_
