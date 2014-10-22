//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/WS/Responses/CZExpGetConfigResponse.h $
//	$Revision: 3905 $
//	$Date: 2012-02-10 15:35:39 +0100 (Fri, 10 Feb 2012) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Aman Alam
//	Created: 28-11-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZExpGetConfigResponse_
#define _h_CZExpGetConfigResponse_
#pragma once

#include "AZExpWSResponse.h"

class CZExpGetConfigResponse : public AZExpWSResponse
{
public:
						CZExpGetConfigResponse();
	virtual				~CZExpGetConfigResponse();
	
	const int			GetDPIHighRes() const;
	void				SetDPIHighRes(
									const int 					inDPIHighRes);

	const int			GetDPILowRes() const;
	void				SetDPILowRes(
									const int 					inDPILowRes);

	const bool			IsExportTiff() const;
	void				SetExportTiff(
									const bool					inExportTiff);

	const bool			IsExportLowRes() const;
	void				SetExportLowRes(
									const bool					inExportLowRes);

	const bool			IsExportPagePreview() const;
	void				SetExportPagePreview(
									const bool					inExportPagePreview);

	const bool			IsExportPDF() const;
	void				SetExportPDF(
									const bool					inExportPDF);

	const PMString 		GetStyleName() const;
	void				SetStyleName(
									const PMString &			inStyleName);

protected:
private:
	int				mDPIHighRes;
	int				mDPILowRes;
	bool			mExportTiff;
	bool			mExportPDF;
	bool			mExportLowRes;
	bool			mExportPagePreview;
	PMString		mStyleName;
};

#endif //_h_CZExpGetConfigResponse_
