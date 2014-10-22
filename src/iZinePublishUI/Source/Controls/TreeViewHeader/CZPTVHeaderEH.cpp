//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/TreeViewHeader/CZPTVHeaderEH.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 18-10-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IApplication.h"
#include "IActionManager.h"
#include "IMenuManager.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes
#include "PanelEventHandler.h"

//OS / ThirdParty includes

#include "IZPUIID.h"

//IZP Interfaces

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass PanelEventHandler
class CZPTVHeaderEH : public inheritClass
{
public:
						CZPTVHeaderEH(IPMUnknown * boss);
	virtual				~CZPTVHeaderEH();

	bool16				RButtonDn(IEvent* e);

	/** Right mouse button released.
	 @param e [IN] event of interest
	 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
	 */
	bool16				RButtonUp(IEvent* e);
protected:
private:
};


CREATE_PMINTERFACE(CZPTVHeaderEH, kZPTVHeaderEHImpl)

CZPTVHeaderEH::CZPTVHeaderEH(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPTVHeaderEH::~CZPTVHeaderEH()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// RButtonDn
//----------------------------------------------------------------------------------------
bool16
CZPTVHeaderEH::RButtonDn(
	IEvent* e)
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	do
	{
		InterfacePtr<IApplication> app( gSession->QueryApplication() );
		ASSERT( app );
		if(!app)
			break;
		
		InterfacePtr<IActionManager> actionMgr( app->QueryActionManager() );
		ASSERT( actionMgr );
		if(!actionMgr)
			break;
		
		InterfacePtr<IMenuManager> menuMgr( actionMgr, UseDefaultIID() );
		ASSERT( menuMgr );
		if(!menuMgr)
			break;
	
		menuMgr->HandlePopupMenu( kIZPTVHeaderMenuPath, e->GlobalWhere(), e->GlobalWhere(), kTrue, this );
		return kTrue;
	}while(kFalse);

	return inheritClass::RButtonDn( e );
}

//----------------------------------------------------------------------------------------
// RButtonUp
//----------------------------------------------------------------------------------------
bool16
CZPTVHeaderEH::RButtonUp(
	IEvent* e)
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	do
	{
		InterfacePtr<IApplication> app( gSession->QueryApplication() );
		ASSERT( app );
		if(!app)
			break;
		
		InterfacePtr<IActionManager> actionMgr( app->QueryActionManager() );
		ASSERT( actionMgr );
		if(!actionMgr)
			break;
		
		InterfacePtr<IMenuManager> menuMgr( actionMgr, UseDefaultIID() );
		ASSERT( menuMgr );
		if(!menuMgr)
			break;
	
		menuMgr->HandlePopupMenu( kIZPTVHeaderMenuPath, e->GlobalWhere(), e->GlobalWhere(), kTrue, this );
		return kTrue;
	}while(kFalse);

	return inheritClass::RButtonUp( e );
}
