//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Requests/CZPUploadExportRequest.h $
//	$Revision: 3335 $
//	$Date: 2011-06-22 10:43:51 +0200 (Wed, 22 Jun 2011) $
//	$Author: aman.alam $
//	
//	Creator: Aman Alam
//	Created: 22-6-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPUploadExportRequest_
#define _h_CZPUploadExportRequest_
#pragma once

#include "AZPUploadFileRequest.h"
class AZPSoapResponse;

class CZPUploadExportRequest : public AZPUploadFileRequest
{
public:
						CZPUploadExportRequest();
	virtual				~CZPUploadExportRequest();

	const PMString &	GetImageFileName() const;
	void				SetImageFileName(
							const PMString &			inImageFileName );

	const bool			GetIsXML() const;
	void				SetIsXML(
							const bool					inIsXML );

	const bool			GetIsHighRes() const;
	void				SetIsHighRes(
							const bool					inIsHighRes );

#ifdef Debug_WS
	const char *		GetName() const;
#endif
protected:
	AZPSoapResponse *	CreateResponseObj();
private:
	PMString		mImageFileName;
	bool			mIsXML;
	bool			mIsHighRes;
};

#endif //_h_CZPUploadExportRequest_
