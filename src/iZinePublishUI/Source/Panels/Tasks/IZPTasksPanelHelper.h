//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Panels/Tasks/IZPTasksPanelHelper.h $
//	$Revision: 3041 $
//	$Date: 2011-05-19 08:54:18 +0200 (Thu, 19 May 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 25-10-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPTasksPanelHelper_
#define _h_IZPTasksPanelHelper_
#pragma once

class IStringData;

class IZPTasksPanelHelper : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPTASKSPANELHELPER };
	
	virtual const IStringData *	GetSelectedTask() const = 0;

	virtual PMString		GetSelectedTitleID() const = 0;
	virtual PMString		GetSelectedEditionID() const = 0;
	
	virtual PMString		DBObjPopGetSelectedID(
							WidgetID					inWidgetID,
							bool						inEmptyIfNotCurrentlySelected = false ) const = 0;
};


#endif //_h_IZPTasksPanelHelper_
