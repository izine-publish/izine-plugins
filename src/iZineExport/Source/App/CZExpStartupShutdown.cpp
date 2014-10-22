//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/App/CZExpStartupShutdown.cpp $
//	$Revision: 2634 $
//	$Date: 2011-03-30 08:12:33 +0200 (Wed, 30 Mar 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 9-11-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IStartupShutdownService.h"

//SDK General includes

#include "IZExpID.h"

//IZP Interfaces

//IZP General includes
//#include "ZPObjectModelHack.h"

//#include "IZPLog.h"

#define thisFileLA	enLMA_None
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

class CZExpStartupShutdown : public CPMUnknown<IStartupShutdownService>
{
public:
				CZExpStartupShutdown(IPMUnknown * boss);
	virtual		~CZExpStartupShutdown();

	//	Called by the core when app is starting up
	virtual void Startup();

	//	Called by the core when app is shutting down
	virtual void Shutdown();
protected:
private:
};


CREATE_PMINTERFACE(CZExpStartupShutdown, kZExpStartupShutdownImpl)

CZExpStartupShutdown::CZExpStartupShutdown(IPMUnknown * boss) :
CPMUnknown<IStartupShutdownService>(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZExpStartupShutdown::~CZExpStartupShutdown()
{

}

//----------------------------------------------------------------------------------------
// Startup
//	Called by the core when app is starting up
//----------------------------------------------------------------------------------------
void
CZExpStartupShutdown::Startup()
{
	//LogFunctionEnterExit;
	
	//Hack Object model for debugging.
	//ZPObjectModelHack::SwapImpls( kUtilsBoss, IID_IZEXPTEXTUTILS, ITextUtils::kDefaultIID, kZExpTextUtilsImpl );
}


//----------------------------------------------------------------------------------------
// Shutdown
//	Called by the core when app is shutting down
//----------------------------------------------------------------------------------------
void
CZExpStartupShutdown::Shutdown()
{
	//LogFunctionEnterExit;

}

