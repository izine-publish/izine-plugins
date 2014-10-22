//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/Dialogs/CZExpPrefsDialogObserver.cpp $
//	$Revision: 4014 $
//	$Date: 2012-05-23 07:42:13 +0200 (Wed, 23 May 2012) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 23-5-2012
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
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
#include "CAlert.h"
#include "CDialogObserver.h"

#include "IZExpID.h"

//IZP Interfaces

//IZP General includes
#include "IZPLog.h"

#pragma mark -

#define thisFileLA	enLMA_Dialogs
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

/** Implements IObserver based on the partial implementation CDialogObserver.
 @ingroup izinepublish
 */
class CZExpPrefsDialogObserver : public CDialogObserver
{
public:
	/**
	 Constructor.
	 @param boss interface ptr from boss object on which this interface is aggregated.
	 */
	CZExpPrefsDialogObserver(IPMUnknown* boss);
	
	
	/** Destructor. */
	virtual ~CZExpPrefsDialogObserver();
	
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
};


CREATE_PMINTERFACE(CZExpPrefsDialogObserver, kZExpPrefsDialogObserverImpl)

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZExpPrefsDialogObserver::CZExpPrefsDialogObserver(IPMUnknown* boss)
: CDialogObserver(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZExpPrefsDialogObserver::~CZExpPrefsDialogObserver()
{

}


//----------------------------------------------------------------------------------------
// AutoAttach
//----------------------------------------------------------------------------------------
void
CZExpPrefsDialogObserver::AutoAttach()
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
		this->AttachToWidget(kZExpPrefsDlgUseProxyCheckBoxWidgetID, IID_ITRISTATECONTROLDATA, panelControlData );

		EnableDisableControls();
	} while (kFalse);
}

//----------------------------------------------------------------------------------------
// AutoDetach
//----------------------------------------------------------------------------------------
void
CZExpPrefsDialogObserver::AutoDetach()
{
	// Call base class AutoDetach() so that default behavior will occur (OK and Cancel buttons, etc.).
	CDialogObserver::AutoDetach();
	do
	{
		InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());
		if(!panelControlData)
			break;
		// Detach Proxy Server CheckBox
		this->DetachFromWidget(kZExpPrefsDlgUseProxyCheckBoxWidgetID, IID_ITRISTATECONTROLDATA, panelControlData );

	} while (kFalse);
}

//----------------------------------------------------------------------------------------
// Update
//----------------------------------------------------------------------------------------
void
CZExpPrefsDialogObserver::Update(
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

			if(theSelectedWidget == kZExpPrefsDlgUseProxyCheckBoxWidgetID && protocol == IID_ITRISTATECONTROLDATA)
			{
				this->EnableDisableControls();
			}
		}
	} while (kFalse);
}

//----------------------------------------------------------------------------------------
// EnableDisableControls
//----------------------------------------------------------------------------------------
void
CZExpPrefsDialogObserver::EnableDisableControls()
{
	do
	{
		InterfacePtr<const IPanelControlData> iPanelControlData(this, UseDefaultIID());
			if(!iPanelControlData)
				break;

		InterfacePtr<IDialogController> iDlgController(this, UseDefaultIID());
		if(!iDlgController)
			break;

		InterfacePtr<IControlView> iProxyServerEditBoxView(iPanelControlData->FindWidget(kZExpPrefsDlgProxyServerEditBoxWidgetID), UseDefaultIID());
		if(!iProxyServerEditBoxView)
			break;

		InterfacePtr<IControlView> iProxyPortEditBoxView(iPanelControlData->FindWidget(kZExpPrefsDlgProxyPortEditBoxWidgetID), UseDefaultIID());
		if(!iProxyPortEditBoxView)
			break;

		if(iDlgController->GetTriStateControlData(kZExpPrefsDlgUseProxyCheckBoxWidgetID) == ITriStateControlData::kSelected)
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
