//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/App/CZPApp.cpp $
//	$Revision: 4080 $
//	$Date: 2012-10-12 08:52:24 +0200 (Fri, 12 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 29-3-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IObserver.h"

//SDK General includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPApp.h"
#include "IZPAppController.h"
#include "IZPAssetMgr.h"
#include "IZPEditionMgr.h"
#include "IZPLoginController.h"
#include "IZPTaskMgr.h"
#include "IZPTitleMgr.h"
#include "IZPTitleStatusMgr.h"
#include "IZPUserMgr.h"
#include "IZPWSResponseHandler.h"


//IZP General includes
#include "IZPLog.h"

#define thisFileLA	enLMA_App
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

class CZPApp : public CPMUnknown<IZPApp>
{
public:
				CZPApp(IPMUnknown * boss);
	virtual		~CZPApp();
	
	void							Initialize();
	void							Shutdown();
	
	IZPAppController *				GetAppController() const;
	IZPLoginController *			GetLoginController() const;
	IZPWSResponseHandler *			GetDefaultWSResponseHandler() const;

	IZPAssetMgr *					GetAssetMgr() const;
	IZPTitleMgr *					GetTitleMgr() const;
	IZPEditionMgr *					GetEditionMgr() const;
	IZPUserMgr *					GetUserMgr() const;
	IZPTaskMgr *					GetTaskMgr() const;

protected:
private:
	InterfacePtr<IZPAppController>		mAppController;

	InterfacePtr<IZPAssetMgr>			mAssetMgr;
	bool								mInitDone;
};

CREATE_PMINTERFACE(CZPApp, kZPAppImpl)

CZPApp::CZPApp(IPMUnknown * boss) :
CPMUnknown<IZPApp>(boss)
, mInitDone( false )
{
	LogFunctionEnterExit;
	if( !mAppController )
	{
		mInitDone = false;
		IZPAppController* theApp = ::CreateObject2<IZPAppController>( kZPAppBoss );
		mAppController.reset( theApp );
	}

	if( !mAssetMgr )
	{
		IZPAssetMgr* theAssetMgr = ::CreateObject2<IZPAssetMgr>( kZPDataMgrBoss );
		mAssetMgr.reset( theAssetMgr );
	}
}

CZPApp::~CZPApp()
{
	LogFunctionEnterExit;
}

//----------------------------------------------------------------------------------------
// Initialize
//----------------------------------------------------------------------------------------
void
CZPApp::Initialize()
{
	LogFunctionEnterExit;
	if( mInitDone )
		return;

	InterfacePtr<IObserver> appObserver( this->GetAppController(), UseDefaultIID() );
	ASSERT(appObserver);

	appObserver->AutoAttach();

	mInitDone = true;
}

//----------------------------------------------------------------------------------------
// Shutdown
//----------------------------------------------------------------------------------------
void
CZPApp::Shutdown()
{
	LogFunctionEnterExit;
	InterfacePtr<IObserver> appObserver( this->GetAppController(), UseDefaultIID() );
	ASSERT(appObserver);

	appObserver->AutoDetach();

	//free all managers data.
	IZPAssetMgr * assetMgr = this->GetAssetMgr();
	ASSERT( assetMgr );
	assetMgr ->Shutdown();

	IZPTitleMgr * titleMgr = this->GetTitleMgr();
	ASSERT( titleMgr );
	titleMgr->Shutdown();

	IZPEditionMgr * editionMgr = this->GetEditionMgr();
	ASSERT( editionMgr );
	editionMgr->Shutdown();

	InterfacePtr<IZPTitleStatusMgr> statusMgr( titleMgr, UseDefaultIID() );
	ASSERT( statusMgr );
	statusMgr->Shutdown();

	IZPUserMgr * userMgr = this->GetUserMgr();
	ASSERT( userMgr );
	if( userMgr )
		userMgr->Shutdown();
}


//----------------------------------------------------------------------------------------
// GetAppController
//----------------------------------------------------------------------------------------
IZPAppController *
CZPApp::GetAppController()const
{
	//LogFunctionEnterExit;
	return mAppController;
}


//----------------------------------------------------------------------------------------
// GetLoginController
//----------------------------------------------------------------------------------------
IZPLoginController *
CZPApp::GetLoginController()const
{
	//LogFunctionEnterExit;
	return InterfacePtr<IZPLoginController> ( mAppController, UseDefaultIID() );
}

//----------------------------------------------------------------------------------------
// GetDefaultWSResponseHandler
//----------------------------------------------------------------------------------------
IZPWSResponseHandler *
CZPApp::GetDefaultWSResponseHandler()const
{
	return InterfacePtr<IZPWSResponseHandler> ( mAppController, UseDefaultIID() );
}

//----------------------------------------------------------------------------------------
// GetAssetMgr
//----------------------------------------------------------------------------------------
IZPAssetMgr *
CZPApp::GetAssetMgr()const
{
	//LogFunctionEnterExit;
	return mAssetMgr;
}


//----------------------------------------------------------------------------------------
// GetTitleMgr
//----------------------------------------------------------------------------------------
IZPTitleMgr *
CZPApp::GetTitleMgr()const
{
	LogFunctionEnterExit;
	return InterfacePtr<IZPTitleMgr> ( mAssetMgr, UseDefaultIID() );
}


//----------------------------------------------------------------------------------------
// GetEditionMgr
//----------------------------------------------------------------------------------------
IZPEditionMgr *
CZPApp::GetEditionMgr()const
{
	//LogFunctionEnterExit;
	return InterfacePtr<IZPEditionMgr> ( mAssetMgr, UseDefaultIID() );
}

//----------------------------------------------------------------------------------------
// GetUserMgr
//----------------------------------------------------------------------------------------
IZPUserMgr *
CZPApp::GetUserMgr()const
{
	//LogFunctionEnterExit;
	return InterfacePtr<IZPUserMgr> ( mAssetMgr, UseDefaultIID() );
}

//----------------------------------------------------------------------------------------
// GetTaskMgr
//----------------------------------------------------------------------------------------
IZPTaskMgr *
CZPApp::GetTaskMgr()const
{
	//LogFunctionEnterExit;
	return InterfacePtr<IZPTaskMgr> ( mAssetMgr, UseDefaultIID() );
}
