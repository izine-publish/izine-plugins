//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Responses/CZPGetTaskStatusListResponse.h $
//	$Revision: 2199 $
//	$Date: 2010-09-29 16:51:26 +0200 (Wed, 29 Sep 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 23-9-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPGetTaskStatusListResponse_
#define _h_CZPGetTaskStatusListResponse_
#pragma once

#include "AZPSoapResponse.h"

class CZPGetTaskStatusListResponse : public AZPSoapResponse
{
public:
						CZPGetTaskStatusListResponse();
	virtual				~CZPGetTaskStatusListResponse();

	ZPIDPairSList &			GetTaskStatusList();
	const ZPIDPairSList &	GetTaskStatusList() const;

protected:
private:
	ZPIDPairSList	mTaskStatusList;
};

#endif //_h_CZPGetTaskStatusListResponse_
