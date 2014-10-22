//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Requests/CZPGetTaskHistoryRequest.h $
//	$Revision: 2344 $
//	$Date: 2010-10-18 15:43:44 +0200 (Mon, 18 Oct 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 18-10-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPGetTaskHistoryRequest_
#define _h_CZPGetTaskHistoryRequest_
#pragma once

#include "AZPSoapRequest.h"
class AZPSoapResponse;

class CZPGetTaskHistoryRequest : public AZPSoapRequest
{
public:
						CZPGetTaskHistoryRequest();
	virtual				~CZPGetTaskHistoryRequest();

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

#endif //_h_CZPGetTaskHistoryRequest_
