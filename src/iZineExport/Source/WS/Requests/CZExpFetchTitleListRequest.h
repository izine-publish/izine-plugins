//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/WS/Requests/CZExpFetchTitleListRequest.h $
//	$Revision: 3223 $
//	$Date: 2011-06-14 08:58:14 +0200 (Tue, 14 Jun 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 14-6-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZExpFetchTitleListRequest_
#define _h_CZExpFetchTitleListRequest_
#pragma once

#include "AZExpWSRequest.h"
class AZExpWSResponse;

class CZExpFetchTitleListRequest : public AZExpWSRequest
{
public:
						CZExpFetchTitleListRequest();
	virtual				~CZExpFetchTitleListRequest();

#ifdef Debug_WS
	const char *		GetName() const;
#endif
protected:
	AZExpWSResponse *	CreateResponseObj();
private:
};

#endif //_h_CZExpFetchTitleListRequest_
