//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/ZPImplDebugHelper.h $
//	$Revision: 1487 $
//	$Date: 2010-06-17 15:35:13 +0200 (Thu, 17 Jun 2010) $
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
