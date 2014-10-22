//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Requests/CZPUploadVersionRequest.h $
//	$Revision: 1397 $
//	$Date: 2010-06-10 12:33:01 +0200 (Thu, 10 Jun 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 26-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPUploadVersionRequest_
#define _h_CZPUploadVersionRequest_
#pragma once

#include "AZPUploadFileRequest.h"
class AZPSoapResponse;

class CZPUploadVersionRequest : public AZPUploadFileRequest
{
public:
						CZPUploadVersionRequest();
	virtual				~CZPUploadVersionRequest();

#ifdef Debug_WS
	const char *		GetName() const;
#endif
protected:
	AZPSoapResponse *	CreateResponseObj();
private:
};

#endif //_h_CZPUploadVersionRequest_
