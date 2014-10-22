//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/SDK/Public/App/IZMApp.h $
//	$Revision: 2782 $
//	$Date: 2011-04-11 16:14:31 +0200 (Mon, 11 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 29-3-2010
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description: Interface to get major global objects/controllers.
//========================================================================================

#ifndef _h_IZMApp_
#define _h_IZMApp_
#pragma once

class IZMAdMgr;
class IZMAdFileMgr;
class IZMTitleMgr;
class IZMEditionMgr;

class IZMApp : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZMAPP };

	virtual void					Initialize() = 0;
	virtual void					Shutdown() = 0;

	virtual IZMAdFileMgr *			GetAdFileMgr() const = 0;
	virtual IZMTitleMgr *			GetTitleMgr() const = 0;
	virtual IZMEditionMgr *			GetEditionMgr() const = 0;
	virtual IZMAdMgr *				GetAdMgr() const = 0;
	
};

#endif //_h_IZMApp_
