//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Requests/CZPDownloadThumbnailRequest.h $
//	$Revision: 1835 $
//	$Date: 2010-07-26 10:35:29 +0200 (Mon, 26 Jul 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 26-7-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPDownloadThumbnailRequest_
#define _h_CZPDownloadThumbnailRequest_
#pragma once

#include "AZPDownloadFileRequest.h"
class AZPSoapResponse;

class CZPDownloadThumbnailRequest : public AZPDownloadFileRequest
{
public:
						CZPDownloadThumbnailRequest();
	virtual				~CZPDownloadThumbnailRequest();

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

#endif //_h_CZPDownloadThumbnailRequest_
