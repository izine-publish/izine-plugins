//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Responses/CZPFetchTitleListResponse.h $
//	$Revision: 1061 $
//	$Date: 2010-04-26 11:24:57 +0200 (Mon, 26 Apr 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 23-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPFetchTitleListResponse_
#define _h_CZPFetchTitleListResponse_
#pragma once

#include "AZPSoapResponse.h"
#include "ZPSmartPtrVector.h"

class CZPFetchTitleListResponse : public AZPSoapResponse
{
public:
					CZPFetchTitleListResponse();
	virtual			~CZPFetchTitleListResponse();

	const ZPPMStringSList &	GetTitleKeys() const;
	ZPPMStringSList &		GetTitleKeys();
	void					SetTitleKeys(
								const ZPPMStringSList &		inTitleKeys );

	const ZPPMStringSList &	GetTitleNames() const;
	ZPPMStringSList &		GetTitleNames();
	void					SetTitleNames(
								const ZPPMStringSList &		inTitleNames );

protected:
private:
	ZPPMStringSList		mTitleKeys;
	ZPPMStringSList		mTitleNames;
};

#endif //_h_CZPFetchTitleListResponse_
