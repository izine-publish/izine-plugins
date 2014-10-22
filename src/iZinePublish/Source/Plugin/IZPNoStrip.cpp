//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Plugin/IZPNoStrip.cpp $
//	$Revision: 1003 $
//	$Date: 2010-04-15 11:43:44 +0200 (Thu, 15 Apr 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 2010 Mar 19
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description: 
//========================================================================================

#include "VCPlugInHeaders.h"

#include "InterfaceFactory.h"

extern bool16 gFalse;

/** References all implementations to stop the compiler dead stripping them from the executable image.
*/
void DontDeadStrip();

void DontDeadStrip()
{
	if (gFalse)
	{
#include "IZPFactoryList.h"
	}
}

