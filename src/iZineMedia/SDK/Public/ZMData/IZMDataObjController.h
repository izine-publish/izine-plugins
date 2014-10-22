//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/SDK/Public/ZMData/IZMDataObjController.h $
//	$Revision: 2750 $
//	$Date: 2011-04-08 11:33:52 +0200 (Fri, 08 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 8-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZMDataObjController_
#define _h_IZMDataObjController_
#pragma once


class IZMDataObjController : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZMDATAOBJCONTROLLER };

	virtual void		RequestUpdateFromServer(
							PMIID						inWhichProtocol = kInvalidInterfaceID ) = 0;

};


#endif //_h_IZMDataObjController_
