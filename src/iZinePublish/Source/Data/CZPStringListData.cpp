//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Data/CZPStringListData.cpp $
//	$Revision: 1143 $
//	$Date: 2010-05-10 13:27:40 +0200 (Mon, 10 May 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 6-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces

//IZP General includes
#include "CZPStringListData.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

CREATE_PMINTERFACE(CZPStringListData_<IStringListData>, kZPStringListDataImpl)

