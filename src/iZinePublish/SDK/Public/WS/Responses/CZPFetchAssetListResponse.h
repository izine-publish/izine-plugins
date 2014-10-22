//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Responses/CZPFetchAssetListResponse.h $
//	$Revision: 1211 $
//	$Date: 2010-05-24 14:15:47 +0200 (Mon, 24 May 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 4-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPFetchAssetListResponse_
#define _h_CZPFetchAssetListResponse_
#pragma once

#include "AZPSoapResponse.h"
class CZPFetchAssetListResponse : public AZPSoapResponse
{
public:
						CZPFetchAssetListResponse();
	virtual				~CZPFetchAssetListResponse();

	ZPIDPairSList &		GetAssetList();
	const ZPIDPairSList & GetAssetList() const;

	ZPAssetDetailSList & GetAssetDetailList();
	const ZPAssetDetailSList & GetAssetDetailList() const;

protected:
private:
	ZPIDPairSList		mAssetList;
	ZPAssetDetailSList	mAssetDetailList;
};

#endif //_h_CZPFetchAssetListResponse_
