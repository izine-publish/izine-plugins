//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/App/CZPUIStartupShutdown.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 8-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IObserver.h"
#include "IStartupShutdownService.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes

#include "IZPUIID.h"

//IZP Interfaces
#include "IZPUIHistoryPanelController.h"

//IZP General includes

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

class CZPUIStartupShutdown : public CPMUnknown<IStartupShutdownService>
{
public:
				CZPUIStartupShutdown(IPMUnknown * boss);
	virtual		~CZPUIStartupShutdown();

	//	Called by the core when app is starting up
	virtual void Startup();

	//	Called by the core when app is shutting down
	virtual void Shutdown();
protected:
private:
};


CREATE_PMINTERFACE(CZPUIStartupShutdown, kZPUIStartupShutdownImpl)

CZPUIStartupShutdown::CZPUIStartupShutdown(IPMUnknown * boss) :
CPMUnknown<IStartupShutdownService>(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPUIStartupShutdown::~CZPUIStartupShutdown()
{

}


//----------------------------------------------------------------------------------------
// Startup
//	Called by the core when app is starting up
//----------------------------------------------------------------------------------------
void
CZPUIStartupShutdown::Startup()
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IObserver> zpCurrentDocSelectionChangeObserver( gSession, IID_IZPUICURRDOCSELCHANGEOBSERVER );
	if( zpCurrentDocSelectionChangeObserver )
		zpCurrentDocSelectionChangeObserver->AutoAttach();
}


//----------------------------------------------------------------------------------------
// Shutdown
//	Called by the core when app is shutting down
//----------------------------------------------------------------------------------------
void
CZPUIStartupShutdown::Shutdown()
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	IZPUIHistoryPanelController * historyController = IZPUIHistoryPanelController::GetHistoryController(false);
	if (historyController)
		historyController->Release();

	InterfacePtr<IObserver> zpCurrentDocSelectionChangeObserver( gSession, IID_IZPUICURRDOCSELCHANGEOBSERVER );
	if( zpCurrentDocSelectionChangeObserver )
		zpCurrentDocSelectionChangeObserver->AutoDetach();
}

