//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/AZPSoapRequest.h $
//	$Revision: 2987 $
//	$Date: 2011-05-11 09:50:41 +0200 (Wed, 11 May 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 31-3-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_AZPSoapRequest_
#define _h_AZPSoapRequest_
#pragma once

#include "ZPWSTypeDefs.h"

class IZPWSConfig;
class IZPUserCredentials;
class AZPSoapResponse;

class AZPSoapRequest
{
public:
	virtual						~AZPSoapRequest();

	enRequestType				GetRequestType() const;

	bool						IsAsync() const;
	void						SetAsync(
									bool						inIsAsync );

	const IZPWSConfig *			GetWSConfig() const;
	void						SetWSConfig(
									const IZPWSConfig *			inWSConfig );

	const IZPUserCredentials *	GetUserCredentials() const;
	void						SetUserCredentials(
									const IZPUserCredentials *	inUserCredentials );

	virtual AZPSoapResponse *	CreateResponseObj() = 0;

	//Used for debugging.
#ifdef Debug_WS
	virtual const char *		GetName() const = 0;
	virtual void				PrintToLogs() const;
#endif
	
protected:
								AZPSoapRequest(
									enRequestType				inReqType,
									bool						isAsync = false);	//Making it true will make all sync call's response to be processed as async

private:
	enRequestType				mReqType;
	bool						mIsAsync;
	const IZPWSConfig *			mWSConfig;
	const IZPUserCredentials *	mUserCredentials;
};

#endif //_h_AZPSoapRequest_
