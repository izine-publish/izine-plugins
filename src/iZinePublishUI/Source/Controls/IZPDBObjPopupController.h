//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/IZPDBObjPopupController.h $
//	$Revision: 2207 $
//	$Date: 2010-09-30 16:49:47 +0200 (Thu, 30 Sep 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 7-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPDBObjPopupController_
#define _h_IZPDBObjPopupController_
#pragma once



class IZPDBObjPopupController : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPDBOBJPOPUPCONTROLLER };

	virtual void		ClearValues() = 0;
	virtual void		FillValues() = 0;
	virtual void		Refresh() = 0 ;
	virtual void		SendMessage_SelectionChanged() const = 0;

	virtual void		SaveSelection() = 0;
	virtual void		RestoreSelection() = 0;
};

#endif //_h_IZPDBObjPopupController_
