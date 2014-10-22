//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/TaskCategoryPopup/CZPTaskCategoryPopupObserver.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 14-10-2010
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
#include "IZPTaskCategoryMgr.h"
#include "IZPTitleMgr.h"

//IZP General includes
#include "AZPUIDBObjDropDownObserver.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass AZPUIDBObjDropDownObserver
class CZPTaskCategoryPopupObserver : public inheritClass
{
public:
						CZPTaskCategoryPopupObserver(IPMUnknown * boss);
	virtual				~CZPTaskCategoryPopupObserver();

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


CREATE_PMINTERFACE(CZPTaskCategoryPopupObserver, kZPTaskCategoryPopupObserverImpl)

CZPTaskCategoryPopupObserver::CZPTaskCategoryPopupObserver(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPTaskCategoryPopupObserver::~CZPTaskCategoryPopupObserver()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// AutoAttach
//----------------------------------------------------------------------------------------
void
CZPTaskCategoryPopupObserver::AutoAttach()
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	inheritClass::AutoAttach();
	
	InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<const IZPTaskCategoryMgr> taskCategoryMgr( zpApp->GetTitleMgr(), UseDefaultIID() );
	ASSERT(taskCategoryMgr);

	InterfacePtr<ISubject> dataMgrSubject( taskCategoryMgr, UseDefaultIID() );
	ASSERT(dataMgrSubject);

	if(!dataMgrSubject->IsAttached(this, IZPTaskCategoryMgr::kDefaultIID))
		dataMgrSubject->AttachObserver(this, IZPTaskCategoryMgr::kDefaultIID);
}


//----------------------------------------------------------------------------------------
// AutoDetach
//----------------------------------------------------------------------------------------
void
CZPTaskCategoryPopupObserver::AutoDetach()
{
	inheritClass::AutoDetach();
	//Note: don't detach the login/config controllers.

	//TODO:
}


//----------------------------------------------------------------------------------------
// Update
//----------------------------------------------------------------------------------------
void
CZPTaskCategoryPopupObserver::Update(
	const ClassID&				theChange,
	ISubject*					theSubject,
	const PMIID &				protocol,
	void*						changedBy)
{
	inheritClass::Update( theChange, theSubject, protocol, changedBy );

	InterfacePtr<IZPDBObjPopupController> popupController( this , UseDefaultIID() );
	ASSERT(popupController);

	if( theChange == kZPTaskCategoryListWillChange )
	{
		popupController->SaveSelection();
	}
	else if( theChange == kZPTaskCategoryListChanged )
	{
		popupController->Refresh();
	}
}

