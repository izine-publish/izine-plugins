//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/IZPWSController.h $
//	$Revision: 2985 $
//	$Date: 2011-05-11 08:46:10 +0200 (Wed, 11 May 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 29-3-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#ifndef _h_IZPWSController_
#define _h_IZPWSController_
#pragma once

#include "ZPWSTypeDefs.h"

class IZPSoapRequest;

class IZPWSController : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPWSCONTROLLER };
	
	virtual void		AddRequest(
							IZPSoapRequest *			inSoapRequest,
							bool						inAsync = true) = 0;

	virtual void		HandleRequestProcessingError(
							IZPSoapRequest *					inSoapRequest,
							ZPWSError							inErrorCode) = 0;

	static	const char *GetErrorDescription(
							ZPWSError							inErrorCode);

	//Used to call the request's response handler. Must be executed in main thread only.
	virtual void		CallResponseHandler(
							const IZPSoapRequest *				inSoapRequest,
							AZPSoapResponse *					inResponse,
							ZPWSError							inErrVal) = 0;

	//Used to call the request's error handler. Must be executed in main thread only.
	virtual void		CallRequestErrorHandler(
							const IZPSoapRequest *				inSoapRequest,
							ZPWSError							inErrorCode) = 0;

};

#endif //_h_IZPWSController_
