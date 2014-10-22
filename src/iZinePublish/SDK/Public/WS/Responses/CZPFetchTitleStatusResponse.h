//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Responses/CZPFetchTitleStatusResponse.h $
//	$Revision: 1111 $
//	$Date: 2010-05-05 10:56:48 +0200 (Wed, 05 May 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 29-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPFetchTitleStatusResponse_
#define _h_CZPFetchTitleStatusResponse_
#pragma once

#include "AZPSoapResponse.h"
class CZPFetchTitleStatusResponse : public AZPSoapResponse
{
public:
						CZPFetchTitleStatusResponse();
	virtual				~CZPFetchTitleStatusResponse();

	ZPTitleStatusInfoSList &	GetInfoList();
	const ZPTitleStatusInfoSList &
						GetInfoList() const;
protected:
private:
	ZPTitleStatusInfoSList	mInfoList;
};

#endif //_h_CZPFetchTitleStatusResponse_
