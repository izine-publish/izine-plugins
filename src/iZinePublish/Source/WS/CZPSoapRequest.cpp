//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/CZPSoapRequest.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 16-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPApp.h"
#include "IZPAppController.h"
#include "IZPSoapRequest.h"
#include "IZPWSResponseHandler.h"

//IZP General includes
#include "AZPSoapRequest.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZPSoapRequest>
class CZPSoapRequest : public inheritClass
{
public:
				CZPSoapRequest(IPMUnknown * boss);
	virtual		~CZPSoapRequest();

	const AZPSoapRequest *	GetRequestData() const;
	AZPSoapRequest *		GetRequestData();
	void					SetRequestData(
								AZPSoapRequest *		inRequestData );

	IZPWSResponseHandler *	GetResponseHandler() const;
	void					SetResponseHandler(
								IZPWSResponseHandler *	inRespHandler );

#include "ZPImplDebugHelper.h"
protected:
private:
	AZPSoapRequest *	mSoapRequest;
	InterfacePtr<IZPWSResponseHandler> mRespHandler;
};


CREATE_PMINTERFACE(CZPSoapRequest, kZPSoapRequestImpl)

CZPSoapRequest::CZPSoapRequest(IPMUnknown * boss)
: inheritClass(boss)
, mSoapRequest( NULL )
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<IZPWSResponseHandler> defaultRespHandler( zpApp->GetDefaultWSResponseHandler(), UseDefaultIID() );
	ASSERT( defaultRespHandler );
	mRespHandler = defaultRespHandler;
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPSoapRequest::~CZPSoapRequest()
{
	if( mSoapRequest )
		delete mSoapRequest;
}

//----------------------------------------------------------------------------------------
// GetRequestData
//----------------------------------------------------------------------------------------
const AZPSoapRequest *
CZPSoapRequest::GetRequestData()const
{
	return mSoapRequest;
}

//----------------------------------------------------------------------------------------
// GetRequestData
//----------------------------------------------------------------------------------------
AZPSoapRequest *
CZPSoapRequest::GetRequestData()
{
	return mSoapRequest;
}


//----------------------------------------------------------------------------------------
// SetRequestData
//----------------------------------------------------------------------------------------
void
CZPSoapRequest::SetRequestData(
	AZPSoapRequest *		inRequestData)
{
	if( mSoapRequest )
		delete mSoapRequest;
	mSoapRequest = inRequestData;
}

//----------------------------------------------------------------------------------------
// GetResponseHandler
//----------------------------------------------------------------------------------------
IZPWSResponseHandler *
CZPSoapRequest::GetResponseHandler()const
{
	if( mRespHandler )
		return mRespHandler;
	else
	{
		ASSERT( gZPApp );
		InterfacePtr<IZPWSResponseHandler> defaultRespHandler( gZPApp, IZPWSResponseHandler::kDefaultIID );
		return defaultRespHandler;
	}
}

//----------------------------------------------------------------------------------------
// SetResponseHandler
//----------------------------------------------------------------------------------------
void
CZPSoapRequest::SetResponseHandler(
	IZPWSResponseHandler *	inRespHandler)
{
	if( inRespHandler )
		inRespHandler->AddRef();
	mRespHandler.reset(inRespHandler);
}


