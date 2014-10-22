//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Responses/CZPFetchEditionListResponse.h $
//	$Revision: 1115 $
//	$Date: 2010-05-05 11:42:40 +0200 (Wed, 05 May 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 29-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPFetchEditionListResponse_
#define _h_CZPFetchEditionListResponse_
#pragma once

#include "AZPSoapResponse.h"
class CZPFetchEditionListResponse : public AZPSoapResponse
{
public:
						CZPFetchEditionListResponse();
	virtual				~CZPFetchEditionListResponse();
	
	ZPTitleEditionInfoSList &	GetInfoList();
	const ZPTitleEditionInfoSList &
						GetInfoList() const;
protected:
private:
	ZPTitleEditionInfoSList	mInfoList;
};

#endif //_h_CZPFetchEditionListResponse_
