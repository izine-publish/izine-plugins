//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Controls/EditionsPopup/CZMEditionPopupObserver.cpp $
//	$Revision: 3028 $
//	$Date: 2011-05-18 08:09:30 +0200 (Wed, 18 May 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 7-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
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

#include "iZMediaID.h"

//IZP Interfaces
#include "IZMEditionList.h"
#include "IZMEditionMgr.h"
#include "IZMDataObjPopupController.h"

//IZP General includes
#include "AZMUIDataObjDropDownObserver.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass AZMUIDataObjDropDownObserver
class CZMEditionPopupObserver : public inheritClass
{
public:
						CZMEditionPopupObserver(IPMUnknown * boss);
	virtual				~CZMEditionPopupObserver();

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


CREATE_PMINTERFACE(CZMEditionPopupObserver, kZMEditionPopupObserverImpl)

CZMEditionPopupObserver::CZMEditionPopupObserver(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZMEditionPopupObserver::~CZMEditionPopupObserver()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// AutoAttach
//----------------------------------------------------------------------------------------
void
CZMEditionPopupObserver::AutoAttach()
{
	inheritClass::AutoAttach();

}


//----------------------------------------------------------------------------------------
// AutoDetach
//----------------------------------------------------------------------------------------
void
CZMEditionPopupObserver::AutoDetach()
{
	inheritClass::AutoDetach();
	//Note: don't detach the login/config controllers.

}


//----------------------------------------------------------------------------------------
// Update
//----------------------------------------------------------------------------------------
void
CZMEditionPopupObserver::Update(
	const ClassID&				theChange,
	ISubject*					theSubject,
	const PMIID &				protocol,
	void*						changedBy)
{
	inheritClass::Update( theChange, theSubject, protocol, changedBy );

	InterfacePtr<IZMDataObjPopupController> popupController( this , UseDefaultIID() );
	ASSERT(popupController);

	if( theChange == kZMEditionListWillChange )
	{
		popupController->SaveSelection();
	}
	else if( theChange == kZMEditionListChanged && protocol == IZMEditionList::kDefaultIID)
	{
		if( changedBy )
		{	//Only Refresh if it is of associated title
			InterfacePtr<const IZMEditionList> titleEditilList( (const IPMUnknown*)changedBy, UseDefaultIID() );
			InterfacePtr<const IStringData> titleID( titleEditilList, IID_IZMDATAID);
			ASSERT( titleID );

			InterfacePtr<const IStringData> selfEditionTitleID( this, IID_IZMTITLEIDSTRINGDATA );
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
