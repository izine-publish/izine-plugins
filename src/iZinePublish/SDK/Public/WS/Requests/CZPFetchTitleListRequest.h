//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Requests/CZPFetchTitleListRequest.h $
//	$Revision: 1047 $
//	$Date: 2010-04-23 14:44:17 +0200 (Fri, 23 Apr 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 23-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPFetchTitleListRequest_
#define _h_CZPFetchTitleListRequest_
#pragma once

#include "AZPSoapRequest.h"

class CZPFetchTitleListRequest : public AZPSoapRequest
{
public:
					CZPFetchTitleListRequest();
	virtual			~CZPFetchTitleListRequest();

#ifdef Debug_WS
	const char *		GetName() const;
#endif
protected:
	AZPSoapResponse *	CreateResponseObj();
private:
};

#endif //_h_CZPFetchTitleListRequest_
