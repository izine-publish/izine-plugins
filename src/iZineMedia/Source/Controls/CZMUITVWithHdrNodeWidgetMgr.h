//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Controls/CZMUITVWithHdrNodeWidgetMgr.h $
//	$Revision: 2772 $
//	$Date: 2011-04-11 12:03:16 +0200 (Mon, 11 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 11-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZMUITVWithHdrNodeWidgetMgr_
#define _h_CZMUITVWithHdrNodeWidgetMgr_
#pragma once

class IPanelControlData;
class IControlView;

#include "IZMUITVWithHdrNodeWidgetMgr.h"
#include "ZMUITypes.h"
#include "IZMTVColumnsInfo.h"

#define inheritClass_TVWithHdrNodeWidgetMgr CPMUnknown<IZMUITVWithHdrNodeWidgetMgr>

class CZMUITVWithHdrNodeWidgetMgr : public inheritClass_TVWithHdrNodeWidgetMgr
{
public:
						CZMUITVWithHdrNodeWidgetMgr(IPMUnknown * boss);
	virtual				~CZMUITVWithHdrNodeWidgetMgr();
protected:
	virtual void				SetColumnText(
									WidgetID							widgetID,
									const PMString &					inValue) const;

	virtual void				SetIcon(
									WidgetID							inWidgetID,
									const PluginID &					inIconPluginID,
									const RsrcID &						inIconID) const;

	virtual void				ShowColumn(
									WidgetID							inWidgetid) const;
	virtual void				HideColumn(
									WidgetID							inWidgetid) const;

	virtual bool				CreateColumnWidget(	//As all the columns are text, so no need to pass the column type
									IPanelControlData *					inControlPanelData,
									IControlView * &					oControlView,
									const WidgetID &					inWidgetID,
									const enTVColumnType				inWidgetType );
private:
};


#endif //_h_CZMUITVWithHdrNodeWidgetMgr_
