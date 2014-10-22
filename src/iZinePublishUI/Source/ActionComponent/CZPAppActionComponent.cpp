//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/ActionComponent/CZPAppActionComponent.cpp $
//	$Revision: 4089 $
//	$Date: 2012-10-30 06:59:29 +0100 (Tue, 30 Oct 2012) $
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
#include "IActionStateList.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes
#include "CActionComponent.h"
#include "CAlert.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPID.h"
#include "IZPUIID.h"

//IZP Interfaces
#include "IZPApp.h"
#include "IZPAppController.h"
#include "IZPDefaultSession.h"
#include "IZPUIDialogFacade.h"
#include "IZPLoginController.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IApplication.h"
#include "IPanelMgr.h"
#endif

//IZP General includes
#include "../../iZinePublish/Source/WS/CZPTestWSCalling.h"
#include "IZPLog.h"

#pragma mark -

#define thisFileLA	enLMA_ActionComponent
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

/** Implements IActionComponent; performs the actions that are executed when the plug-in's
	menu items are selected.

	@ingroup izinepublish

*/
class CZPAppActionComponent : public CActionComponent
{
public:
						CZPAppActionComponent(IPMUnknown* boss);
						~CZPAppActionComponent();

		virtual void DoAction(IActiveContext* ac, ActionID actionID, GSysPoint mousePoint, IPMUnknown* widget);
		virtual void UpdateActionStates(IActiveContext* ac, IActionStateList *listToUpdate, GSysPoint mousePoint, IPMUnknown* widget);
private:
	void DoAbout();
	
	void ShowPrefsPanel();
	
	void DoLogin();
	void DoLogoff();

private:

private:
};

CREATE_PMINTERFACE(CZPAppActionComponent, kZPAppActionComponentImpl)

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPAppActionComponent::CZPAppActionComponent(IPMUnknown* boss)
: CActionComponent(boss)
{
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAppActionComponent::~CZPAppActionComponent()
{

}

void
CZPAppActionComponent::UpdateActionStates(
	IActiveContext* ac, IActionStateList *listToUpdate, GSysPoint mousePoint, IPMUnknown* widget)
{
	LogFunctionEnterExit;
	
	do {
		for(int i=0; i < listToUpdate->Length(); i++) {
			ActionID actionID = listToUpdate->GetNthAction(i);
			int16 state = listToUpdate->GetNthActionState(i);

			switch (actionID.Get())
			{
				case kIZPLoginActionID:
				{
					if (gZPApp->GetAppState() == eAppState_NotLoggedIn)
						state = kEnabledAction;
					else
						state = kDisabled_Unselected;
					break;
				}
				case kIZPLogoffActionID:
				{
					if (gZPApp->GetAppState() == eAppState_LoggedIn)
						state = kEnabledAction;
					else
						state = kDisabled_Unselected;
					break;
				}
				default:
					break;
			}
			listToUpdate->SetNthActionState(i, state);
		}
	} while (kFalse);
}

/* DoAction
*/
void CZPAppActionComponent::DoAction(IActiveContext* ac, ActionID actionID, GSysPoint mousePoint, IPMUnknown* widget)
{
	LogFunctionEnterExit;
	
	switch (actionID.Get())
	{
		case kIZPPopupAboutThisActionID:
		case kIZPAboutActionID:
		{
			this->DoAbout();
			break;
		}
		case kIZPLoginActionID:
		{
			this->DoLogin();
			break;
		}
		case kIZPLogoffActionID:
		{
			this->DoLogoff();
			break;
		}
		case kIZPPreferencesPanelActionID:
			this->ShowPrefsPanel();
			break;
		case kIZPDbgMenuWSUploadVersionActionID:
			CZPTestWSCalling::TestUploadVersion();
			break;
		case kIZPDbgMenuWSDownloadVersionActionID:
			CZPTestWSCalling::TestDownloadVersion();
			break;
		case kIZPDbgMenuWSUpdateStatusActionID:
			CZPTestWSCalling::TestUpdateStatus();
			break;
#if defined(InDnCS5) || defined(InDnCS5_5)
		case kIZPAssetsPanelWidgetActionID:
		{
			 InterfacePtr<IApplication> app(GetExecutionContextSession()->QueryApplication());
			 ASSERT(app);

			 InterfacePtr<IPanelMgr> panelMgr(app->QueryPanelManager());
			 ASSERT(panelMgr);
				 
			 panelMgr->ShowPanelByWidgetID(kZPUIAssetsPanelWidgetID);
			break;
		}
		case kIZPTasksPanelWidgetActionID:
		{
			InterfacePtr<IApplication> app(GetExecutionContextSession()->QueryApplication());
			 ASSERT(app);

			InterfacePtr<IPanelMgr> panelMgr(app->QueryPanelManager());
			ASSERT(panelMgr);

			panelMgr->ShowPanelByWidgetID(kZPUITasksPanelWidgetID);
			break;
		}
#endif
		default:
			break;
	}
}

//----------------------------------------------------------------------------------------
// DoAbout
//----------------------------------------------------------------------------------------
void
CZPAppActionComponent::DoAbout()
{
	LogFunctionEnterExit;
	
	CAlert::ModalAlert
	(
		kIZPAboutBoxStringKey,				// Alert string
		kOKString, 						// OK button
		kNullString, 						// No second button
		kNullString, 						// No third button
		1,							// Set OK button to default
		CAlert::eInformationIcon				// Information icon.
	);
}

//----------------------------------------------------------------------------------------
// ShowPrefsPanel
//----------------------------------------------------------------------------------------
void
CZPAppActionComponent::ShowPrefsPanel()
{
	LogFunctionEnterExit;
	
	do
	{
		Utils<IZPUIDialogFacade>()->ShowPrefs();
	} while (kFalse);
}

//----------------------------------------------------------------------------------------
// DoLogin
//----------------------------------------------------------------------------------------
void
CZPAppActionComponent::DoLogin()
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif	
	InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<IZPLoginController> loginController( zpApp->GetLoginController(), UseDefaultIID() );
	ASSERT( loginController );

	loginController->ShowLoginDialog();
}

//----------------------------------------------------------------------------------------
// DoLogoff
//----------------------------------------------------------------------------------------
void
CZPAppActionComponent::DoLogoff()
{
	LogFunctionEnterExit;
	
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	do {
		InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
		ASSERT( zpApp );

		InterfacePtr<IZPLoginController> loginController( zpApp->GetLoginController(), UseDefaultIID() );
		ASSERT( loginController );

		loginController->DoLogoff();
	} while (kFalse);
}

