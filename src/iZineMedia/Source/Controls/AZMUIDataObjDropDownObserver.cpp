//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Controls/AZMUIDataObjDropDownObserver.cpp $
//	$Revision: 2902 $
//	$Date: 2011-04-29 14:55:17 +0200 (Fri, 29 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 6-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "ISubject.h"
#include "IStringListControlData.h"

//SDK General includes
#include "CObserver.h"

//OS / ThirdParty includes

#include "iZMediaID.h"

//IZP Interfaces
#include "IZMApp.h"
#include "IZMDataObjPopupController.h"

//IZP General includes
#include "AZMUIDataObjDropDownObserver.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_None
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

AZMUIDataObjDropDownObserver::AZMUIDataObjDropDownObserver(IPMUnknown * boss)
: inheritClass_AZMUIDataObjDropDownObserver(boss)
{
	//Patch: This will create app object. gZPApp might be nil before this, if panel was open when app quit last time.
	InterfacePtr<IZMApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	zpApp->Initialize();
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
AZMUIDataObjDropDownObserver::~AZMUIDataObjDropDownObserver()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// AutoAttach
//----------------------------------------------------------------------------------------
void
AZMUIDataObjDropDownObserver::AutoAttach()
{
	LogFunctionEnterExit;

	if( gSession->GetState() == ISession::kShuttingDown )
		return;

	InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
	ASSERT(selfSubject);
	if( !selfSubject->IsAttached( this, IStringListControlData::kDefaultIID ) )
		selfSubject->AttachObserver( this, IStringListControlData::kDefaultIID );

	//Load values if already logged in.
	InterfacePtr<IZMDataObjPopupController> popupController( this , UseDefaultIID() );
	//if( gZPApp->GetAppState() == eAppState_LoggedIn )
		popupController->Refresh();
	//else
	//	popupController->ClearValues();	//Showing first time, without login, clear the values.
}


//----------------------------------------------------------------------------------------
// AutoDetach
//----------------------------------------------------------------------------------------
void
AZMUIDataObjDropDownObserver::AutoDetach()
{
	LogFunctionEnterExit;
	//Note: don't detach the login/config controllers.

	//TODO:
}


//----------------------------------------------------------------------------------------
// Update
//----------------------------------------------------------------------------------------
void
AZMUIDataObjDropDownObserver::Update(
	const ClassID&				theChange,
	ISubject*					theSubject,
	const PMIID &				protocol,
	void*						changedBy)
{
	LogFunctionEnterExit;

	InterfacePtr<IZMDataObjPopupController> popupController( this , UseDefaultIID() );
	ASSERT( popupController );

	if ( theChange == kPopupChangeStateMessage )
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
AZMUIDataObjDropDownObserver::RemoveListeningObject(
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
AZMUIDataObjDropDownObserver::AddListeningToObject(
	const IPMUnknown *			inObjectToListen,
	const PMIID &				inProtocol)
{
	InterfacePtr<ISubject> inSubject( inObjectToListen, UseDefaultIID() );
	ASSERT(inSubject);

	if( false == inSubject->IsAttached( this, inProtocol ) )
		inSubject->AttachObserver( this, inProtocol );
}
