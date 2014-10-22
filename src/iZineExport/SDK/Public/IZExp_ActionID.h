//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/SDK/Public/IZExp_ActionID.h $
//	$Revision: 3162 $
//	$Date: 2011-06-08 07:38:24 +0200 (Wed, 08 Jun 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 22-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZExp_ActionID_
#define _h_IZExp_ActionID_
#pragma once


// Actions handled by App Action component
DECLARE_PMID(kActionIDSpace, kIZExpAboutActionID,															kIZExpUIPrefix + 0 )
DECLARE_PMID(kActionIDSpace, kIZExpExportFrontDocActionID,													kIZExpUIPrefix + 1 )
DECLARE_PMID(kActionIDSpace, kIZExpExportActionID,															kIZExpUIPrefix + 2 )
DECLARE_PMID(kActionIDSpace, kIZExpPrefActionID,															kIZExpUIPrefix + 3 )

// Menu item positions:
#define kIZExpAboutThisMenuItemPosition				11.0
#define kIZExpExportMenuItemPosition				1401
#define kIZExpPreferencesMenuPosition				1541

// "Plug-ins" sub-menu item position for dialog:


#endif //_h_IZExp_ActionID_
