//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Responses/CZPCreateTaskResponse.h $
//	$Revision: 2405 $
//	$Date: 2010-10-26 16:41:30 +0200 (Tue, 26 Oct 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 23-9-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPCreateTaskResponse_
#define _h_CZPCreateTaskResponse_
#pragma once

#include "AZPSoapResponse.h"

class CZPCreateTaskResponse : public AZPSoapResponse
{
public:
						CZPCreateTaskResponse(
							enResponseType						inRespType = eResp_CreateTask);
	virtual				~CZPCreateTaskResponse();

	const ZPTaskInfo &	GetTaskInfo() const;
	ZPTaskInfo &		GetTaskInfo();

	const PMString &	GetSubject() const;
	void				SetSubject(
							const PMString &			inSubject );
	const PMString &	GetTaskID() const;
	void				SetTaskID(
							const PMString &			inTaskID );

protected:
private:

	ZPTaskInfo	mTaskInfo;
	ZPIDPair	mIDInfo;
};

#endif //_h_CZPCreateTaskResponse_
