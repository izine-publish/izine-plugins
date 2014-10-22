//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/ActionComponent/CZExpAppActionComponent.cpp $
//	$Revision: 3893 $
//	$Date: 2012-02-10 10:08:26 +0100 (Fri, 10 Feb 2012) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 12-11-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IActionStateList.h"
#include "IActiveContext.h"
#include "IApplication.h"
#include "IDialog.h"
#include "IDialogCreator.h"
#include "IDialogMgr.h"
#include "IK2ServiceProvider.h"
#include "IK2ServiceRegistry.h"
#include "IPanelControlData.h"
#include "ISelectableDialogSwitcher.h"
#include "IWorkspace.h"
#ifdef InDnCS5
#include "IExecutionContext.h"
#endif

//SDK General includes
#include "CActionComponent.h"
#include "CAlert.h"
#include "CoreResTypes.h"
#include "ErrorUtils.h"
#include "LocaleSetting.h"
#include "PMLocaleId.h"
#include "RsrcSpec.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "IZExpID.h"

//IZP Interfaces
#include "IZExpPrefs.h"

//IZP General includes
//#include "IZPLog.h"

#pragma mark -

#define LogFunctionEnterExit	//IZPLog_func( enLMA_ActionComponent )

/** Implements IActionComponent; performs the actions that are executed when the plug-in's
	menu items are selected.

	@ingroup iZineExport
*/
#define inheritClass CActionComponent
class CZExpAppActionComponent : public inheritClass
{
public:
						CZExpAppActionComponent(IPMUnknown* boss);
						~CZExpAppActionComponent();

	void				DoAction(
							IActiveContext*				ac,
							ActionID					actionID,
							GSysPoint					mousePoint,
							IPMUnknown*					widget);
	void				UpdateActionStates(
							IActiveContext*				ac,
							IActionStateList *			listToUpdate,
							GSysPoint					mousePoint,
							IPMUnknown*					widget);

	void				ShowPrefsPanel();
	void				CreateExportDialog();
private:
	void DoAbout();
	
private:

private:
};

CREATE_PMINTERFACE(CZExpAppActionComponent, kZExpActionComponentImpl)

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZExpAppActionComponent::CZExpAppActionComponent(IPMUnknown* boss)
: inheritClass(boss)
{
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZExpAppActionComponent::~CZExpAppActionComponent()
{

}

//----------------------------------------------------------------------------------------
// UpdateActionStates
//----------------------------------------------------------------------------------------
void
CZExpAppActionComponent::UpdateActionStates(
	IActiveContext*				ac,
	IActionStateList *			listToUpdate,
	GSysPoint					mousePoint,
	IPMUnknown*					widget)
{
	LogFunctionEnterExit;
	
	do {
		for(int i=0; i < listToUpdate->Length(); i++) {
			ActionID actionID = listToUpdate->GetNthAction(i);
			int16 state = listToUpdate->GetNthActionState(i);

			switch (actionID.Get())
			{
				default:
					break;
			}
			listToUpdate->SetNthActionState(i, state);
		}
	} while (kFalse);
}

//----------------------------------------------------------------------------------------
// DoAction
//----------------------------------------------------------------------------------------
void
CZExpAppActionComponent::DoAction(
	IActiveContext*				ac,
	ActionID					actionID,
	GSysPoint					mousePoint,
	IPMUnknown*					widget)
{
	LogFunctionEnterExit;
	
	switch (actionID.Get())
	{
		case kIZExpAboutActionID:
		{
			this->DoAbout();
			break;
		}
		case kIZExpExportActionID:
		{
			this->CreateExportDialog();
			break;
		}
		case kIZExpPrefActionID:
		{
			this->ShowPrefsPanel();
			break;
		}
		default:
			break;
	}
}

//----------------------------------------------------------------------------------------
// DoAbout
//----------------------------------------------------------------------------------------
void
CZExpAppActionComponent::DoAbout()
{
	LogFunctionEnterExit;
	
	CAlert::ModalAlert
	(
		kIZExpAboutBoxStringKey,		// Alert string
		kOKString, 						// OK button
		kNullString, 					// No second button
		kNullString, 					// No third button
		1,								// Set OK button to default
		CAlert::eInformationIcon		// Information icon.
	);
}

//----------------------------------------------------------------------------------------
// ShowPrefsPanel
//----------------------------------------------------------------------------------------
void
CZExpAppActionComponent::ShowPrefsPanel()
{
	do
	{
#ifdef InDnCS5
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
		InterfacePtr<IK2ServiceRegistry> serviceReg(gSession, IID_IK2SERVICEREGISTRY);
		if(!serviceReg)
			break;
		
		InterfacePtr<IK2ServiceProvider> selectableDialogService(serviceReg->QueryServiceProviderByClassID(kSelectableDialogServiceImpl, kGlobalPrefsDialogBoss));
		if(!selectableDialogService)
			break;
		
		// Create a dialog using the global prefs selectable dialog service provider
		InterfacePtr<IDialogCreator> dialogCreator(selectableDialogService, IID_IDIALOGCREATOR);
		if(!dialogCreator)
			break;
		
		IDialog* dialog = dialogCreator->CreateDialog();
		if(!dialog)
			break;
		
		// Set the panel in the dialog to our preferences panel
		InterfacePtr<const IPanelControlData> panelData(dialog, UseDefaultIID());
		if(!panelData)
			break;
	
		IControlView* dialogView = panelData->FindWidget(kPreferencesDialogWidgetID);
		InterfacePtr<ISelectableDialogSwitcher> dialogSwitcher(dialogView, IID_ISELECTABLEDIALOGSWITCHER);
		if(!dialogSwitcher)
			break;
		
		// Set the service to service Impl for the global prefs switchable dialog
		dialogSwitcher->SetDialogServiceID(kGlobalPrefsDialogServiceImpl);
		dialogSwitcher->SwitchDialogPanelByID(kIZExpPrefsPanelWidgetID);	// Our panel widget

		// Open the dialog
		dialog->Open();
	}while(kFalse);
}

//----------------------------------------------------------------------------------------
// CreateExportDialog
//----------------------------------------------------------------------------------------
void
CZExpAppActionComponent::CreateExportDialog()
{
#ifdef InDnCS5
	const IExecutionContext* ec = GetExecutionContext();
	ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IWorkspace> theWorkspace(gSession->QueryWorkspace());
	ASSERT(theWorkspace);

	InterfacePtr<const IZExpPrefs> iZExpPrefs( theWorkspace, UseDefaultIID() );
	ASSERT(iZExpPrefs);
	PMString plnId = iZExpPrefs->GetPluginId();
	plnId.StripWhiteSpace(PMString::kLeadingAndTrailingWhiteSpace);
	if(plnId.CharCount() == 0)
	{
		ErrorUtils::PMSetGlobalErrorCode(kIZExpPluginIdEmptyError);
		return;
	}

	InterfacePtr<IApplication> application(gSession->QueryApplication());
	ASSERT(application);
	
	// Get the application interface and the DialogMgr.
	InterfacePtr<IDialogMgr> dialogMgr(application, UseDefaultIID());
	ASSERT(dialogMgr);
	
	// Load the plug-in's resource.
	PMLocaleId nLocale = LocaleSetting::GetLocale();
	
	RsrcSpec dialogSpec(
				nLocale,						// Locale index from PMLocaleIDs.h.
				kIZExpPluginID,					// Our Plug-in ID
				kViewRsrcType,					// This is the kViewRsrcType.
				kZExpExportDialogResourceID,	// Resource ID for our dialog.
				kTrue							// Initially visible.
				);
					
	// CreateNewDialog takes the dialogSpec created above, and also
	// the type of dialog being created (kMovableModal).
	IDialog* dialog = dialogMgr->CreateNewDialog(dialogSpec, IDialog::kMovableModal);
	ASSERT(dialog);

	dialog->Open();
	dialog->WaitForDialog();
}
