//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/AZPDBObjPopupWithFixedValController.h $
//	$Revision: 2253 $
//	$Date: 2010-10-07 14:30:35 +0200 (Thu, 07 Oct 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 7-10-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_AZPDBObjPopupWithFixedValController_
#define _h_AZPDBObjPopupWithFixedValController_
#pragma once

#include "AZPDBObjPopupController.h"

#define inheritClass_AZPDBObjPopupWithFixedValController AZPDBObjPopupController
class AZPDBObjPopupWithFixedValController : public inheritClass_AZPDBObjPopupWithFixedValController
{
public:
						AZPDBObjPopupWithFixedValController( IPMUnknown * boss );
	virtual				~AZPDBObjPopupWithFixedValController();
protected:
	void				ClearValues();
	void				FillValues();
	
	PMString			GetNthObjID(
							int							inIndex) const;
	int32				GetIndexOfObjID(
							const PMString &			inObjID ) const;

	virtual void		AppendFixedValues();
	virtual void		AppendDBObjectValues() = 0;
	virtual PMString	GetNthDBObjID(
							int							inIndex ) const = 0;
	virtual PMString	GetNthFixedObjID(
							int							inIndex ) const;
	virtual int32		GetIndexOfDBObjID(
							const PMString &			inObjID ) const = 0;
	virtual int32		GetIndexOfFixedObjID(
							const PMString &			inObjID ) const;

private:

protected:
	int					mFixedValuesCount;
};


#endif //_h_AZPDBObjPopupWithFixedValController_
