//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Data/DataManagers/CZPAssetStatusMgr.cpp $
//	$Revision: 1180 $
//	$Date: 2010-05-19 13:26:10 +0200 (Wed, 19 May 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 29-3-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPAssetStatusMgr.h"

//IZP General includes

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

class CZPAssetStatusMgr : public CPMUnknown<IZPAssetStatusMgr>
{
public:
				CZPAssetStatusMgr(IPMUnknown * boss);
	virtual		~CZPAssetStatusMgr();

	void				Shutdown();

protected:
private:
};


CREATE_PMINTERFACE(CZPAssetStatusMgr, kZPAssetStatusMgrImpl)

CZPAssetStatusMgr::CZPAssetStatusMgr(IPMUnknown * boss) :
CPMUnknown<IZPAssetStatusMgr>(boss)
{

}

CZPAssetStatusMgr::~CZPAssetStatusMgr()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// Shutdown
//----------------------------------------------------------------------------------------
void
CZPAssetStatusMgr::Shutdown()
{

}
