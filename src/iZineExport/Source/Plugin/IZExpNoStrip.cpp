//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/Plugin/IZExpNoStrip.cpp $
//	$Revision: 2482 $
//	$Date: 2010-11-09 15:30:38 +0100 (Tue, 09 Nov 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 9-11-2010
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
#include "IZExpFactoryList.h"
	}
}

