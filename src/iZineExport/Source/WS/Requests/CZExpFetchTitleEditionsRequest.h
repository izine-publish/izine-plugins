//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/WS/Requests/CZExpFetchTitleEditionsRequest.h $
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
#ifndef _h_CZExpFetchTitleEditionsRequest_
#define _h_CZExpFetchTitleEditionsRequest_
#pragma once

#include "AZExpWSRequest.h"
class AZExpWSResponse;

class CZExpFetchTitleEditionsRequest : public AZExpWSRequest
{
public:
						CZExpFetchTitleEditionsRequest();
	virtual				~CZExpFetchTitleEditionsRequest();

	const PMString &	GetTitleId() const;
	void				SetTitleId(
							const PMString &			inTitleId );

#ifdef Debug_WS
	const char *		GetName() const;
#endif
protected:
	AZExpWSResponse *	CreateResponseObj();
private:
	PMString			mTitleId;
};

#endif //_h_CZExpFetchTitleEditionsRequest_
