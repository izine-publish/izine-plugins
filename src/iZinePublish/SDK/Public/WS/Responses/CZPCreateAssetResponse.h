//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Responses/CZPCreateAssetResponse.h $
//	$Revision: 1768 $
//	$Date: 2010-07-19 09:29:32 +0200 (Mon, 19 Jul 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 26-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPCreateAssetResponse_
#define _h_CZPCreateAssetResponse_
#pragma once

#include "AZPSoapResponse.h"

class CZPCreateAssetResponse : public AZPSoapResponse
{
public:
						CZPCreateAssetResponse();
	virtual				~CZPCreateAssetResponse();

	const PMString &	GetAssetID() const;
	void				SetAssetID(
							const PMString &			inAssetID );

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
	ZPAssetInfo		mAssetInfo;
};

#endif //_h_CZPCreateAssetResponse_
