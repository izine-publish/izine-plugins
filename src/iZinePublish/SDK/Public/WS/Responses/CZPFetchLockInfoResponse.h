//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Responses/CZPFetchLockInfoResponse.h $
//	$Revision: 1780 $
//	$Date: 2010-07-20 09:29:49 +0200 (Tue, 20 Jul 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 20-7-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPFetchLockInfoResponse_
#define _h_CZPFetchLockInfoResponse_
#pragma once

#include "AZPSoapResponse.h"

class CZPFetchLockInfoResponse : public AZPSoapResponse
{
public:
						CZPFetchLockInfoResponse();
	virtual				~CZPFetchLockInfoResponse();

	ZPAssetLockInfoSList &	GetLockInfoList();
	const ZPAssetLockInfoSList &	GetLockInfoList() const;

protected:
private:

	ZPAssetLockInfoSList	mLockInfoList;
};

#endif //_h_CZPFetchLockInfoResponse_
