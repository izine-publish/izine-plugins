//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Requests/CZPDownloadVersionRequest.h $
//	$Revision: 1835 $
//	$Date: 2010-07-26 10:35:29 +0200 (Mon, 26 Jul 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 26-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPDownloadVersionRequest_
#define _h_CZPDownloadVersionRequest_
#pragma once

#include "AZPDownloadFileRequest.h"
class AZPSoapResponse;

class CZPDownloadVersionRequest : public AZPDownloadFileRequest
{
public:
						CZPDownloadVersionRequest();
	virtual				~CZPDownloadVersionRequest();

	const PMString &	GetVersionID() const;
	void				SetVersionID(
							const PMString &			inVersionID );

#ifdef Debug_WS
	const char *		GetName() const;
#endif
protected:
	AZPSoapResponse *	CreateResponseObj();
private:
	PMString		mVersionID;
	
};

#endif //_h_CZPDownloadVersionRequest_
