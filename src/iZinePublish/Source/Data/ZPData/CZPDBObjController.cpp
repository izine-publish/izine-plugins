//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Data/ZPData/CZPDBObjController.cpp $
//	$Revision: 2249 $
//	$Date: 2010-10-06 16:44:33 +0200 (Wed, 06 Oct 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 21-6-2010
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
#include "CZPDBObjController.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

CREATE_PMINTERFACE(CZPDBObjController, kZPDBObjControllerImpl)

CZPDBObjController::CZPDBObjController(IPMUnknown * boss)
: inheritClass_CZPDBObjController(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPDBObjController::~CZPDBObjController()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// RequestUpdateFromServer
//----------------------------------------------------------------------------------------
void
CZPDBObjController::RequestUpdateFromServer(
	PMIID						inWhichProtocol )
{

}

