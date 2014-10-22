//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Responses/CZPAcquireLockResponse.h $
//	$Revision: 1458 $
//	$Date: 2010-06-16 12:41:35 +0200 (Wed, 16 Jun 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 26-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPAcquireLockResponse_
#define _h_CZPAcquireLockResponse_
#pragma once

#include "AZPSoapResponse.h"

class CZPAcquireLockResponse : public AZPSoapResponse
{
public:
						CZPAcquireLockResponse();
	virtual				~CZPAcquireLockResponse();

	const ZPAssetLockInfo &	GetLockInfo() const;
	ZPAssetLockInfo &	GetLockInfo();
	void				SetLockInfo(
							const ZPAssetLockInfo &		inLockInfo );

protected:
private:
	ZPAssetLockInfo		mLockInfo;
};

#endif //_h_CZPAcquireLockResponse_
