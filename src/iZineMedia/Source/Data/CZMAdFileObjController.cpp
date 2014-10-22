//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Data/CZMAdFileObjController.cpp $
//	$Revision: 2750 $
//	$Date: 2011-04-08 11:33:52 +0200 (Fri, 08 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 8-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes

//OS / ThirdParty includes

#include "iZMediaID.h"

//IZP Interfaces

//IZP General includes
#include "CZMDataObjController.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CZMDataObjController
class CZMAdFileObjController : public inheritClass
{
public:
						CZMAdFileObjController(IPMUnknown * boss);
	virtual				~CZMAdFileObjController();
protected:
private:
};


CREATE_PMINTERFACE(CZMAdFileObjController, kZMAdFileObjControllerImpl)

CZMAdFileObjController::CZMAdFileObjController(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZMAdFileObjController::~CZMAdFileObjController()
{

}

#pragma mark -
