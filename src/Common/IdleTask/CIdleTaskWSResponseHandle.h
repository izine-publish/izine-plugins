//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/Common/IdleTask/CIdleTaskWSResponseHandle.h $
//	$Revision: 3276 $
//	$Date: 2011-06-17 08:47:26 +0200 (Fri, 17 Jun 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 11-5-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_CIdleTaskWSResponseHandle_
#define _h_CIdleTaskWSResponseHandle_
#pragma once

//SDK Interfaces

//SDK General includes

//#include "IZPID.h"

//IZP Interfaces

//IZP General includes
#include "AZPIdleTask.h"


class IZPWSController;
class IZPSoapRequest;
class AZPSoapResponse;

#pragma mark -

class CIdleTaskWSResponseHandle : public AZPIdleTask
{
public:
					CIdleTaskWSResponseHandle();
	virtual			~CIdleTaskWSResponseHandle();

	virtual uint32	CanPerformTask(
						uint32				inAppFlags);	//Flag defined in IIdleTask
	virtual	void	PerformTask();

	void					SetData(
								IZPWSController *						inWSController,
								const IZPSoapRequest *					inSoapRequest,
								AZPSoapResponse *						inResponse,
								ZPWSError								inErrVal);
	void					SetData(
								IZPWSController *						inWSController,
								const IZPSoapRequest *					inSoapRequest,
								ZPWSError								inErrVal);
protected:
private:

	IZPWSController *						mWSController;
	const IZPSoapRequest *					mSoapRequest;
	AZPSoapResponse *						mResponse;
	ZPWSError								mErrVal;
};

#endif //_h_CIdleTaskWSResponseHandle_
