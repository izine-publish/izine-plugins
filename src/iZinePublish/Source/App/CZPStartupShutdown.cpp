//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/App/CZPStartupShutdown.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 8-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IGlobalTimeData.h"
#include "IObserver.h"
#include "IStartupShutdownService.h"
#include "IStringData.h"
#include "IWorkspace.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes
#include "FileUtils.h"
#include "InCopyBridgeUIID.h"
#include "Utils.h"
#include "IZPID.h"

//IZP Interfaces
#include "IZPApp.h"
#include "IZPAppController.h"
#include "IZPAssetsLocalInfoMgr.h"
#include "IZPFileUtils.h"
#include "IZPLoginController.h"
#include "IZPUserCredentials.h"
#include "IZPWSCallHelper.h"
#include "IZPWSConfig.h"

//IZP General includes
#include "CIdleTaskShowLoginDlg.h"
#include "CZPIdleTaskMgr.h"
#include "ZPConstants.h"
#include "ZPObjectModelHack.h"


#include "IZPLog.h"

#define thisFileLA	enLMA_App
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

class CZPStartupShutdown : public CPMUnknown<IStartupShutdownService>
{
public:
				CZPStartupShutdown(IPMUnknown * boss);
	virtual		~CZPStartupShutdown();

	//	Called by the core when app is starting up
	virtual void Startup();

	//	Called by the core when app is shutting down
	virtual void Shutdown();
protected:
private:
	void		CleanCache();
};


CREATE_PMINTERFACE(CZPStartupShutdown, kZPStartupShutdownImpl)

CZPStartupShutdown::CZPStartupShutdown(IPMUnknown * boss) :
CPMUnknown<IStartupShutdownService>(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPStartupShutdown::~CZPStartupShutdown()
{

}

//----------------------------------------------------------------------------------------
// Startup
//	Called by the core when app is starting up
//----------------------------------------------------------------------------------------
void
CZPStartupShutdown::Startup()
{
	LogFunctionEnterExit;

	//Initializer is not always called. Do app init here as well.
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IZPApp> zpApp( gSession, UseDefaultIID() );	//This will create app object. gZPApp might be nil before this.
	ASSERT( zpApp );

	zpApp->Initialize();

	CleanCache();

	//Hack Object model for debugging.
	//ZPObjectModelHack::SwapImpls( kICBridgeExportDialogCmdBoss, IID_IDLGRESULTDATA, IID_IZPStringDataDelegate, kStringDataImpl );

#if 0	//Disabled as login does not work if opened from here.
	//queue Show login dialog task
	CIdleTaskShowLoginDlg * loginTask = new CIdleTaskShowLoginDlg();
	CZPIdleTaskMgr * taskMgr = CZPIdleTaskMgr::GetInstance();
	taskMgr->AddIdleTask( loginTask );
#else
	//Auto-login
#ifdef InDnCS3
	do
	{
		InterfacePtr<const IWorkspace> theWorkspace(gSession->QueryWorkspace());
		ASSERT(theWorkspace);
		if (!theWorkspace)
			break;
		
		InterfacePtr<IZPWSConfig> iZPWSConfig( theWorkspace, UseDefaultIID() );
		ASSERT(iZPWSConfig);

		InterfacePtr<const IZPUserCredentials> iZPUserCredentials(theWorkspace, UseDefaultIID());
		ASSERT(iZPUserCredentials);

		InterfacePtr<IZPLoginController> loginController( zpApp->GetLoginController(), UseDefaultIID() );
		ASSERT( loginController );		

		if( iZPUserCredentials->GetUserName().IsEmpty() == kFalse && iZPWSConfig->GetURL().IsEmpty() == kFalse )
			loginController->DoLogin(iZPWSConfig, iZPUserCredentials);	
	}while(kFalse);
#endif
#endif
}


//----------------------------------------------------------------------------------------
// Shutdown
//	Called by the core when app is shutting down
//----------------------------------------------------------------------------------------
void
CZPStartupShutdown::Shutdown()
{
	LogFunctionEnterExit;

#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IZPApp> zpApp( gSession, UseDefaultIID() );	//This will create app object. gZPApp might be nil before this.
	ASSERT( zpApp );

	zpApp->Shutdown();
	gZPApp->Release();	//gZPApp will nil beyond this point.
}

//----------------------------------------------------------------------------------------
// CleanCache
//----------------------------------------------------------------------------------------
void
CZPStartupShutdown::CleanCache()
{
	LogFunctionEnterExit;

#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IWorkspace> theWorkspace(gSession->QueryWorkspace());
	ASSERT(theWorkspace);
	if (!theWorkspace)
		return;

	InterfacePtr<IGlobalTimeData> lastCleanCacheDate( theWorkspace, IID_IZPLASTCLEARCACHEDATE );
	ASSERT(lastCleanCacheDate);

	int64 secondsInDays = 86400L * kRunCacheCleanAfterDays;
	int64 beforeTime = secondsInDays * GlobalTime::kOneSecond;	// 1 day = 86400 sec
	const GlobalTime& lastTime = lastCleanCacheDate->Get();
	if (lastTime.GetTime() > 0 && lastTime.IsHistory(beforeTime) == false)
		return;

	//Delete files
	Utils<IZPFileUtils> zpFileUtils;
	IDFile rootPath;
	if (zpFileUtils->GetLocalStoreFolder( rootPath ) )
	{
		PMString cachePath;
		zpFileUtils->ConvertIDFileToStr(rootPath, cachePath);
#ifdef CacheWithFolder
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Removing all files from root cache folder");
		zpFileUtils->RemoveAllFiles(cachePath, "*");
#else
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Removing all pdf files");
		zpFileUtils->RemoveAllFiles(cachePath, "pdf");
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Removing all jpg files");
		zpFileUtils->RemoveAllFiles(cachePath, "jpg");
#endif

		//zpFileUtils->RemoveFiles(chachePath, "e36b49bb-6c6c-4557-b81f-e8d65ac55f92");
	}

	InterfacePtr<IZPAssetsLocalInfoMgr> assetsLocalInfoMgr( gSession, UseDefaultIID() );
	ASSERT( assetsLocalInfoMgr );
	if (assetsLocalInfoMgr)
	{
		assetsLocalInfoMgr->RemoveMissingCacheLocalInfo();	//missing folder
		assetsLocalInfoMgr->ClearCache_OldAssets();
#ifdef CacheWithFolder
		assetsLocalInfoMgr->RemoveMissingLocalAssetsInfo();
#endif
	}

	IDataBase * workspaceDb = ::GetDataBase( theWorkspace );
	workspaceDb->BeginTransaction();

	//This will store current clear time in persist data.
	GlobalTime globalTime;
	globalTime.CurrentTime();
	lastCleanCacheDate->Set(globalTime);

	workspaceDb->EndTransaction();
}
