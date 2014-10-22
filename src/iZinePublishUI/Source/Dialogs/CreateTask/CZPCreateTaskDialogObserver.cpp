//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Dialogs/CreateTask/CZPCreateTaskDialogObserver.cpp $
//	$Revision: 2506 $
//	$Date: 2010-11-11 09:59:06 +0100 (Thu, 11 Nov 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 23-9-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IBoolData.h"
#include "IBooleanControlData.h"
#include "IControlView.h"
#include "IDialog.h"
#include "IDialogController.h"
#include "IPanelControlData.h"
#include "IStringData.h"
#include "IStringListControlData.h"
#include "ISubject.h"
#include "IWidgetParent.h"

//SDK General includes
#include "CDialogObserver.h"

//OS / ThirdParty includes

#include "IZPUIID.h"
#include "IZPID.h"

//IZP Interfaces
#include "IZPAppController.h"
#include "IZPDBObjController.h"
#include "IZPDefaultSession.h"
#include "IZPLoginController.h"
#include "IZPUserCredentials.h"

//IZP General includes
#include "AZPDBObjPopupController.h"
#include "ZPDataHelper.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_None
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CDialogObserver
class CZPCreateTaskDialogObserver : public inheritClass
{
public:
						CZPCreateTaskDialogObserver(IPMUnknown * boss);
	virtual				~CZPCreateTaskDialogObserver();

	void				AutoAttach();
	void				AutoDetach();
	void				Update(
							const ClassID&				theChange,
							ISubject*					theSubject,
							const PMIID &				protocol,
							void*						changedBy);
	bool				HandleOKButtonPress(			//Return true if handled
							const ClassID&				theChange,
							ISubject*					theSubject,
							const PMIID &				protocol,
							void*						changedBy);
protected:
	void				DoAttachDetach(
							const WidgetID&				widgetID,
							const PMIID&				pmiid,
							bool16						attaching);

	void				HandlePopupListChange(
							const ClassID&				theChange,
							ISubject*					theSubject,
							const PMIID &				protocol,
							void*						changedBy );
	void				HandlePopupSelectionChange(
							const ClassID&				theChange,
							ISubject*					theSubject,
							const PMIID &				protocol,
							void*						changedBy );
	
	PMString			GetSelectedTitleID() const;
	PMString			GetSelectedEditionID() const;
	
	void				EnableDisableControls(
							bool						inEnable );

	IControlView *		GetWidgetView(
							const WidgetID &			inWidgetID ) const;
	
	void				RefreshEditionPopup();
	void				RefreshDocumentsPopup();
	void				RefreshAssignToUserPopup();

private:
};


CREATE_PMINTERFACE(CZPCreateTaskDialogObserver, kZPCreateTaskDialogObserverImpl)

CZPCreateTaskDialogObserver::CZPCreateTaskDialogObserver(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPCreateTaskDialogObserver::~CZPCreateTaskDialogObserver()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// AutoAttach
//----------------------------------------------------------------------------------------
void
CZPCreateTaskDialogObserver::AutoAttach()
{
	do
	{
		inheritClass::AutoAttach();
		ASSERT( gZPApp );
		InterfacePtr<ISubject> loginControllerSubject( gZPApp, UseDefaultIID() );
		ASSERT(loginControllerSubject);
		if(!loginControllerSubject->IsAttached(this, IZPLoginController::kDefaultIID))
			loginControllerSubject->AttachObserver(this, IZPLoginController::kDefaultIID);

		this->DoAttachDetach( kZPUICreateTaskDlgPopTitleWidgetID, IStringListControlData::kDefaultIID, kTrue);
		this->DoAttachDetach( kZPUICreateTaskDlgPopEditionWidgetID, IStringListControlData::kDefaultIID, kTrue);
		this->DoAttachDetach( kZPUICreateTaskDlgPopAsgnToUserWidgetID, IStringListControlData::kDefaultIID, kTrue);
		
	}while(kFalse);
}

//----------------------------------------------------------------------------------------
// AutoDetach
//----------------------------------------------------------------------------------------
void
CZPCreateTaskDialogObserver::AutoDetach()
{
	inheritClass::AutoDetach();
}

//----------------------------------------------------------------------------------------
// Update
//----------------------------------------------------------------------------------------
void
CZPCreateTaskDialogObserver::Update(
	const ClassID&				theChange,
	ISubject*					theSubject,
	const PMIID &				protocol,
	void*						changedBy)
{
	LogFunctionEnterExit;
	
	do
	{
		if( this->HandleOKButtonPress(theChange, theSubject, protocol, changedBy))
			break;

		inheritClass::Update(theChange, theSubject, protocol, changedBy);
	
		if( protocol == IZPLoginController::kDefaultIID && theChange != kZPLogin_CanLogoutMsg )
		{	//disable controls if state is not logged in, 
			this->EnableDisableControls( false );
		}
		else if ( theChange == kZPUIDBObjDropDown_ListLengthChangedMsg )
		{
			this->HandlePopupListChange( theChange, theSubject, protocol, changedBy );
		}
		else if ( theChange == kZPUIDBObjDropDown_SelectionChangedMsg )
		{
			this->HandlePopupSelectionChange( theChange, theSubject, protocol, changedBy );
		}
		else if (theChange == kTrueStateMessage)
		{
			InterfacePtr<IControlView> controlView(theSubject, UseDefaultIID());
			ASSERT(controlView);
			if(!controlView)
				break;
					
			// Get the button ID from the view.
			WidgetID widgetID = controlView->GetWidgetID();
			//switch( widgetID.Get() )
			//{
			//default:
			//	break;
			//}
		}
	}while(false);
}

//----------------------------------------------------------------------------------------
// HandleOKButtonPress
//Return true if handled
//----------------------------------------------------------------------------------------
bool
CZPCreateTaskDialogObserver::HandleOKButtonPress(
	const ClassID&				theChange,
	ISubject*					theSubject,
	const PMIID &				protocol,
	void*						changedBy)
{
	bool toReturn = false;
	do
	{
		InterfacePtr<IControlView> view(theSubject, IID_ICONTROLVIEW);
		if (!view)
			break;
	
		//----------------------------
		// Handle OK Button only
		//----------------------------
		if (theChange != kTrueStateMessage) // react only when the button is turned on
			break;
		
		if (view->GetWidgetID() != kOKButtonWidgetID)
			break;
		
		InterfacePtr<IDialogController> dialogController(this, IID_IDIALOGCONTROLLER);
		WidgetID illegalWidget = dialogController->ValidateDialog();
		
		if (illegalWidget != kDefaultWidgetId) 
			break;
				
		// Some dialogs seem to be firing off multiple commands in 
		// ApplyFields. This makes undoing the dialog as a single command
		// impossible. In order to catch those offenders, I'll group the
		// commands in a command sequence.
		PMString name;
		dialogController->GetName(name);
		CmdUtils::SequenceContext	sequence(&name);
		dialogController->ApplyDialog(view->GetWidgetID());

		//TODO: Check if can close the dialog now.
		
		InterfacePtr<IWidgetParent> parentHolder(this, IID_IWIDGETPARENT);
		InterfacePtr<IDialog> dialog((IDialog*) parentHolder->QueryParentFor(IID_IDIALOG));
		
		if( dialog->GetBufferCommands())
			FireOffEndDialogCmd(kTrue);
		
		// With some preview dialog, dialog may have already been closed due to an error
		// occuring in a sequence the dialog fires. See DialogCommandInterceptor::ProcessACommand.
		CloseDialog(!dialog->IsOpen());

		toReturn = true;
	}while(kFalse);

	return toReturn;
}

//----------------------------------------------------------------------------------------
// DoAttachDetach
//----------------------------------------------------------------------------------------
void
CZPCreateTaskDialogObserver::DoAttachDetach(
	const WidgetID&				widgetID,
	const PMIID&				pmiid,
	bool16						attaching)
{
	do
	{
		InterfacePtr<const IPanelControlData> selfPanelControlData(this, UseDefaultIID());
		ASSERT(selfPanelControlData);
		if (!selfPanelControlData)
		{
			break;
		}
		IControlView* widgetView = selfPanelControlData->FindWidget(widgetID);
		ASSERT(widgetView);
		if (!widgetView)
		{
			break;
		}
		InterfacePtr<ISubject> widgetSubject(widgetView, UseDefaultIID());
		ASSERT(widgetSubject);
		if (!widgetSubject)
		{
			break;
		}
		
		bool isAttached = widgetSubject->IsAttached(this, pmiid);
		if (attaching && !isAttached)
				widgetSubject->AttachObserver(this, pmiid);
		else if( !attaching && isAttached )
				widgetSubject->DetachObserver(this, pmiid);
	} while (kFalse);
}
#pragma mark -
//----------------------------------------------------------------------------------------
// HandlePopupListChange
//----------------------------------------------------------------------------------------
void
CZPCreateTaskDialogObserver::HandlePopupListChange(
	const ClassID&				theChange,
	ISubject*					theSubject,
	const PMIID &				protocol,
	void*						changedBy)
{
	LogFunctionEnterExit;
	InterfacePtr<IControlView> controlView(theSubject, UseDefaultIID());
	ASSERT(controlView);

	WidgetID theChangedWidgetID = controlView->GetWidgetID();
	if( theChangedWidgetID.Get() == kZPUICreateTaskDlgPopTitleWidgetID )
	{	//Refresh the edition popup.
		//this->RefreshEditionPopup();	//Commented as It should be done on selection change only.
		//this->RefreshAssignToUserPopup();	//Commented
	}
	else if( theChangedWidgetID.Get() == kZPUICreateTaskDlgPopEditionWidgetID )
	{	//Refresh the Tasks treeview
		//this->RefreshTasksTreeView();	//Commented as It should be done on selection change only.
	}
}

//----------------------------------------------------------------------------------------
// HandlePopupSelectionChange
//----------------------------------------------------------------------------------------
void
CZPCreateTaskDialogObserver::HandlePopupSelectionChange(
	const ClassID&				theChange,
	ISubject*					theSubject,
	const PMIID &				protocol,
	void*						changedBy)
{
	LogFunctionEnterExit;

	InterfacePtr<IControlView> controlView(theSubject, UseDefaultIID());
	ASSERT(controlView);

	WidgetID theChangedWidgetID = controlView->GetWidgetID();
	if( theChangedWidgetID.Get() == kZPUICreateTaskDlgPopTitleWidgetID )
	{
		this->RefreshEditionPopup();
		this->RefreshAssignToUserPopup();
		//TODO: we should disable tree view till editions popup is not refreshed.
		// as the tree view might be showing data of some other title.
	}
	else if( theChangedWidgetID.Get() == kZPUICreateTaskDlgPopEditionWidgetID )
	{	//Refresh the Assigned to Document popup
		this->RefreshDocumentsPopup();
	}
}

#pragma mark -
//----------------------------------------------------------------------------------------
// RefreshEditionPopup
//----------------------------------------------------------------------------------------
void
CZPCreateTaskDialogObserver::RefreshEditionPopup()
{
	LogFunctionEnterExit;

	const PMString theTitleID = this->GetSelectedTitleID();
	//if( theTitleID.empty() == kFalse )
	{
		InterfacePtr<IStringData> editionTitleID( this->GetWidgetView(kZPUICreateTaskDlgPopEditionWidgetID ), IID_IZPTITLEIDSTRINGDATA );
		ASSERT(editionTitleID);
		editionTitleID->Set( theTitleID );
		
		InterfacePtr<AZPDBObjPopupController> editionController( editionTitleID, IZPDBObjPopupController::kDefaultIID );
		ASSERT( editionController );

		editionController->Refresh();
	}
}

//----------------------------------------------------------------------------------------
// RefreshDocumentsPopup
//----------------------------------------------------------------------------------------
void
CZPCreateTaskDialogObserver::RefreshDocumentsPopup()
{
	LogFunctionEnterExit;
	const PMString theEditionID = this->GetSelectedEditionID();
	
	IControlView * assetsPopupControlView = this->GetWidgetView( kZPUICreateTaskDlgPopAsgnToDocWidgetID );
	ASSERT( assetsPopupControlView );

	InterfacePtr<IStringData> assetsPopEditionID( assetsPopupControlView, IID_IZPEDITIONIDSTRINGDATA );
	ASSERT( assetsPopEditionID );
	assetsPopEditionID->Set( theEditionID );

	InterfacePtr<AZPDBObjPopupController> assetsPopController( assetsPopupControlView, IZPDBObjPopupController::kDefaultIID );
	ASSERT( assetsPopController );

	assetsPopController->Refresh();
}

//----------------------------------------------------------------------------------------
// RefreshAssignToUserPopup
//----------------------------------------------------------------------------------------
void
CZPCreateTaskDialogObserver::RefreshAssignToUserPopup()
{
	LogFunctionEnterExit;

	const PMString theTitleID = this->GetSelectedTitleID();
	//if( theTitleID.empty() == kFalse )
	{
		InterfacePtr<IStringData> userTitleID( this->GetWidgetView(kZPUICreateTaskDlgPopAsgnToUserWidgetID ), IID_IZPTITLEIDSTRINGDATA );
		ASSERT(userTitleID);
		userTitleID->Set( theTitleID );
		
		InterfacePtr<AZPDBObjPopupController> userController( userTitleID, IZPDBObjPopupController::kDefaultIID );
		ASSERT( userController );

		userController->Refresh();
	}

}
#pragma mark -
//----------------------------------------------------------------------------------------
// GetSelectedTitleID
//----------------------------------------------------------------------------------------
PMString
CZPCreateTaskDialogObserver::GetSelectedTitleID()const
{
	InterfacePtr<const IStringData> selectedTitleID( this->GetWidgetView(kZPUICreateTaskDlgPopTitleWidgetID), IID_IZPUISELECTEDID );
	ASSERT(selectedTitleID);

	return selectedTitleID->Get();
}

//----------------------------------------------------------------------------------------
// GetSelectedEditionID
//----------------------------------------------------------------------------------------
PMString
CZPCreateTaskDialogObserver::GetSelectedEditionID()const
{
	InterfacePtr<const IStringData> selectedEditionID( this->GetWidgetView(kZPUICreateTaskDlgPopEditionWidgetID), IID_IZPUISELECTEDID );
	ASSERT(selectedEditionID);

	return selectedEditionID->Get();
}

//----------------------------------------------------------------------------------------
// EnableDisableControls
//----------------------------------------------------------------------------------------
void
CZPCreateTaskDialogObserver::EnableDisableControls(
	bool						inEnable)
{
	LogFunctionEnterExit;

	InterfacePtr<IControlView> selfControlView( this, UseDefaultIID());
	ASSERT(selfControlView);

	if( inEnable )
		selfControlView->Enable();
	else
		selfControlView->Disable();
}

//----------------------------------------------------------------------------------------
// GetWidgetView
//----------------------------------------------------------------------------------------
IControlView *
CZPCreateTaskDialogObserver::GetWidgetView(
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
