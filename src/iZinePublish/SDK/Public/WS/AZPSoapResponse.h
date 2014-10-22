//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/AZPSoapResponse.h $
//	$Revision: 3998 $
//	$Date: 2012-04-02 13:17:17 +0200 (Mon, 02 Apr 2012) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 31-3-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#ifndef _h_AZPSoapResponse_
#define _h_AZPSoapResponse_
#pragma once

#include "ZPWSTypeDefs.h"
#include "ZPString.h"

#ifndef ZPEXPORTS
#ifdef ZPBuild
#define ZPEXPORTS PLUGIN_DECL
#else
#define ZPEXPORTS PUBLIC_DECL
#endif
#endif

class ZPEXPORTS AZPSoapResponse
{
public:
	virtual			~AZPSoapResponse();

	virtual const int	GetErrorCode() const;
	virtual void		SetErrorCode(
							const int					inErrorCode );
	
	virtual const char *GetErrorDescription() const;
	virtual void		SetErrorDescription(
							const char *				inErrorDescription );

	const char *		GetErrorDetails() const;
	void				SetErrorDetails(
							const char *				inErrorDetails );
	void				SetErrorDetails(
							const PMString &			inErrorDetails );
	
	const enResponseType	GetRespType() const;
	
protected:
					AZPSoapResponse(
						enResponseType						inRespType);

private:
	enResponseType	mRespType;

	int				mErrorCode;
	ZPString256		mErrorDescription;
	ZPString512		mErrorDetails;

};

#endif //_h_AZPSoapResponse_
