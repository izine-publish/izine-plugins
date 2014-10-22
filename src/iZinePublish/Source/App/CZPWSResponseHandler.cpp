//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/App/CZPWSResponseHandler.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 21-4-2010
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
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPAlertUtils.h"
#include "IZPApp.h"
#include "IZPAssetMgr.h"
#include "IZPConfigController.h"
#include "IZPPreviewMgr.h"
#include "IZPSoapRequest.h"
#include "IZPTaskCategoryMgr.h"
#include "IZPTaskStatusMgr.h"
#include "IZPTaskMgr.h"
#include "IZPUserCredentials.h"
#include "IZPUserMgr.h"
#include "IZPWSController.h"
#include "IZPWSResponseHandler.h"

//IZP General includes
#include "AZPSoapRequest.h"
#include "AZPSoapResponse.h"
#include "StUtils.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_WS
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZPWSResponseHandler>
class CZPWSResponseHandler : public inheritClass
{
public:
				CZPWSResponseHandler(IPMUnknown * boss);
	virtual		~CZPWSResponseHandler();

	bool				PreProcessWSResponse(
							const IZPSoapRequest *			inSoapRequest,
							AZPSoapResponse *				inResponse,
							ZPWSError						inStatus);

	void				HandleWSResponse(
							const IZPSoapRequest *			inSoapRequest,
							AZPSoapResponse *				inResponse,
							ZPWSError						inStatus);
	
	void				HandleWSRequestError(
							const IZPSoapRequest *			inSoapRequest,
							ZPWSError						inError );
protected:
	
private:
};


CREATE_PMINTERFACE(CZPWSResponseHandler, kZPWSResponseHandlerImpl)

CZPWSResponseHandler::CZPWSResponseHandler(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPWSResponseHandler::~CZPWSResponseHandler()
{

}

//----------------------------------------------------------------------------------------
// PreProcessWSResponse
//----------------------------------------------------------------------------------------
bool
CZPWSResponseHandler::PreProcessWSResponse(
	const IZPSoapRequest *			inSoapRequest,
	AZPSoapResponse *				inResponse,
	ZPWSError						inStatus)
{
	LogFunctionEnterExit;
	if( inStatus != eWSErr_Success )
	{
		//Show Error
#if 0
		PMString errorMessage( "Server Error:");
		errorMessage.Append("\n");
		errorMessage.Append( inResponse->GetErrorDescription() );
		errorMessage.Append("\n");
		errorMessage.Append( inResponse->GetErrorDetails());
#ifdef DEBUG
		errorMessage.Append("\n");
		errorMessage.Append( "Error code: " );
		errorMessage.AppendNumber( inStatus );
#endif
		CAlert::InformationAlert(errorMessage);
#else
		PMString errorDetail ( inResponse->GetErrorDetails() );
#ifdef DEBUG
		errorDetail.Append("\n");
		errorDetail.Append( "Error code: " );
		errorDetail.AppendNumber( inStatus );
#endif
		Utils<IZPAlertUtils>()->ShowAlert( "Server Error:", inResponse->GetErrorDescription(), errorDetail );
#endif
		return false;
	}
	return true;
	//TODO: error handing for each response
	//like logout, network error etc.
}

//----------------------------------------------------------------------------------------
// HandleWSResponse
//----------------------------------------------------------------------------------------
void
CZPWSResponseHandler::HandleWSResponse(
	const IZPSoapRequest *			inSoapRequest,
	AZPSoapResponse *				inResponse,
	ZPWSError						inStatus)
{
	StPtrDeleter<AZPSoapResponse>	autoDelResponse( inResponse, true );

	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	if( false == this->PreProcessWSResponse( inSoapRequest, inResponse, inStatus ) )
		return;

	const AZPSoapRequest * theRequest = inSoapRequest->GetRequestData();
	switch( theRequest->GetRequestType() )
	{
	case eReq_GetRole:
	case eReq_FetchTitleList:
	case eReq_FetchEditionList:
	case eReq_FetchTitleStatus:
		{
			InterfacePtr<IZPConfigController> theConfigController( this, UseDefaultIID() );
			ASSERT(theConfigController);

			theConfigController->HandleWSResponse( inSoapRequest, inResponse, inStatus );
		}
		break;
	case eReq_FetchAssetList:
	case eReq_FetchAssets:
	case eReq_FetchLockInfo:
	case eReq_DeleteAsset:
		{
			InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
			ASSERT( zpApp );

			InterfacePtr<IZPAssetMgr> theAssetMgr( zpApp->GetAssetMgr(), UseDefaultIID() );
			ASSERT(theAssetMgr);

			theAssetMgr->HandleWSResponse( inSoapRequest, inResponse, inStatus );
		}
		break;
	case eReq_DownloadThumbnail:
		{
			InterfacePtr<IZPPreviewMgr> thePreviewMgr( gSession, UseDefaultIID() );
			ASSERT(thePreviewMgr);
			thePreviewMgr->HandleWSResponse( inSoapRequest, inResponse, inStatus );
		}
		break;
	case eReq_GetTaskList:
	case eReq_CreateTask:
	case eReq_DeleteTask:
	case eReq_UpdateTask:
		{
			InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
			ASSERT( zpApp );

			InterfacePtr<IZPTaskMgr> theTaskMgr( zpApp->GetTaskMgr(), UseDefaultIID() );
			ASSERT(theTaskMgr);

			theTaskMgr->HandleWSResponse( inSoapRequest, inResponse, inStatus );
		}
		break;
	case eReq_GetTaskStatusList:
		{
			InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
			ASSERT( zpApp );

			InterfacePtr<IZPTaskStatusMgr> theTaskStatusMgr( zpApp->GetAssetMgr(), UseDefaultIID() );
			ASSERT(theTaskStatusMgr);

			theTaskStatusMgr->HandleWSResponse( inSoapRequest, inResponse, inStatus );
		}
		break;
	case eReq_GetTaskCategoryList:
		{
			InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
			ASSERT( zpApp );

			InterfacePtr<IZPTaskCategoryMgr> theTaskCategoryMgr( zpApp->GetAssetMgr(), UseDefaultIID() );
			ASSERT(theTaskCategoryMgr);

			theTaskCategoryMgr->HandleWSResponse( inSoapRequest, inResponse, inStatus );
		}
		break;
	case eReq_GetUsersForTitle:
		{
			InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
			ASSERT( zpApp );

			InterfacePtr<IZPUserMgr> theUserMgr( zpApp->GetUserMgr(), UseDefaultIID() );
			ASSERT(theUserMgr);

			theUserMgr->HandleWSResponse( inSoapRequest, inResponse, inStatus );
		}
		break;
	default:
		{	//Not handled specially, so broadcase message that response is available.
			InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
			ASSERT( selfSubject );
			selfSubject->Change( kZPWSResponseAvailable, IID_IZPWSRESPONSEHANDLER, inResponse );
		}
	}
}

//----------------------------------------------------------------------------------------
// HandleWSRequestError
//----------------------------------------------------------------------------------------
void
CZPWSResponseHandler::HandleWSRequestError(
	const IZPSoapRequest *			inSoapRequest,
	ZPWSError						inError)
{
	LogFunctionEnterExit;
	if( inError != eWSErr_DuplicateRequest )
	{
		PMString errorDetail( IZPWSController::GetErrorDescription( inError ) );
#ifdef DEBUG
		errorDetail.Append("\n");
		errorDetail.Append( "Error code: " );
		errorDetail.AppendNumber( inError );
#endif

#ifdef Debug_WS
		PMString error( inSoapRequest->GetRequestData()->GetName() );
		error.Append (" failed." );
#else
		PMString error ("Server request failed.");
#endif

		Utils<IZPAlertUtils>()->ShowAlert( error, errorDetail );
	}
}



