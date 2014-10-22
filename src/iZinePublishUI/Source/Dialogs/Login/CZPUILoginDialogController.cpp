//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Dialogs/Login/CZPUILoginDialogController.cpp $
//	$Revision: 3448 $
//	$Date: 2011-07-05 15:38:17 +0200 (Tue, 05 Jul 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 12-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IActiveContext.h"
#include "IApplication.h"
#include "IDocumentCommands.h"
#include "IDropDownListController.h"
#include "IInCopyDocUtils.h"
#include "IPanelControlData.h"
#include "IPanelMgr.h"
#include "IPMStream.h"
#include "IStringData.h"
#include "IStringListControlData.h"
#include "ITextControlData.h"
#include "ITriStateControlData.h"
#include "IUserInfo.h"
#include "IWorkspace.h"

//SDK General includes
#include "CAlert.h"
#include "CDialogController.h"
#include "Command.h"
#include "ErrorUtils.h"
#include "K2Vector.tpp"
#include "ProgressBar.h"
#include "SDKFileHelper.h"
#include "StreamUtil.h"
#include "UIDList.h"
#include "UsernameID.h"
#include "Utils.h"

#include "IZPID.h"
#include "IZPUIID.h"

//IZP Interfaces
#include "IZPUserCredentials.h"
#include "IZPWSConfig.h"
#include "IZPLoginController.h"
#include "IZPAppController.h"

//IZP General includes
#include "IZPLog.h"

#pragma mark -

#define thisFileLA	enLMA_Dialogs
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )


/** IZPOpenFromRepDialogController
 Methods allow for the initialization, validation, and application of dialog widget  values.
 Implements IDialogController based on the partial implementation CDialogController.
  
 @ingroup izinepublish
 */
class CZPUILoginDialogController : public CDialogController
{
public:
				CZPUILoginDialogController(IPMUnknown * boss);
	virtual		~CZPUILoginDialogController();

	/** Initialize each widget in the dialog with its default value.
	 Called when the dialog is opened.
	 */
	virtual void InitializeDialogFields(IActiveContext* dlgContext);
	
	/** Validate the values in the widgets.
	 By default, the widget with ID kOKButtonWidgetID causes
	 ValidateFields to be called. When all widgets are valid,
	 ApplyFields will be called.
	 @return kDefaultWidgetId if all widget values are valid, WidgetID of the widget to select otherwise.
	 
	 */
	virtual WidgetID ValidateDialogFields(IActiveContext* myContext);
	
	PMString GetEditBoxText(const WidgetID& widgetId);
	void SetEditBoxText(const WidgetID& widgetId, PMString value);

	/** Retrieve the values from the widgets and act on them.
	 @param widgetId identifies the widget on which to act.
	 */
	virtual void ApplyDialogFields(IActiveContext* myContext, const WidgetID& widgetId);

protected:
private:
	void			DoLogin(
						const IZPWSConfig *				inZPWSConfig,
						const IZPUserCredentials*		inZPUserCredentials);
	void			SaveDialogValues(
						IZPWSConfig *					ioZPWSConfig,
						IZPUserCredentials*				ioZPUserCredentials);
};


CREATE_PMINTERFACE(CZPUILoginDialogController, kZPUILoginDialogControllerImpl)

CZPUILoginDialogController::CZPUILoginDialogController(IPMUnknown * boss) :
CDialogController(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPUILoginDialogController::~CZPUILoginDialogController()
{

}

//----------------------------------------------------------------------------------------
// SetEditBoxText
//----------------------------------------------------------------------------------------

void
CZPUILoginDialogController::SetEditBoxText(
	const WidgetID& widgetId, PMString value)
{
	//Codecleanup: use SetTextControlData instead of this function
	InterfacePtr<const IPanelControlData> iPanel(this, UseDefaultIID());
	ASSERT(iPanel);
	
	InterfacePtr<IControlView> controlView (iPanel->FindWidget(widgetId), UseDefaultIID());
	ASSERT(controlView);
	
	InterfacePtr<ITextControlData> controlData(controlView, UseDefaultIID());
	ASSERT(controlData);
	
	controlData->SetString(value);
}


//----------------------------------------------------------------------------------------
// GetEditBoxText
//----------------------------------------------------------------------------------------
PMString
CZPUILoginDialogController::GetEditBoxText(
	const WidgetID& widgetId)
{
	//Codecleanup: use GetTextControlData instead of this function
	InterfacePtr<const IPanelControlData> iPanel(this, UseDefaultIID());
	ASSERT(iPanel);
	
	InterfacePtr<IControlView> controlView (iPanel->FindWidget(widgetId), UseDefaultIID());
	ASSERT(controlView);
	
	InterfacePtr<ITextControlData> controlData(controlView, UseDefaultIID());
	ASSERT(controlData);
	
	return (controlData->GetString());
}


//----------------------------------------------------------------------------------------
// InitializeDialogFields
//----------------------------------------------------------------------------------------
void
CZPUILoginDialogController::InitializeDialogFields(
	IActiveContext* dlgContext)
{
	LogFunctionEnterExit;
	
	CDialogController::InitializeDialogFields(dlgContext);
	do
	{
		InterfacePtr<const IPanelControlData> iPanelControlData(this, UseDefaultIID());
		ASSERT(iPanelControlData);
		
		InterfacePtr<const IZPUserCredentials> iZPUserCredentials(this, UseDefaultIID());
		ASSERT(iZPUserCredentials);
		
		this->SetEditBoxText(kZPUILoginDlgUsernameEditBoxWidgetID, iZPUserCredentials->GetUserName());
		this->SetEditBoxText(kZPUILoginDlgPasswordEditBoxWidgetID, iZPUserCredentials->GetPwd());

		InterfacePtr<const IZPWSConfig> iZPWSConfig( this, UseDefaultIID() );
		ASSERT(iZPWSConfig);

		PMString theProxyPort;
		theProxyPort.AppendNumber(iZPWSConfig->GetProxyPort());
		theProxyPort.SetTranslatable(kFalse);
		
		this->SetEditBoxText(kZPUILoginDlgWebserviceURLEditBoxWidgetID, iZPWSConfig->GetURL());
		this->SetEditBoxText(kZPUILoginDlgProxyServerEditBoxWidgetID, iZPWSConfig->GetProxyServer());
		this->SetEditBoxText(kZPUILoginDlgProxyPortEditBoxWidgetID, theProxyPort);
		this->SetTriStateControlData( kZPUILoginDlgUseProxyCheckBoxWidgetID, iZPWSConfig->NeedsProxyServer());

		InterfacePtr<IControlView> iProxyServerEditBoxView(iPanelControlData->FindWidget(kZPUILoginDlgProxyServerEditBoxWidgetID), UseDefaultIID());
		if(!iProxyServerEditBoxView)
			break;

		InterfacePtr<IControlView> iProxyPortEditBoxView(iPanelControlData->FindWidget(kZPUILoginDlgProxyPortEditBoxWidgetID), UseDefaultIID());
		if(!iProxyPortEditBoxView)
			break;

		if(iZPWSConfig->NeedsProxyServer())
		{
			iProxyServerEditBoxView->Enable();
			iProxyPortEditBoxView->Enable();
		}
		else
		{
			iProxyServerEditBoxView->Disable();
			iProxyPortEditBoxView->Disable();
		}

		InterfacePtr<IControlView> iLoginBtnView(iPanelControlData->FindWidget(kOKButtonWidgetID), UseDefaultIID());
		if(!iLoginBtnView)
			break;

		if( gZPApp->GetAppState() == eAppState_NotLoggedIn )
			iLoginBtnView->Enable();
		else
			iLoginBtnView->Disable();

	}while(kFalse);
}


//----------------------------------------------------------------------------------------
// ValidateDialogFields
//----------------------------------------------------------------------------------------
WidgetID
CZPUILoginDialogController::ValidateDialogFields(
	IActiveContext* myContext)
{
	LogFunctionEnterExit;
	
	WidgetID result = CDialogController::ValidateDialogFields(myContext);
	do
	{
		InterfacePtr<const IPanelControlData> iPanelControlData(this, UseDefaultIID());
		ASSERT(iPanelControlData);

		PMString theUserName = this->GetEditBoxText(kZPUILoginDlgUsernameEditBoxWidgetID);
		if (theUserName.IsEmpty())
		{
			result = kZPUILoginDlgUsernameEditBoxWidgetID;
			break;
		}

		PMString thePassword = this->GetEditBoxText(kZPUILoginDlgPasswordEditBoxWidgetID);
		if (thePassword.IsEmpty())
		{
			result = kZPUILoginDlgPasswordEditBoxWidgetID;
			break;
		}
		
		PMString server = this->GetEditBoxText(kZPUILoginDlgWebserviceURLEditBoxWidgetID);
		if (server.IsEmpty())
		{
			result = kZPUILoginDlgWebserviceURLEditBoxWidgetID;
			break;
		}

		bool16 bUseProxyServer = this->GetTriStateControlData( kZPUILoginDlgUseProxyCheckBoxWidgetID) == ITriStateControlData::kSelected;
		PMString strProxyServer;
		int32 proxyPort = 0;

		if(bUseProxyServer)
		{
			strProxyServer = this->GetEditBoxText(kZPUILoginDlgProxyServerEditBoxWidgetID);
			if(strProxyServer.IsEmpty())
			{
				result = kZPUILoginDlgProxyServerEditBoxWidgetID;
				break;
			}

			PMString strProxyPort = this->GetEditBoxText(kZPUILoginDlgProxyPortEditBoxWidgetID);
			if(strProxyPort.IsEmpty())
			{
				result = kZPUILoginDlgProxyPortEditBoxWidgetID;
				break;
			}
			proxyPort = strProxyPort.GetAsNumber();
		}
		
		InterfacePtr<IZPUserCredentials> iZPUserCredentials(this, UseDefaultIID());
		ASSERT(iZPUserCredentials);
		
		InterfacePtr<IZPWSConfig> iZPWSConfig( this, UseDefaultIID() );
		ASSERT(iZPWSConfig);
		this->SaveDialogValues(iZPWSConfig, iZPUserCredentials);
		//Send Login Request:
		this->DoLogin(iZPWSConfig, iZPUserCredentials);

		result = kZPUILoginDlgPasswordEditBoxWidgetID;	//Don't close the dialog here.
	} while (kFalse);
	
	return result;
}

//----------------------------------------------------------------------------------------
// ApplyDialogFields
//----------------------------------------------------------------------------------------
void
CZPUILoginDialogController::ApplyDialogFields(
	IActiveContext* myContext, const WidgetID& widgetId)
{
	LogFunctionEnterExit;
	
	do
	{
		InterfacePtr<IZPUserCredentials> izpUserCredentials( this, UseDefaultIID());
		ASSERT(izpUserCredentials);
		if(!izpUserCredentials)
			break;

		InterfacePtr<IZPWSConfig> iZPWSConfig( this, UseDefaultIID() );
		ASSERT(iZPWSConfig);

		this->SaveDialogValues(iZPWSConfig, izpUserCredentials);
				
		////IZPPrefs workPrefs = iWorkPrefs->GetPrefs();
		////prefs.strDocumentPath = workPrefs.strDocumentPath;
		////
		////if(prefs != workPrefs)
		////{
		////	InterfacePtr<ICommand> prefsDataCmd(CmdUtils::CreateCommand(kIZPPrefsDataCmdBoss));
		////	if(!prefsDataCmd)
		////		break;
		////	
		////	InterfacePtr<IIZPPrefsData> iPrefs(prefsDataCmd,UseDefaultIID());
		////	if(!iPrefs)
		////		break;
		////	
		////	iPrefs->SetPrefs(prefs);
		////	
		////	CmdUtils::ProcessCommand(prefsDataCmd);
		////}
	
		InterfacePtr<IBoolData> resultData(this, UseDefaultIID());
		ASSERT(resultData);
		
		resultData->Set(kTrue);
		
	}while(kFalse);
}

//----------------------------------------------------------------------------------------
// DoLogin
//----------------------------------------------------------------------------------------
void
CZPUILoginDialogController::DoLogin(
	const IZPWSConfig *					inZPWSConfig,
	const IZPUserCredentials*			inZPUserCredentials)
{
	ASSERT( gZPApp );
	InterfacePtr<IZPLoginController> loginController( gZPApp, UseDefaultIID() );
	ASSERT( loginController );
	loginController->DoLogin( inZPWSConfig, inZPUserCredentials );
}

//----------------------------------------------------------------------------------------
// SaveDialogValues
//----------------------------------------------------------------------------------------
void
CZPUILoginDialogController::SaveDialogValues(
	IZPWSConfig *					ioZPWSConfig,
	IZPUserCredentials*				ioZPUserCredentials)
	{
	do
	{
		InterfacePtr<const IPanelControlData> iPanelControlData(this, UseDefaultIID());
		if(!iPanelControlData)
			break;

		PMString theUserName = this->GetEditBoxText(kZPUILoginDlgUsernameEditBoxWidgetID);
		PMString thePassword = this->GetEditBoxText(kZPUILoginDlgPasswordEditBoxWidgetID);
		
		PMString theServerURL = this->GetEditBoxText(kZPUILoginDlgWebserviceURLEditBoxWidgetID);
		PMString strProxyServer = this->GetEditBoxText(kZPUILoginDlgProxyServerEditBoxWidgetID);
		PMString strProxyPort = this->GetEditBoxText(kZPUILoginDlgProxyPortEditBoxWidgetID);
		bool16 bUseProxyServer = this->GetTriStateControlData( kZPUILoginDlgUseProxyCheckBoxWidgetID) == ITriStateControlData::kSelected;

		// save values with dialog
		ioZPUserCredentials->SetUserName( theUserName );
		ioZPUserCredentials->SetPwd( thePassword );

		ioZPWSConfig->SetURL( theServerURL );
		ioZPWSConfig->SetNeedsProxyServer( bUseProxyServer );
		ioZPWSConfig->SetProxyServer( strProxyServer );
		ioZPWSConfig->SetProxyPort( strProxyPort.GetAsNumber() );
	}while(kFalse);
}


