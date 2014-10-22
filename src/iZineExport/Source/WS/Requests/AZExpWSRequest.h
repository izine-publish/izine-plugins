//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/WS/Requests/AZExpWSRequest.h $
//	$Revision: 3239 $
//	$Date: 2011-06-14 12:01:02 +0200 (Tue, 14 Jun 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 14-6-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_AZExpWSRequest_
#define _h_AZExpWSRequest_
#pragma once

#include "ZExpWSTypeDefs.h"

//class IZPWSConfig;
//class IZPUserCredentials;
class AZExpWSResponse;

class AZExpWSRequest
{
public:
	virtual						~AZExpWSRequest();

	enWSRequestType				GetRequestType() const;

	virtual AZExpWSResponse *	CreateResponseObj() = 0;

	virtual const char *		GetRequestURL() const;

	virtual const PMString &	GetPluginID() const;
	//Used for debugging.
#ifdef Debug_WS
	virtual const char *		GetName() const = 0;
	virtual void				PrintToLogs() const;
#endif
	
protected:
								AZExpWSRequest(
									enWSRequestType				inReqType);
//									bool						isAsync = false);	//Making it true will make all sync call's response to be processed as async

private:
	enWSRequestType				mReqType;
};

#endif //_h_AZExpWSRequest_
