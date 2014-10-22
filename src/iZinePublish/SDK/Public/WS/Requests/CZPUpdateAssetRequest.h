//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Requests/CZPUpdateAssetRequest.h $
//	$Revision: 1984 $
//	$Date: 2010-08-17 16:09:39 +0200 (Tue, 17 Aug 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 17-8-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPUpdateAssetRequest_
#define _h_CZPUpdateAssetRequest_
#pragma once

#include "CZPCreateAssetRequest.h"
class AZPSoapResponse;

class CZPUpdateAssetRequest : public CZPCreateAssetRequest
{
public:
						CZPUpdateAssetRequest();
	virtual				~CZPUpdateAssetRequest();

	const PMString &	GetAssetID() const;
	void				SetAssetID(
							const PMString &			inAssetID );

#ifdef Debug_WS
	const char *		GetName() const;
#endif
protected:
	AZPSoapResponse *	CreateResponseObj();
private:
	PMString			mAssetID;
};

#endif //_h_CZPUpdateAssetRequest_
