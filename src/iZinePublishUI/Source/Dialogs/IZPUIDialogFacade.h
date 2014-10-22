//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Dialogs/IZPUIDialogFacade.h $
//	$Revision: 3510 $
//	$Date: 2011-07-19 12:20:33 +0200 (Tue, 19 Jul 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 22-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPUIDialogFacade_
#define _h_IZPUIDialogFacade_
#pragma once

class IDialog;
class IStringData;

class IZPUIDialogFacade : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPUIDIALOGFACADE };

	virtual void		ShowPrefs() const = 0;
	virtual void		ShowCreateTaskDialog(			//inTaskID is not nil if updating. inCopyValues is true if duplicating task
							const IStringData *			inTaskID = nil,
							const PMString &			inEditionID = kNullString,
							bool						inCopyValues = false ) const = 0;


	virtual IDialog*	CreateDialogSaveToRepository() const = 0;
	virtual void		SetSaveToRepositoryDialogTitle(
							const IDialog *				inDialog ) const = 0;

	virtual IDialog *	CreateDialogCreateTask(
							bool						inIsModeless ) const = 0;

	virtual IDialog*	CreateDialogStatusUpdate() const = 0;

	virtual IDialog *	CreateDialogRenameAsset(
							bool						inIsModeless ) const = 0;

	virtual void		ShowTaskHistoryDialog(
							const unsigned char *		inHTMLString ) const = 0;

	virtual int			ShowAlertDialog(
							const PMString &			inMessage,
							const PMString &			inButton1String,
							const PMString &			inButton2String = kNullString,
							const PMString &			inButton3String = kNullString,
							const PMString &			inButton4String = kNullString,
							int							inDefaultButton = 1) const = 0;
};


#endif //_h_IZPUIDialogFacade_
