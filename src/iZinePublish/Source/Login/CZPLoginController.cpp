//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Login/CZPLoginController.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 29-3-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IBoolData.h"
#include "IK2ServiceProvider.h"
#include "IK2ServiceRegistry.h"
#include "ISubject.h"
#include "IWorkspace.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes
#include "UIDList.h"
#include "Utils.h"

#include "IZPID.h"

//IZP Interfaces
#include "IZPAppController.h"
#include "IZPCommandFacade.h"
#include "IZPDefaultSession.h"
#include "IZPLoginController.h"
#include "IZPLoginDlgCreator.h"
#include "IZPSessionData.h"
#include "IZPSoapRequest.h"
#include "IZPUserCredentials.h"
#include "IZPWSConfig.h"
#include "IZPWSController.h"
#include "IZPWSResponseHandler.h"

//IZP General includes
#include "CZPLoginRequest.h"

#include "IZPLog.h"

#define thisFileLA	enLMA_App
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

class CZPLoginController : public CPMUnknown<IZPLoginController>
{
public:
						CZPLoginController(IPMUnknown * boss);
	virtual				~CZPLoginController();

	void				ShowLoginDialog();

	void				DoLogin(
							const IZPWSConfig *					inWSConfig,
							const IZPUserCredentials *			inUserCeredentials );

	void				DoLogoff();	//Logoff the default session
	void				DoLogoff(
							const IZPWSConfig *					inWSConfig,
							const IZPUserCredentials *			inUserCeredentials );
		

	void				HandleAuthenticationSuccess(
							const IZPWSConfig *					inWSConfig,
							const IZPUserCredentials *			inUserCeredentials,
							const PMString &					inUserID,
							const PMString &					inUserDisplayName );

	void				AddLoginWaitCount();
	void				ReduceLoginWaitCount();
protected:
	void				SaveData(
							const IZPWSConfig *					inWSConfig );
	void				SaveData(
							const IZPUserCredentials *			inUserCeredentials );
	void				UpdateDefaultSession(
							const IZPUserCredentials *			inNewUserCeredentials,
							const PMString &					inUserID,
							const PMString &					inUserDisplayName );
	void				SendLoginDoneMessage();
	void				BroadcastMessage(
							const ClassID&						inTheChange,
							void *								inChangedBy = NULL);
private:
	int					mLoginWaitRequestCount;
};


CREATE_PMINTERFACE(CZPLoginController, kZPLoginControllerImpl)

CZPLoginController::CZPLoginController(IPMUnknown * boss) :
CPMUnknown<IZPLoginController>(boss)
, mLoginWaitRequestCount(0)
{

}

CZPLoginController::~CZPLoginController()
{

}


//----------------------------------------------------------------------------------------
// ShowLoginDialog
//----------------------------------------------------------------------------------------
void
CZPLoginController::ShowLoginDialog()
{
	LogFunctionEnterExit;
	do
	{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
		InterfacePtr<IK2ServiceRegistry> serviceRegistry(gSession, UseDefaultIID());
		ASSERT(serviceRegistry);
		if(!serviceRegistry)
			break;
		
		InterfacePtr<IK2ServiceProvider> provider(serviceRegistry->QueryDefaultServiceProvider (kZPLoginUIService));
		ASSERT(provider);
		if(!provider)
			break;

		InterfacePtr<IZPLoginDlgCreator> dlgCreator(provider , UseDefaultIID() );
		ASSERT(dlgCreator);
		if( !dlgCreator )
			break;

		InterfacePtr<const IWorkspace> theWorkspace(gSession->QueryWorkspace());
		ASSERT(theWorkspace);
		if (!theWorkspace)
			break;
		
		InterfacePtr<IZPWSConfig> iZPWSConfig( theWorkspace, UseDefaultIID() );
		ASSERT(iZPWSConfig);

		InterfacePtr<const IZPUserCredentials> iZPUserCredentials(theWorkspace, UseDefaultIID());
		ASSERT(iZPUserCredentials);
		
		dlgCreator->ShowLoginDialog( iZPWSConfig, iZPUserCredentials );

	}while(kFalse);
}


//----------------------------------------------------------------------------------------
// DoLogin
//----------------------------------------------------------------------------------------
void
CZPLoginController::DoLogin(
	const IZPWSConfig *					inWSConfig,
	const IZPUserCredentials *			inUserCeredentials)
{
	LogFunctionEnterExit;
	do
	{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
		this->BroadcastMessage( kZPLogin_BeforeLoginMsg );

		this->BroadcastMessage( kZPLogin_AuthenticationInProgressMsg );
		//Create request for authentication

		InterfacePtr<IZPWSController> wsController( gSession , UseDefaultIID() );
		ASSERT(wsController);

		//InterfacePtr<IZPSoapRequest> soapRequest;
		//soapRequest.reset( ::CreateObject2<IZPSoapRequest>(kZPSoapRequestBoss) );
		//
		//CZPLoginRequest * theLoginRequest = new CZPLoginRequest();
		//soapRequest->SetRequestData( theLoginRequest );	//theLoginRequest
		NewWSRequest( CZPLoginRequest )

		theRequest->SetWSConfig( inWSConfig );
		theRequest->SetUserCredentials( inUserCeredentials );

		InterfacePtr<IZPWSResponseHandler> loginRespHandler( this, IID_IZPWSLOGINRESPONSEHANDLER );
		ASSERT(loginRespHandler);
		soapRequest->SetResponseHandler( loginRespHandler );

		mLoginWaitRequestCount = 0;
		wsController->AddRequest( soapRequest );
	}while(kFalse);
}


//----------------------------------------------------------------------------------------
// HandleAuthenticationSuccess
//----------------------------------------------------------------------------------------
void
CZPLoginController::HandleAuthenticationSuccess(
	const IZPWSConfig *					inWSConfig,
	const IZPUserCredentials *			inUserCredentials,
	const PMString &					inUserID,
	const PMString &					inUserDisplayName )
{
	LogFunctionEnterExit;
	//Save WS Config & User Credentials with workspace
	this->SaveData( inWSConfig );
	this->SaveData( inUserCredentials );
	
	//Create/Update default login
	this->UpdateDefaultSession( inUserCredentials, inUserID, inUserDisplayName );

	//Send the authentication message.
	this->BroadcastMessage( kZPLogin_AuthenticationDoneMsg );

	//Set the user name with InCopy
	Utils<IZPCommandFacade>()->SetInCopyUser( inUserCredentials->GetUserName() );

	//Check if there is any pending request for login process
	//Otherwise send the login done message
	if( mLoginWaitRequestCount == 0 )
		this->SendLoginDoneMessage();
}

//----------------------------------------------------------------------------------------
// DoLogoff
//----------------------------------------------------------------------------------------
void
CZPLoginController::DoLogoff()	//Logoff the default session
{
	//ask if logout can be done
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	bool allowLogout = true;
	this->BroadcastMessage( kZPLogin_CanLogoutMsg, &allowLogout);

	if( allowLogout == false )
		return;

	this->BroadcastMessage( kZPLogin_BeforeLogoutMsg );
	//TODO: do logout
	
	InterfacePtr<const IWorkspace> theWorkspace(gSession->QueryWorkspace());
	ASSERT(theWorkspace);
	InterfacePtr<IZPWSConfig>	wsConfig( theWorkspace, UseDefaultIID() );
	ASSERT( wsConfig );

	InterfacePtr<IZPDefaultSession> defaultSession( gSession, UseDefaultIID() );
	ASSERT( defaultSession );

	InterfacePtr<IBoolData> isValidData( defaultSession->GetUserCredentials(), IID_IZPISVALID );
	ASSERT( isValidData );

	isValidData->Set( kFalse );

	this->DoLogoff( wsConfig, defaultSession->GetUserCredentials() );
}

//----------------------------------------------------------------------------------------
// DoLogoff
//----------------------------------------------------------------------------------------
void
CZPLoginController::DoLogoff(
	const IZPWSConfig *					inWSConfig,
	const IZPUserCredentials *			inUserCeredentials)
{
	//TODO:
		//////if (!IZPServer::Logoff()) {
		//////	CAlert::InformationAlert("Error occured while logging off on server.");
		//////}
		//////
		//////InterfacePtr<IIZPSessionData> sessionData(gSession, UseDefaultIID());
		//////ASSERT(sessionData);

		//////sessionData->LogOff();
		//////sessionData->SetName(kNullString);
		//////sessionData->SetRole(kNullString);
		//////
		//////InterfacePtr<IApplication> application(gSession->QueryApplication());
		//////ASSERT(application);
		//////
		//////InterfacePtr<IPanelMgr> panelMgr(application->QueryPanelManager());
		//////if (panelMgr==nil)
		//////	break;
		//////
		//////IControlView* ptrPanelView = panelMgr->GetPanelFromWidgetID(kIZPAssetsPanelWidgetID);
		//////if(ptrPanelView == nil)
		//////	break;

		//////InterfacePtr<IIZPAssetsPanelUtils> utils(ptrPanelView, UseDefaultIID());
		//////ASSERT (utils);
		//////
		//////utils->ClearTreeView();
		//////
		//////ptrPanelView->Disable(kTrue);
	InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
	ASSERT(selfSubject);
	
	//TODO: remove when request is send, move it to Handle response
	this->BroadcastMessage( kZPLogin_NotLoggedInMsg );
}

//----------------------------------------------------------------------------------------
// SaveData
//----------------------------------------------------------------------------------------
void
CZPLoginController::SaveData(
	const IZPWSConfig *					inWSConfig)
{
	//Save the values with persistent workspace impl
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IWorkspace> theWorkspace(gSession->QueryWorkspace());
	ASSERT(theWorkspace);

	InterfacePtr<ICommand> wsConfigSaveCmd(CmdUtils::CreateCommand(kZPSaveWSConfigCmdBoss));
	if(!wsConfigSaveCmd)
		return;
	
	InterfacePtr<IZPWSConfig> iZPWSConfig( wsConfigSaveCmd, UseDefaultIID() );
	ASSERT(iZPWSConfig);
	
	iZPWSConfig->Copy( inWSConfig );

	UIDList itemList(::GetUIDRef(theWorkspace));
	wsConfigSaveCmd->SetItemList( itemList );
	
	CmdUtils::ProcessCommand(wsConfigSaveCmd);
}

//----------------------------------------------------------------------------------------
// SaveData
//----------------------------------------------------------------------------------------
void
CZPLoginController::SaveData(
	const IZPUserCredentials *			inUserCeredentials)
{
	//Save the values with persistent workspace impl
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IWorkspace> theWorkspace(gSession->QueryWorkspace());
	ASSERT(theWorkspace);

	InterfacePtr<ICommand> userCredentialSaveCmd( Utils<IZPCommandFacade>()->CreateCommand_SaveZPUserCredential( ::GetUIDRef(theWorkspace) ) );
	if(!userCredentialSaveCmd)
		return;
	
	InterfacePtr<IZPUserCredentials> iZPUserCredentials(userCredentialSaveCmd, UseDefaultIID());
	ASSERT(iZPUserCredentials);

	iZPUserCredentials->Copy( inUserCeredentials );

	CmdUtils::ProcessCommand(userCredentialSaveCmd);
}

//----------------------------------------------------------------------------------------
// UpdateDefaultSession
//----------------------------------------------------------------------------------------
void
CZPLoginController::UpdateDefaultSession(
	const IZPUserCredentials *			inNewUserCeredentials,
	const PMString &					inUserID,
	const PMString &					inUserDisplayName)
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IZPDefaultSession> defaultSession( gSession, UseDefaultIID() );
	ASSERT( defaultSession );

	const IZPUserCredentials * defaultCredentials = defaultSession->GetUserCredentials();
	if( defaultCredentials == nil )
	{
		//Create default session object
		defaultCredentials = ::CreateObject2<IZPUserCredentials>( kZPUserSessionBoss );
		defaultSession->SetUserCredentials( defaultCredentials );
		defaultCredentials->Release();
	}

	InterfacePtr<IZPUserCredentials> credentialToUpdate( defaultCredentials, UseDefaultIID() );
	ASSERT( credentialToUpdate );

	credentialToUpdate->Copy( inNewUserCeredentials );

	InterfacePtr<IZPSessionData> sessionData( defaultCredentials, UseDefaultIID() );
	ASSERT( sessionData );
	sessionData->SetUserID( inUserID );
	sessionData->SetUserDisplayName( inUserDisplayName );

	InterfacePtr<IBoolData> isValidData( credentialToUpdate, IID_IZPISVALID );
	ASSERT( isValidData );

	isValidData->Set( kTrue );
}

//----------------------------------------------------------------------------------------
// AddLoginWaitCount
//----------------------------------------------------------------------------------------
void
CZPLoginController::AddLoginWaitCount()
{
	LogFunctionEnterExit;
	if( gZPApp->GetAppState() == eAppState_Authenticated )
		++mLoginWaitRequestCount;
}

//----------------------------------------------------------------------------------------
// ReduceLoginWaitCount
//----------------------------------------------------------------------------------------
void
CZPLoginController::ReduceLoginWaitCount()
{
	LogFunctionEnterExit;
	if( gZPApp->GetAppState() == eAppState_Authenticated && mLoginWaitRequestCount > 0)
	{
		--mLoginWaitRequestCount;
		if( mLoginWaitRequestCount == 0 )
			this->SendLoginDoneMessage();
	}
}

//----------------------------------------------------------------------------------------
// BroadcastMessage
//----------------------------------------------------------------------------------------
void
CZPLoginController::BroadcastMessage(
	const ClassID&				inTheChange,
	void *						inChangedBy)
{
	InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
	ASSERT(selfSubject);
	selfSubject->Change( inTheChange, IZPLoginController::kDefaultIID, inChangedBy );
}



//----------------------------------------------------------------------------------------
// SendLoginDoneMessage
//----------------------------------------------------------------------------------------
void
CZPLoginController::SendLoginDoneMessage()
{
	LogFunctionEnterExit;
	ASSERT(gZPApp->GetAppState() == eAppState_Authenticated);

	//Before LoggedIn message will set the app state, so that it can be used else where.
	this->BroadcastMessage( kZPLogin_BeforeLoggedInMsg );

	//Send the logged in message.
	this->BroadcastMessage( kZPLogin_LoggedInMsg );
}
