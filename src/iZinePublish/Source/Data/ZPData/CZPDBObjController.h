//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Data/ZPData/CZPDBObjController.h $
//	$Revision: 3276 $
//	$Date: 2011-06-17 08:47:26 +0200 (Fri, 17 Jun 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 21-6-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZPDBObjController_
#define _h_CZPDBObjController_
#pragma once

//SDK Interfaces

//SDK General includes

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPDBObjController.h"

//IZP General includes

#pragma mark -

#define inheritClass_CZPDBObjController CPMUnknown<IZPDBObjController>
class CZPDBObjController : public inheritClass_CZPDBObjController
{
public:
						CZPDBObjController(IPMUnknown * boss);
	virtual				~CZPDBObjController();

	virtual void		RequestUpdateFromServer(
							PMIID						inWhichProtocol );
protected:
private:
};


#endif //_h_CZPDBObjController_
