//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/Dialogs/CZExpPrefsDialogController.cpp $
//	$Revision: 4013 $
//	$Date: 2012-05-22 15:06:00 +0200 (Tue, 22 May 2012) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Aman Alam
//	Created: 6-6-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IPanelControlData.h"
#include "ITextControlData.h"
#include "IWorkspace.h"
#ifdef InDnCS5
#include "IExecutionContext.h"
#endif

//SDK General includes
#include "CDialogController.h"
#include "UIDList.h"

//OS / ThirdParty includes

#include "IZExpID.h"

//IZP Interfaces
#include "IZExpPrefs.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_Dialogs
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CDialogController
class CZExpPrefsDialogController : public inheritClass
{
public:
						CZExpPrefsDialogController(IPMUnknown * boss);
	virtual				~CZExpPrefsDialogController();

	/** Initializes each widget in the dialog with its default value.
	 Called when the dialog is opened.
	 @param dlgContext
	 */
	void				InitializeDialogFields(IActiveContext* dlgContext);
	
	WidgetID			ValidateDialogFields(IActiveContext* myContext);
	/**	Retrieve the values from the widgets and act on them.
	 @param myContext
	 @param widgetId identifies the widget on which to act.
	 */
	void				ApplyDialogFields(IActiveContext* myContext, const WidgetID& widgetId);

	PMString			GetEditBoxText(const WidgetID& widgetId);
	void				SetEditBoxText(const WidgetID& widgetId, PMString value);

protected:
private:
};


CREATE_PMINTERFACE(CZExpPrefsDialogController, kZExpPrefsDialogControllerImpl)

CZExpPrefsDialogController::CZExpPrefsDialogController(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZExpPrefsDialogController::~CZExpPrefsDialogController()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// InitializeDialogFields
//----------------------------------------------------------------------------------------
void
CZExpPrefsDialogController::InitializeDialogFields(
	IActiveContext* dlgContext)
{
#ifdef InDnCS5
	const IExecutionContext* ec = GetExecutionContext();
	ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IWorkspace> iWorkspace(gSession->QueryWorkspace());

	InterfacePtr<const IZExpPrefs> expPrefs( iWorkspace, UseDefaultIID() );
	ASSERT(expPrefs);

	const PMString & userName = expPrefs->GetUserName();
	this->SetTextControlData( kZExpUserNameWidgetID, userName );

	const PMString & password = expPrefs->GetPassword();
	this->SetTextControlData( kZExpPasswordWidgetID, password );

	const PMString & pluginId = expPrefs->GetPluginId();
	this->SetTextControlData( kZExpPluginIdWidgetID, pluginId );

	PMString theProxyPort;
	theProxyPort.AppendNumber(expPrefs->GetProxyPort());
	theProxyPort.SetTranslatable(kFalse);
		
	this->SetEditBoxText(kZExpPrefsDlgProxyServerEditBoxWidgetID, expPrefs->GetProxyServer());
	this->SetEditBoxText(kZExpPrefsDlgProxyPortEditBoxWidgetID, theProxyPort);
	this->SetTriStateControlData( kZExpPrefsDlgUseProxyCheckBoxWidgetID, expPrefs->NeedsProxyServer());
}

//----------------------------------------------------------------------------------------
// ValidateDialogFields
//----------------------------------------------------------------------------------------
WidgetID
CZExpPrefsDialogController::ValidateDialogFields(
	IActiveContext* myContext)
{
	LogFunctionEnterExit;
	
	WidgetID result = CDialogController::ValidateDialogFields(myContext);
	do
	{
		InterfacePtr<const IPanelControlData> iPanelControlData(this, UseDefaultIID());
		ASSERT(iPanelControlData);

		bool16 bUseProxyServer = this->GetTriStateControlData( kZExpPrefsDlgUseProxyCheckBoxWidgetID) == ITriStateControlData::kSelected;
		PMString strProxyServer;
		int32 proxyPort = 0;

		if(bUseProxyServer)
		{
			strProxyServer = this->GetEditBoxText(kZExpPrefsDlgProxyServerEditBoxWidgetID);
			if(strProxyServer.IsEmpty())
			{
				result = kZExpPrefsDlgProxyServerEditBoxWidgetID;
				break;
			}

			PMString strProxyPort = this->GetEditBoxText(kZExpPrefsDlgProxyPortEditBoxWidgetID);
			if(strProxyPort.IsEmpty())
			{
				result = kZExpPrefsDlgProxyPortEditBoxWidgetID;
				break;
			}
			proxyPort = strProxyPort.GetAsNumber();
			if( proxyPort == 0 )
			{
				result = kZExpPrefsDlgProxyPortEditBoxWidgetID;
				break;
			}
		}
	} while (kFalse);
	
	return result;
}

//----------------------------------------------------------------------------------------
// ApplyDialogFields
//----------------------------------------------------------------------------------------
void
CZExpPrefsDialogController::ApplyDialogFields(
	IActiveContext* myContext, const WidgetID& widgetId)
{
	InterfacePtr<ICommand> saveExpPrefsCmd(CmdUtils::CreateCommand(kZExpSavePrefsCmdBoss));
	if(!saveExpPrefsCmd)
		return;

#ifdef InDnCS5
	const IExecutionContext* ec = GetExecutionContext();
	ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IWorkspace> sessionWS( gSession->QueryWorkspace() );
	saveExpPrefsCmd->SetItemList( UIDList( sessionWS ) );

	InterfacePtr<IZExpPrefs> expPrefs( saveExpPrefsCmd, UseDefaultIID() );
	ASSERT(expPrefs);

	PMString userName = this->GetTextControlData( kZExpUserNameWidgetID );
	expPrefs->SetUserName( userName );

	PMString password = this->GetTextControlData( kZExpPasswordWidgetID );
	expPrefs->SetPassword( password );

	PMString pluginId = this->GetTextControlData( kZExpPluginIdWidgetID );
	expPrefs->SetPluginId( pluginId );

	PMString strProxyServer = this->GetEditBoxText(kZExpPrefsDlgProxyServerEditBoxWidgetID);
	PMString strProxyPort = this->GetEditBoxText(kZExpPrefsDlgProxyPortEditBoxWidgetID);
	bool16 bUseProxyServer = this->GetTriStateControlData( kZExpPrefsDlgUseProxyCheckBoxWidgetID) == ITriStateControlData::kSelected;

	expPrefs->SetNeedsProxyServer( bUseProxyServer );
	expPrefs->SetProxyServer( strProxyServer );
	expPrefs->SetProxyPort( strProxyPort.GetAsNumber() );

	CmdUtils::ProcessCommand( saveExpPrefsCmd );
}


//----------------------------------------------------------------------------------------
// SetEditBoxText
//----------------------------------------------------------------------------------------
void
CZExpPrefsDialogController::SetEditBoxText(
	const WidgetID& widgetId, PMString value)
{
	//Codecleanup: use SetTextControlData instead of this function
	InterfacePtr<const IPanelControlData> iPanel(this, UseDefaultIID());
	ASSERT(iPanel);
	
	InterfacePtr<IControlView> controlView (iPanel->FindWidget(widgetId), UseDefaultIID());
	ASSERT(controlView);
	
	InterfacePtr<ITextControlData> controlData(controlView, UseDefaultIID());
	ASSERT(controlData);
	
	if( controlData )
		controlData->SetString(value);
}


//----------------------------------------------------------------------------------------
// GetEditBoxText
//----------------------------------------------------------------------------------------
PMString
CZExpPrefsDialogController::GetEditBoxText(
	const WidgetID& widgetId)
{
	//Codecleanup: use GetTextControlData instead of this function
	InterfacePtr<const IPanelControlData> iPanel(this, UseDefaultIID());
	ASSERT(iPanel);
	
	InterfacePtr<IControlView> controlView (iPanel->FindWidget(widgetId), UseDefaultIID());
	ASSERT(controlView);
	
	InterfacePtr<ITextControlData> controlData(controlView, UseDefaultIID());
	ASSERT(controlData);
	
	if( controlData )
		return (controlData->GetString());
	return "";
}
