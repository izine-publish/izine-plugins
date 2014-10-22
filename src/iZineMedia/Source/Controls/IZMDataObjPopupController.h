//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Controls/IZMDataObjPopupController.h $
//	$Revision: 2707 $
//	$Date: 2011-04-06 14:38:20 +0200 (Wed, 06 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 6-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZMDataObjPopupController_
#define _h_IZMDataObjPopupController_
#pragma once


class IZMDataObjPopupController : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZMDATAOBJPOPUPCONTROLLER };

	virtual void		ClearValues() = 0;
	virtual void		FillValues() = 0;
	virtual void		Refresh() = 0 ;
	virtual void		SendMessage_SelectionChanged() const = 0;

	virtual void		SaveSelection() = 0;
	virtual void		RestoreSelection() = 0;
};

#endif //_h_IZMDataObjPopupController_
