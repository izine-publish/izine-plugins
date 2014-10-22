//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/IZPSoapRequest.h $
//	$Revision: 1395 $
//	$Date: 2010-06-10 12:15:04 +0200 (Thu, 10 Jun 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 16-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description: It is a wrapper for AZPSoapRequest objects.
//========================================================================================
#ifndef _h_IZPSoapRequest_
#define _h_IZPSoapRequest_
#pragma once

class AZPSoapRequest;
class IZPWSResponseHandler;

class IZPSoapRequest : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPSOAPREQUEST };
	
	virtual const AZPSoapRequest *	GetRequestData() const = 0;
	virtual AZPSoapRequest *		GetRequestData()= 0;
	//this object will become owner of inRequestData
	virtual void					SetRequestData(
										AZPSoapRequest *		inRequestData ) = 0;

	virtual IZPWSResponseHandler *	GetResponseHandler() const = 0;
	virtual void					SetResponseHandler(
										IZPWSResponseHandler *	inRespHandler ) = 0;

#include "ZPInterfaceDebugHelper.h"
};


#define NewWSRequest(reqClass)	\
		reqClass * theRequest = new reqClass();										\
		NewWSRequestWithData( theRequest )

#define NewWSRequestWithData(theRequest)	\
		InterfacePtr<IZPSoapRequest> soapRequest;									\
		soapRequest.reset( ::CreateObject2<IZPSoapRequest>(kZPSoapRequestBoss) );	\
		soapRequest->SetRequestData( theRequest );


#define NewWSSyncRequest(reqClass)	\
		reqClass * theRequest = new reqClass();												\
		NewWSSyncRequestWithRequestData( theRequest )

#define NewWSSyncRequestWithRequestData(theRequest)	\
		InterfacePtr<IZPSoapRequest> soapRequest;											\
		soapRequest.reset( ::CreateObject2<IZPSoapRequest>(kZPSyncSoapRequestBoss) );		\
		soapRequest->SetRequestData( theRequest );											\
		InterfacePtr<IZPWSResponseHandler> theRespHandler( soapRequest, UseDefaultIID() );	\
		ASSERT(theRespHandler);																\
		soapRequest->SetResponseHandler( theRespHandler );									\
		InterfacePtr<IZPSoapResponse> soapResponse( soapRequest, UseDefaultIID() );			\
		ASSERT( soapResponse );


#endif //_h_IZPSoapRequest_
