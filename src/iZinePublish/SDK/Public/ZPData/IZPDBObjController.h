//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/ZPData/IZPDBObjController.h $
//	$Revision: 2249 $
//	$Date: 2010-10-06 16:44:33 +0200 (Wed, 06 Oct 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 21-6-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPDBObjController_
#define _h_IZPDBObjController_
#pragma once


class IZPDBObjController : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPDBOBJCONTROLLER };

	virtual void		RequestUpdateFromServer(
							PMIID						inWhichProtocol = kInvalidInterfaceID ) = 0;

};


#endif //_h_IZPDBObjController_
