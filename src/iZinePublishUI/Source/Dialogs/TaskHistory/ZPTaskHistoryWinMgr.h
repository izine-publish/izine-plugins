//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Dialogs/TaskHistory/ZPTaskHistoryWinMgr.h $
//	$Revision: 2411 $
//	$Date: 2010-10-27 12:32:52 +0200 (Wed, 27 Oct 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 19-10-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#ifndef _h_ZPTaskHistoryWinMgr_
#define _h_ZPTaskHistoryWinMgr_
#pragma once

void		ShowTaskHistoryWindow(
				const unsigned char *		inHistoryContent );

#pragma mark -
//Implemented by Cpp

extern "C"
void	HandleHistoryPanelClose();

#endif