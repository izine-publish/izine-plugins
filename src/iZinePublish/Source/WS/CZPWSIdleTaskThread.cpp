//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/CZPWSIdleTaskThread.cpp $
//	$Revision: 1274 $
//	$Date: 2010-06-01 08:46:28 +0200 (Tue, 01 Jun 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 18-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IIdleTaskMgr.h"

//SDK General includes
#include "CIdleTaskThread.h"

#include "IZPID.h"

//IZP Interfaces
#include "IZPWSThreadHandler.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_WS
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CIdleTaskThread 
class CZPWSIdleTaskThread : public inheritClass
{
public:
						CZPWSIdleTaskThread(IPMUnknown * boss);
	virtual				~CZPWSIdleTaskThread();

	const char *  		TaskName ();
	void  				RunThread (uint32 flags);
	uint32  			RunTask (uint32 appFlags, IdleTimer *timeCheck);
protected:
private:
};


CREATE_PMINTERFACE(CZPWSIdleTaskThread, kZPWSIdleTaskThreadImpl)

CZPWSIdleTaskThread::CZPWSIdleTaskThread(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPWSIdleTaskThread::~CZPWSIdleTaskThread()
{

}

//----------------------------------------------------------------------------------------
// TaskName
//----------------------------------------------------------------------------------------
const char *
CZPWSIdleTaskThread::TaskName ()
{
	return "iZine WS Thread";
}

//----------------------------------------------------------------------------------------
// RunTask
//----------------------------------------------------------------------------------------
uint32
CZPWSIdleTaskThread::RunTask (
	uint32 appFlags, IdleTimer *timeCheck)
{
	appFlags = appFlags & IIdleTaskMgr::kModalDialogUp;
	uint32 toReturn = inheritClass::RunTask( 0, timeCheck );
	return toReturn;
}



//----------------------------------------------------------------------------------------
// RunThread
//----------------------------------------------------------------------------------------
void
CZPWSIdleTaskThread::RunThread (
	uint32 flags)
{
	LogFunctionEnterExit;
	//Process the request queue
	InterfacePtr<IZPWSThreadHandler> threadHandler( this, UseDefaultIID() );
	ASSERT(threadHandler);
	do
	{
		if(false == threadHandler->ProcessRequestList())
			break;
		this->YieldToEventLoop(10);
	}while(true);
}


