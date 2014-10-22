//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Requests/CZPFetchLockInfoRequest.h $
//	$Revision: 1784 $
//	$Date: 2010-07-20 14:35:42 +0200 (Tue, 20 Jul 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 20-7-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPFetchLockInfoRequest_
#define _h_CZPFetchLockInfoRequest_
#pragma once

#include "AZPSoapRequest.h"
#include "ZPTypes.h"

class AZPSoapResponse;

class CZPFetchLockInfoRequest : public AZPSoapRequest
{
public:
						CZPFetchLockInfoRequest();
	virtual				~CZPFetchLockInfoRequest();

	PMStringList &		GetAssetIDs();
	const PMStringList &	GetAssetIDs() const;

	void				SetAssetIDs(
							const ZPAssetIDsList &		inAssetIDs );

#ifdef Debug_WS
	const char *		GetName() const;
#endif
protected:
	AZPSoapResponse *	CreateResponseObj();
private:

	PMStringList		mAssetIDs;
};

#endif //_h_CZPFetchLockInfoRequest_
