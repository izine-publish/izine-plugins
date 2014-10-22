//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/Session/IZPSessionData.h $
//	$Revision: 1364 $
//	$Date: 2010-06-08 09:50:35 +0200 (Tue, 08 Jun 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 29-3-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#ifndef _h_IZPSessionData_
#define _h_IZPSessionData_
#pragma once

class IZPSessionData : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPSESSIONDATA };

	virtual const PMString &	GetUserID() const = 0;
	virtual void				SetUserID(
									const PMString &			inUserID ) = 0;
	
	virtual const PMString &	GetUserDisplayName() const = 0;
	virtual void				SetUserDisplayName(
									const PMString &			inUserName ) = 0;
};

#endif //_h_IZPSessionData_
