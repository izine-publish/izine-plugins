//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Responses/CZPUploadVersionResponse.h $
//	$Revision: 3519 $
//	$Date: 2011-07-20 09:06:15 +0200 (Wed, 20 Jul 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 26-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPUploadVersionResponse_
#define _h_CZPUploadVersionResponse_
#pragma once

#include "AZPSoapResponse.h"

class CZPUploadVersionResponse : public AZPSoapResponse
{
public:
						CZPUploadVersionResponse();
	virtual				~CZPUploadVersionResponse();

	const PMString &	GetVersionId() const;
	void				SetVersionId(
							const PMString &			inVersionId );

protected:
private:
	PMString			mVersionId;
};

#endif //_h_CZPUploadVersionResponse_
