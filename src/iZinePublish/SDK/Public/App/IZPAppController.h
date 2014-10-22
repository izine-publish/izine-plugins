//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/App/IZPAppController.h $
//	$Revision: 1157 $
//	$Date: 2010-05-17 12:21:28 +0200 (Mon, 17 May 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 29-3-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#ifndef _h_IZPAppController_
#define _h_IZPAppController_
#pragma once

#include "ZPTypes.h"

class IZPAppController : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPAPPCONTROLLER };

	virtual enAppState		GetAppState() const = 0;
	virtual void			SetAppState(
								enAppState				inAppState ) = 0;

};

#ifdef ZPBuild
PLUGIN_DECL
#else
PUBLIC_DECL
#endif
extern IZPAppController *	gZPApp;	//Easy access to global object.

#endif //_h_IZPAppController_
