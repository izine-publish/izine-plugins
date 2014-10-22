//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Responses/CZPGetTaskListResponse.h $
//	$Revision: 2240 $
//	$Date: 2010-10-06 12:33:06 +0200 (Wed, 06 Oct 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 23-9-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPGetTaskListResponse_
#define _h_CZPGetTaskListResponse_
#pragma once

#include "AZPSoapResponse.h"

class CZPGetTaskListResponse : public AZPSoapResponse
{
public:
						CZPGetTaskListResponse();
	virtual				~CZPGetTaskListResponse();

	ZPIDPairSList &		GetTaskIDPairList();
	const ZPIDPairSList & GetTaskIDPairList() const;

	ZPTaskInfoSList & GetTaskInfoList();
	const ZPTaskInfoSList & GetTaskInfoList() const;

protected:
private:
	ZPIDPairSList		mTaskList;
	ZPTaskInfoSList		mTaskInfoList;
};

#endif //_h_CZPGetTaskListResponse_
