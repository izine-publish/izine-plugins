//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/App/IZPLoginController.h $
//	$Revision: 1460 $
//	$Date: 2010-06-16 13:23:32 +0200 (Wed, 16 Jun 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 29-3-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#ifndef _h_IZPLoginController_
#define _h_IZPLoginController_
#pragma once

class IZPWSConfig;
class IZPUserCredentials;

class IZPLoginController : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPLOGINCONTROLLER };

	//Create and displays the login dialog.
	virtual void		ShowLoginDialog() = 0;

	//Initiates the login process
	// Messages are sent in the following order.
	// BeforeLogin, Authenticating, Authenticated, FetchingConfigData, FetchedConfigData, LoggedIn 
	// After sucessful authentication settings are stored with workspace.
	virtual void		DoLogin(
							const IZPWSConfig *					inWSConfig,
							const IZPUserCredentials *			inUserCeredentials ) = 0;

	virtual void		DoLogoff() = 0;	//Logoff the default session
	virtual void		DoLogoff(
							const IZPWSConfig *					inWSConfig,
							const IZPUserCredentials *			inUserCeredentials ) = 0;
							
	//For internal use only
	virtual void		HandleAuthenticationSuccess(
							const IZPWSConfig *					inWSConfig,
							const IZPUserCredentials *			inUserCeredentials,
							const PMString &					inUserID,
							const PMString &					inUserDisplayName  ) = 0;

	virtual void		AddLoginWaitCount() = 0;
	virtual void		ReduceLoginWaitCount() =0;
};

#endif //_h_IZPLoginController_
