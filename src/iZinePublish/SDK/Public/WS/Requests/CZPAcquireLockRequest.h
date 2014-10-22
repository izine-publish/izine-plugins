//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Requests/CZPAcquireLockRequest.h $
//	$Revision: 1446 $
//	$Date: 2010-06-16 05:15:31 +0200 (Wed, 16 Jun 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 26-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPAcquireLockRequest_
#define _h_CZPAcquireLockRequest_
#pragma once

#include "AZPSoapRequest.h"
class AZPSoapResponse;

class CZPAcquireLockRequest : public AZPSoapRequest
{
public:
						CZPAcquireLockRequest();
	virtual				~CZPAcquireLockRequest();

	const PMString &	GetAssetID() const;
	void				SetAssetID(
							const PMString &			inAssetID );

	const PMString &	GetLinkedWithDocID() const;
	void				SetLinkedWithDocID(
							const PMString &			inLinkedWithDocID );
	const PMString &	GetLinkedWithDocName() const;
	void				SetLinkedWithDocName(
							const PMString &			inLinkedWithDocName );

	const PMString &	GetAppName() const;
	void				SetAppName(
							const PMString &			inAppName );

#ifdef Debug_WS
	const char *		GetName() const;
#endif
protected:
	AZPSoapResponse *	CreateResponseObj();
private:
	PMString		mAssetID;
	PMString		mLinkedWithDocID;
	PMString		mLinkedWithDocName;
	PMString		mAppName;
};

#endif //_h_CZPAcquireLockRequest_
