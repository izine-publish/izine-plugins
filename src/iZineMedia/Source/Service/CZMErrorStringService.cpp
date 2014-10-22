//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Service/CZMErrorStringService.cpp $
//	$Revision: 2818 $
//	$Date: 2011-04-20 10:10:33 +0200 (Wed, 20 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 20-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes
#include "CErrorStringService.h"

//OS / ThirdParty includes

#include "IZMediaID.h"

//IZP Interfaces

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CErrorStringService
class CZMErrorStringService : public inheritClass
{
public:
						CZMErrorStringService(IPMUnknown * boss);
	virtual				~CZMErrorStringService();
protected:
private:
};


CREATE_PMINTERFACE(CZMErrorStringService, kZMErrorStringServiceImpl)

CZMErrorStringService::CZMErrorStringService(IPMUnknown * boss)
: inheritClass(boss, kIZMediaPluginID, kSDKDefErrorStringResourceID )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZMErrorStringService::~CZMErrorStringService()
{

}

#pragma mark -
