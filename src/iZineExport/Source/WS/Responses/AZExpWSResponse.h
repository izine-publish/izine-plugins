//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/WS/Responses/AZExpWSResponse.h $
//	$Revision: 3223 $
//	$Date: 2011-06-14 08:58:14 +0200 (Tue, 14 Jun 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 31-3-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#ifndef _h_AZExpWSResponse_
#define _h_AZExpWSResponse_
#pragma once

#include "ZExpWSTypeDefs.h"
#include "ZPString.h"

#ifndef ZPEXPORTS
//#ifdef ZPBuild
#define ZPEXPORTS PLUGIN_DECL
//#else
//#define ZPEXPORTS PUBLIC_DECL
//#endif
#endif

class ZPEXPORTS AZExpWSResponse
{
public:
	virtual			~AZExpWSResponse();

	virtual const int	GetErrorCode() const;
	virtual void		SetErrorCode(
							const int					inErrorCode );
	
	virtual const char *GetErrorDescription() const;
	virtual void		SetErrorDescription(
							const char *				inErrorDescription );

	const char *		GetErrorDetails() const;
	void				SetErrorDetails(
							const char *				inErrorDetails );
	
	const enResponseType	GetRespType() const;
	
protected:
					AZExpWSResponse(
						enResponseType						inRespType);

private:
	enResponseType	mRespType;

	int				mErrorCode;
	ZPString256		mErrorDescription;
	ZPString256		mErrorDetails;

};

#endif //_h_AZExpWSResponse_
