//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Dialogs/IZMUIDialogFacade.h $
//	$Revision: 2746 $
//	$Date: 2011-04-08 09:05:59 +0200 (Fri, 08 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 8-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPUIDialogFacade_
#define _h_IZPUIDialogFacade_
#pragma once

class IDialog;

class IZMUIDialogFacade : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZMUIDIALOGFACADE };

	virtual void		ShowPrefs() const = 0;
	
	///virtual int			ShowAlertDialog(
	///						const PMString &			inMessage,
	///						const PMString &			inButton1String,
	///						const PMString &			inButton2String = kNullString,
	///						const PMString &			inButton3String = kNullString,
	///						const PMString &			inButton4String = kNullString,
	///						int							inDefaultButton = 1) const = 0;
};


#endif //_h_IZPUIDialogFacade_
