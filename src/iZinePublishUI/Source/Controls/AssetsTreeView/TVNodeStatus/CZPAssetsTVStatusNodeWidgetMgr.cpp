//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/AssetsTreeView/TVNodeStatus/CZPAssetsTVStatusNodeWidgetMgr.cpp $
//	$Revision: 1429 $
//	$Date: 2010-06-15 06:22:30 +0200 (Tue, 15 Jun 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 25-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes

//OS / ThirdParty includes

#include "IZPUIID.h"

//IZP Interfaces
#include "IZPUITVNodeWidgetMgr.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AssetsTV
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZPUITVNodeWidgetMgr>
class CZPAssetsTVStatusNodeWidgetMgr : public inheritClass
{
public:
						CZPAssetsTVStatusNodeWidgetMgr(IPMUnknown * boss);
	virtual				~CZPAssetsTVStatusNodeWidgetMgr();

						//Updates the UI controls of this node.
	void				Refresh() const;
protected:
private:
};


CREATE_PMINTERFACE(CZPAssetsTVStatusNodeWidgetMgr, kZPAssetsTVStatusNodeWidgetMgrImpl)

CZPAssetsTVStatusNodeWidgetMgr::CZPAssetsTVStatusNodeWidgetMgr(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAssetsTVStatusNodeWidgetMgr::~CZPAssetsTVStatusNodeWidgetMgr()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// Refresh
//----------------------------------------------------------------------------------------
void
CZPAssetsTVStatusNodeWidgetMgr::Refresh()const
{
	//TODO: move code from tree view widget mgr to here.
}


