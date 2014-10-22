//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/CZPWSResponseHolder.cpp $
//	$Revision: 1424 $
//	$Date: 2010-06-14 09:10:08 +0200 (Mon, 14 Jun 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 4-6-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPAppController.h"
#include "IZPSoapRequest.h"
#include "IZPSoapResponse.h"
#include "IZPWSResponseHandler.h"

//IZP General includes
#include "AZPSoapResponse.h"
#include "StUtils.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_WS
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZPWSResponseHandler>
class CZPWSResponseHolder : public inheritClass
{
public:
						CZPWSResponseHolder(IPMUnknown * boss);
	virtual				~CZPWSResponseHolder();

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


CREATE_PMINTERFACE(CZPWSResponseHolder, kZPWSResponseHolderImpl)

CZPWSResponseHolder::CZPWSResponseHolder(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPWSResponseHolder::~CZPWSResponseHolder()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// PreProcessWSResponse
//----------------------------------------------------------------------------------------
bool
CZPWSResponseHolder::PreProcessWSResponse(
	const IZPSoapRequest *			inSoapRequest,
	AZPSoapResponse *				inResponse,
	ZPWSError						inStatus)
{
	LogFunctionEnterExit;
	InterfacePtr<IZPWSResponseHandler> appDefaultWSResponseHandler( gZPApp, UseDefaultIID() );
	ASSERT( appDefaultWSResponseHandler );

	return appDefaultWSResponseHandler->PreProcessWSResponse( inSoapRequest, inResponse, inStatus );

}

//----------------------------------------------------------------------------------------
// HandleWSResponse
//----------------------------------------------------------------------------------------
void
CZPWSResponseHolder::HandleWSResponse(
	const IZPSoapRequest *			inSoapRequest,
	AZPSoapResponse *				inResponse,
	ZPWSError						inStatus)
{
	LogFunctionEnterExit;
	StPtrDeleter<AZPSoapResponse>	autoDelResponse( inResponse, true );

	//Patch: For sync request as this response holder is on the same boss,
	// so boss release will not occur till request's response handler is set to nil. Deadlock condition.
	InterfacePtr<IControllingUnknown> selfUnknown( this, UseDefaultIID() );
	InterfacePtr<IControllingUnknown> requestUnknown( inSoapRequest, UseDefaultIID() );
	if( selfUnknown.get() == requestUnknown.get() )
		(const_cast<IZPSoapRequest *>(inSoapRequest))->SetResponseHandler( nil );


	if( false == this->PreProcessWSResponse( inSoapRequest, inResponse, inStatus ) )
		return;

	InterfacePtr<IZPSoapResponse> selfSoapResponse( this, UseDefaultIID() );
	ASSERT( selfSoapResponse );
	selfSoapResponse->SetWSError( inStatus );
	selfSoapResponse->SetResponseData( autoDelResponse.Forget() );
}

//----------------------------------------------------------------------------------------
// HandleWSRequestError
//----------------------------------------------------------------------------------------
void
CZPWSResponseHolder::HandleWSRequestError(
	const IZPSoapRequest *			inSoapRequest,
	ZPWSError						inError)
{
	LogFunctionEnterExit;
	InterfacePtr<IZPWSResponseHandler> appDefaultWSResponseHandler( gZPApp, UseDefaultIID() );
	ASSERT( appDefaultWSResponseHandler );

	appDefaultWSResponseHandler->HandleWSRequestError( inSoapRequest, inError );

	InterfacePtr<IZPSoapResponse> selfSoapResponse( this, UseDefaultIID() );
	ASSERT( selfSoapResponse );
	selfSoapResponse->SetWSError( inError );
}
