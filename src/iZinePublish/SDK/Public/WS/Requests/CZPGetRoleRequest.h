//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Requests/CZPGetRoleRequest.h $
//	$Revision: 1045 $
//	$Date: 2010-04-23 13:29:03 +0200 (Fri, 23 Apr 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 22-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPGetRoleRequest_
#define _h_CZPGetRoleRequest_
#pragma once

#include "AZPSoapRequest.h"

class CZPGetRoleRequest : public AZPSoapRequest
{
public:
						CZPGetRoleRequest();
	virtual				~CZPGetRoleRequest();

#ifdef Debug_WS
	const char *		GetName() const;
#endif
protected:
	AZPSoapResponse *	CreateResponseObj();
private:
};

#endif //_h_CZPGetRoleRequest_
