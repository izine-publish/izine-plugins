//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Data/CZMDataObjController.h $
//	$Revision: 2750 $
//	$Date: 2011-04-08 11:33:52 +0200 (Fri, 08 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 21-6-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CZMDataObjController_
#define _h_CZMDataObjController_
#pragma once

//#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes

//OS / ThirdParty includes

#include "iZMediaID.h"

//IZP Interfaces
#include "IZMDataObjController.h"

//IZP General includes

#pragma mark -

#define inheritClass_CZMDataObjController CPMUnknown<IZMDataObjController>
class CZMDataObjController : public inheritClass_CZMDataObjController
{
public:
						CZMDataObjController(IPMUnknown * boss);
	virtual				~CZMDataObjController();

	virtual void		RequestUpdateFromServer(
							PMIID						inWhichProtocol );
protected:
private:
};


#endif //_h_CZMDataObjController_
