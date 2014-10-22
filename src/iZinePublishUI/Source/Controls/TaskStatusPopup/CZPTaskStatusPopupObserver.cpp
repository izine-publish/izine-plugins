//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/TaskStatusPopup/CZPTaskStatusPopupObserver.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
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
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes

//OS / ThirdParty includes

#include "IZPUIID.h"
#include "IZPID.h"

//IZP Interfaces
#include "IZPApp.h"
#include "IZPAppController.h"
#include "IZPDBObjPopupController.h"
#include "IZPTaskStatusMgr.h"
#include "IZPTitleMgr.h"

//IZP General includes
#include "AZPUIDBObjDropDownObserver.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass AZPUIDBObjDropDownObserver
class CZPTaskStatusPopupObserver : public inheritClass
{
public:
						CZPTaskStatusPopupObserver(IPMUnknown * boss);
	virtual				~CZPTaskStatusPopupObserver();

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


CREATE_PMINTERFACE(CZPTaskStatusPopupObserver, kZPTaskStatusPopupObserverImpl)

CZPTaskStatusPopupObserver::CZPTaskStatusPopupObserver(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPTaskStatusPopupObserver::~CZPTaskStatusPopupObserver()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// AutoAttach
//----------------------------------------------------------------------------------------
void
CZPTaskStatusPopupObserver::AutoAttach()
{
	inheritClass::AutoAttach();
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif	
	InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<const IZPTaskStatusMgr> taskStatusMgr( zpApp->GetTitleMgr(), UseDefaultIID() );
	ASSERT(taskStatusMgr);

	InterfacePtr<ISubject> dataMgrSubject( taskStatusMgr, UseDefaultIID() );
	ASSERT(dataMgrSubject);

	if(!dataMgrSubject->IsAttached(this, IZPTaskStatusMgr::kDefaultIID))
		dataMgrSubject->AttachObserver(this, IZPTaskStatusMgr::kDefaultIID);
}


//----------------------------------------------------------------------------------------
// AutoDetach
//----------------------------------------------------------------------------------------
void
CZPTaskStatusPopupObserver::AutoDetach()
{
	inheritClass::AutoDetach();
	//Note: don't detach the login/config controllers.

	//TODO:
}


//----------------------------------------------------------------------------------------
// Update
//----------------------------------------------------------------------------------------
void
CZPTaskStatusPopupObserver::Update(
	const ClassID&				theChange,
	ISubject*					theSubject,
	const PMIID &				protocol,
	void*						changedBy)
{
	inheritClass::Update( theChange, theSubject, protocol, changedBy );

	InterfacePtr<IZPDBObjPopupController> popupController( this , UseDefaultIID() );
	ASSERT(popupController);

	if( theChange == kZPTaskStatusListWillChange )
	{
		popupController->SaveSelection();
	}
	else if( theChange == kZPTaskStatusListChanged )
	{
		popupController->Refresh();
	}
}

