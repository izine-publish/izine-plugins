//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Requests/CZPUpdateAssetStatusRequest.h $
//	$Revision: 2929 $
//	$Date: 2011-05-04 11:14:15 +0200 (Wed, 04 May 2011) $
//	$Author: aman.alam $
//	
//	Creator: Aman Alam
//	Created: 4-5-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPUpdateAssetStatusRequest_
#define _h_CZPUpdateAssetStatusRequest_
#pragma once

#include "AZPSoapRequest.h"
class AZPSoapResponse;

class CZPUpdateAssetStatusRequest : public AZPSoapRequest
{
public:
						CZPUpdateAssetStatusRequest();
	virtual				~CZPUpdateAssetStatusRequest();

	const PMString &	GetAssetID() const;
	void				SetAssetID(
							const PMString &			inAssetID );
	const PMString &	GetOldStatusID() const;
	void				SetOldStatusID(
							const PMString &			inOldStatusID );
	const PMString &	GetNewStatusID() const;
	void				SetNewStatusID(
							const PMString &			inNewStatusID );

#ifdef Debug_WS
	const char *		GetName() const;
#endif
protected:
	AZPSoapResponse *	CreateResponseObj();
private:

	PMString 			mAssetID;
	PMString 			mOldStatusID;
	PMString 			mNewStatusID;
};

#endif //_h_CZPUpdateAssetStatusRequest_
