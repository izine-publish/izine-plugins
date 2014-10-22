//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Requests/CZPFetchEditionListRequest.h $
//	$Revision: 1203 $
//	$Date: 2010-05-24 08:16:41 +0200 (Mon, 24 May 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 29-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPFetchEditionListRequest_
#define _h_CZPFetchEditionListRequest_
#pragma once


#include "AZPSoapRequest.h"

class CZPFetchEditionListRequest : public AZPSoapRequest
{
public:
						CZPFetchEditionListRequest();
	virtual				~CZPFetchEditionListRequest();

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

#endif //_h_CZPFetchEditionListRequest_
