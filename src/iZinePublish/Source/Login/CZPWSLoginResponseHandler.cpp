//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Login/CZPWSLoginResponseHandler.cpp $
//	$Revision: 1460 $
//	$Date: 2010-06-16 13:23:32 +0200 (Wed, 16 Jun 2010) $
//	$Author: rajkumar.sehrawat $
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

//SDK General includes
#include "CAlert.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPLoginController.h"
#include "IZPSoapRequest.h"
#include "IZPWSResponseHandler.h"
#include "IZPAlertUtils.h"
#include "IZPWSController.h"

//IZP General includes
#include "AZPSoapRequest.h"
#include "AZPSoapResponse.h"
#include "CZPLoginResponse.h"
#include "StUtils.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZPWSResponseHandler>
class CZPWSLoginResponseHandler : public inheritClass
{
public:
				CZPWSLoginResponseHandler(IPMUnknown * boss);
	virtual		~CZPWSLoginResponseHandler();

	//Should be called for each response received.
	bool				PreProcessWSResponse(
							const IZPSoapRequest *			inSoapRequest,
							AZPSoapResponse *				inResponse,
							ZPWSError						inStatus );

	void				HandleWSResponse(
							const IZPSoapRequest *			inSoapRequest,
							AZPSoapResponse *				inResponse,
							ZPWSError						inStatus );

	void				HandleWSRequestError(
							const IZPSoapRequest *			inSoapRequest,
							ZPWSError						inError );
protected:
private:
};


CREATE_PMINTERFACE(CZPWSLoginResponseHandler, kZPWSLoginResponseHandlerImpl)

CZPWSLoginResponseHandler::CZPWSLoginResponseHandler(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPWSLoginResponseHandler::~CZPWSLoginResponseHandler()
{

}

//----------------------------------------------------------------------------------------
// PreProcessWSResponse
//----------------------------------------------------------------------------------------
bool
CZPWSLoginResponseHandler::PreProcessWSResponse(
	const IZPSoapRequest *			inSoapRequest,
	AZPSoapResponse *				inResponse,
	ZPWSError						inStatus)
{
	return true;
}

//----------------------------------------------------------------------------------------
// HandleWSResponse
//----------------------------------------------------------------------------------------
void
CZPWSLoginResponseHandler::HandleWSResponse(
	const IZPSoapRequest *			inSoapRequest,
	AZPSoapResponse *				inResponse,
	ZPWSError						inStatus)
{
	StPtrDeleter<AZPSoapResponse>	autoDelResponse( inResponse, true );

	//Send message not logged in.
	InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
	ASSERT(selfSubject);

	if( inStatus != eWSErr_Success )
	{
		//Show Error
#if 0
		PMString errorMessage( "Login failed.");
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
		errorDetail.SetTranslatable( kFalse );
		Utils<IZPAlertUtils>()->ShowAlert( "Login failed.", inResponse->GetErrorDescription(), errorDetail );
#endif
		selfSubject->Change( kZPLogin_NotLoggedInMsg, IZPLoginController::kDefaultIID );
		return;
	}

	InterfacePtr<IZPLoginController> loginController( this , UseDefaultIID() );
	ASSERT( loginController );
	const AZPSoapRequest * theRequest = inSoapRequest->GetRequestData();
	const CZPLoginResponse * theResponse = dynamic_cast<const CZPLoginResponse*>( inResponse );
	ASSERT( theResponse );
	loginController->HandleAuthenticationSuccess( theRequest->GetWSConfig(), theRequest->GetUserCredentials(), theResponse->GetUserID(), theResponse->GetDisplayName() );
}

//----------------------------------------------------------------------------------------
// HandleWSRequestError
//----------------------------------------------------------------------------------------
void
CZPWSLoginResponseHandler::HandleWSRequestError(
	const IZPSoapRequest *			inSoapRequest,
	ZPWSError						inError)
{
	//Show Error
	if( inError != eWSErr_DuplicateRequest )
	{
		PMString errorDetail( IZPWSController::GetErrorDescription( inError ) );
#ifdef DEBUG
		errorDetail.Append("\n");
		errorDetail.Append( "Error code: " );
		errorDetail.AppendNumber( inError );
#endif
		errorDetail.SetTranslatable( kFalse );
		Utils<IZPAlertUtils>()->ShowAlert( "Login failed.", errorDetail );
	}

	//Send message not logged in.
	InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
	ASSERT(selfSubject);

	selfSubject->Change( kZPLogin_NotLoggedInMsg, IZPLoginController::kDefaultIID );
}
