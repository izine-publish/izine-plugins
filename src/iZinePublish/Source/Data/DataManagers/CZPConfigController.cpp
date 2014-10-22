//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Data/DataManagers/CZPConfigController.cpp $
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
#include "ISubject.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPApp.h"
#include "IZPConfigController.h"
#include "IZPDefaultSession.h"
#include "IZPEditionMgr.h"
#include "IZPSoapRequest.h"
#include "IZPTitleMgr.h"
#include "IZPTitleStatusMgr.h"
#include "IZPUserCredentials.h"
#include "IZPUserRole.h"
#include "IZPWSController.h"

//IZP General includes
#include "CZPFetchEditionListRequest.h"
#include "CZPFetchEditionListResponse.h"
#include "CZPFetchTitleListRequest.h"
#include "CZPFetchTitleListResponse.h"
#include "CZPFetchTitleStatusRequest.h"
#include "CZPFetchTitleStatusResponse.h"
#include "CZPGetRoleRequest.h"
#include "CZPGetRoleResponse.h"
#include "ZPTypes.h"
#include "ZPConstants.h"

#include "IZPLog.h"

#define thisFileLA	enLMA_App
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

class CZPConfigController : public CPMUnknown<IZPConfigController>
{
public:
						CZPConfigController(IPMUnknown * boss);
	virtual				~CZPConfigController();

	void				FetchAllConfigData();

	void				FetchRole();
	void				FetchTitleList();
	void				FetchEditions(
							const PMString &				inTitleID);
	void				FetchTitleStatus(
							const PMString &				inTitleID);

	void				HandleWSResponse(
							const IZPSoapRequest *			inSoapRequest,
							AZPSoapResponse *				inResponse,
							ZPWSError						inStatus);

protected:
	void				HandleResponse_Role( 
							const AZPSoapRequest *			inRequest,
							const AZPSoapResponse *			inResponse );
	void				HandleResponse_FetchTitleList( 
							const AZPSoapRequest *			inRequest,
							const AZPSoapResponse *			inResponse );
	void				HandleResponse_FetchEditionList( 
							const AZPSoapRequest *			inRequest,
							const AZPSoapResponse *			inResponse );
	void				HandleResponse_FetchTitleStatus( 
							const AZPSoapRequest *			inRequest,
							const AZPSoapResponse *			inResponse );
private:
};


CREATE_PMINTERFACE(CZPConfigController, kZPConfigControllerImpl)

CZPConfigController::CZPConfigController(IPMUnknown * boss) :
CPMUnknown<IZPConfigController>(boss)
{

}

CZPConfigController::~CZPConfigController()
{

}

//----------------------------------------------------------------------------------------
// FetchAllConfigData
//----------------------------------------------------------------------------------------
// Sequence of request, Role, Title, Shelve, Status
void
CZPConfigController::FetchAllConfigData()
{
	LogFunctionEnterExit;

	this->FetchRole();
	this->FetchTitleList();
	//this->FetchEditions();
}

//----------------------------------------------------------------------------------------
// FetchRole
//----------------------------------------------------------------------------------------
void
CZPConfigController::FetchRole()
{
	LogFunctionEnterExit;

#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IZPWSController> wsController( gSession , UseDefaultIID() );
	ASSERT(wsController);

	NewWSRequest( CZPGetRoleRequest )

	//Send the fetching message. One message per request
	InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
	ASSERT(selfSubject);
	selfSubject->Change( kZPLogin_ConfigDataFetchingMsg, IZPConfigController::kDefaultIID );

	wsController->AddRequest( soapRequest );
}

//----------------------------------------------------------------------------------------
// FetchTitleList
//----------------------------------------------------------------------------------------
void
CZPConfigController::FetchTitleList()
{
	LogFunctionEnterExit;

#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IZPWSController> wsController( gSession , UseDefaultIID() );
	ASSERT(wsController);

	NewWSRequest( CZPFetchTitleListRequest )

	//Send the fetching message.
	InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
	ASSERT(selfSubject);
	selfSubject->Change( kZPLogin_ConfigDataFetchingMsg, IZPConfigController::kDefaultIID );

	wsController->AddRequest( soapRequest );
}

//----------------------------------------------------------------------------------------
// FetchEditions
//----------------------------------------------------------------------------------------
void
CZPConfigController::FetchEditions(
	const PMString &				inTitleID)
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IZPWSController> wsController( gSession , UseDefaultIID() );
	ASSERT(wsController);

	NewWSRequest( CZPFetchEditionListRequest )

	if( inTitleID.empty() )
		theRequest->SetTitleID( STR_EmptyGUID );
	else
		theRequest->SetTitleID( inTitleID );

	//Send the fetching message.
	InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
	ASSERT(selfSubject);
	selfSubject->Change( kZPLogin_ConfigDataFetchingMsg, IZPConfigController::kDefaultIID );

	wsController->AddRequest( soapRequest );
}

//----------------------------------------------------------------------------------------
// FetchTitleStatus
//----------------------------------------------------------------------------------------
void
CZPConfigController::FetchTitleStatus(
	const PMString &				inTitleID)
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IZPWSController> wsController( gSession , UseDefaultIID() );
	ASSERT(wsController);

	NewWSRequest( CZPFetchTitleStatusRequest )

	if( inTitleID.empty() )
		theRequest->SetTitleId( STR_EmptyGUID );
	else
		theRequest->SetTitleId( inTitleID );

	//Send the fetching message.
	InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
	ASSERT(selfSubject);
	selfSubject->Change( kZPLogin_ConfigDataFetchingMsg, IZPConfigController::kDefaultIID );

	wsController->AddRequest( soapRequest );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// HandleWSResponse
//----------------------------------------------------------------------------------------
void
CZPConfigController::HandleWSResponse(
	const IZPSoapRequest *			inSoapRequest,
	AZPSoapResponse *				inResponse,
	ZPWSError						inStatus)
{
	LogFunctionEnterExit;
	
	const AZPSoapRequest * theRequest = inSoapRequest->GetRequestData();
	switch( theRequest->GetRequestType() )
	{
	case eReq_GetRole:
		this->HandleResponse_Role( theRequest, inResponse );
		break;
	case eReq_FetchTitleList:
		this->HandleResponse_FetchTitleList( theRequest, inResponse );
		break;
	case eReq_FetchTitleStatus:
		this->HandleResponse_FetchTitleStatus( theRequest, inResponse );
		break;
	case eReq_FetchEditionList:
		this->HandleResponse_FetchEditionList( theRequest, inResponse );
		break;
	}
}

//----------------------------------------------------------------------------------------
// HandleResponse_Role
//----------------------------------------------------------------------------------------
void
CZPConfigController::HandleResponse_Role(
	const AZPSoapRequest *			inRequest,
	const AZPSoapResponse *			inResponse)
{
	LogFunctionEnterExit;

#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IZPDefaultSession> defaultSession( gSession, UseDefaultIID() );
	ASSERT( defaultSession );

	InterfacePtr<IZPUserRole> userRole( defaultSession->GetUserCredentials(), UseDefaultIID() );
	ASSERT( userRole );

	const CZPGetRoleResponse * theResponse = dynamic_cast<const CZPGetRoleResponse*>(inResponse);
	ASSERT(theResponse);
	userRole->SetRole( theResponse->GetRole() );

	//Send the fetched message. One message per request
	InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
	ASSERT(selfSubject);
	selfSubject->Change( kZPLogin_ConfigDataFetchedMsg, IZPConfigController::kDefaultIID );
}

//----------------------------------------------------------------------------------------
// HandleResponse_FetchTitleList
//----------------------------------------------------------------------------------------
void
CZPConfigController::HandleResponse_FetchTitleList(
	const AZPSoapRequest *			inRequest,
	const AZPSoapResponse *			inResponse)
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<IZPTitleMgr> titleMgr( zpApp->GetTitleMgr(), UseDefaultIID() );
	ASSERT(titleMgr);

	const CZPFetchTitleListResponse * inTitleResponse = dynamic_cast<const CZPFetchTitleListResponse *> (inResponse );
	ASSERT( inTitleResponse );
	titleMgr->UpdateTitleList( inTitleResponse->GetTitleKeys(), inTitleResponse->GetTitleNames() );

	this->FetchEditions( kEmptyGUID );	//Note: current WS implentation works for single title only.
	this->FetchTitleStatus( kEmptyGUID ); //Note: current WS implentation works without using title id.

	//Send the fetched message. One message per request
	InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
	ASSERT(selfSubject);
	selfSubject->Change( kZPLogin_ConfigDataFetchedMsg, IZPConfigController::kDefaultIID );
}

//----------------------------------------------------------------------------------------
// HandleResponse_FetchTitleStatus
//----------------------------------------------------------------------------------------
void
CZPConfigController::HandleResponse_FetchTitleStatus(
	const AZPSoapRequest *			inRequest,
	const AZPSoapResponse *			inResponse)
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<IZPTitleStatusMgr> titleStatusMgr( zpApp->GetTitleMgr(), UseDefaultIID() );
	ASSERT(titleStatusMgr);

	const CZPFetchTitleStatusResponse * inTitleStatusResponse = dynamic_cast<const CZPFetchTitleStatusResponse *> (inResponse );
	ASSERT( inTitleStatusResponse );
	
	const ZPTitleStatusInfoSList & titleStatusInfoList = inTitleStatusResponse->GetInfoList();
	titleStatusMgr->UpdateStatusList( titleStatusInfoList );

	//Send the fetched message. One message per request
	InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
	ASSERT(selfSubject);
	selfSubject->Change( kZPLogin_ConfigDataFetchedMsg, IZPConfigController::kDefaultIID );
}

//----------------------------------------------------------------------------------------
// HandleResponse_FetchEditionList
//----------------------------------------------------------------------------------------
void
CZPConfigController::HandleResponse_FetchEditionList(
	const AZPSoapRequest *			inRequest,
	const AZPSoapResponse *			inResponse)
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<IZPEditionMgr> editionMgr( zpApp->GetEditionMgr(), UseDefaultIID() );
	ASSERT(editionMgr);

	const CZPFetchEditionListResponse * inEditionListResponse = dynamic_cast<const CZPFetchEditionListResponse *> (inResponse );
	ASSERT( inEditionListResponse );
	
	const ZPTitleEditionInfoSList & titleEditionInfoList = inEditionListResponse->GetInfoList();
	editionMgr->UpdateEditionList( titleEditionInfoList );

	//Send the fetched message. One message per request
	InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
	ASSERT(selfSubject);
	selfSubject->Change( kZPLogin_ConfigDataFetchedMsg, IZPConfigController::kDefaultIID );
}


