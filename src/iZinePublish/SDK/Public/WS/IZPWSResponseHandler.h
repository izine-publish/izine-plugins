//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/IZPWSResponseHandler.h $
//	$Revision: 1325 $
//	$Date: 2010-06-04 09:06:30 +0200 (Fri, 04 Jun 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 21-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPWSResponseHandler_
#define _h_IZPWSResponseHandler_
#pragma once

#include "ZPWSTypeDefs.h"

class IZPSoapRequest;
class AZPSoapResponse;

class IZPWSResponseHandler : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPWSRESPONSEHANDLER };

	//Should be called for each response received.
	virtual bool	PreProcessWSResponse(
						const IZPSoapRequest *			inSoapRequest,
						AZPSoapResponse *				inResponse,
						ZPWSError						inStatus ) = 0;

	//Note: handler function will own the response pointer. It must be deleted.
	virtual void	HandleWSResponse(
						const IZPSoapRequest *			inSoapRequest,
						AZPSoapResponse *				inResponse,
						ZPWSError						inStatus ) = 0;

	virtual void	HandleWSRequestError(
						const IZPSoapRequest *			inSoapRequest,
						ZPWSError						inError ) = 0;
};


#endif //_h_IZPWSResponseHandler_
