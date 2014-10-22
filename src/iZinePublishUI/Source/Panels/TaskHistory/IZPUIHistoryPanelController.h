//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Panels/TaskHistory/IZPUIHistoryPanelController.h $
//	$Revision: 2984 $
//	$Date: 2011-05-11 06:27:57 +0200 (Wed, 11 May 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 27-10-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPUIHistoryPanelController_
#define _h_IZPUIHistoryPanelController_
#pragma once

class IStringData;

class IZPUIHistoryPanelController : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPUIHISTORYPANELCONTROLLER };

	static IZPUIHistoryPanelController *	GetHistoryController(bool inFlagCreateIfNil = true);

	virtual void		ShowHistoryForTask(
							const IStringData *			inDBTaskID ) = 0;
	virtual void		ShowHistoryForTask(
							const PMString &			inTaskID ) = 0;

	virtual void		DoHistoryFetch(
							bool						inFetchIfPanelIsVisible = false ) = 0;
	virtual void		UpdatePanel(
							const unsigned char *		inHistoryHTML ) = 0;

	virtual void		HandlePanelClosed() = 0;

};


#endif //_h_IZPUIHistoryPanelController_
