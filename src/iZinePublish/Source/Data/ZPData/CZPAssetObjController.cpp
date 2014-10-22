//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Data/ZPData/CZPAssetObjController.cpp $
//	$Revision: 1507 $
//	$Date: 2010-06-21 14:30:28 +0200 (Mon, 21 Jun 2010) $
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

#define inheritClass CZPDBObjController
class CZPAssetObjController : public inheritClass
{
public:
						CZPAssetObjController(IPMUnknown * boss);
	virtual				~CZPAssetObjController();
protected:
private:
};


CREATE_PMINTERFACE(CZPAssetObjController, kZPAssetObjControllerImpl)

CZPAssetObjController::CZPAssetObjController(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAssetObjController::~CZPAssetObjController()
{

}

#pragma mark -
