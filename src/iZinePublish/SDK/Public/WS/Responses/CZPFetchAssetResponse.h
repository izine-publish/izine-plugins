//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Responses/CZPFetchAssetResponse.h $
//	$Revision: 1241 $
//	$Date: 2010-05-26 12:07:45 +0200 (Wed, 26 May 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 26-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPFetchAssetResponse_
#define _h_CZPFetchAssetResponse_
#pragma once

#include "AZPSoapResponse.h"

class CZPFetchAssetResponse : public AZPSoapResponse
{
public:
						CZPFetchAssetResponse();
	virtual				~CZPFetchAssetResponse();

	const PMString &	GetName() const;
	void				SetName(
							const PMString &			inName );

	const PMString &	GetEditionID() const;
	void				SetEditionID(
							const PMString &			inEditionID );

	ZPAssetDetailInfo &	GetDetailInfo();
	const ZPAssetDetailInfo &
						GetDetailInfo() const;

protected:
private:
	PMString			mName;
	PMString			mEditionID;
	ZPAssetDetailInfo	mDetailInfo;
	
};

#endif //_h_CZPFetchAssetResponse_
