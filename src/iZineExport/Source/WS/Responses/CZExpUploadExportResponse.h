//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/WS/Responses/CZExpUploadExportResponse.h $
//	$Revision: 3232 $
//	$Date: 2011-06-14 10:04:23 +0200 (Tue, 14 Jun 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 14-6-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZExpUploadExportResponse_
#define _h_CZExpUploadExportResponse_
#pragma once

#include "AZExpWSResponse.h"

class CZExpUploadExportResponse : public AZExpWSResponse
{
public:
						CZExpUploadExportResponse();
	virtual				~CZExpUploadExportResponse();

	const bool			GetUploadSuccess() const;
	void				SetUploadSuccess(
							const bool					inUploadSuccess );

protected:
private:
	bool			mUploadSuccess;
};

#endif //_h_CZExpUploadExportResponse_
