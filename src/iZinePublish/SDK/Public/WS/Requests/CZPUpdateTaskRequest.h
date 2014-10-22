//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Requests/CZPUpdateTaskRequest.h $
//	$Revision: 2318 $
//	$Date: 2010-10-15 13:35:17 +0200 (Fri, 15 Oct 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 23-9-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPUpdateTaskRequest_
#define _h_CZPUpdateTaskRequest_
#pragma once

#include "CZPCreateTaskRequest.h"
class AZPSoapResponse;

class CZPUpdateTaskRequest : public CZPCreateTaskRequest
{
public:
						CZPUpdateTaskRequest();
	virtual				~CZPUpdateTaskRequest();

	const PMString &	GetTaskID() const;
	void				SetTaskID(
							const PMString &			inTaskID );

	const PMString &	GetComments() const;
	void				SetComments(
							const PMString &			inComments );

	void				SetTaskOldVersion(
							int							inOldVersion );
#ifdef Debug_WS
	const char *		GetName() const;
#endif
protected:
	AZPSoapResponse *	CreateResponseObj();
private:

	PMString	mTaskID;
	PMString	mComments;
};

#endif //_h_CZPUpdateTaskRequest_
