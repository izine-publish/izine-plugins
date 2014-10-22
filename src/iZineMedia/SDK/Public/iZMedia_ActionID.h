//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/SDK/Public/iZMedia_ActionID.h $
//	$Revision: 2799 $
//	$Date: 2011-04-12 12:20:30 +0200 (Tue, 12 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 22-4-2010
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZMedia_ActionID_
#define _h_IZMedia_ActionID_
#pragma once


// Actions handled by App Action component
DECLARE_PMID(kActionIDSpace, kIZMediaAboutActionID,															kIZMediaPrefix + 0 )
DECLARE_PMID(kActionIDSpace, kIZMPreferencesPanelActionID,													kIZMediaPrefix + 1 )

// gap of 10
	//Panel
DECLARE_PMID(kActionIDSpace, kIZMAdsPanelWidgetActionID,													kIZMediaPrefix + 100 )

DECLARE_PMID(kActionIDSpace, kIZMTVHeaderShowColumnsStartActionID,											kIZMediaPrefix + 150 )
// keep gap of 15 
DECLARE_PMID(kActionIDSpace, kIZMTVHeaderShowColumnsEndActionID,											kIZMediaPrefix + 165 )
DECLARE_PMID(kActionIDSpace, kIZMTVHeaderShowColumnsActionID,												kIZMediaPrefix + 166 )



// Menu item positions:
#define kIZMediaAboutThisMenuItemPosition				11.0
#define kIZMPreferencesMenuPosition						1550

// "Plug-ins" sub-menu item position for dialog:


#endif //_h_IZMedia_ActionID_
