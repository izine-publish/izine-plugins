//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Controls/TreeViewHeader/CZMTVHeaderEH.cpp $
//	$Revision: 2724 $
//	$Date: 2011-04-07 09:30:18 +0200 (Thu, 07 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 7-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IApplication.h"
#include "IActionManager.h"
#include "IMenuManager.h"

//SDK General includes
#include "PanelEventHandler.h"

//OS / ThirdParty includes

#include "iZMediaID.h"

//IZP Interfaces

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass PanelEventHandler
class CZMTVHeaderEH : public inheritClass
{
public:
						CZMTVHeaderEH(IPMUnknown * boss);
	virtual				~CZMTVHeaderEH();

	bool16				RButtonDn(IEvent* e);

	/** Right mouse button released.
	 @param e [IN] event of interest
	 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
	 */
	bool16				RButtonUp(IEvent* e);
protected:
private:
};


CREATE_PMINTERFACE(CZMTVHeaderEH, kZMTVHeaderEHImpl)

CZMTVHeaderEH::CZMTVHeaderEH(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZMTVHeaderEH::~CZMTVHeaderEH()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// RButtonDn
//----------------------------------------------------------------------------------------
bool16
CZMTVHeaderEH::RButtonDn(
	IEvent* e)
{
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
	
		menuMgr->HandlePopupMenu( kIZMTVHeaderMenuPath, e->GlobalWhere(), e->GlobalWhere(), kTrue, this );
		return kTrue;
	}while(kFalse);

	return inheritClass::RButtonDn( e );
}

//----------------------------------------------------------------------------------------
// RButtonUp
//----------------------------------------------------------------------------------------
bool16
CZMTVHeaderEH::RButtonUp(
	IEvent* e)
{
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
	
		menuMgr->HandlePopupMenu( kIZMTVHeaderMenuPath, e->GlobalWhere(), e->GlobalWhere(), kTrue, this );
		return kTrue;
	}while(kFalse);

	return inheritClass::RButtonUp( e );
}
