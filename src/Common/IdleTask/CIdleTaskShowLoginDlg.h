//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/Common/IdleTask/CIdleTaskShowLoginDlg.h $
//	$Revision: 3276 $
//	$Date: 2011-06-17 08:47:26 +0200 (Fri, 17 Jun 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 11-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CIdleTaskShowLoginDlg_
#define _h_CIdleTaskShowLoginDlg_
#pragma once


//SDK Interfaces

//SDK General includes

//#include "IZPID.h"

//IZP Interfaces

//IZP General includes
#include "AZPIdleTask.h"

#pragma mark -

class CIdleTaskShowLoginDlg : public AZPIdleTask
{
public:
					CIdleTaskShowLoginDlg();
	virtual			~CIdleTaskShowLoginDlg();

	virtual uint32	CanPerformTask(
						uint32				inAppFlags);	//Flag defined in IIdleTask
	virtual	void	PerformTask();
protected:
private:
};

#endif //_h_CIdleTaskShowLoginDlg_
