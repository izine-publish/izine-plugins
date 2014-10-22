//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Responses/CZPGetTaskCategoryListResponse.h $
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
#ifndef _h_CZPGetTaskCategoryListResponse_
#define _h_CZPGetTaskCategoryListResponse_
#pragma once

#include "AZPSoapResponse.h"

class CZPGetTaskCategoryListResponse : public AZPSoapResponse
{
public:
						CZPGetTaskCategoryListResponse();
	virtual				~CZPGetTaskCategoryListResponse();

	ZPIDPairSList &			GetTaskCategoryList();
	const ZPIDPairSList &	GetTaskCategoryList() const;

protected:
private:
	ZPIDPairSList	mTaskCategoryList;
};

#endif //_h_CZPGetTaskCategoryListResponse_
