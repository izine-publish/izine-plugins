//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/App/CZPAppObserver.cpp $
//	$Revision: 3566 $
//	$Date: 2011-08-04 12:14:39 +0200 (Thu, 04 Aug 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 29-3-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "ISubject.h"

//SDK General includes
#include "CObserver.h"

#include "IZPID.h"

//IZP Interfaces
#include "IZPAppController.h"
#include "IZPConfigController.h"
#include "IZPLoginController.h"

//IZP General includes

#include "IZPLog.h"

#define thisFileLA	enLMA_App
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

class CZPAppObserver : public CObserver
{
public:
				CZPAppObserver(IPMUnknown * boss);
	virtual		~CZPAppObserver();

	/** Called by the startup to allow the observer to detach from the subjects being observed. */
	virtual void AutoAttach();
	
	/** Called by the shutdown to allow the observer to detach from the subjects being observed. */
	virtual void AutoDetach();

	void		Update(const ClassID& theChange, ISubject* theSubject, const PMIID &protocol, void* changedBy);
protected:
private:
};


CREATE_PMINTERFACE(CZPAppObserver, kZPAppObserverImpl)

CZPAppObserver::CZPAppObserver(IPMUnknown * boss) :
CObserver(boss)
{

}

CZPAppObserver::~CZPAppObserver()
{

}

//----------------------------------------------------------------------------------------
// AutoAttach
//----------------------------------------------------------------------------------------
void
CZPAppObserver::AutoAttach()
{
	LogFunctionEnterExit;
	do
	{
		InterfacePtr<ISubject> selfSubject(this, UseDefaultIID());
		ASSERT(selfSubject);
		if (!selfSubject)
			break;
		
		if(!selfSubject->IsAttached(this, IZPLoginController::kDefaultIID))
			selfSubject->AttachObserver(this, IZPLoginController::kDefaultIID);
		if(!selfSubject->IsAttached(this, IZPConfigController::kDefaultIID))
			selfSubject->AttachObserver(this, IZPConfigController::kDefaultIID);
	}while(kFalse);
}

//----------------------------------------------------------------------------------------
// AutoDetach
//----------------------------------------------------------------------------------------
void
CZPAppObserver::AutoDetach()
{
	LogFunctionEnterExit;
	do
	{
		InterfacePtr<ISubject> selfSubject(this, UseDefaultIID());
		ASSERT(selfSubject);
		if (!selfSubject)
			break;
		
		if(selfSubject->IsAttached(this, IZPLoginController::kDefaultIID))
			selfSubject->DetachObserver(this, IZPLoginController::kDefaultIID);
		if(selfSubject->IsAttached(this, IZPConfigController::kDefaultIID))
			selfSubject->DetachObserver(this, IZPConfigController::kDefaultIID);
	}while(kFalse);
}

//----------------------------------------------------------------------------------------
// Update
//----------------------------------------------------------------------------------------
void
CZPAppObserver::Update(
	const ClassID& theChange, ISubject* theSubject, const PMIID &protocol, void* changedBy)
{
	//LogFunctionEnterExit;
	//Login messages
	if( theChange == kZPLogin_AuthenticationInProgressMsg )
		gZPApp->SetAppState( eAppState_Authenticating );
	else if( theChange == kZPLogin_AuthenticationDoneMsg )
	{
		gZPApp->SetAppState( eAppState_Authenticated );
		InterfacePtr<IZPConfigController> configController( this, UseDefaultIID() );
		ASSERT( configController );
		configController->FetchAllConfigData();
	}
	else if( theChange == kZPLogin_ConfigDataFetchingMsg 
		|| theChange == kZPLogin_ConfigDataFetchedMsg )
	{
		InterfacePtr<IZPLoginController> loginController( this, UseDefaultIID() );
		ASSERT( loginController );
		if( theChange == kZPLogin_ConfigDataFetchedMsg )
			loginController->ReduceLoginWaitCount();
		else
			loginController->AddLoginWaitCount();
	}
	else if( theChange == kZPLogin_BeforeLoggedInMsg)
		gZPApp->SetAppState( eAppState_LoggedIn );
	else if ( theChange == kZPLogin_NotLoggedInMsg )
		gZPApp->SetAppState( eAppState_NotLoggedIn );
}

