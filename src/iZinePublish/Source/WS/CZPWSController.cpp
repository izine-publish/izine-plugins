//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/CZPWSController.cpp $
//	$Revision: 2986 $
//	$Date: 2011-05-11 09:14:59 +0200 (Wed, 11 May 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 29-3-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes

//OS / ThirdParty includes
#include "stdsoap2.h"

#include "IZPID.h"

//IZP Interfaces
#include "IZPSoapRequest.h"
#include "IZPUserCredentials.h"
#include "IZPWSConfig.h"
#include "IZPWSController.h"
#include "IZPWSThreadHandler.h"
#include "IZPWSResponseHandler.h"

//IZP General includes
#include "AZPSoapRequest.h"
#include "AZPSoapResponse.h"
#include "CZPWSCallProcessors.h"
#include "CIdleTaskWSResponseHandle.h"
#include "CZPIdleTaskMgr.h"

#include "IZPLog.h"

#define thisFileLA	enLMA_WS
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

class CZPWSController : public CPMUnknown<IZPWSController>
{
public:
						CZPWSController(IPMUnknown * boss);
	virtual				~CZPWSController();

	void				AddRequest(
							IZPSoapRequest *					inSoapRequest,
							bool								inAsync );
	void				HandleRequestProcessingError(
							IZPSoapRequest *					inSoapRequest,
							ZPWSError							inErrorCode);
protected:
	void				ProcessRequest(
							IZPSoapRequest *					inSoapRequest );
	
	void				HandleResponse(
							const IZPSoapRequest *				inSoapRequest,
							AZPSoapResponse *					inResponse,
							ZPWSError							inErrVal);

	//Used to call the request's response handler. Must be executed in main thread only.
	void				CallResponseHandler(
							const IZPSoapRequest *				inSoapRequest,
							AZPSoapResponse *					inResponse,
							ZPWSError							inErrVal);

	//Used to call the request's error handler. Must be executed in main thread only.
	void				CallRequestErrorHandler(
							const IZPSoapRequest *				inSoapRequest,
							ZPWSError							inErrorCode);

private:
	InterfacePtr<IZPWSThreadHandler>	mThreadHandler;
	ZPSoapRequestProcessor				mReqProcessors[eReqCount];
};


CREATE_PMINTERFACE(CZPWSController, kZPWSControllerImpl)

CZPWSController::CZPWSController(IPMUnknown * boss) :
CPMUnknown<IZPWSController>(boss)
{
	IZPWSThreadHandler* theThreadHandler = ::CreateObject2<IZPWSThreadHandler>( kZPWSThreadBoss );
	mThreadHandler.reset(theThreadHandler);
	::memset(mReqProcessors, 0, sizeof(mReqProcessors));
	CZPWSCallProcessors::InitProcessorsArray(mReqProcessors);
}

CZPWSController::~CZPWSController()
{
}

//----------------------------------------------------------------------------------------
// AddRequest
//----------------------------------------------------------------------------------------
void
CZPWSController::AddRequest(
	IZPSoapRequest *			inSoapRequest,
	bool						inAsync)
{
	LogFunctionEnterExit;
	inSoapRequest->AddRef();	//Will be released by handleResponse, if sync

	if( inAsync )
	{
		inSoapRequest->GetRequestData()->SetAsync(true);
		mThreadHandler->AddRequest( inSoapRequest );
#ifdef DEBUG
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, " request refcount : %d", inSoapRequest->InterfaceRefCount());
#endif
		inSoapRequest->Release();
		return;
	}

	this->ProcessRequest( inSoapRequest );	//Release on request will be made from main thread.
}

//----------------------------------------------------------------------------------------
// ProcessRequest
//----------------------------------------------------------------------------------------
void
CZPWSController::ProcessRequest(
	IZPSoapRequest *			inSoapRequest)
{
	LogFunctionEnterExit;
	AZPSoapResponse * theResponse = nil;
	ZPSoapRequestProcessor theRequestProcessor = nil;

	try
	{
		theResponse = inSoapRequest->GetRequestData()->CreateResponseObj();
		
		if( theResponse == NULL )
		{	//If theResponse is null, then do error handling.
			this->HandleRequestProcessingError( inSoapRequest, eWSErr_ObjectAllocError );
		}
		
		//Get the request processor function pointer and call it.
		enRequestType theRequestType = inSoapRequest->GetRequestData()->GetRequestType();
		if( theRequestType > 0 && theRequestType < eReqCount )
			theRequestProcessor = mReqProcessors[ theRequestType ];
	}
	catch(...)
	{
		this->HandleRequestProcessingError( inSoapRequest, eWSErr_UnknownError );
	}
	if( theRequestProcessor )
	{
		ZPWSError theCallStatusCode = theRequestProcessor( inSoapRequest->GetRequestData(), theResponse );

		this->HandleResponse( inSoapRequest, theResponse, theCallStatusCode );
	}
	else
	{
		this->HandleRequestProcessingError( inSoapRequest, eWSErr_NoRequestProcessor );
		if( theResponse )
			delete theResponse;
	}

}

//----------------------------------------------------------------------------------------
// HandleResponse
//----------------------------------------------------------------------------------------
// Called in case where response object is available.
void
CZPWSController::HandleResponse(
	const IZPSoapRequest *					inSoapRequest,
	AZPSoapResponse *						inResponse,
	ZPWSError								inErrVal)
{
	LogFunctionEnterExit;
	InterfacePtr<const IZPSoapRequest> autoReleaseRequest;
	autoReleaseRequest.reset( inSoapRequest );	//This will release the request.
	const AZPSoapRequest *	inRequest = inSoapRequest->GetRequestData();
	do
	{
		if( inResponse )
		{
			int newErrorCode = inErrVal;

			/* gSOAP HTTP response status codes 100 to 599 are reserved */
			/* Codes 600 to 999 are user definable */
			/* Exceptional gSOAP HTTP response status codes >= 1000 */
			// these are till 1003
			// So keeping iZinePublish Errors above 2000
			if( inErrVal >= 1 && inErrVal < 2000 )
			{
				//Handle the gSoap Errors here. Convert them to meaning full error code and description.
				if( soap_xml_error_check( inErrVal ) )
				{
					newErrorCode = eWSErr_XMLError;
				}
				else if ( soap_soap_error_check( inErrVal ) )
				{
					newErrorCode = eWSErr_IncompatibleServer;
				}
				else if ( soap_tcp_error_check( inErrVal ) )
				{
					newErrorCode = eWSErr_NetworkError;
				}
				else if ( soap_ssl_error_check( inErrVal ) )
				{
					newErrorCode = eWSErr_SSLError;
				}
				else if ( soap_zlib_error_check( inErrVal ) )
				{
					newErrorCode = eWSErr_CompressionError;
				}
				else if ( soap_http_error_check( inErrVal ) )
				{
					//TODO: Handle HTTP Authentication errors
					if (inErrVal == eWSErr_HTTP_AuthenticationError)
					{
						//Someday: show some dialog to ask for user/pwd for website authentication
					}
					else
					{
						newErrorCode = eWSErr_HTTPError;
					}
				}
				
				inResponse->SetErrorCode( newErrorCode );
				inResponse->SetErrorDescription( this->GetErrorDescription( newErrorCode ) );
				//Error detail is aready set by call processors
				break;
			}
			//else if( some WSError )
			{	//Check if it is a invalid login i.e login expired, invalid session.
				//There is not need to send more requests, so clear the request list in such case.
				//if( check response code from Web service. )
					//Someday: Clear the request list
			}
		}
		
		//Update error desc with response
		//TODO:
	}while( false);
	
	if( inRequest->IsAsync() )
	{
		//Call response handler in main thread. Don't wait for response handling to end.
		//TODO: calling in main thread.
#if 0
		this->CallResponseHandler( autoReleaseRequest.forget(), inResponse, inErrVal );
#else
		CIdleTaskWSResponseHandle * wsResponseTask = new CIdleTaskWSResponseHandle();
		wsResponseTask->SetData( this, autoReleaseRequest.forget(), inResponse, inErrVal );
		CZPIdleTaskMgr * taskMgr = CZPIdleTaskMgr::GetInstance();
		taskMgr->AddIdleTask( wsResponseTask );
#endif
	}
	else
	{
		//Call response handler.
		this->CallResponseHandler( autoReleaseRequest.forget(), inResponse, inErrVal );
	}
}

//----------------------------------------------------------------------------------------
// HandleRequestProcessingError
//----------------------------------------------------------------------------------------
// Called web service call attempt is not made.
// mostly when request is discarded.
void
CZPWSController::HandleRequestProcessingError(
	IZPSoapRequest *				inSoapRequest,
	ZPWSError						inErrorCode)
{
	LogFunctionEnterExit;
	
	AZPSoapRequest*	inRequest = inSoapRequest->GetRequestData();
	if( inRequest->IsAsync() )
	{
		//Call error handler in main thread. Don't wait for it to end.
		//TODO: calling in main thread
#if 0
		this->CallRequestErrorHandler( inSoapRequest, inErrorCode );
#else
		CIdleTaskWSResponseHandle * wsResponseTask = new CIdleTaskWSResponseHandle();
		wsResponseTask->SetData( this, inSoapRequest, inErrorCode );
		CZPIdleTaskMgr * taskMgr = CZPIdleTaskMgr::GetInstance();
		taskMgr->AddIdleTask( wsResponseTask );
#endif
	}
	else
	{
		//Call error handler.
		this->CallRequestErrorHandler( inSoapRequest, inErrorCode );
	}
}

#pragma mark -
//----------------------------------------------------------------------------------------
// CallResponseHandler
//----------------------------------------------------------------------------------------
//Used to call the request's response handler. Must be executed in main thread only.
void
CZPWSController::CallResponseHandler(
	const IZPSoapRequest *				inSoapRequest,
	AZPSoapResponse *					inResponse,
	ZPWSError							inErrVal)
{
	LogFunctionEnterExit;
	InterfacePtr<const IZPSoapRequest> autoReleaseRequest;
	autoReleaseRequest.reset( inSoapRequest );	//This will release the request.

	IZPWSResponseHandler * theRespHandler = inSoapRequest->GetResponseHandler();
	ASSERT(theRespHandler);

	theRespHandler->HandleWSResponse( inSoapRequest, inResponse, inErrVal );
	//Request object will be destroyed automatically after this, if not referenced elsewhere.
}

//----------------------------------------------------------------------------------------
// CallRequestErrorHandler
//----------------------------------------------------------------------------------------
//Used to call the request's error handler. Must be executed in main thread only.
void
CZPWSController::CallRequestErrorHandler(
	const IZPSoapRequest *				inSoapRequest,
	ZPWSError							inErrorCode)
{
	LogFunctionEnterExit;
	InterfacePtr<const IZPSoapRequest> autoReleaseRequest;
	autoReleaseRequest.reset( inSoapRequest );		//This will release the request.

	IZPWSResponseHandler * theRespHandler = inSoapRequest->GetResponseHandler();
	ASSERT(theRespHandler);
	theRespHandler->HandleWSRequestError( inSoapRequest, inErrorCode );
	//Request object will be destroyed automatically after this, if not referenced elsewhere.
}


#pragma mark -
//----------------------------------------------------------------------------------------
// GetErrorDescription
//----------------------------------------------------------------------------------------
const char *
IZPWSController::GetErrorDescription(
	ZPWSError							inErrorCode)
{
	switch( inErrorCode )
	{
	case SOAP_OK :
		return "Success";
	case eWSErr_XMLError :
		return "Soap packet parsing error";
	case eWSErr_IncompatibleServer:
		return "Server and client have different soap version.";
	case eWSErr_NetworkError:
		return "Network error.";
	case eWSErr_SSLError:
		return "SSL error";
	case eWSErr_CompressionError:
		return "Compression error";
	case eWSErr_HTTPError:
		return "HTTP prototol error";
		
	//programming errors
	case eWSErr_UnknownError:
		return "Undefined error";
	case eWSErr_InvalidRequest:
		return "Invalid request";
	case eWSErr_InvalidRequestType:
		return "Invalid request type";
	case eWSErr_NoRequestProcessor:
		return "No function defined for calling web service for the request type.";
	
	//Common errors
	case eWSErr_InvalidResponseObj:
		return "Not able to create response object.";
	case eWSErr_InvalidRequestParams:
		return "Request have some missing parameters.";
	case eWSErr_UserNotLogedIn:
		return "User have not logged in.";
	case eWSErr_DuplicateRequest:
		return "Similar request already in queue.";
	case eWSErr_RequestDiscarded:
		return "Request not processed. Discarded due to other request in list.";
	case eWSErr_ObjectAllocError:
		return "Memory allocation error";
	}
	return NULL;	//""
}



