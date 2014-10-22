//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/Services/CZExpErrorStringService.cpp $
//	$Revision: 3882 $
//	$Date: 2012-02-09 11:54:34 +0100 (Thu, 09 Feb 2012) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 9-2-2012
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes
#include "CErrorStringService.h"

//OS / ThirdParty includes

#include "IZExpID.h"

//IZP Interfaces

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CErrorStringService
class CZExpErrorStringService : public inheritClass
{
public:
						CZExpErrorStringService(IPMUnknown * boss);
	virtual				~CZExpErrorStringService();
protected:
private:
};


CREATE_PMINTERFACE(CZExpErrorStringService, kZExpErrorStringServiceImpl)

CZExpErrorStringService::CZExpErrorStringService(IPMUnknown * boss)
: inheritClass(boss, kIZExpPluginID, kSDKDefErrorStringResourceID )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZExpErrorStringService::~CZExpErrorStringService()
{

}

#pragma mark -
