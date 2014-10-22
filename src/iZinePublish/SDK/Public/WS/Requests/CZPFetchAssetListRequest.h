//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Requests/CZPFetchAssetListRequest.h $
//	$Revision: 2236 $
//	$Date: 2010-10-05 16:05:53 +0200 (Tue, 05 Oct 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 4-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPFetchAssetListRequest_
#define _h_CZPFetchAssetListRequest_
#pragma once

#include "AZPSoapRequest.h"
class AZPSoapResponse;

class CZPFetchAssetListRequest : public AZPSoapRequest
{
public:
						CZPFetchAssetListRequest();
	virtual				~CZPFetchAssetListRequest();

	const PMString &	GetEditionID() const;
	void				SetEditionID(
							const PMString &			inEditionID );

	const K2Vector<PMString>&
						GetAssetTypes() const;
	void				SetAssetTypes(
							const K2Vector<PMString>&	inAssetTypes );

#ifdef Debug_WS
	const char *		GetName() const;
#endif
protected:
	AZPSoapResponse *	CreateResponseObj();
private:

	PMString			mEditionID;
	K2Vector<PMString>	mAssetTypes;
};

#endif //_h_CZPFetchAssetListRequest_
