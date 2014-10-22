//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/App/CZPErrorStringService.cpp $
//	$Revision: 3566 $
//	$Date: 2011-08-04 12:14:39 +0200 (Thu, 04 Aug 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 13-5-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes
#include "CErrorStringService.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_None
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CErrorStringService
class CZPErrorStringService : public inheritClass
{
public:
						CZPErrorStringService(IPMUnknown * boss);
	virtual				~CZPErrorStringService();
protected:
private:
};


CREATE_PMINTERFACE(CZPErrorStringService, kZPErrorStringServiceImpl)

CZPErrorStringService::CZPErrorStringService(IPMUnknown * boss)
: inheritClass(boss, kIZPPluginID, kSDKDefErrorStringResourceID )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPErrorStringService::~CZPErrorStringService()
{

}

#pragma mark -
