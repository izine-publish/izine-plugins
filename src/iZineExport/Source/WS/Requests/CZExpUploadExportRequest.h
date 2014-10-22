//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/WS/Requests/CZExpUploadExportRequest.h $
//	$Revision: 3738 $
//	$Date: 2012-01-16 15:23:41 +0100 (Mon, 16 Jan 2012) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 14-6-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZExpUploadExportRequest_
#define _h_CZExpUploadExportRequest_
#pragma once

#include "AZExpWSRequest.h"
class AZExpWSResponse;

class CZExpUploadExportRequest : public AZExpWSRequest
{
public:
						CZExpUploadExportRequest();
	virtual				~CZExpUploadExportRequest();

	const PMString &	GetTitleId() const;
	void				SetTitleId(
							const PMString &			inTitleId );

	const PMString &	GetEditionId() const;
	void				SetEditionId(
							const PMString &			inEditionId );

	const PMString &	GetArticleId() const;
	void				SetArticleId(
							const PMString &			inArticleId );

	const PMString &	GetDocName() const;
	void				SetDocName(
							const PMString &			inDocName );

	const PMString &	GetImageFileName() const;
	void				SetImageFileName(
							const PMString &			inImageFileName );

	const PMString &	GetFilePath() const;
	void				SetFilePath(
							const PMString &			inFilePath );

	const uchar *		GetContent() const;
	void				SetContent(
							const uchar *				inContent );

	const int			GetBuffSize() const;
	void				SetBuffSize(
							const int					inBuffSize );

	const PMString &	GetOtherParamStr() const;
	void				SetOtherParamStr(
							const PMString &			inOtherParams );
#ifdef Debug_WS
	const char *		GetName() const;
#endif
protected:
	AZExpWSResponse *	CreateResponseObj();
private:
	PMString 			mTitleId;
	PMString 			mEditionId;
	PMString			mArticleId;
	PMString 			mDocName;
	PMString 			mImageFileName;
	PMString			mFilePath;
	PMString			mOtherParamStr;
	const uchar *		mContent;		//TODO: must be retain till request is process
	int					mBuffSize;
};

#endif //_h_CZExpUploadExportRequest_
