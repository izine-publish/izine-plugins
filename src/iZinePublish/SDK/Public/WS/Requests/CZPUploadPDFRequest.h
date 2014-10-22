//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Requests/CZPUploadPDFRequest.h $
//	$Revision: 1398 $
//	$Date: 2010-06-10 12:34:45 +0200 (Thu, 10 Jun 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 10-6-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPUploadPDFRequest_
#define _h_CZPUploadPDFRequest_
#pragma once

#include "AZPUploadFileRequest.h"
class AZPSoapResponse;

class CZPUploadPDFRequest : public AZPUploadFileRequest
{
public:
						CZPUploadPDFRequest();
	virtual				~CZPUploadPDFRequest();

#ifdef Debug_WS
	const char *		GetName() const;
#endif
protected:
	AZPSoapResponse *	CreateResponseObj();
private:
};

#endif //_h_CZPUploadPDFRequest_
