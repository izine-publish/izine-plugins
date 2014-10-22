//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/Session/IZPUserCredentials.h $
//	$Revision: 1005 $
//	$Date: 2010-04-16 03:29:59 +0200 (Fri, 16 Apr 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 29-3-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#ifndef _h_IZPUserCredentials_
#define _h_IZPUserCredentials_
#pragma once

#include "ZPString.h"


class IZPUserCredentials : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPUSERCREDENTIALS };

	virtual void					Copy(
										const IZPUserCredentials *	inValuesFrom ) = 0;

	virtual const PMString &		GetUserName() const = 0;
	virtual void					SetUserName( 
										const PMString &			inUserName) = 0;
	virtual void					SetUserName(
										const char *				inUserName) = 0;

	virtual const PMString &		GetPwd() const = 0;
	virtual void					SetPwd( 
										const PMString &			inPwd) = 0;
	virtual void					SetPwd(
										const char *				inPwd) = 0;

};

#endif //_h_IZPUserCredentials_
