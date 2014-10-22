//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Requests/CZPUploadPagePreviewRequest.h $
//	$Revision: 1407 $
//	$Date: 2010-06-11 04:12:35 +0200 (Fri, 11 Jun 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 10-6-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPUploadPagePreviewRequest_
#define _h_CZPUploadPagePreviewRequest_
#pragma once

#include "AZPUploadFileRequest.h"
class AZPSoapResponse;

class CZPUploadPagePreviewRequest : public AZPUploadFileRequest
{
public:
						CZPUploadPagePreviewRequest();
	virtual				~CZPUploadPagePreviewRequest();

	const PMString &	GetPageID() const;
	void				SetPageID(
							const PMString &			inPageID );

	const PMString &	GetPageTypeID() const;
	void				SetPageTypeID(
							const PMString &			inPageTypeID );

	const int			GetPageNumber() const;
	void				SetPageNumber(
							const int					inPageNumber );

#ifdef Debug_WS
	const char *		GetName() const;
#endif
protected:
	AZPSoapResponse *	CreateResponseObj();
private:

	PMString		mPageID;
	PMString		mPageTypeID;
	int				mPageNumber;
};

#endif //_h_CZPUploadPagePreviewRequest_
