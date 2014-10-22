//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Plugin/iZMediaNoStrip.cpp $
//	$Revision: 2739 $
//	$Date: 2011-04-07 13:10:59 +0200 (Thu, 07 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 9-11-2010
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
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
#include "iZMediaFactoryList.h"
	}
}

