//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/App/IZPApp.h $
//	$Revision: 2213 $
//	$Date: 2010-10-01 12:44:26 +0200 (Fri, 01 Oct 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 29-3-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description: Interface to get major global objects/controllers.
//========================================================================================

#ifndef _h_IZPApp_
#define _h_IZPApp_
#pragma once

class IZPAppController;
class IZPLoginController;
class IZPWSResponseHandler;

class IZPAssetMgr;
class IZPTitleMgr;
class IZPEditionMgr;
class IZPUserMgr;
class IZPTaskMgr;

class IZPApp : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPAPP };

	virtual void					Initialize() = 0;
	virtual void					Shutdown() = 0;

	virtual IZPAppController *		GetAppController() const = 0;
	virtual IZPLoginController *	GetLoginController() const = 0;

	virtual IZPAssetMgr *			GetAssetMgr() const = 0;
	virtual IZPTitleMgr *			GetTitleMgr() const = 0;
	virtual IZPEditionMgr *			GetEditionMgr() const = 0;
	virtual IZPUserMgr *			GetUserMgr() const = 0;
	virtual IZPTaskMgr *			GetTaskMgr() const = 0;

	virtual IZPWSResponseHandler *	GetDefaultWSResponseHandler() const = 0;
};

#endif //_h_IZPApp_
