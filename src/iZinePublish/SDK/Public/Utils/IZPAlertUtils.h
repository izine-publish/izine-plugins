//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/Utils/IZPAlertUtils.h $
//	$Revision: 1356 $
//	$Date: 2010-06-07 15:24:22 +0200 (Mon, 07 Jun 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 23-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPAlertUtils_
#define _h_IZPAlertUtils_
#pragma once

#include "CAlert.h"

class IZPAlertUtils : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPALERTUTILS };

	//Display the message, each value in new line.
	virtual void		ShowAlert(
							const PMString &			inMainError,
							const PMString &			inErrorDescription = kNullString,
							const PMString &			inErrorDetail = kNullString,
							CAlert::eAlertIcon			inIcon = CAlert::eWarningIcon ) = 0;
};


#endif //_h_IZPAlertUtils_
