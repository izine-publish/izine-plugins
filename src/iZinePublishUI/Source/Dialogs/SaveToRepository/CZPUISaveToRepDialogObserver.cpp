//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Dialogs/SaveToRepository/CZPUISaveToRepDialogObserver.cpp $
//	$Revision: 3367 $
//	$Date: 2011-06-23 15:24:08 +0200 (Thu, 23 Jun 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 1-6-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IControlView.h"
#include "IDialogController.h"
#include "IPanelControlData.h"
#include "IStringData.h"
#include "IStringListControlData.h"
#include "ISubject.h"
#include "ITriStateControlData.h"

//SDK General includes
#include "CDialogObserver.h"

//OS / ThirdParty includes

#include "IZPUIID.h"

//IZP Interfaces

//IZP General includes
#include "AZPDBObjPopupController.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_Dialogs
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CDialogObserver
class CZPUISaveToRepDialogObserver : public inheritClass
{
public:
						CZPUISaveToRepDialogObserver(IPMUnknown * boss);
	virtual				~CZPUISaveToRepDialogObserver();

	virtual void		AutoAttach();

	virtual void		AutoDetach();

	virtual void		DoAttachDetach(
							const WidgetID& widgetID, const PMIID& pmiid, bool16 attaching);

	virtual void		Update(
							const ClassID&				theChange,
							ISubject*					theSubject,
							const PMIID&				protocol,
							void*						changedBy );
protected:
	void				HandlePopupSelectionChange(
							const ClassID&				theChange,
							ISubject*					theSubject,
							const PMIID &				protocol,
							void*						changedBy );

	void				HandleIsTemplateCheckBoxChange(
							const ClassID&				theChange,
							ISubject*					theSubject,
							const PMIID &				protocol,
							void*						changedBy );

	PMString			GetSelectedTitleID() const;
	
	IControlView *		GetWidgetView(
							const WidgetID &			inWidgetID ) const;
	
	void				RefreshEditionPopup();
	void				RefreshTitleStatusPopup();
	
private:
};


CREATE_PMINTERFACE(CZPUISaveToRepDialogObserver, kZPUISaveToRepDialogObserverImpl)

CZPUISaveToRepDialogObserver::CZPUISaveToRepDialogObserver(IPMUnknown * boss)
: inheritClass(boss)
{
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPUISaveToRepDialogObserver::~CZPUISaveToRepDialogObserver()
{
}

#pragma mark -

//----------------------------------------------------------------------------------------
// DoAttachDetach
//----------------------------------------------------------------------------------------
void
CZPUISaveToRepDialogObserver::DoAttachDetach(const WidgetID& widgetID, const PMIID& pmiid, bool16 attaching)
{
	do
	{
		InterfacePtr<const IPanelControlData> panelControlData(this, UseDefaultIID());
		ASSERT(panelControlData);
		if (!panelControlData)
		{
			break;
		}
		IControlView* view = panelControlData->FindWidget(widgetID);
		ASSERT(view);
		if (!view)
		{
			break;
		}
		InterfacePtr<ISubject> subject(view, UseDefaultIID());
		ASSERT(subject);
		if (!subject)
		{
			break;
		}
		
		if (attaching)
		{
			if (!subject->IsAttached(this, pmiid))
			{
				subject->AttachObserver(this, pmiid);
			}
		}
		else
		{
			if (subject->IsAttached(this, pmiid))
			{
				subject->DetachObserver(this, pmiid);
			}
		}
	} while (kFalse);
}



//----------------------------------------------------------------------------------------
// AutoAttach
//----------------------------------------------------------------------------------------
void
CZPUISaveToRepDialogObserver::AutoAttach()
{
	// Call the base class AutoAttach() function so that default behavior
	// will still occur (OK and Cancel buttons, etc.).
	CDialogObserver::AutoAttach();

	//TODO: check if logged. Without login, it must never come here.
	//ASSERT( gZPApp );
	//InterfacePtr<ISubject> loginControllerSubject( gZPApp, UseDefaultIID() );
	//ASSERT(loginControllerSubject);
	//if(!loginControllerSubject->IsAttached(this, IZPLoginController::kDefaultIID))
	//	loginControllerSubject->AttachObserver(this, IZPLoginController::kDefaultIID);

	do
	{
		this->DoAttachDetach(kZPUISaveToRepPublicationDropDownWidgetID, IStringListControlData::kDefaultIID, kTrue);
		this->DoAttachDetach(kZPUISaveToRepIsTemplateCheckBoxWidgetID, ITriStateControlData::kDefaultIID, kTrue);
	} while (kFalse);
}

//----------------------------------------------------------------------------------------
// AutoDetach
//----------------------------------------------------------------------------------------
void
CZPUISaveToRepDialogObserver::AutoDetach()
{
	// Call base class AutoDetach() so that default behavior will occur (OK and Cancel buttons, etc.).
	CDialogObserver::AutoDetach();
	do
	{
		//this->DoAttachDetach(kIZPSaveToRepPublicationDropDownWidgetID, IStringListControlData::kDefaultIID, kFalse);
		this->DoAttachDetach(kZPUISaveToRepIsTemplateCheckBoxWidgetID, ITriStateControlData::kDefaultIID, kFalse);
	} while (kFalse);
}

//----------------------------------------------------------------------------------------
// Update
//----------------------------------------------------------------------------------------
void
CZPUISaveToRepDialogObserver::Update
(
	const ClassID& theChange,
	ISubject* theSubject,
	const PMIID& protocol,
	void* changedBy
)
{
	LogFunctionEnterExit;
	
	// Call the base class Update function so that default behavior will still occur (OK and Cancel buttons, etc.).
	CDialogObserver::Update(theChange, theSubject, protocol, changedBy);
	do
	{
		if ( theChange == kZPUIDBObjDropDown_SelectionChangedMsg )
		{
			this->HandlePopupSelectionChange( theChange, theSubject, protocol, changedBy );
		}
		else if( theChange == kTrueStateMessage || theChange == kFalseStateMessage)
		{
			this->HandleIsTemplateCheckBoxChange( theChange, theSubject, protocol, changedBy );
		}
	} while (kFalse);
}

//----------------------------------------------------------------------------------------
// HandlePopupSelectionChange
//----------------------------------------------------------------------------------------
void
CZPUISaveToRepDialogObserver::HandlePopupSelectionChange(
	const ClassID&				theChange,
	ISubject*					theSubject,
	const PMIID &				protocol,
	void*						changedBy)
{
	LogFunctionEnterExit;

	InterfacePtr<IControlView> controlView(theSubject, UseDefaultIID());
	ASSERT(controlView);

	WidgetID theChangedWidgetID = controlView->GetWidgetID();
	if( theChangedWidgetID.Get() == kZPUISaveToRepPublicationDropDownWidgetID )
	{
		this->RefreshEditionPopup();
		this->RefreshTitleStatusPopup();
	}
}

//----------------------------------------------------------------------------------------
// HandleIsTemplateCheckBoxChange
//----------------------------------------------------------------------------------------
void
CZPUISaveToRepDialogObserver::HandleIsTemplateCheckBoxChange(
	const ClassID&				theChange,
	ISubject*					theSubject,
	const PMIID &				protocol,
	void*						changedBy)
{
	LogFunctionEnterExit;

	InterfacePtr<IControlView> controlView(theSubject, UseDefaultIID());
	ASSERT(controlView);

	WidgetID theChangedWidgetID = controlView->GetWidgetID();
	if( theChangedWidgetID.Get() == kZPUISaveToRepIsTemplateCheckBoxWidgetID )
	{
		do
		{
			InterfacePtr<IDialogController> iDlgController(this, UseDefaultIID());
			if(!iDlgController)
				break;

			InterfacePtr<const IPanelControlData> iPanelControlData(this, UseDefaultIID());
			if(!iPanelControlData)
				break;

			InterfacePtr<IControlView> statusDropDownWidgetControlView(iPanelControlData->FindWidget(kZPUISaveToRepStatusDropDownWidgetID), UseDefaultIID());
			if(!statusDropDownWidgetControlView)
				break;

			if(iDlgController->GetTriStateControlData(kZPUISaveToRepIsTemplateCheckBoxWidgetID) == ITriStateControlData::kSelected)
				statusDropDownWidgetControlView->Disable();
			else
				statusDropDownWidgetControlView->Enable();
			
		}while(false);
	}
}

//----------------------------------------------------------------------------------------
// RefreshEditionPopup
//----------------------------------------------------------------------------------------
void
CZPUISaveToRepDialogObserver::RefreshEditionPopup()
{
	LogFunctionEnterExit;

	const PMString theTitleID = this->GetSelectedTitleID();
	//if( theTitleID.empty() == kFalse )
	{
		InterfacePtr<IStringData> editionTitleID( this->GetWidgetView(kZPUISaveToRepEditionDropDownWidgetID), IID_IZPTITLEIDSTRINGDATA );
		ASSERT(editionTitleID);
		editionTitleID->Set( theTitleID );
		
		InterfacePtr<AZPDBObjPopupController> editionController( editionTitleID, IZPDBObjPopupController::kDefaultIID );
		ASSERT( editionController );

		editionController->Refresh();
	}
}

//----------------------------------------------------------------------------------------
// RefreshEditionPopup
//----------------------------------------------------------------------------------------
void
CZPUISaveToRepDialogObserver::RefreshTitleStatusPopup()
{
	LogFunctionEnterExit;

	const PMString theTitleID = this->GetSelectedTitleID();
	//if( theTitleID.empty() == kFalse )
	{
		InterfacePtr<IStringData> statusTitleID( this->GetWidgetView(kZPUISaveToRepStatusDropDownWidgetID), IID_IZPTITLEIDSTRINGDATA );
		ASSERT(statusTitleID);
		statusTitleID->Set( theTitleID );
		
		InterfacePtr<AZPDBObjPopupController> titleStatusController( statusTitleID, IZPDBObjPopupController::kDefaultIID );
		ASSERT( titleStatusController );

		titleStatusController->Refresh();
	}
}

//----------------------------------------------------------------------------------------
// GetSelectedTitleID
//----------------------------------------------------------------------------------------
PMString
CZPUISaveToRepDialogObserver::GetSelectedTitleID()const
{
	InterfacePtr<const IStringData> selectedTitleID( this->GetWidgetView(kZPUISaveToRepPublicationDropDownWidgetID), IID_IZPUISELECTEDID );
	ASSERT(selectedTitleID);

	return selectedTitleID->Get();
}

//----------------------------------------------------------------------------------------
// GetWidgetView
//----------------------------------------------------------------------------------------
IControlView *
CZPUISaveToRepDialogObserver::GetWidgetView(
	const WidgetID &			inWidgetID) const
{
	InterfacePtr<const IPanelControlData> selfPanelControlData(this, UseDefaultIID());
	ASSERT(selfPanelControlData);
	if (selfPanelControlData)
	{
		IControlView* widgetView = selfPanelControlData->FindWidget( inWidgetID );
		ASSERT(widgetView);
		return widgetView;
	}
	return NULL;
}
