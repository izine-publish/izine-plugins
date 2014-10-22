//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Requests/CZPDeleteTaskRequest.h $
//	$Revision: 2238 $
//	$Date: 2010-10-06 10:40:56 +0200 (Wed, 06 Oct 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 23-9-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPDeleteTaskRequest_
#define _h_CZPDeleteTaskRequest_
#pragma once

#include "AZPSoapRequest.h"
class AZPSoapResponse;

class CZPDeleteTaskRequest : public AZPSoapRequest
{
public:
						CZPDeleteTaskRequest();
	virtual				~CZPDeleteTaskRequest();

	const PMString &	GetTaskID() const;
	void				SetTaskID(
							const PMString &			inTaskID );

#ifdef Debug_WS
	const char *		GetName() const;
#endif
protected:
	AZPSoapResponse *	CreateResponseObj();
private:
	PMString	mTaskID;
};

#endif //_h_CZPDeleteTaskRequest_
