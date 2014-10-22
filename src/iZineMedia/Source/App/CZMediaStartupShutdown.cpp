//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/App/CZMediaStartupShutdown.cpp $
//	$Revision: 2901 $
//	$Date: 2011-04-29 14:32:58 +0200 (Fri, 29 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 9-11-2010
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IObserver.h"
#include "IStartupShutdownService.h"

//SDK General includes
//#include "ITextUtils.h"
//#include "IPageList.h"
//#include "DocumentID.h"

#include "IZMediaID.h"

//IZP Interfaces
#include "IZMModelController.h"
#include "IZMApp.h"

//IZP General includes
//#include "ZPObjectModelHack.h"

#include "IZPLog.h"

#define thisFileLA	enLMA_App
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

class CZMStartupShutdown : public CPMUnknown<IStartupShutdownService>
{
public:
				CZMStartupShutdown(IPMUnknown * boss);
	virtual		~CZMStartupShutdown();

	//	Called by the core when app is starting up
	virtual void Startup();

	//	Called by the core when app is shutting down
	virtual void Shutdown();
protected:
private:
};


CREATE_PMINTERFACE(CZMStartupShutdown, kZMStartupShutdownImpl)

CZMStartupShutdown::CZMStartupShutdown(IPMUnknown * boss) :
CPMUnknown<IStartupShutdownService>(boss)
{
	LogFunctionEnterExit;
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZMStartupShutdown::~CZMStartupShutdown()
{
	LogFunctionEnterExit;
}

//----------------------------------------------------------------------------------------
// Startup
//	Called by the core when app is starting up
//----------------------------------------------------------------------------------------
void
CZMStartupShutdown::Startup()
{
	LogFunctionEnterExit;
	
	//Hack Object model for debugging.
	//ZPObjectModelHack::SwapImpls( kDocBoss, IPageList::kDefaultIID, IID_IZMPAGELIST, kPageListImpl );

	//Initializer is not always called. Do app init here as well.
	InterfacePtr<IZMApp> zmApp( gSession, UseDefaultIID() );
	ASSERT( zmApp );

	zmApp->Initialize();

	InterfacePtr<IObserver> appObserver( gSession, IID_IZMAPPOBSERVER );
	ASSERT( appObserver );
	appObserver->SetAttachIID( IID_IZMAPPOBSERVER );
	appObserver->AutoAttach();


	InterfacePtr<IZMModelController> modelController( gSession, UseDefaultIID() );
	ASSERT( modelController );

	modelController->FetchTitleList();
}


//----------------------------------------------------------------------------------------
// Shutdown
//	Called by the core when app is shutting down
//----------------------------------------------------------------------------------------
void
CZMStartupShutdown::Shutdown()
{
	LogFunctionEnterExit;

	InterfacePtr<IObserver> appObserver( gSession, IID_IZMAPPOBSERVER );
	ASSERT( appObserver );
	appObserver->AutoDetach();

	InterfacePtr<IZMApp> zmApp( gSession, UseDefaultIID() );
	ASSERT( zmApp );

	zmApp->Shutdown();
}

