//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/IZPWSThreadHandler.h $
//	$Revision: 1017 $
//	$Date: 2010-04-19 10:54:06 +0200 (Mon, 19 Apr 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 18-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPWSThreadHandler_
#define _h_IZPWSThreadHandler_
#pragma once

class IZPSoapRequest;

class IZPWSThreadHandler : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPWSTHREADHANDLER };

	virtual void		AddRequest(
							IZPSoapRequest *					inRequest) = 0;
	//Return true if it needs to be called again.
	virtual bool		ProcessRequestList() = 0;
};


#endif //_h_IZPWSThreadHandler_
