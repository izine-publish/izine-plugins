//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/TitlesPopup/CZPTitlePopupObserver.cpp $
//	$Revision: 1146 $
//	$Date: 2010-05-10 13:39:59 +0200 (Mon, 10 May 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 29-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "ISubject.h"

//SDK General includes
#include "CObserver.h"

//OS / ThirdParty includes

#include "IZPUIID.h"
#include "IZPID.h"

//IZP Interfaces
#include "IZPAppController.h"
#include "IZPTitleMgr.h"
#include "IZPDBObjPopupController.h"

//IZP General includes
#include "AZPUIDBObjDropDownObserver.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass AZPUIDBObjDropDownObserver
class CZPTitlePopupObserver : public inheritClass
{
public:
						CZPTitlePopupObserver(IPMUnknown * boss);
	virtual				~CZPTitlePopupObserver();

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


CREATE_PMINTERFACE(CZPTitlePopupObserver, kZPTitlePopupObserverImpl)

CZPTitlePopupObserver::CZPTitlePopupObserver(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPTitlePopupObserver::~CZPTitlePopupObserver()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// AutoAttach
//----------------------------------------------------------------------------------------
void
CZPTitlePopupObserver::AutoAttach()
{
	inheritClass::AutoAttach();
	
	ASSERT( gZPApp );
	InterfacePtr<ISubject> appControllerSubject( gZPApp, UseDefaultIID() );
	ASSERT(appControllerSubject);

	if(!appControllerSubject->IsAttached(this, IZPTitleMgr::kDefaultIID))
		appControllerSubject->AttachObserver(this, IZPTitleMgr::kDefaultIID);
}


//----------------------------------------------------------------------------------------
// AutoDetach
//----------------------------------------------------------------------------------------
void
CZPTitlePopupObserver::AutoDetach()
{
	inheritClass::AutoDetach();
	//Note: don't detach the login/config controllers.

	//TODO:
}


//----------------------------------------------------------------------------------------
// Update
//----------------------------------------------------------------------------------------
void
CZPTitlePopupObserver::Update(
	const ClassID&				theChange,
	ISubject*					theSubject,
	const PMIID &				protocol,
	void*						changedBy)
{
	inheritClass::Update( theChange, theSubject, protocol, changedBy );

	InterfacePtr<IZPDBObjPopupController> popupController( this , UseDefaultIID() );
	ASSERT(popupController);

	if( theChange == kZPTitleListWillChange )
	{
		popupController->SaveSelection();
	}
	else if( theChange == kZPTitleListChanged )
	{
		popupController->Refresh();
	}
}

