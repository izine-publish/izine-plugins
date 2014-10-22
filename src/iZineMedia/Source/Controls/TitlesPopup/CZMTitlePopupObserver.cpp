//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Controls/TitlesPopup/CZMTitlePopupObserver.cpp $
//	$Revision: 2715 $
//	$Date: 2011-04-06 15:51:32 +0200 (Wed, 06 Apr 2011) $
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

//SDK General includes
#include "CObserver.h"

//OS / ThirdParty includes

#include "iZMediaID.h"

//IZP Interfaces
#include "IZMApp.h"
#include "IZMTitleMgr.h"
#include "IZMDataObjPopupController.h"

//IZP General includes
#include "AZMUIDataObjDropDownObserver.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass AZMUIDataObjDropDownObserver
class CZMTitlePopupObserver : public inheritClass
{
public:
						CZMTitlePopupObserver(IPMUnknown * boss);
	virtual				~CZMTitlePopupObserver();

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


CREATE_PMINTERFACE(CZMTitlePopupObserver, kZMTitlePopupObserverImpl)

CZMTitlePopupObserver::CZMTitlePopupObserver(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZMTitlePopupObserver::~CZMTitlePopupObserver()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// AutoAttach
//----------------------------------------------------------------------------------------
void
CZMTitlePopupObserver::AutoAttach()
{
	inheritClass::AutoAttach();
	
	InterfacePtr<IZMApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<ISubject> mgrSubject( zpApp->GetTitleMgr(), UseDefaultIID() );
	ASSERT(mgrSubject);

	if(!mgrSubject->IsAttached(this, IZMTitleMgr::kDefaultIID))
		mgrSubject->AttachObserver(this, IZMTitleMgr::kDefaultIID);
}


//----------------------------------------------------------------------------------------
// AutoDetach
//----------------------------------------------------------------------------------------
void
CZMTitlePopupObserver::AutoDetach()
{
	inheritClass::AutoDetach();
	//Note: don't detach the login/config controllers.

	//TODO:
}


//----------------------------------------------------------------------------------------
// Update
//----------------------------------------------------------------------------------------
void
CZMTitlePopupObserver::Update(
	const ClassID&				theChange,
	ISubject*					theSubject,
	const PMIID &				protocol,
	void*						changedBy)
{
	inheritClass::Update( theChange, theSubject, protocol, changedBy );

	InterfacePtr<IZMDataObjPopupController> popupController( this , UseDefaultIID() );
	ASSERT(popupController);

	if( theChange == kZMTitleListWillChange )
	{
		popupController->SaveSelection();
	}
	else if( theChange == kZMTitleListChanged )
	{
		popupController->Refresh();
	}
}

