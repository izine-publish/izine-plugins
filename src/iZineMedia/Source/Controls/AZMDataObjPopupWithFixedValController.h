//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Controls/AZMDataObjPopupWithFixedValController.h $
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
#ifndef _h_AZMDataObjPopupWithFixedValController_
#define _h_AZMDataObjPopupWithFixedValController_
#pragma once


#include "AZMDataObjPopupController.h"

#define inheritClass_AZMDataObjPopupWithFixedValController AZMDataObjPopupController
class AZMDataObjPopupWithFixedValController : public inheritClass_AZMDataObjPopupWithFixedValController
{
public:
						AZMDataObjPopupWithFixedValController( IPMUnknown * boss );
	virtual				~AZMDataObjPopupWithFixedValController();
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

#endif //_h_AZMDataObjPopupWithFixedValController_
