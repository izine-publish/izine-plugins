//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/TitleUsersPopup/CZPTitleUsersPopupObserver.cpp $
//	$Revision: 2214 $
//	$Date: 2010-10-01 12:46:44 +0200 (Fri, 01 Oct 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 30-9-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "ISubject.h"

//SDK General includes

//OS / ThirdParty includes

#include "IZPUIID.h"
#include "IZPID.h"

//IZP Interfaces
#include "IZPAppController.h"
#include "IZPDBObjPopupController.h"
#include "IZPUserList.h"
#include "IZPUserMgr.h"

//IZP General includes
#include "AZPUIDBObjDropDownObserver.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass AZPUIDBObjDropDownObserver
class CZPTitleUsersPopupObserver : public inheritClass
{
public:
						CZPTitleUsersPopupObserver(IPMUnknown * boss);
	virtual				~CZPTitleUsersPopupObserver();

	void				AutoAttach();
	void				AutoDetach();
	void				Update(
							const ClassID&				theChange,
							ISubject*					theSubject,
							const PMIID &				protocol,
							void*						changedBy);
protected:
private:
};


CREATE_PMINTERFACE(CZPTitleUsersPopupObserver, kZPTitleUsersPopupObserverImpl)

CZPTitleUsersPopupObserver::CZPTitleUsersPopupObserver(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPTitleUsersPopupObserver::~CZPTitleUsersPopupObserver()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// AutoAttach
//----------------------------------------------------------------------------------------
void
CZPTitleUsersPopupObserver::AutoAttach()
{
	inheritClass::AutoAttach();

	ASSERT( gZPApp );
	InterfacePtr<ISubject> appControllerSubject( gZPApp, UseDefaultIID() );
	ASSERT(appControllerSubject);

	//if(!appControllerSubject->IsAttached(this, IZPUserMgr::kDefaultIID))
	//	appControllerSubject->AttachObserver(this, IZPUserMgr::kDefaultIID);
}

//----------------------------------------------------------------------------------------
// AutoDetach
//----------------------------------------------------------------------------------------
void
CZPTitleUsersPopupObserver::AutoDetach()
{
	inheritClass::AutoDetach();
	//Note: don't detach the login/config controllers.

	//TODO:
}


//----------------------------------------------------------------------------------------
// Update
//----------------------------------------------------------------------------------------
void
CZPTitleUsersPopupObserver::Update(
	const ClassID&				theChange,
	ISubject*					theSubject,
	const PMIID &				protocol,
	void*						changedBy)
{
	inheritClass::Update( theChange, theSubject, protocol, changedBy );

	InterfacePtr<IZPDBObjPopupController> popupController( this , UseDefaultIID() );
	ASSERT(popupController);

	if( theChange == kZPUserListWillChangeMsg )
	{
		popupController->SaveSelection();
	}
	else if( theChange == kZPUserListChangedMsg && protocol == IZPUserList::kDefaultIID)
	{
		popupController->Refresh();
	}
}
