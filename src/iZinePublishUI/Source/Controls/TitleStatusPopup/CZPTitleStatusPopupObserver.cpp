//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/TitleStatusPopup/CZPTitleStatusPopupObserver.cpp $
//	$Revision: 1296 $
//	$Date: 2010-06-02 14:18:19 +0200 (Wed, 02 Jun 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 2-6-2010
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
#include "IZPStatusList.h"
#include "IZPTitleStatusMgr.h"
#include "IZPDBObjPopupController.h"

//IZP General includes
#include "AZPUIDBObjDropDownObserver.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass AZPUIDBObjDropDownObserver
class CZPTitleStatusPopupObserver : public inheritClass
{
public:
						CZPTitleStatusPopupObserver(IPMUnknown * boss);
	virtual				~CZPTitleStatusPopupObserver();

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


CREATE_PMINTERFACE(CZPTitleStatusPopupObserver, kZPTitleStatusPopupObserverImpl)

CZPTitleStatusPopupObserver::CZPTitleStatusPopupObserver(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPTitleStatusPopupObserver::~CZPTitleStatusPopupObserver()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// AutoAttach
//----------------------------------------------------------------------------------------
void
CZPTitleStatusPopupObserver::AutoAttach()
{
	inheritClass::AutoAttach();

	ASSERT( gZPApp );
	InterfacePtr<ISubject> appControllerSubject( gZPApp, UseDefaultIID() );
	ASSERT(appControllerSubject);

	//if(!appControllerSubject->IsAttached(this, IZPTitleStatusMgr::kDefaultIID))
	//	appControllerSubject->AttachObserver(this, IZPTitleStatusMgr::kDefaultIID);
}


//----------------------------------------------------------------------------------------
// AutoDetach
//----------------------------------------------------------------------------------------
void
CZPTitleStatusPopupObserver::AutoDetach()
{
	inheritClass::AutoDetach();
	//Note: don't detach the login/config controllers.

	//TODO:
}


//----------------------------------------------------------------------------------------
// Update
//----------------------------------------------------------------------------------------
void
CZPTitleStatusPopupObserver::Update(
	const ClassID&				theChange,
	ISubject*					theSubject,
	const PMIID &				protocol,
	void*						changedBy)
{
	inheritClass::Update( theChange, theSubject, protocol, changedBy );

	InterfacePtr<IZPDBObjPopupController> popupController( this , UseDefaultIID() );
	ASSERT(popupController);

	if( theChange == kZPTitleStatusListWillChange )
	{
		popupController->SaveSelection();
	}
	else if( theChange == kZPTitleStatusListChanged && protocol == IZPStatusList::kDefaultIID)
	{
		popupController->Refresh();
	}
}

