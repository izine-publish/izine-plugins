//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Requests/CZPFetchAssetsRequest.h $
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

#ifndef _h_CZPFetchAssetsRequest_
#define _h_CZPFetchAssetsRequest_
#pragma once

#include "AZPSoapRequest.h"
#include "ZPTypes.h"
class AZPSoapResponse;

class CZPFetchAssetsRequest : public AZPSoapRequest
{
public:
						CZPFetchAssetsRequest();
	virtual				~CZPFetchAssetsRequest();

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

#endif //_h_CZPFetchAssetsRequest_
