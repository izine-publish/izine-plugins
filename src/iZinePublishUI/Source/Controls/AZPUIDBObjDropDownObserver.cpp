//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/AZPUIDBObjDropDownObserver.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 6-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "ISubject.h"
#include "IStringListControlData.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes
#include "CObserver.h"

//OS / ThirdParty includes

#include "IZPUIID.h"
#include "IZPID.h"

//IZP Interfaces
#include "IZPApp.h"
#include "IZPAppController.h"
#include "IZPLoginController.h"
#include "IZPDBObjPopupController.h"

//IZP General includes
#include "AZPUIDBObjDropDownObserver.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_None
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

AZPUIDBObjDropDownObserver::AZPUIDBObjDropDownObserver(IPMUnknown * boss)
: inheritClass_AZPUIDBObjDropDownObserver(boss)
{
	//Patch: This will create app object. gZPApp might be nil before this, if panel was open when app quit last time.
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IZPApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	zpApp->Initialize();
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
AZPUIDBObjDropDownObserver::~AZPUIDBObjDropDownObserver()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// AutoAttach
//----------------------------------------------------------------------------------------
void
AZPUIDBObjDropDownObserver::AutoAttach()
{
	LogFunctionEnterExit;

	InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
	ASSERT(selfSubject);
	if( !selfSubject->IsAttached( this, IStringListControlData::kDefaultIID ) )
		selfSubject->AttachObserver( this, IStringListControlData::kDefaultIID );

	ASSERT( gZPApp );
	InterfacePtr<ISubject> appControllerSubject( gZPApp, UseDefaultIID() );
	ASSERT(appControllerSubject);

	if(!appControllerSubject->IsAttached(this, IZPLoginController::kDefaultIID))
	{
		appControllerSubject->AttachObserver(this, IZPLoginController::kDefaultIID);

		//Load values if already logged in.
		InterfacePtr<IZPDBObjPopupController> popupController( this , UseDefaultIID() );
		if( gZPApp->GetAppState() == eAppState_LoggedIn )
			popupController->Refresh();
		else
			popupController->ClearValues();	//Showing first time, without login, clear the values.
	}

	//Someday: Observe the IID_IZPUISELECTEDID on self, to update the currently selected value.
}


//----------------------------------------------------------------------------------------
// AutoDetach
//----------------------------------------------------------------------------------------
void
AZPUIDBObjDropDownObserver::AutoDetach()
{
	LogFunctionEnterExit;
	//Note: don't detach the login/config controllers.

	//TODO:
}


//----------------------------------------------------------------------------------------
// Update
//----------------------------------------------------------------------------------------
void
AZPUIDBObjDropDownObserver::Update(
	const ClassID&				theChange,
	ISubject*					theSubject,
	const PMIID &				protocol,
	void*						changedBy)
{
	LogFunctionEnterExit;

	InterfacePtr<IZPDBObjPopupController> popupController( this , UseDefaultIID() );
	ASSERT( popupController );

	if( theChange == kZPLogin_LoggedInMsg )
	{
		if( gZPApp->GetAppState() == eAppState_LoggedIn )
			popupController->Refresh();
	}
	else if ( theChange == kZPLogin_NotLoggedInMsg )
	{
		//TODO: enable disable.control view
	}
	else if ( theChange == kPopupChangeStateMessage )
	{
		//Update selected ID
		popupController->SaveSelection();
		popupController->SendMessage_SelectionChanged();
	}
}

//----------------------------------------------------------------------------------------
// RemoveListeningObject
//----------------------------------------------------------------------------------------
void
AZPUIDBObjDropDownObserver::RemoveListeningObject(
	const IPMUnknown *			inObjectToListen,
	const PMIID &				inProtocol)
{
	InterfacePtr<ISubject> inSubject( inObjectToListen, UseDefaultIID() );
	ASSERT(inSubject);

	if( inSubject->IsAttached( this, inProtocol ) )
		inSubject->DetachObserver( this, inProtocol );
}

//----------------------------------------------------------------------------------------
// AddListeningToObject
//----------------------------------------------------------------------------------------
void
AZPUIDBObjDropDownObserver::AddListeningToObject(
	const IPMUnknown *			inObjectToListen,
	const PMIID &				inProtocol)
{
	InterfacePtr<ISubject> inSubject( inObjectToListen, UseDefaultIID() );
	ASSERT(inSubject);

	if( false == inSubject->IsAttached( this, inProtocol ) )
		inSubject->AttachObserver( this, inProtocol );
}
