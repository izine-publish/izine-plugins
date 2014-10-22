//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/WS/Responses/CZExpFetchTitleEditionsResponse.h $
//	$Revision: 3230 $
//	$Date: 2011-06-14 10:00:48 +0200 (Tue, 14 Jun 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 14-6-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZExpFetchTitleEditionsResponse_
#define _h_CZExpFetchTitleEditionsResponse_
#pragma once

#include "AZExpWSResponse.h"

class CZExpFetchTitleEditionsResponse : public AZExpWSResponse
{
public:
						CZExpFetchTitleEditionsResponse();
	virtual				~CZExpFetchTitleEditionsResponse();
	
	const ZExpIDPairList &	GetEditionList() const;
	ZExpIDPairList &		GetEditionList();
protected:
private:
	ZExpIDPairList 		mEditionList;
};

#endif //_h_CZExpFetchTitleEditionsResponse_
