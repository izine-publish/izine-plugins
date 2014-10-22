//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/ZPInterfaceDebugHelper.h $
//	$Revision: 1487 $
//	$Date: 2010-06-17 15:35:13 +0200 (Thu, 17 Jun 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 19-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description: this file should be included within an interface declaration class only.
//========================================================================================
#ifndef _h_ZPInterfaceDebugHelper_
#define _h_ZPInterfaceDebugHelper_
#pragma once

#ifdef DEBUG

	virtual int32		InterfaceRefCount() const = 0;

#endif

#endif //_h_ZPInterfaceDebugHelper_
