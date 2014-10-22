//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Requests/CZPUpdateAssetNameRequest.h $
//	$Revision: 3508 $
//	$Date: 2011-07-19 11:59:58 +0200 (Tue, 19 Jul 2011) $
//	$Author: aman.alam $
//	
//	Creator: Aman Alam
//	Created: 19-7-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPUpdateAssetNameRequest_
#define _h_CZPUpdateAssetNameRequest_
#pragma once

#include "CZPCreateAssetRequest.h"
class AZPSoapResponse;

class CZPUpdateAssetNameRequest : public CZPCreateAssetRequest
{
public:
						CZPUpdateAssetNameRequest();
	virtual				~CZPUpdateAssetNameRequest();

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

#endif //_h_CZPUpdateAssetNameRequest_
