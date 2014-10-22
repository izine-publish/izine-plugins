//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/CZPWSThreadHandler.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 18-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IIdleTaskThread.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPWSThreadHandler.h"
#include "IZPSoapRequest.h"
#include "IZPWSCallHelper.h"
#include "IZPWSController.h"

//IZP General includes
#include "AZPSoapRequest.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_WS
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZPWSThreadHandler>
class CZPWSThreadHandler : public inheritClass
{
public:
				CZPWSThreadHandler(IPMUnknown * boss);
	virtual		~CZPWSThreadHandler();

	void		AddRequest(
					IZPSoapRequest *					inRequest);

	bool		HasMatchingRequest(
					const AZPSoapRequest *				inSoapRequest,
					IZPSoapRequest * &					oMatchingRequest ) const;

	bool		ProcessRequestList();
protected:

private:
	K2Vector<IZPSoapRequest*>	mReqList;
	typedef K2Vector<IZPSoapRequest*>::iterator		RequestListIter;
	IZPSoapRequest *			mCurrentRequest;
};


CREATE_PMINTERFACE(CZPWSThreadHandler, kZPWSThreadHandlerImpl)

CZPWSThreadHandler::CZPWSThreadHandler(IPMUnknown * boss)
: inheritClass(boss)
, mCurrentRequest( nil )
{
	//TODO: create a mutex object
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPWSThreadHandler::~CZPWSThreadHandler()
{
	//TODO: destroy the mutex object
}

//----------------------------------------------------------------------------------------
// AddRequest
//----------------------------------------------------------------------------------------
void
CZPWSThreadHandler::AddRequest(
	IZPSoapRequest *					inRequest)
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	IZPSoapRequest * oldRequest = nil;
	IZPSoapRequest * discardedRequest = nil;
	AZPSoapRequest * theSoapRequest = inRequest->GetRequestData();
	bool addNewRequest = true;
	enRequestType newRequestType = theSoapRequest->GetRequestType();
	

	InterfacePtr<const IZPWSCallHelper> callHelper( this, UseDefaultIID() );
	enRequestDiscardType newRequestDiscardType = callHelper->GetRequestDiscardType(theSoapRequest);

	inRequest->AddRef();	//Released after request processing or error handling or after discarding.

	//Thread critical start
	//TODO: lock mutex
	
	//Find which request to discard, if needed
	//Check discard type
	//if discard type is discard new, then
	//	first check the current request being processed
	//	then check the list for similar request.
	if(( newRequestDiscardType == eReqDiscard_New && mCurrentRequest && mCurrentRequest->GetRequestData()->GetRequestType() == newRequestType )
	|| this->HasMatchingRequest( theSoapRequest , oldRequest ))
	{
		if( newRequestDiscardType == eReqDiscard_New )
		{
			discardedRequest = inRequest;
			addNewRequest = false;
		}
		else if( oldRequest != nil )
		{
			discardedRequest = oldRequest;
			RequestListIter foundIter = std::find( mReqList.begin(), mReqList.end(), oldRequest );
			if( foundIter != mReqList.end() )
				mReqList.erase( foundIter );
		}
	}
	if( addNewRequest)
	{
		mReqList.push_back(inRequest);
		//Start idle thread if it is not running.
		if( mReqList.size() == 1 )
		{
			InterfacePtr<IIdleTaskThread> selfIdler( this, UseDefaultIID() );
			ASSERT(selfIdler);
			selfIdler->InstallTask(1);
		}
	}

	//TODO: release mutex
	//Thread critical end


	//if request is being discarded then call the error handler
	if( discardedRequest )
	{
		InterfacePtr<IZPWSController> wsController( gSession , UseDefaultIID() );
		ASSERT(wsController);
		wsController->HandleRequestProcessingError( discardedRequest, eWSErr_DuplicateRequest );
		//discardedRequest->Release();
	}
}


//----------------------------------------------------------------------------------------
// ProcessRequestList
//----------------------------------------------------------------------------------------
bool
CZPWSThreadHandler::ProcessRequestList()
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	do
	{
		if( mReqList.size() > 0 )
		{
			//Thread critical start
			//TODO: lock mutex
			IZPSoapRequest * theReqToProcess = mReqList[0];
			mReqList.erase(mReqList.begin());
			//TODO: release mutex
			//Thread critical end
			mCurrentRequest = theReqToProcess;
			InterfacePtr<IZPWSController> wsController( gSession , UseDefaultIID() );
			ASSERT(wsController);
			wsController->AddRequest( theReqToProcess, false );
			mCurrentRequest = nil;
			theReqToProcess->Release();
		}
		else
		{
			//InterfacePtr<IIdleTaskThread> selfIdler( this, UseDefaultIID() );
			//ASSERT(selfIdler);
			//if(selfIdler)
			//	selfIdler->UninstallTask();
		}
		
		//false returning will Stop idle thread, if no pending request.
		return ( mReqList.size() > 0 );
	}while(kFalse);
}


//----------------------------------------------------------------------------------------
// HasMatchingRequest
//----------------------------------------------------------------------------------------
bool
CZPWSThreadHandler::HasMatchingRequest(
	const AZPSoapRequest *				inSoapRequest,
	IZPSoapRequest * &					oMatchingRequest) const
{
	oMatchingRequest = nil;
	return false;

	//TODO:
}


