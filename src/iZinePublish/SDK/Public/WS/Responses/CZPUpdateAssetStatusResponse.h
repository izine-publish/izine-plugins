//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Responses/CZPUpdateAssetStatusResponse.h $
//	$Revision: 2929 $
//	$Date: 2011-05-04 11:14:15 +0200 (Wed, 04 May 2011) $
//	$Author: aman.alam $
//	
//	Creator: Aman Alam
//	Created: 4-5-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPUpdateAssetStatusResponse_
#define _h_CZPUpdateAssetStatusResponse_
#pragma once

#include "AZPSoapResponse.h"

class CZPUpdateAssetStatusResponse : public AZPSoapResponse
{
public:
						CZPUpdateAssetStatusResponse();
	virtual				~CZPUpdateAssetStatusResponse();

	const bool			GetIsSuccessful() const;
	void				SetIsSuccessful(
							const bool					inIsSuccessful );

protected:
private:
	bool		mIsSuccessful;
};

#endif //_h_CZPUpdateAssetStatusResponse_
