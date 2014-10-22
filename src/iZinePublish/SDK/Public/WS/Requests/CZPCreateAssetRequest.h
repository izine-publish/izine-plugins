//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Requests/CZPCreateAssetRequest.h $
//	$Revision: 1982 $
//	$Date: 2010-08-17 15:06:40 +0200 (Tue, 17 Aug 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 26-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPCreateAssetRequest_
#define _h_CZPCreateAssetRequest_
#pragma once

#include "AZPSoapRequest.h"
class AZPSoapResponse;

class CZPCreateAssetRequest : public AZPSoapRequest
{
public:
						CZPCreateAssetRequest(
							enRequestType				inReqType = eReq_CreateAsset );
	virtual				~CZPCreateAssetRequest();

	const PMString &	GetAssetName() const;
	void				SetAssetName(
							const PMString &			inName );

	const PMString &	GetEditionID() const;
	void				SetEditionID(
							const PMString &			inEditionID );

	ZPAssetDetailInfo &	GetDetailInfo();
	const ZPAssetDetailInfo &
						GetDetailInfo() const;

#ifdef Debug_WS
	virtual const char *	GetName() const;
#endif
protected:
	virtual AZPSoapResponse *	CreateResponseObj();
private:
	PMString			mAssetName;
	PMString			mEditionID;
	ZPAssetDetailInfo	mDetailInfo;
};

#endif //_h_CZPCreateAssetRequest_
