//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Requests/CZPGetTaskCategoryListRequest.h $
//	$Revision: 2302 $
//	$Date: 2010-10-14 14:18:41 +0200 (Thu, 14 Oct 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 14-10-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPGetTaskCategoryListRequest_
#define _h_CZPGetTaskCategoryListRequest_
#pragma once

#include "AZPSoapRequest.h"
class AZPSoapResponse;

class CZPGetTaskCategoryListRequest : public AZPSoapRequest
{
public:
						CZPGetTaskCategoryListRequest();
	virtual				~CZPGetTaskCategoryListRequest();

#ifdef Debug_WS
	const char *		GetName() const;
#endif
protected:
	AZPSoapResponse *	CreateResponseObj();
private:
};

#endif //_h_CZPGetTaskCategoryListRequest_
