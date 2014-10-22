//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Responses/CZPLoginResponse.h $
//	$Revision: 1362 $
//	$Date: 2010-06-08 09:33:11 +0200 (Tue, 08 Jun 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 16-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPLoginResponse_
#define _h_CZPLoginResponse_
#pragma once


#include "AZPSoapResponse.h"

class CZPLoginResponse : public AZPSoapResponse
{
public:
					CZPLoginResponse();
	virtual			~CZPLoginResponse();
	
	const PMString &	GetUserID() const;
	void				SetUserID(
							const PMString &			inUserID );
	
	const PMString &	GetDisplayName() const;
	void				SetDisplayName(
							const PMString &			inDisplayName );

protected:
	
private:
	PMString	mUserID;
	PMString	mDisplayName;
};

#endif //_h_CZPLoginResponse_
