//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/Common/IdleTask/CIdleTaskShowLoginDlg.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 11-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IApplication.h"
#include "IIdleTask.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPApp.h"
#include "IZPLoginController.h"

//IZP General includes
#include "CIdleTaskShowLoginDlg.h"

#include "IZPLog.h"
#pragma mark -

#define thisFileLA	enLMA_Idle
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CIdleTaskShowLoginDlg::CIdleTaskShowLoginDlg()
: AZPIdleTask( eIdleTask_ShowLoginDialog, 0 ,1 )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CIdleTaskShowLoginDlg::~CIdleTaskShowLoginDlg()
{

}

//----------------------------------------------------------------------------------------
// PerformTask
//----------------------------------------------------------------------------------------
void
CIdleTaskShowLoginDlg::PerformTask()
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<IZPLoginController> loginController( zpApp->GetLoginController(), UseDefaultIID() );
	ASSERT( loginController );

	loginController->ShowLoginDialog();
}

//----------------------------------------------------------------------------------------
// CanPerformTask
//----------------------------------------------------------------------------------------
uint32
CIdleTaskShowLoginDlg::CanPerformTask(
	uint32				inAppFlags)	//Flag defined in IIdleTask
{
	//LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IApplication> theApp( gSession->QueryApplication() );

	if(gSession->GetState() == ISession::kRunning && theApp && theApp->GetApplicationState() == IApplication::kRunning )
		return IIdleTask::kEndOfTime;
	return IIdleTask::kOnFlagChange;
}

