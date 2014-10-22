//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/EditionsPopup/CZPEditionPopupObserver.cpp $
//	$Revision: 3027 $
//	$Date: 2011-05-18 08:07:37 +0200 (Wed, 18 May 2011) $
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
#include "IStringData.h"
#include "ISubject.h"

//SDK General includes
#include "CObserver.h"

//OS / ThirdParty includes

#include "IZPUIID.h"
#include "IZPID.h"

//IZP Interfaces
#include "IZPAppController.h"
#include "IZPEditionList.h"
#include "IZPEditionMgr.h"
#include "IZPDBObjPopupController.h"

//IZP General includes
#include "AZPUIDBObjDropDownObserver.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass AZPUIDBObjDropDownObserver
class CZPEditionPopupObserver : public inheritClass
{
public:
						CZPEditionPopupObserver(IPMUnknown * boss);
	virtual				~CZPEditionPopupObserver();

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


CREATE_PMINTERFACE(CZPEditionPopupObserver, kZPEditionPopupObserverImpl)

CZPEditionPopupObserver::CZPEditionPopupObserver(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPEditionPopupObserver::~CZPEditionPopupObserver()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// AutoAttach
//----------------------------------------------------------------------------------------
void
CZPEditionPopupObserver::AutoAttach()
{
	inheritClass::AutoAttach();

	ASSERT( gZPApp );
	InterfacePtr<ISubject> appControllerSubject( gZPApp, UseDefaultIID() );
	ASSERT(appControllerSubject);

	//if(!appControllerSubject->IsAttached(this, IZPEditionMgr::kDefaultIID))
	//	appControllerSubject->AttachObserver(this, IZPEditionMgr::kDefaultIID);
}


//----------------------------------------------------------------------------------------
// AutoDetach
//----------------------------------------------------------------------------------------
void
CZPEditionPopupObserver::AutoDetach()
{
	inheritClass::AutoDetach();
	//Note: don't detach the login/config controllers.

	//TODO:
}


//----------------------------------------------------------------------------------------
// Update
//----------------------------------------------------------------------------------------
void
CZPEditionPopupObserver::Update(
	const ClassID&				theChange,
	ISubject*					theSubject,
	const PMIID &				protocol,
	void*						changedBy)
{
	inheritClass::Update( theChange, theSubject, protocol, changedBy );

	InterfacePtr<IZPDBObjPopupController> popupController( this , UseDefaultIID() );
	ASSERT(popupController);

	if( theChange == kZPEditionListWillChange )
	{
		popupController->SaveSelection();
	}
	else if( theChange == kZPEditionListChanged && protocol == IZPEditionList::kDefaultIID)
	{
		if( changedBy )
		{	//Only Refresh if it is of associated title
			InterfacePtr<const IZPEditionList> titleEditilList( (const IPMUnknown*)changedBy, UseDefaultIID() );
			InterfacePtr<const IStringData> titleID( titleEditilList, IID_IZPDATAID);
			ASSERT( titleID );

			InterfacePtr<const IStringData> selfEditionTitleID( this, IID_IZPTITLEIDSTRINGDATA );
			ASSERT(selfEditionTitleID);
			const PMString & selfTitleID = selfEditionTitleID->Get();
			const PMString & msgTitleID = titleID->Get();
			if( selfTitleID.IsEmpty() == kFalse && selfTitleID.Compare( kTrue, msgTitleID ) == 0 )
				popupController->Refresh();
		}
		//else
		//	popupController->Refresh();
	}
}
