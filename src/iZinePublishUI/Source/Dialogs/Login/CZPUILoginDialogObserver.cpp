//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Dialogs/Login/CZPUILoginDialogObserver.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 14-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IBoolData.h"
#include "IControlView.h"
#include "IDialogController.h"
#include "IPanelControlData.h"
#include "ISubject.h"
#include "ITextControlData.h"
#include "ITreeViewMgr.h"
#include "ITriStateControlData.h"

//SDK General includes
#include "AcquireModalCursor.h"
#include "CAlert.h"
#include "CDialogObserver.h"
#include "IconRsrcDefs.h"

#include "IZPID.h"
#include "IZPUIID.h"

//IZP Interfaces
#include "IZPAppController.h"
#include "IZPLoginController.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#include "IApplication.h"
#include "IPanelControlData.h"
#include "ITreeViewMgr.h"
#include "IPanelMgr.h"
#endif

//IZP General includes
#include "IZPLog.h"

#pragma mark -

#define thisFileLA	enLMA_Dialogs
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

/** Implements IObserver based on the partial implementation CDialogObserver.
 @ingroup izinepublish
 */
class CZPUILoginDialogObserver : public CDialogObserver
{
public:
	/**
	 Constructor.
	 @param boss interface ptr from boss object on which this interface is aggregated.
	 */
	CZPUILoginDialogObserver(IPMUnknown* boss);
	
	
	/** Destructor. */
	virtual ~CZPUILoginDialogObserver();
	
	/**
	 Called by the application to allow the observer to attach to the subjects
	 to be observed, in this case the dialog's info button widget. If you want
	 to observe other widgets on the dialog you could add them here.
	 */
	virtual void AutoAttach();
	
	/** Called by the application to allow the observer to detach from the subjects being observed. */
	virtual void AutoDetach();
	
	/**
	 Called by the host when the observed object changes, in this case when
	 the dialog's info button is clicked.
	 @param theChange specifies the class ID of the change to the subject. Frequently this is a command ID.
	 @param theSubject points to the ISubject interface for the subject that has changed.
	 @param protocol specifies the ID of the changed interface on the subject boss.
	 @param changedBy points to additional data about the change. Often this pointer indicates the class ID of the command that has caused the change.
	 */
	virtual void Update(
					const ClassID&			theChange,
					ISubject*				theSubject,
					const PMIID&			protocol,
					void*					changedBy);

private:
	void		EnableDisableControls();
	void		ShowHideProxyControls();
	void		EnableDisableBtn_Login(
					bool					inEnabled );
};


CREATE_PMINTERFACE(CZPUILoginDialogObserver, kZPUILoginDialogObserverImpl)

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPUILoginDialogObserver::CZPUILoginDialogObserver(IPMUnknown* boss)
: CDialogObserver(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPUILoginDialogObserver::~CZPUILoginDialogObserver()
{

}


//----------------------------------------------------------------------------------------
// AutoAttach
//----------------------------------------------------------------------------------------
void
CZPUILoginDialogObserver::AutoAttach()
{
	// Call the base class AutoAttach() function so that default behavior
	// will still occur (OK and Cancel buttons, etc.).
	CDialogObserver::AutoAttach();
	do
	{
		InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());
		if(!panelControlData)
			break;
		// Attach Proxy Server CheckBox
		this->AttachToWidget(kZPUILoginDlgUseProxyCheckBoxWidgetID, IID_ITRISTATECONTROLDATA, panelControlData );

		//Attach Disclosure button
		this->AttachToWidget(kZPUILoginDlgProxyExpanderWidgetID, ITriStateControlData::kDefaultIID, panelControlData);
		
		ASSERT( gZPApp );
		InterfacePtr<ISubject> loginControllerSubject( gZPApp, UseDefaultIID() );
		ASSERT(loginControllerSubject);
		if(!loginControllerSubject->IsAttached(this, IZPLoginController::kDefaultIID))
			loginControllerSubject->AttachObserver(this, IZPLoginController::kDefaultIID);

		ShowHideProxyControls();
	} while (kFalse);
}

//----------------------------------------------------------------------------------------
// AutoDetach
//----------------------------------------------------------------------------------------
void
CZPUILoginDialogObserver::AutoDetach()
{
	// Call base class AutoDetach() so that default behavior will occur (OK and Cancel buttons, etc.).
	CDialogObserver::AutoDetach();
	do
	{
		InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());
		if(!panelControlData)
			break;
		// Detach Proxy Server CheckBox
		this->DetachFromWidget(kZPUILoginDlgUseProxyCheckBoxWidgetID, IID_ITRISTATECONTROLDATA, panelControlData );

		//Detach Disclosure button
		this->DetachFromWidget(kZPUILoginDlgProxyExpanderWidgetID, ITriStateControlData::kDefaultIID, panelControlData);

		InterfacePtr<ISubject> loginControllerSubject( gZPApp, UseDefaultIID() );
		ASSERT(loginControllerSubject);
		if(loginControllerSubject->IsAttached(this, IZPLoginController::kDefaultIID))
			loginControllerSubject->DetachObserver(this, IZPLoginController::kDefaultIID);
	} while (kFalse);
}

//----------------------------------------------------------------------------------------
// Update
//----------------------------------------------------------------------------------------
void
CZPUILoginDialogObserver::Update(
	const ClassID&			theChange,
	ISubject*				theSubject,
	const PMIID&			protocol,
	void*					changedBy)
{
	LogFunctionEnterExit;
	
	// Call the base class Update function so that default behavior will still occur (OK and Cancel buttons, etc.).
	CDialogObserver::Update(theChange, theSubject, protocol, changedBy);
	
	do
	{
		if( theChange == kTrueStateMessage || theChange == kFalseStateMessage)
		{
			InterfacePtr<IControlView> controlView(theSubject, UseDefaultIID());
			ASSERT(controlView);
			if(!controlView)
				break;
			
			// Get the button ID from the view.
			WidgetID theSelectedWidget = controlView->GetWidgetID();

			if(theSelectedWidget == kZPUILoginDlgUseProxyCheckBoxWidgetID && protocol == IID_ITRISTATECONTROLDATA)
			{
				this->EnableDisableControls();
			}
		}
		else if(theChange == kZPUIDisclosureBtnStateChangedMsg && protocol == ITriStateControlData::kDefaultIID)
		{
			this->ShowHideProxyControls();
		}
		else if ( theChange == kZPLogin_BeforeLoginMsg )
		{
			//Someday: busy cursor
			this->EnableDisableBtn_Login( kFalse );
#if defined(InDnCS5) || defined(InDnCS5_5)			
			do{
				const IExecutionContext* ec = GetExecutionContext();
				ISession* gSession = ec->GetSession();

				InterfacePtr<IApplication> application(gSession->QueryApplication());
				ASSERT(application);

				InterfacePtr<IPanelMgr> panelMgr(application->QueryPanelManager());
				ASSERT(panelMgr);				

				IControlView* assetsPanelView = panelMgr->GetPanelFromWidgetID( kZPUIAssetsPanelWidgetID );
				ASSERT(panelMgr);				

				InterfacePtr<IPanelControlData> panelData(assetsPanelView, UseDefaultIID());
				ASSERT(panelData);				

				IControlView* treeWidget = panelData->FindWidget(kZPUIAssetsPanelLstAssetsWidgetID);				
				ASSERT(treeWidget);							

				InterfacePtr<ITreeViewMgr> iTreeViewMgr(treeWidget, UseDefaultIID() );
				ASSERT(iTreeViewMgr);				

				iTreeViewMgr->ClearTree(kTrue);		// Clear tree
				iTreeViewMgr->ChangeRoot(kTrue);		
			}while(false);
#endif
		}
		else if ( theChange == kZPLogin_LoggedInMsg )
		{
			this->EnableDisableBtn_Login( kTrue );
			//TODO: check the login state.
			this->CloseDialog();
		}
		else if ( theChange == kZPLogin_NotLoggedInMsg )
		{
			this->EnableDisableBtn_Login( kTrue );
		}
		
	} while (kFalse);
}

//----------------------------------------------------------------------------------------
// EnableDisableControls
//----------------------------------------------------------------------------------------
void
CZPUILoginDialogObserver::EnableDisableControls()
{
	do
	{
		InterfacePtr<const IPanelControlData> iPanelControlData(this, UseDefaultIID());
			if(!iPanelControlData)
				break;

		InterfacePtr<IDialogController> iDlgController(this, UseDefaultIID());
		if(!iDlgController)
			break;

		InterfacePtr<IControlView> iProxyServerEditBoxView(iPanelControlData->FindWidget(kZPUILoginDlgProxyServerEditBoxWidgetID), UseDefaultIID());
		if(!iProxyServerEditBoxView)
			break;

		InterfacePtr<IControlView> iProxyPortEditBoxView(iPanelControlData->FindWidget(kZPUILoginDlgProxyPortEditBoxWidgetID), UseDefaultIID());
		if(!iProxyPortEditBoxView)
			break;

		if(iDlgController->GetTriStateControlData(kZPUILoginDlgUseProxyCheckBoxWidgetID) == ITriStateControlData::kSelected)
		{
			iProxyServerEditBoxView->Enable();
			iProxyPortEditBoxView->Enable();
		}
		else
		{
			iProxyServerEditBoxView->Disable();
			iProxyPortEditBoxView->Disable();
		}
	}while(kFalse);
}

//----------------------------------------------------------------------------------------
// ShowHideProxyControls
//----------------------------------------------------------------------------------------
void
CZPUILoginDialogObserver::ShowHideProxyControls()
{
	do
	{
		InterfacePtr<const IPanelControlData> iPanelControlData(this, UseDefaultIID());
		if(!iPanelControlData)
			break;

		InterfacePtr<IControlView> iProxyServerGroupView(iPanelControlData->FindWidget(kZPUILoginDlgProxyGroupWidgetID), UseDefaultIID());
		if(!iProxyServerGroupView)
			break;

		InterfacePtr<IBoolData> isExpandedBoolData(iPanelControlData->FindWidget(kZPUILoginDlgProxyExpanderWidgetID), IID_IZPISEXPANDED);
		if(!isExpandedBoolData)
			break;

		InterfacePtr<IControlView> iSperatorView(iPanelControlData->FindWidget(kZPUILoginDlgProxySperatorWidgetID), UseDefaultIID());
		if(!iSperatorView)
			break;

		bool flagProxyVisible = false;
		if ( isExpandedBoolData->Get() )
		{
			iProxyServerGroupView->Show();
			iSperatorView->Hide();
			flagProxyVisible = true;
		}
		else
		{
			iProxyServerGroupView->Hide();
			iSperatorView->Show();
			flagProxyVisible = false;
		}

		//Manage height of dialog and others
		InterfacePtr<IControlView> loginDialogView(iPanelControlData->FindWidget(kZPUILoginDlgWidgetID), UseDefaultIID());
		if(!loginDialogView)
			break;

		InterfacePtr<IControlView> okButtonView(iPanelControlData->FindWidget(kOKButtonWidgetID), UseDefaultIID());
		if(!okButtonView)
			break;

		InterfacePtr<IControlView> cancelButtonView(iPanelControlData->FindWidget(kCancelButton_WidgetID), UseDefaultIID());
		if(!cancelButtonView)
			break;

		PMRect groupFrame = iProxyServerGroupView->GetFrame();
		PMRect loginDialogFrame = loginDialogView->GetFrame();
		PMRect okButtonFrame = okButtonView->GetFrame();
		PMRect cancelButtonFrame = cancelButtonView->GetFrame();

		if (flagProxyVisible)
		{
			okButtonFrame.Top(groupFrame.Bottom() + 10);
			okButtonFrame.Bottom(okButtonFrame.Top() + 20);
			okButtonView->SetFrame(okButtonFrame);

			cancelButtonFrame.Top(groupFrame.Bottom() + 10);
			cancelButtonFrame.Bottom(cancelButtonFrame.Top() + 20);
			cancelButtonView->SetFrame(cancelButtonFrame);
		}
		else
		{
			InterfacePtr<IControlView> iProxyServerDisclosureView(isExpandedBoolData, UseDefaultIID());
			if(!iProxyServerDisclosureView)
				break;

			PMRect disclosureFrame = iProxyServerDisclosureView->GetFrame();
			okButtonFrame.Top(disclosureFrame.Bottom() + 10);
			okButtonFrame.Bottom(okButtonFrame.Top() + 20);
			okButtonView->SetFrame(okButtonFrame);

			cancelButtonFrame.Top(disclosureFrame.Bottom() + 10);
			cancelButtonFrame.Bottom(cancelButtonFrame.Top() + 20);
			cancelButtonView->SetFrame(cancelButtonFrame);
		}

		loginDialogFrame.SetHeight(okButtonFrame.Bottom() + 20);
		loginDialogView->SetFrame(loginDialogFrame);

	}while(kFalse);
}

//----------------------------------------------------------------------------------------
// EnableDisableBtn_Login
//----------------------------------------------------------------------------------------
void
CZPUILoginDialogObserver::EnableDisableBtn_Login(
	bool					inEnabled)
{
	do
	{
		InterfacePtr<const IPanelControlData> iPanelControlData(this, UseDefaultIID());
			if(!iPanelControlData)
				break;

		InterfacePtr<IControlView> okBtnView(iPanelControlData->FindWidget(kOKButtonWidgetID), UseDefaultIID());
		if(!okBtnView)
			break;

		if(inEnabled)
			okBtnView->Enable();
		else
			okBtnView->Disable();
	}while(kFalse);
}


