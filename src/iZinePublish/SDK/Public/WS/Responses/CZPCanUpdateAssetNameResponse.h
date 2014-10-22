//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Responses/CZPCanUpdateAssetNameResponse.h $
//	$Revision: 3541 $
//	$Date: 2011-07-26 14:33:45 +0200 (Tue, 26 Jul 2011) $
//	$Author: aman.alam $
//	
//	Creator: Aman Alam
//	Created: 26-7-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPCanUpdateAssetNameResponse_
#define _h_CZPCanUpdateAssetNameResponse_
#pragma once

#include "AZPSoapResponse.h"

class CZPCanUpdateAssetNameResponse : public AZPSoapResponse
{
public:
						CZPCanUpdateAssetNameResponse();
	virtual				~CZPCanUpdateAssetNameResponse();

	const bool			GetIsSuccessful() const;
	void				SetIsSuccessful(
							const bool					inIsSuccessful );

protected:
private:
	bool				mIsSuccessful;
};

#endif //_h_CZPCanUpdateAssetNameResponse_
