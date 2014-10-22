//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Requests/CZPCheckInAssetRequest.h $
//	$Revision: 1769 $
//	$Date: 2010-07-19 09:47:19 +0200 (Mon, 19 Jul 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 26-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPCheckInAssetRequest_
#define _h_CZPCheckInAssetRequest_
#pragma once

#include "AZPSoapRequest.h"
class AZPSoapResponse;

class CZPCheckInAssetRequest : public AZPSoapRequest
{
public:
						CZPCheckInAssetRequest();
	virtual				~CZPCheckInAssetRequest();

	const PMString &	GetAssetID() const;
	void				SetAssetID(
							const PMString &			inAssetID );

	const PMString &	GetNewVersionID() const;
	void				SetNewVersionID(
							const PMString &			inNewVersionID );

	const PMString &	GetStatusID() const;
	void				SetStatusID(
							const PMString &			inStatusID );
	const PMString &	GetComment() const;
	void				SetComment(
							const PMString &			inComment );

	int					GetHeadVersion() const;
	void				SetHeadVersion(
							int							inHeadVersion );

#ifdef Debug_WS
	const char *		GetName() const;
#endif
protected:
	AZPSoapResponse *	CreateResponseObj();
private:
	PMString		mAssetID;
	PMString		mNewVersionID;
	PMString		mStatusID;
	PMString		mComment;
	int				mHeadVersion;	//Used to make sure that check-in is being made after updating to head.
									//WS call will fail if this version number is non-zero and not equal to current head.
};

#endif //_h_CZPCheckInAssetRequest_
