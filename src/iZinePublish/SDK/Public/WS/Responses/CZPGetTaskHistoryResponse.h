//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Responses/CZPGetTaskHistoryResponse.h $
//	$Revision: 2355 $
//	$Date: 2010-10-19 13:16:41 +0200 (Tue, 19 Oct 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 18-10-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPGetTaskHistoryResponse_
#define _h_CZPGetTaskHistoryResponse_
#pragma once

#include "AZPSoapResponse.h"

class ZPEXPORTS CZPGetTaskHistoryResponse : public AZPSoapResponse
{
public:
						CZPGetTaskHistoryResponse();
	virtual				~CZPGetTaskHistoryResponse();

	const unsigned char *	GetDataPtr() const;
	void				SetDataPtr(
							unsigned char *				inDataPtr,
							bool						inOwnsPtr = true);

	unsigned char *		ForgetDataPtr() const;

protected:
private:
	
	mutable bool	mOwnsPtr;
	unsigned char *	mDataPtr;
};

#endif //_h_CZPGetTaskHistoryResponse_
