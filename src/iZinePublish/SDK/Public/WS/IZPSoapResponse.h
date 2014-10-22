//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/IZPSoapResponse.h $
//	$Revision: 1326 $
//	$Date: 2010-06-04 09:17:48 +0200 (Fri, 04 Jun 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 4-6-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description: wrapper for holding soap response object. Most used in sync calls.
//========================================================================================
#ifndef _h_IZPSoapResponse_
#define _h_IZPSoapResponse_
#pragma once

class AZPSoapResponse;

#include "ZPWSTypedefs.h"

class IZPSoapResponse : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPSOAPRESPONSE };
	virtual const AZPSoapResponse *	GetResponseData() const = 0;
	virtual void					SetResponseData(
										const AZPSoapResponse *	inResponseData ) = 0;
	virtual const ZPWSError			GetWSError() const = 0;
	virtual void					SetWSError(
										const ZPWSError				inWSError ) = 0;

};


#endif //_h_IZPSoapResponse_
