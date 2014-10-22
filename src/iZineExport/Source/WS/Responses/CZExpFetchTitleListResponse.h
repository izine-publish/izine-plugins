//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/WS/Responses/CZExpFetchTitleListResponse.h $
//	$Revision: 3228 $
//	$Date: 2011-06-14 09:41:01 +0200 (Tue, 14 Jun 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 14-6-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZExpFetchTitleListResponse_
#define _h_CZExpFetchTitleListResponse_
#pragma once

#include "AZExpWSResponse.h"

class CZExpFetchTitleListResponse : public AZExpWSResponse
{
public:
						CZExpFetchTitleListResponse();
	virtual				~CZExpFetchTitleListResponse();

	const ZExpIDPairList &	GetTitleList() const;
	ZExpIDPairList &		GetTitleList();
protected:
private:
	ZExpIDPairList 		mTitleList;
};

#endif //_h_CZExpFetchTitleListResponse_
