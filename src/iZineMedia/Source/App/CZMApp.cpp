//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/App/CZMApp.cpp $
//	$Revision: 2782 $
//	$Date: 2011-04-11 16:14:31 +0200 (Mon, 11 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 29-3-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IObserver.h"

//SDK General includes

#include "IZMediaID.h"

//IZP Interfaces
#include "IZMApp.h"
#include "IZMAdMgr.h"
#include "IZMAdFileMgr.h"
#include "IZMEditionMgr.h"
#include "IZMTitleMgr.h"

//IZP General includes
#include "IZPLog.h"

#define thisFileLA	enLMA_App
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

class CZMApp : public CPMUnknown<IZMApp>
{
public:
				CZMApp(IPMUnknown * boss);
	virtual		~CZMApp();
	
	void							Initialize();
	void							Shutdown();
	
	IZMAdFileMgr *					GetAdFileMgr() const;
	IZMTitleMgr *					GetTitleMgr() const;
	IZMEditionMgr *					GetEditionMgr() const;
	IZMAdMgr *						GetAdMgr() const;
	
protected:
private:
	InterfacePtr<IZMTitleMgr>			mTitleMgr;
	bool								mInitDone;
};

CREATE_PMINTERFACE(CZMApp, kZMAppImpl)

CZMApp::CZMApp(IPMUnknown * boss) :
CPMUnknown<IZMApp>(boss)
, mInitDone( false )
{
	LogFunctionEnterExit;
	IZMTitleMgr* theTitleMgr = ::CreateObject2<IZMTitleMgr>( kZMDataMgrBoss );
	mTitleMgr.reset( theTitleMgr );
}

CZMApp::~CZMApp()
{
	LogFunctionEnterExit;
}

//----------------------------------------------------------------------------------------
// Initialize
//----------------------------------------------------------------------------------------
void
CZMApp::Initialize()
{
	LogFunctionEnterExit;
	if( mInitDone )
		return;
	
	mInitDone = true;
}

//----------------------------------------------------------------------------------------
// Shutdown
//----------------------------------------------------------------------------------------
void
CZMApp::Shutdown()
{
	//free all managers data.
	IZMAdFileMgr * adFileMgr = this->GetAdFileMgr();
	ASSERT( adFileMgr );
	adFileMgr ->Shutdown();

	IZMTitleMgr * titleMgr = this->GetTitleMgr();
	ASSERT( titleMgr );
	titleMgr->Shutdown();

	IZMEditionMgr * editionMgr = this->GetEditionMgr();
	ASSERT( editionMgr );
	editionMgr->Shutdown();

}

//----------------------------------------------------------------------------------------
// GetAdMgr
//----------------------------------------------------------------------------------------
IZMAdMgr *
CZMApp::GetAdMgr()const
{
	return InterfacePtr<IZMAdMgr> ( mTitleMgr, UseDefaultIID() );
}

//----------------------------------------------------------------------------------------
// GetAdFileMgr
//----------------------------------------------------------------------------------------
IZMAdFileMgr *
CZMApp::GetAdFileMgr()const
{
	return InterfacePtr<IZMAdFileMgr> ( mTitleMgr, UseDefaultIID() );
}


//----------------------------------------------------------------------------------------
// GetTitleMgr
//----------------------------------------------------------------------------------------
IZMTitleMgr *
CZMApp::GetTitleMgr()const
{
	return mTitleMgr;
}


//----------------------------------------------------------------------------------------
// GetEditionMgr
//----------------------------------------------------------------------------------------
IZMEditionMgr *
CZMApp::GetEditionMgr()const
{
	return InterfacePtr<IZMEditionMgr> ( mTitleMgr, UseDefaultIID() );
}

