//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/SDK/Public/ZMImplDebugHelper.h $
//	$Revision: 2662 $
//	$Date: 2011-04-04 13:29:51 +0200 (Mon, 04 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 19-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description: this file should be included within an impl class only.
//========================================================================================
#ifndef _h_ZPImplDebugHelper_
#define _h_ZPImplDebugHelper_
#pragma once

#ifdef DEBUG

	int32		InterfaceRefCount() const
	{
		return fHelperInterface.fInterfaceRefCount;
	}

#endif

#endif //_h_ZPImplDebugHelper_
