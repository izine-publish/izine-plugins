//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Requests/CZPGetTaskStatusListRequest.h $
//	$Revision: 2154 $
//	$Date: 2010-09-23 13:53:54 +0200 (Thu, 23 Sep 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 23-9-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPGetTaskStatusListRequest_
#define _h_CZPGetTaskStatusListRequest_
#pragma once

#include "AZPSoapRequest.h"
class AZPSoapResponse;

class CZPGetTaskStatusListRequest : public AZPSoapRequest
{
public:
						CZPGetTaskStatusListRequest();
	virtual				~CZPGetTaskStatusListRequest();

#ifdef Debug_WS
	const char *		GetName() const;
#endif
protected:
	AZPSoapResponse *	CreateResponseObj();
private:
};

#endif //_h_CZPGetTaskStatusListRequest_
