//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Responses/CZPGetRoleResponse.h $
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
#ifndef _h_CZPGetRoleResponse_
#define _h_CZPGetRoleResponse_
#pragma once

#include "AZPSoapResponse.h"

class CZPGetRoleResponse : public AZPSoapResponse
{
public:
					CZPGetRoleResponse();
	virtual			~CZPGetRoleResponse();

	const PMString &	GetRole() const;
	void				SetRole(
							const PMString &				inRole );
protected:
private:
	PMString			mRole;
};

#endif //_h_CZPGetRoleResponse_
