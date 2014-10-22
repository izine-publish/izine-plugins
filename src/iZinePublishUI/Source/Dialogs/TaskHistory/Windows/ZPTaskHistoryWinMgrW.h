//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Dialogs/TaskHistory/Windows/ZPTaskHistoryWinMgrW.h $
//	$Revision: 2369 $
//	$Date: 2010-10-22 12:32:55 +0200 (Fri, 22 Oct 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 22-10-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_ZPTaskHistoryWinMgrW_
#define _h_ZPTaskHistoryWinMgrW_
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif
void ShowTaskHistoryWindow(
		const unsigned char *				inHTMLString,
		HINSTANCE							hInstance,
		HWND								inParentWindow);

#ifdef __cplusplus
}
#endif
#endif //_h_ZPTaskHistoryWinMgrW_
