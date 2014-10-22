//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Requests/CZPGetUsersForTitleRequest.h $
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
#ifndef _h_CZPGetUsersForTitleRequest_
#define _h_CZPGetUsersForTitleRequest_
#pragma once

#include "AZPSoapRequest.h"
class AZPSoapResponse;

class CZPGetUsersForTitleRequest : public AZPSoapRequest
{
public:
						CZPGetUsersForTitleRequest();
	virtual				~CZPGetUsersForTitleRequest();

	const PMString &	GetTitleID() const;
	void				SetTitleID(
							const PMString &			inTitleID );

#ifdef Debug_WS
	const char *		GetName() const;
#endif
protected:
	AZPSoapResponse *	CreateResponseObj();
private:
	PMString	mTitleID;
};

#endif //_h_CZPGetUsersForTitleRequest_
