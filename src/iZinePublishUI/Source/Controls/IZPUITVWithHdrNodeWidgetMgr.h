//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/IZPUITVWithHdrNodeWidgetMgr.h $
//	$Revision: 2287 $
//	$Date: 2010-10-14 09:34:34 +0200 (Thu, 14 Oct 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 14-10-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPUITVWithHdrNodeWidgetMgr_
#define _h_IZPUITVWithHdrNodeWidgetMgr_
#pragma once

#include "IZPUITVNodeWidgetMgr.h"

class IZPTVColumnsPref;

class IZPUITVWithHdrNodeWidgetMgr : public IZPUITVNodeWidgetMgr
{
public:
	virtual void	AddRemoveColumnWidgets(
						const IZPTVColumnsPref *		inColPrefs ) = 0;
	virtual void	RepositionColumnWidgets(
						const IZPTVColumnsPref *		inColPrefs ) const = 0;
};


#endif //_h_IZPUITVWithHdrNodeWidgetMgr_
