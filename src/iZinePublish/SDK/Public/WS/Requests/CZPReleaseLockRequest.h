//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Requests/CZPReleaseLockRequest.h $
//	$Revision: 1243 $
//	$Date: 2010-05-26 12:56:07 +0200 (Wed, 26 May 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 26-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPReleaseLockRequest_
#define _h_CZPReleaseLockRequest_
#pragma once

#include "AZPSoapRequest.h"
class AZPSoapResponse;

class CZPReleaseLockRequest : public AZPSoapRequest
{
public:
						CZPReleaseLockRequest();
	virtual				~CZPReleaseLockRequest();

	const PMString &	GetAssetID() const;
	void				SetAssetID(
							const PMString &			inAssetID );

#ifdef Debug_WS
	const char *		GetName() const;
#endif
protected:
	AZPSoapResponse *	CreateResponseObj();
private:
	PMString	mAssetID;
};

#endif //_h_CZPReleaseLockRequest_
