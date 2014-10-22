//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Requests/CZPFetchTitleStatusRequest.h $
//	$Revision: 1122 $
//	$Date: 2010-05-06 13:02:46 +0200 (Thu, 06 May 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 29-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPFetchTitleStatusRequest_
#define _h_CZPFetchTitleStatusRequest_
#pragma once


#include "AZPSoapRequest.h"

class CZPFetchTitleStatusRequest : public AZPSoapRequest
{
public:
						CZPFetchTitleStatusRequest();
	virtual				~CZPFetchTitleStatusRequest();

	const PMString &	GetTitleId() const;
	void				SetTitleId(
							const PMString &			inTitleId );

#ifdef Debug_WS
	const char *		GetName() const;
#endif
protected:
	AZPSoapResponse *	CreateResponseObj();
private:
	PMString	mTitleId;
};

#endif //_h_CZPFetchTitleStatusRequest_
