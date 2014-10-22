//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Requests/CZPLoginRequest.h $
//	$Revision: 1027 $
//	$Date: 2010-04-21 07:32:28 +0200 (Wed, 21 Apr 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 16-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPLoginRequest_
#define _h_CZPLoginRequest_
#pragma once

#include "AZPSoapRequest.h"

class CZPLoginRequest : public AZPSoapRequest
{
public:
						CZPLoginRequest();
	virtual				~CZPLoginRequest();
#ifdef Debug_WS
	const char *		GetName() const;
	void				PrintToLogs() const;
#endif
protected:
	AZPSoapResponse *	CreateResponseObj();
private:
};

#endif //_h_CZPLoginRequest_
