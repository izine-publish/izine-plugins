//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/IZPWSCallHelper.h $
//	$Revision: 1017 $
//	$Date: 2010-04-19 10:54:06 +0200 (Mon, 19 Apr 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 29-3-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#ifndef _h_IZPWSCallHelper_
#define _h_IZPWSCallHelper_
#pragma once

#include "ZPWSTypeDefs.h"

class AZPSoapRequest;

class IZPWSCallHelper : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPWSCALLHELPER };

	virtual enRequestDiscardType
						GetRequestDiscardType(
							const AZPSoapRequest *				inRequest ) const = 0;
};

#endif //_h_IZPWSCallHelper_
