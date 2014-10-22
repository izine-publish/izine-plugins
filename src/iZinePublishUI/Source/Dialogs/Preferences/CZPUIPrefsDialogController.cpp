//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Dialogs/Preferences/CZPUIPrefsDialogController.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 13-9-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IPanelControlData.h"
#include "IWorkspace.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes
#include "CDialogController.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPUIID.h"
#include "IZPID.h"

//IZP Interfaces
#include "IZPCommandFacade.h"
#include "IZPInDnUtils.h"
#include "IZPPrefs.h"
#include "IZPUserCredentials.h"

//IZP General includes
#include "ZPDataHelper.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CDialogController
class CZPUIPrefsDialogController : public inheritClass
{
public:
						CZPUIPrefsDialogController(IPMUnknown * boss);
	virtual				~CZPUIPrefsDialogController();

	/** Initializes each widget in the dialog with its default value.
	 Called when the dialog is opened.
	 @param dlgContext
	 */
	void				InitializeDialogFields(IActiveContext* dlgContext);
	
	/**	Retrieve the values from the widgets and act on them.
	 @param myContext
	 @param widgetId identifies the widget on which to act.
	 */
	void				ApplyDialogFields(IActiveContext* myContext, const WidgetID& widgetId);
protected:
private:
};


CREATE_PMINTERFACE(CZPUIPrefsDialogController, kZPUIPrefsDialogControllerImpl)

CZPUIPrefsDialogController::CZPUIPrefsDialogController(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPUIPrefsDialogController::~CZPUIPrefsDialogController()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// InitializeDialogFields
//----------------------------------------------------------------------------------------
void
CZPUIPrefsDialogController::InitializeDialogFields(
	IActiveContext* dlgContext)
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IWorkspace> iWorkspace(gSession->QueryWorkspace());
		
	InterfacePtr<const IZPPrefs> zpPrefs( iWorkspace, UseDefaultIID() );
	ASSERT(zpPrefs);

	int refreshDuration = zpPrefs->GetAssetMonitorAutoUpdateDuration();
	PMString refreshDurationStr;
	refreshDurationStr.AppendNumber( refreshDuration );
	refreshDurationStr.SetTranslatable( kFalse );
	this->SetTextControlData( kZPUIPrefsAMRefreshDurationEditBoxWidgetID, refreshDurationStr );

	this->SetTriStateControlData( kZPUIPrefsAutoUpdateLinkedStoriesCheckBoxWidgetID, zpPrefs->GetAutoUpdateLinkedStoriesContent() );

	this->SetTriStateControlData( kZPUIPrefsAutoUnlockStoriesCheckBoxWidgetID, zpPrefs->GetAutoUnlockStories() );

	this->SetTriStateControlData( kZPUIPrefsRemoveStylesCheckBoxWidgetID, zpPrefs->GetRemoveStyles() );

	InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());
	ASSERT(panelControlData);

	if( Utils<IZPInDnUtils>()->IsHostAppInCopy() == kFalse )
	{	//Disable the control in InDesign
		IControlView* controlView = panelControlData->FindWidget(kZPUIPrefsAutoUnlockStoriesCheckBoxWidgetID);
		ASSERT(controlView);
		if (controlView)
			controlView->Disable();

		controlView = panelControlData->FindWidget(kZPUIPrefsRemoveStylesCheckBoxWidgetID);
		ASSERT(controlView);
		if (controlView)
			controlView->Disable();
	}

	this->SetTriStateControlData( kZPUIPrefsCreateAssignmentOnStoryImportCheckBoxWidgetID, zpPrefs->GetCreateAssignmentOnStoryImport() );
	if( Utils<IZPInDnUtils>()->IsHostAppInDesign() == kFalse )
	{	//Disable the control in InCopy
		IControlView* controlView = panelControlData->FindWidget(kZPUIPrefsCreateAssignmentOnStoryImportCheckBoxWidgetID);
		ASSERT(controlView);
		if (controlView)
			controlView->Disable();
	}

	this->SetTriStateControlData( kZPUIPrefsIncludeAllAssignmentsOnDocumentCheckinCheckBoxWidgetID, zpPrefs->GetIncludeAllAssignmentsOnDocumentCheckin() );
	if( Utils<IZPInDnUtils>()->IsHostAppInDesign() == kFalse )
	{	//Disable the control in InCopy
		IControlView* controlView = panelControlData->FindWidget(kZPUIPrefsIncludeAllAssignmentsOnDocumentCheckinCheckBoxWidgetID);
		ASSERT(controlView);
		if (controlView)
			controlView->Disable();
	}

	InterfacePtr<const IZPUserCredentials> zpUserCreadentialPrefs( iWorkspace, UseDefaultIID() );
	ASSERT(zpUserCreadentialPrefs);

	this->SetTextControlData( kZPUIPrefsUserNameWidgetID, zpUserCreadentialPrefs->GetUserName() );
	this->SetTextControlData( kZPUIPrefsPasswordWidgetID, zpUserCreadentialPrefs->GetPwd() );

	IControlView* userNameControlView = panelControlData->FindWidget(kZPUIPrefsUserNameWidgetID);
	ASSERT(userNameControlView);
	IControlView* passwordControlView = panelControlData->FindWidget(kZPUIPrefsPasswordWidgetID);
	ASSERT(passwordControlView);
	if( ZPDataHelper::IsLoggedIn() )	//Disable user name and password if already logged in
	{
		if (userNameControlView)
			userNameControlView->Disable();

		if (passwordControlView)
			passwordControlView->Disable();
	}
	else
	{
		if (userNameControlView)
			userNameControlView->Enable();

		if (passwordControlView)
			passwordControlView->Enable();
	}
}

//----------------------------------------------------------------------------------------
// ApplyDialogFields
//----------------------------------------------------------------------------------------
void
CZPUIPrefsDialogController::ApplyDialogFields(
	IActiveContext* myContext, const WidgetID& widgetId)
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<ICommand> zpPrefsSaveCmd( Utils<IZPCommandFacade>()->CreateCommand_SaveZPPrefs( ) );
	if(!zpPrefsSaveCmd)
		return;

	InterfacePtr<IZPPrefs> zpPrefs( zpPrefsSaveCmd, UseDefaultIID() );
	ASSERT(zpPrefs);

	PMReal refreshDuration = this->GetTextValue( kZPUIPrefsAMRefreshDurationEditBoxWidgetID );
	zpPrefs->SetAssetMonitorAutoUpdateDuration( ToInt32(refreshDuration) );

	bool16 autoUpdateLinkedContent = (this->GetTriStateControlData( kZPUIPrefsAutoUpdateLinkedStoriesCheckBoxWidgetID ) == ITriStateControlData::kSelected);
	zpPrefs->SetAutoUpdateLinkedStoriesContent( autoUpdateLinkedContent );

	bool16 autoUnlockStories = (this->GetTriStateControlData( kZPUIPrefsAutoUnlockStoriesCheckBoxWidgetID ) == ITriStateControlData::kSelected);
	zpPrefs->SetAutoUnlockStories( autoUnlockStories );

	bool16 createAssignmentOnStoryImport = (this->GetTriStateControlData( kZPUIPrefsCreateAssignmentOnStoryImportCheckBoxWidgetID) == ITriStateControlData::kSelected);
	zpPrefs->SetCreateAssignmentOnStoryImport( createAssignmentOnStoryImport );

	bool16 includeAllAssignmentsOnDocumentCheckin = (this->GetTriStateControlData( kZPUIPrefsIncludeAllAssignmentsOnDocumentCheckinCheckBoxWidgetID) == ITriStateControlData::kSelected);
	zpPrefs->SetIncludeAllAssignmentsOnDocumentCheckin( includeAllAssignmentsOnDocumentCheckin );

	bool16 removeStyles = (this->GetTriStateControlData( kZPUIPrefsRemoveStylesCheckBoxWidgetID ) == ITriStateControlData::kSelected);
	zpPrefs->SetRemoveStyles( removeStyles );

	//TODO: Compare and then process cmd
	CmdUtils::ProcessCommand( zpPrefsSaveCmd );

	InterfacePtr<const IWorkspace> theWorkspace(gSession->QueryWorkspace());
	ASSERT(theWorkspace);

	InterfacePtr<IZPUserCredentials> prefsUserCredentials( theWorkspace, UseDefaultIID() );
	ASSERT(prefsUserCredentials);

	InterfacePtr<ICommand> zpUserCredentialsSaveCmd( Utils<IZPCommandFacade>()->CreateCommand_SaveZPUserCredential( ::GetUIDRef(theWorkspace) ) );
	if(!zpUserCredentialsSaveCmd)
		return;

	InterfacePtr<IZPUserCredentials> cmdUserCredentials( zpUserCredentialsSaveCmd, UseDefaultIID() );
	ASSERT(cmdUserCredentials);

	const PMString & userName = this->GetTextControlData( kZPUIPrefsUserNameWidgetID );
	const PMString & password = this->GetTextControlData( kZPUIPrefsPasswordWidgetID );
	cmdUserCredentials->SetUserName( userName );
	cmdUserCredentials->SetPwd( password );

	if( prefsUserCredentials->GetUserName().Compare( kFalse, userName ) != 0
	|| prefsUserCredentials->GetPwd().Compare( kTrue, password ) != 0 )
	{
		CmdUtils::ProcessCommand( zpUserCredentialsSaveCmd );
		//TODO: Login again
	}
}
