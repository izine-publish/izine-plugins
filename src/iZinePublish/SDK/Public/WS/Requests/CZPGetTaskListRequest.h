//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Requests/CZPGetTaskListRequest.h $
//	$Revision: 2237 $
//	$Date: 2010-10-05 16:07:26 +0200 (Tue, 05 Oct 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 23-9-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPGetTaskListRequest_
#define _h_CZPGetTaskListRequest_
#pragma once

#include "AZPSoapRequest.h"
class AZPSoapResponse;

class CZPGetTaskListRequest : public AZPSoapRequest
{
public:
						CZPGetTaskListRequest();
	virtual				~CZPGetTaskListRequest();

	const PMString &	GetEditionID() const;
	void				SetEditionID(
							const PMString &			inEditionID );
#ifdef Debug_WS
	const char *		GetName() const;
#endif
protected:
	AZPSoapResponse *	CreateResponseObj();
private:
	PMString			mEditionID;
};

#endif //_h_CZPGetTaskListRequest_
