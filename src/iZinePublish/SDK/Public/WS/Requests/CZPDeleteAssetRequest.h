//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Requests/CZPDeleteAssetRequest.h $
//	$Revision: 1245 $
//	$Date: 2010-05-26 14:15:22 +0200 (Wed, 26 May 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 26-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPDeleteAssetRequest_
#define _h_CZPDeleteAssetRequest_
#pragma once

#include "AZPSoapRequest.h"
class AZPSoapResponse;

class CZPDeleteAssetRequest : public AZPSoapRequest
{
public:
						CZPDeleteAssetRequest();
	virtual				~CZPDeleteAssetRequest();

	const PMString &	GetAssetID() const;
	void				SetAssetID(
							const PMString &			inAssetID );
#ifdef Debug_WS
	const char *		GetName() const;
#endif
protected:
	AZPSoapResponse *	CreateResponseObj();
private:
	PMString		mAssetID;
};

#endif //_h_CZPDeleteAssetRequest_
