//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Requests/CZPCanUpdateAssetNameRequest.h $
//	$Revision: 3541 $
//	$Date: 2011-07-26 14:33:45 +0200 (Tue, 26 Jul 2011) $
//	$Author: aman.alam $
//	
//	Creator: Aman Alam
//	Created: 26-7-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPCanUpdateAssetNameRequest_
#define _h_CZPCanUpdateAssetNameRequest_
#pragma once

#include "CZPCreateAssetRequest.h"
class AZPSoapResponse;

class CZPCanUpdateAssetNameRequest : public CZPCreateAssetRequest
{
public:
						CZPCanUpdateAssetNameRequest();
	virtual				~CZPCanUpdateAssetNameRequest();

	const PMString &	GetAssetID() const;
	void				SetAssetID(
							const PMString &			inAssetID );

	const PMString &	GetEditionID() const;
	void				SetEditionID(
							const PMString &			inEditionID );

	const PMString &	GetTypeID() const;
	void				SetTypeID(
							const PMString &			inTypeID );

	const PMString &	GetParentID() const;
	void				SetParentID(
							const PMString &			inParentID );

#ifdef Debug_WS
	const char *		GetName() const;
#endif
protected:
	AZPSoapResponse *	CreateResponseObj();
private:
	PMString			mAssetID;
	PMString			mEditionID;
	PMString			mTypeID;
	PMString			mParentID;
};

#endif //_h_CZPCanUpdateAssetNameRequest_
