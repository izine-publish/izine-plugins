//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Dialogs/CZPUIDialogFacade.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 22-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
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
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes
#include "CoreResTypes.h"
#include "LocaleSetting.h"
#include "RsrcSpec.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPUIID.h"
#include "IZPID.h"

//IZP Interfaces
#include "IZPUIDialogFacade.h"
#include "IZPTaskInfo.h"

//IZP General includes
#include "ZPTypes.h"

#ifdef MACINTOSH
#include "ZPTaskHistoryWinMgr.h"
#else
#include "ZPTaskHistoryWinMgrW.h"
#endif

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZPUIDialogFacade>
class CZPUIDialogFacade : public inheritClass
{
public:
						CZPUIDialogFacade(IPMUnknown * boss);
	virtual				~CZPUIDialogFacade();
	
	void				ShowPrefs() const;
	void				ShowCreateTaskDialog(			//inTaskID is not nil if updating.
							const IStringData *			inTaskID,
							const PMString &			inEditionID,
							bool						inCopyValues ) const;

	IDialog*			CreateDialogSaveToRepository() const;
	void				SetSaveToRepositoryDialogTitle(
							const IDialog *				inDialog ) const;

	IDialog *			CreateDialogCreateTask(
							bool						inIsModeless ) const;
	
	IDialog*			CreateDialogStatusUpdate() const;

	IDialog *			CreateDialogRenameAsset(
							bool						inIsModeless ) const;

	void				ShowTaskHistoryDialog(
							const unsigned char *		inHTMLString ) const;

	int					ShowAlertDialog(
							const PMString &			inMessage,
							const PMString &			inButton1String,
							const PMString &			inButton2String,
							const PMString &			inButton3String,
							const PMString &			inButton4String,
							int							inDefaultButton) const;
protected:
private:
};


CREATE_PMINTERFACE(CZPUIDialogFacade, kZPUIDialogFacadeImpl)

CZPUIDialogFacade::CZPUIDialogFacade(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPUIDialogFacade::~CZPUIDialogFacade()
{

}

//----------------------------------------------------------------------------------------
// ShowPrefs
//----------------------------------------------------------------------------------------
void
CZPUIDialogFacade::ShowPrefs()const
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
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
		dialogSwitcher->SwitchDialogPanelByID(kIZPPrefsPanelWidgetID);	// Our panel widget

		// Open the dialog
		dialog->Open();
	}while(kFalse);
}

//----------------------------------------------------------------------------------------
// ShowCreateTaskDialog
// inTaskID is not nil if updating.
//----------------------------------------------------------------------------------------
void
CZPUIDialogFacade::ShowCreateTaskDialog(
	const IStringData *			inTaskID,
	const PMString &			inEditionID,
	bool						inCopyValues)const
{
	do
	{
		const bool showDialogAsModeless = inTaskID != nil;	//Show modal dialog for creating a new task.

		IDialog* dialog = Utils<IZPUIDialogFacade>()->CreateDialogCreateTask( showDialogAsModeless );
		ASSERT(dialog);
		
		if( showDialogAsModeless )
			dialog->SetDialogPositioningAlgorithm( IDialog::kNoPosition );

		IControlView* dialogPanel = dialog->GetDialogPanel();
		ASSERT(dialogPanel);
		
		InterfacePtr<IStringData> dlgTaskID( dialogPanel, IID_IZPDATAID );
		ASSERT( dlgTaskID );
		
		InterfacePtr<IStringData> dlgTaskSubject( dialogPanel, IID_IZPDATANAME );
		ASSERT( dlgTaskSubject );
		
		InterfacePtr<IZPTaskInfo> dlgTaskInfo( dialogPanel, UseDefaultIID() );
		ASSERT( dlgTaskInfo );

		InterfacePtr<IStringData> dlgTaskEdition( dialogPanel,IID_IZPEDITIONIDSTRINGDATA );
		ASSERT( dlgTaskEdition );

		PMString dialogTitle;
		if( !inTaskID )
		{
			dialogTitle = kIZPCreateTaskDialogTitleKey;

			dlgTaskID->Set( kNullString );
			dlgTaskSubject->Set( kNullString );
			dlgTaskInfo->Copy( nil );
			dlgTaskEdition->Set( inEditionID );
		}
		else
		{
			//ID
			if( inCopyValues )
			{
				dlgTaskID->Set( kNullString );
				dialogTitle = kIZPCreateTaskDialogTitleKey;
			}
			else
			{
				dlgTaskID->Set( inTaskID->Get() );
				dialogTitle = kIZPUpdateTaskDialogTitleKey;
			}
			
			//Subject
			InterfacePtr<const IStringData> taskSubject( inTaskID, IID_IZPDATANAME );
			ASSERT( taskSubject );
			dlgTaskSubject->Set( taskSubject->Get() );

			//Task Info
			InterfacePtr<const IZPTaskInfo> taskInfo( inTaskID, UseDefaultIID() );
			ASSERT( taskInfo );
			dlgTaskInfo->Copy( taskInfo );

			//Get Edition ID of task.
			PMString editionID = inEditionID;
			if( editionID == kNullString )
				editionID = kNullString;	//TODO: Get Edition ID of task.
			dlgTaskEdition->Set( editionID );
			
		}

		//Set the dialog title
		InterfacePtr<IWindow> dlgWindow( dialog, UseDefaultIID() );
		ASSERT( dlgWindow );
		dlgWindow->SetTitle( dialogTitle );
		dlgWindow->CenterOnScreen();


		// Open the dialog.
		if( dialog->IsOpen() )
		{	//Reset as opening it again will not replace the values in UI
			InterfacePtr<IDialogController> dlgController( dialogPanel, UseDefaultIID() );
			ASSERT( dlgController );
			dlgController->ResetDialog();
		}
		else
			dialog->Open();

		if( showDialogAsModeless )
			dlgWindow->BringToFront();

		if( !showDialogAsModeless )
			dialog->WaitForDialog();	//Someday: Don't wait for dialog, as it can only be used to view the task details.
		
		//Create or Update task will be called by dialg apply fiels.

	}while(kFalse);
}

//----------------------------------------------------------------------------------------
// CreateDialogSaveToRepository
//----------------------------------------------------------------------------------------
IDialog*
CZPUIDialogFacade::CreateDialogSaveToRepository()const
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IApplication> application(gSession->QueryApplication());
	ASSERT(application);
	
	// Get the application interface and the DialogMgr.
	InterfacePtr<IDialogMgr> dialogMgr(application, UseDefaultIID());
	ASSERT(dialogMgr);
	
	// Load the plug-in's resource.
	PMLocaleId nLocale = LocaleSetting::GetLocale();
	
	RsrcSpec dialogSpec(
				nLocale,						// Locale index from PMLocaleIDs.h.
				kIZPUIPluginID,					// Our Plug-in ID
				kViewRsrcType,					// This is the kViewRsrcType.
				kZPUISaveToRepDialogResourceID,	// Resource ID for our dialog.
				kTrue							// Initially visible.
				);
					
	// CreateNewDialog takes the dialogSpec created above, and also
	// the type of dialog being created (kMovableModal).
	IDialog* dialog = dialogMgr->CreateNewDialog(dialogSpec, IDialog::kMovableModal);
	ASSERT(dialog);
	
	return dialog;
}

//----------------------------------------------------------------------------------------
// CreateDialogCreateTask
//----------------------------------------------------------------------------------------
IDialog *
CZPUIDialogFacade::CreateDialogCreateTask(
	bool						inIsModeless )const
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IApplication> application(gSession->QueryApplication());
	ASSERT(application);
	
	// Get the application interface and the DialogMgr.
	InterfacePtr<IDialogMgr> dialogMgr(application, UseDefaultIID());
	ASSERT(dialogMgr);
	
	// Load the plug-in's resource.
	PMLocaleId nLocale = LocaleSetting::GetLocale();
	
	RsrcSpec dialogSpec(
				nLocale,						// Locale index from PMLocaleIDs.h.
				kIZPUIPluginID,					// Our Plug-in ID
				kViewRsrcType,					// This is the kViewRsrcType.
				kZPUICreateTaskDialogResourceID,	// Resource ID for our dialog.
				kTrue							// Initially visible.
				);
	
	// CreateNewDialog takes the dialogSpec created above, and also
	// the type of dialog being created (kMovableModal).
	IDialog::DialogType dlgType = IDialog::kMovableModal;
	if( inIsModeless )
		dlgType = IDialog::kModeless;
	IDialog* dialog = dialogMgr->CreateNewDialog(dialogSpec, dlgType);
	ASSERT(dialog);
	
	return dialog;
}

//----------------------------------------------------------------------------------------
// CreateDialogStatusUpdate
//----------------------------------------------------------------------------------------
IDialog*
CZPUIDialogFacade::CreateDialogStatusUpdate()const
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IApplication> application(gSession->QueryApplication());
	ASSERT(application);
	
	// Get the application interface and the DialogMgr.
	InterfacePtr<IDialogMgr> dialogMgr(application, UseDefaultIID());
	ASSERT(dialogMgr);
	
	// Load the plug-in's resource.
	PMLocaleId nLocale = LocaleSetting::GetLocale();
	
	RsrcSpec dialogSpec(
				nLocale,						// Locale index from PMLocaleIDs.h.
				kIZPUIPluginID,					// Our Plug-in ID
				kViewRsrcType,					// This is the kViewRsrcType.
				kZPUIUpdateVersionStatusDialogResourceID,	// Resource ID for our dialog.
				kTrue							// Initially visible.
				);
					
	// CreateNewDialog takes the dialogSpec created above, and also
	// the type of dialog being created (kMovableModal).
	IDialog* dialog = dialogMgr->CreateNewDialog(dialogSpec, IDialog::kMovableModal);
	ASSERT(dialog);
	
	return dialog;
}

//----------------------------------------------------------------------------------------
// CreateDialogRenameAsset
//----------------------------------------------------------------------------------------
IDialog *
CZPUIDialogFacade::CreateDialogRenameAsset(
	bool						inIsModeless) const
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IApplication> application(gSession->QueryApplication());
	ASSERT(application);
	
	// Get the application interface and the DialogMgr.
	InterfacePtr<IDialogMgr> dialogMgr(application, UseDefaultIID());
	ASSERT(dialogMgr);
	
	// Load the plug-in's resource.
	PMLocaleId nLocale = LocaleSetting::GetLocale();
	
	RsrcSpec dialogSpec(
				nLocale,						// Locale index from PMLocaleIDs.h.
				kIZPUIPluginID,					// Our Plug-in ID
				kViewRsrcType,					// This is the kViewRsrcType.
				kZPUIRenameAssetDialogResourceID,	// Resource ID for our dialog.
				kTrue							// Initially visible.
				);
	
	// CreateNewDialog takes the dialogSpec created above, and also
	// the type of dialog being created (kMovableModal).
	IDialog::DialogType dlgType = IDialog::kMovableModal;
	if( inIsModeless )
		dlgType = IDialog::kModeless;
	IDialog* dialog = dialogMgr->CreateNewDialog(dialogSpec, dlgType);
	ASSERT(dialog);
	
	return dialog;
}

//----------------------------------------------------------------------------------------
// SetSaveToRepositoryDialogTitle
//----------------------------------------------------------------------------------------
void
CZPUIDialogFacade::SetSaveToRepositoryDialogTitle(
	const IDialog *				inDialog) const
{
	const IControlView *		dlgPanelView = inDialog->GetDialogPanel();

	InterfacePtr<const IIntData> dlgAssetType( dlgPanelView, IID_IZPASSETTYPE );
	ASSERT( dlgAssetType );

	InterfacePtr<IWindow> dlgWindow( inDialog, UseDefaultIID() );
	ASSERT( dlgWindow );

	PMString dialogTitle;
	enAssetType assetType = (enAssetType)dlgAssetType->Get();//eAssetTypeNone;
	switch( assetType )
	{
	case eAssetTypeNone:
		dialogTitle = kZPUISaveToRepDialogTitle_NoneKey;
		break;
	case eAssetType_IDDocument:
		dialogTitle = kZPUISaveToRepDialogTitle_IDDocumentKey;
		break;
	case eAssetType_IDTemplate:
		dialogTitle = kZPUISaveToRepDialogTitle_IDTemplateKey;
		break;
	case eAssetType_ICTemplate:
		dialogTitle = kZPUISaveToRepDialogTitle_ICTemplateKey;
		break;
	case eAssetType_ICDocument:
		dialogTitle = kZPUISaveToRepDialogTitle_ICDocumentKey;
		break;
	case eAssetType_ICAssignment:
		dialogTitle = kZPUISaveToRepDialogTitle_ICAssignmentKey;
		break;
	case eAssetType_IDStory:
		dialogTitle = kZPUISaveToRepDialogTitle_IDStoryKey;
		break;
	}
	dlgWindow->SetTitle( dialogTitle );
}

//----------------------------------------------------------------------------------------
// ShowTaskHistoryDialog
//----------------------------------------------------------------------------------------
void
CZPUIDialogFacade::ShowTaskHistoryDialog(
	const unsigned char *				inHTMLString ) const
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
#ifdef MACINTOSH
	ShowTaskHistoryWindow( inHTMLString );
#else
	InterfacePtr<IEnvironment> env( gSession, UseDefaultIID() );
	ASSERT(env);
	InterfacePtr<IApplication> app( gSession->QueryApplication() );
	InterfacePtr<IWindow> mainWindow( app, UseDefaultIID() );
	ASSERT(mainWindow);
	ShowTaskHistoryWindow( inHTMLString, env->GetInstance(), mainWindow->GetSysWindow() );
#endif
}

//----------------------------------------------------------------------------------------
// ShowAlertDialog
//----------------------------------------------------------------------------------------
int
CZPUIDialogFacade::ShowAlertDialog(
	const PMString &					inMessage,
	const PMString &					inButton1String,
	const PMString &					inButton2String,
	const PMString &					inButton3String,
	const PMString &					inButton4String,
	int									inDefaultButton) const
{
	int toReturn = inDefaultButton;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
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
			kIZPUIPluginID,				// Our Plug-in ID
			kViewRsrcType,					// This is the kViewRsrcType.
			kZPUIAlertDialogResourceID,	// Resource ID for our dialog.
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

