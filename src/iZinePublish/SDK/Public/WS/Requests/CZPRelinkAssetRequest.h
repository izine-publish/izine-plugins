//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Requests/CZPRelinkAssetRequest.h $
//	$Revision: 1932 $
//	$Date: 2010-08-10 13:29:07 +0200 (Tue, 10 Aug 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 10-8-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPRelinkAssetRequest_
#define _h_CZPRelinkAssetRequest_
#pragma once

#include "AZPSoapRequest.h"
class AZPSoapResponse;

class CZPRelinkAssetRequest : public AZPSoapRequest
{
public:
						CZPRelinkAssetRequest();
	virtual				~CZPRelinkAssetRequest();

	const PMString &	GetAssetID() const;
	void				SetAssetID(
							const PMString &			inAssetID );

	const PMString &	GetLinkedWithAssetID() const;
	void				SetLinkedWithAssetID(
							const PMString &			inLinkedWithAssetID );


#ifdef Debug_WS
	const char *		GetName() const;
#endif
protected:
	AZPSoapResponse *	CreateResponseObj();
private:
	PMString		mAssetID;
	PMString		mLinkedWithAssetID;
};

#endif //_h_CZPRelinkAssetRequest_
