//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/AZPDBObjPopupController.h $
//	$Revision: 2206 $
//	$Date: 2010-09-30 16:46:41 +0200 (Thu, 30 Sep 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 7-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_AZPDBObjPopupController_
#define _h_AZPDBObjPopupController_
#pragma once

//SDK Interfaces

//SDK General includes

//OS / ThirdParty includes

#include "IZPUIID.h"
#include "IZPID.h"


//IZP Interfaces
#include "IZPDBObjPopupController.h"

//IZP General includes

#pragma mark -

//Note: controls derived from this class must be accessed via this interface only.
//IStringListControlData/IDropDownListController must not be used else where.

#define inheritClass_AZPDBObjPopupController CPMUnknown<IZPDBObjPopupController>
class AZPDBObjPopupController : public inheritClass_AZPDBObjPopupController
{
public:
						AZPDBObjPopupController(IPMUnknown * boss);
	virtual				~AZPDBObjPopupController();

	virtual void		ClearValues();
	virtual void		Refresh();
	virtual void		BroadcastMessage(
							const ClassID&				inTheChange,
							void *						inChangedBy = NULL ) const;
	virtual void		SendMessage_ListLengthChanged() const;
	virtual void		SendMessage_SelectionChanged() const;

	virtual void		SaveSelection();
	virtual void		RestoreSelection();

	virtual PMString	GetNthObjID(
							int							inIndex ) const = 0;
	virtual int32		GetIndexOfObjID(
							const PMString &			inObjID ) const = 0;
protected:
private:
};


#endif //_h_AZPDBObjPopupController_
