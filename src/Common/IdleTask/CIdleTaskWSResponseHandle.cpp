//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/Common/IdleTask/CIdleTaskWSResponseHandle.cpp $
//	$Revision: 3565 $
//	$Date: 2011-08-04 12:09:14 +0200 (Thu, 04 Aug 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 11-5-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IApplication.h"
#include "IIdleTask.h"

//SDK General includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPApp.h"
#include "IZPWSController.h"

//IZP General includes
#include "CIdleTaskWSResponseHandle.h"

#include "IZPLog.h"
#pragma mark -

#define thisFileLA	enLMA_Idle
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CIdleTaskWSResponseHandle::CIdleTaskWSResponseHandle()
: AZPIdleTask( eIdleTask_CallWSRespHandler, 0 ,1 )
, mWSController(nil)
, mSoapRequest(nil)
, mResponse(nil)
, mErrVal(0)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CIdleTaskWSResponseHandle::~CIdleTaskWSResponseHandle()
{

}

//----------------------------------------------------------------------------------------
// PerformTask
//----------------------------------------------------------------------------------------
void
CIdleTaskWSResponseHandle::PerformTask()
{
	LogFunctionEnterExit;
	if( mResponse )
		mWSController->CallResponseHandler( mSoapRequest, mResponse, mErrVal );
	else
		mWSController->CallRequestErrorHandler( mSoapRequest, mErrVal );
}

//----------------------------------------------------------------------------------------
// CanPerformTask
//----------------------------------------------------------------------------------------
uint32
CIdleTaskWSResponseHandle::CanPerformTask(
	uint32				inAppFlags)	//Flag defined in IIdleTask
{
	return IIdleTask::kEndOfTime;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// SetData
//----------------------------------------------------------------------------------------
void
CIdleTaskWSResponseHandle::SetData(
	IZPWSController *						inWSController,
	const IZPSoapRequest *					inSoapRequest,
	AZPSoapResponse *						inResponse,
	ZPWSError								inErrVal)
{
	//LogFunctionEnterExit;
	mWSController = inWSController;
	mSoapRequest = inSoapRequest;
	mResponse = inResponse;
	mErrVal = inErrVal;
}

//----------------------------------------------------------------------------------------
// SetData
//----------------------------------------------------------------------------------------
void
CIdleTaskWSResponseHandle::SetData(
	IZPWSController *						inWSController,
	const IZPSoapRequest *					inSoapRequest,
	ZPWSError								inErrVal)
{
	//LogFunctionEnterExit;
	mWSController = inWSController;
	mSoapRequest = inSoapRequest;
	mResponse = nil;
	mErrVal = inErrVal;
}
