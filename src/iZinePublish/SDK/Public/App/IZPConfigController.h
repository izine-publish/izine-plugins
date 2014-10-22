//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/App/IZPConfigController.h $
//	$Revision: 1122 $
//	$Date: 2010-05-06 13:02:46 +0200 (Thu, 06 May 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 29-3-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#ifndef _h_IZPConfigController_
#define _h_IZPConfigController_
#pragma once

#include "ZPWSTypeDefs.h"

class IZPSoapRequest;
class AZPSoapResponse;

class IZPConfigController : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPCONFIGCONTROLLER };

	//Called after authentication is done
	virtual	void		FetchAllConfigData() = 0;

	virtual	void		FetchRole() = 0;
	virtual	void		FetchTitleList() = 0;
	virtual	void		FetchEditions(						//Empty title id will fetch all editions for user.
							const PMString &				inTitleID) = 0;
	virtual	void		FetchTitleStatus(						//Empty title id will fetch all status for user.
							const PMString &				inTitleID) = 0;

	virtual void		HandleWSResponse(
							const IZPSoapRequest *			inSoapRequest,
							AZPSoapResponse *				inResponse,
							ZPWSError						inStatus ) = 0;
};

#endif //_h_IZPConfigController_
