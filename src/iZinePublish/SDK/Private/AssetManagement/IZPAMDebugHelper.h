//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Private/AssetManagement/IZPAMDebugHelper.h $
//	$Revision: 2193 $
//	$Date: 2010-09-29 11:36:01 +0200 (Wed, 29 Sep 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 13-2-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _IZPAMDebugHelper_H_
#define _IZPAMDebugHelper_H_

class IZPAMDebugHelper : public IPMUnknown
{
public:
	enum	{kDefaultIID = IID_IZPAMDEBUGHELPER};

#ifdef DEBUG
	virtual const char *	GetAMServiceName(
								const IAMServiceProvider::AssetService			inService ) = 0;
	virtual const char *	GetAMUIServiceName(
								const IAMUIServiceProvider::UIService			inService ) = 0;
#endif	//Debug
};

#endif
