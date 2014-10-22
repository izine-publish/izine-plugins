//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Responses/CZPFetchAssetsResponse.h $
//	$Revision: 1827 $
//	$Date: 2010-07-23 14:15:26 +0200 (Fri, 23 Jul 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 23-7-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#ifndef _h_CZPFetchAssetsResponse_
#define _h_CZPFetchAssetsResponse_
#pragma once

#include "AZPSoapResponse.h"

class CZPFetchAssetsResponse : public AZPSoapResponse
{
public:
						CZPFetchAssetsResponse();
	virtual				~CZPFetchAssetsResponse();

	ZPAssetInfoSList &	GetAssets();
	const ZPAssetInfoSList &	GetAssets() const;

protected:
private:
	ZPAssetInfoSList	mAssets;
	
};

#endif //_h_CZPFetchAssetsResponse_
