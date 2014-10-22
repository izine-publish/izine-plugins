//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/SDK/Public/ZMInterfaceDebugHelper.h $
//	$Revision: 2662 $
//	$Date: 2011-04-04 13:29:51 +0200 (Mon, 04 Apr 2011) $
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
