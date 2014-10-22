//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/WS/Requests/CZExpValidateMediaContentRequest.h $
//	$Revision: 3941 $
//	$Date: 2012-02-24 07:36:48 +0100 (Fri, 24 Feb 2012) $
//	$Author: aman.alam $
//	
//	Creator: Aman Alam
//	Created: 22-2-2012
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZExpValidateMediaContentRequest_
#define _h_CZExpValidateMediaContentRequest_
#pragma once

#include "AZExpWSRequest.h"
class AZExpWSResponse;

class CZExpValidateMediaContentRequest : public AZExpWSRequest
{
public:
						CZExpValidateMediaContentRequest();
	virtual				~CZExpValidateMediaContentRequest();

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

#endif //_h_CZExpValidateMediaContentRequest_
