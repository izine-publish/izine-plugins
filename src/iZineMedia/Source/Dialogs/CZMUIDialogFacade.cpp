//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Dialogs/CZMUIDialogFacade.cpp $
//	$Revision: 2746 $
//	$Date: 2011-04-08 09:05:59 +0200 (Fri, 08 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 8-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IApplication.h"
#include "IDialog.h"
#include "IDialogController.h"
#include "IDialogCreator.h"
#include "IDialogMgr.h"
#include "IEnvironment.h"
#include "IIntData.h"
#include "IK2ServiceProvider.h"
#include "IK2ServiceRegistry.h"
#include "IPanelControlData.h"
#include "IStringData.h"
#include "IStringListData.h"
#include "ISelectableDialogSwitcher.h"
#include "IWindow.h"

//SDK General includes
#include "CoreResTypes.h"
#include "LocaleSetting.h"
#include "RsrcSpec.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "iZMediaID.h"

//IZP Interfaces
#include "IZMUIDialogFacade.h"

//IZP General includes
#include "ZMTypes.h"


#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZMUIDialogFacade>
class CZMUIDialogFacade : public inheritClass
{
public:
						CZMUIDialogFacade(IPMUnknown * boss);
	virtual				~CZMUIDialogFacade();
	
	void				ShowPrefs() const;
	
	///int					ShowAlertDialog(
	///						const PMString &			inMessage,
	///						const PMString &			inButton1String,
	///						const PMString &			inButton2String,
	///						const PMString &			inButton3String,
	///						const PMString &			inButton4String,
	///						int							inDefaultButton) const;
protected:
private:
};


CREATE_PMINTERFACE(CZMUIDialogFacade, kZMUIDialogFacadeImpl)

CZMUIDialogFacade::CZMUIDialogFacade(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZMUIDialogFacade::~CZMUIDialogFacade()
{

}

//----------------------------------------------------------------------------------------
// ShowPrefs
//----------------------------------------------------------------------------------------
void
CZMUIDialogFacade::ShowPrefs()const
{
	do
	{
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
		dialogSwitcher->SwitchDialogPanelByID(kZMPrefsPanelWidgetID);	// Our panel widget

		// Open the dialog
		dialog->Open();
	}while(kFalse);
}

#if 0
//----------------------------------------------------------------------------------------
// ShowAlertDialog
//----------------------------------------------------------------------------------------
int
CZMUIDialogFacade::ShowAlertDialog(
	const PMString &					inMessage,
	const PMString &					inButton1String,
	const PMString &					inButton2String,
	const PMString &					inButton3String,
	const PMString &					inButton4String,
	int									inDefaultButton) const
{
	int toReturn = inDefaultButton;
	do
	{
		// Get the application interface and the DialogMgr.
		InterfacePtr<IApplication> application(gSession->QueryApplication());
		ASSERT(application);
		if (application == nil) {
			break;
		}
		InterfacePtr<IDialogMgr> dialogMgr(application, UseDefaultIID());
		ASSERT(dialogMgr);
		if (dialogMgr == nil) {
			break;
		}
			
		// Load the plug-in's resource.
		PMLocaleId nLocale = LocaleSetting::GetLocale();
		RsrcSpec dialogSpec
		(
			nLocale,						// Locale index from PMLocaleIDs.h.
			kIZMediaPluginID,				// Our Plug-in ID
			kViewRsrcType,					// This is the kViewRsrcType.
			kZMUIAlertDialogResourceID,	// Resource ID for our dialog.
			kTrue							// Initially visible.
		);
		
		// CreateNewDialog takes the dialogSpec created above, and also
		// the type of dialog being created (kMovableModal).
		IDialog* dialog = dialogMgr->CreateNewDialog(dialogSpec, IDialog::kMovableModal);
		ASSERT(dialog);
		
		IControlView* dialogPanel = dialog->GetDialogPanel();
		ASSERT_MSG(dialogPanel != nil, "missing dialog panel.");
		
		//Copy Values
		InterfacePtr<IIntData> dlgDefaultButtonIndex(dialogPanel, UseDefaultIID());
		ASSERT( dlgDefaultButtonIndex );
		dlgDefaultButtonIndex->Set( inDefaultButton );
		
		InterfacePtr<IStringData> dlgMessage( dialogPanel, UseDefaultIID() );
		ASSERT( dlgMessage );
		dlgMessage->Set( inMessage );

		InterfacePtr<IStringListData> dlgButtonLables( dialogPanel, UseDefaultIID() );
		ASSERT( dlgButtonLables );

		K2Vector<PMString> buttonLabels;
		
		buttonLabels.push_back( inButton1String );
		buttonLabels.push_back( inButton2String );
		buttonLabels.push_back( inButton3String );
		buttonLabels.push_back( inButton4String );
		dlgButtonLables->SetStringList( buttonLabels );

		// Open the dialog.
		dialog->Open();
		dialog->WaitForDialog();

		toReturn = dlgDefaultButtonIndex->Get();
		
	}while(kFalse);
	return toReturn;
}

#endif