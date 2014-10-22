//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Controls/AZMDataObjPopupController.h $
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
#ifndef _h_AZMDataObjPopupController_
#define _h_AZMDataObjPopupController_
#pragma once


//SDK Interfaces

//SDK General includes

//OS / ThirdParty includes

#include "iZMediaID.h"


//IZP Interfaces
#include "IZMDataObjPopupController.h"

//IZP General includes

#pragma mark -

//Note: controls derived from this class must be accessed via this interface only.
//IStringListControlData/IDropDownListController must not be used else where.

#define inheritClass_AZMDataObjPopupController CPMUnknown<IZMDataObjPopupController>
class AZMDataObjPopupController : public inheritClass_AZMDataObjPopupController
{
public:
						AZMDataObjPopupController(IPMUnknown * boss);
	virtual				~AZMDataObjPopupController();

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

#endif //_h_AZMDataObjPopupController_
