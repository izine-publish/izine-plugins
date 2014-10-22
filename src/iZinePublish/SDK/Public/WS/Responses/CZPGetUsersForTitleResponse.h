//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Responses/CZPGetUsersForTitleResponse.h $
//	$Revision: 2213 $
//	$Date: 2010-10-01 12:44:26 +0200 (Fri, 01 Oct 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 23-9-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPGetUsersForTitleResponse_
#define _h_CZPGetUsersForTitleResponse_
#pragma once

#include "AZPSoapResponse.h"

class CZPGetUsersForTitleResponse : public AZPSoapResponse
{
public:
						CZPGetUsersForTitleResponse();
	virtual				~CZPGetUsersForTitleResponse();

	ZPIDPairSList &		GetTitleUsersList();
	const ZPIDPairSList &	GetTitleUsersList() const;
	
protected:
private:
	ZPIDPairSList 		mTitleUsersList;
};

#endif //_h_CZPGetUsersForTitleResponse_
