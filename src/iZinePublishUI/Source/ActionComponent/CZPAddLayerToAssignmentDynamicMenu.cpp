//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/ActionComponent/CZPAddLayerToAssignmentDynamicMenu.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Aman Alam
//	Created: 15-7-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IActionComponent.h"
#include "IActionManager.h"
#include "IActiveContext.h"
#include "IApplication.h"
#include "IDynamicMenu.h"
#include "IMenuManager.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes
#include "AssignmentUIID.h"

//OS / ThirdParty includes

#include "IZPUIID.h"

//IZP Interfaces

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_ActionComponent
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IDynamicMenu>
class CZPAddLayerToAssignmentDynamicMenu : public inheritClass
{
public:
						CZPAddLayerToAssignmentDynamicMenu(IPMUnknown * boss);
	virtual				~CZPAddLayerToAssignmentDynamicMenu();

	void				RebuildMenu(
							ActionID					dynamicActionID,
							IPMUnknown*					widget);
protected:
	void				RebuildShowColumnsMenu(
							ActionID					dynamicActionID,
							IPMUnknown*					widget);
	void				ClearExistingItems(
							IActionManager *			inActionMgr,
							IMenuManager *				inMenuMgr,
							const PMString				inTargetMenuPath,
							ActionID					inStartActionID,
							ActionID					inEndActionID);
private:
};


CREATE_PMINTERFACE(CZPAddLayerToAssignmentDynamicMenu, kZPAddLayerToAssignmentDynamicMenuImpl)

CZPAddLayerToAssignmentDynamicMenu::CZPAddLayerToAssignmentDynamicMenu(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAddLayerToAssignmentDynamicMenu::~CZPAddLayerToAssignmentDynamicMenu()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// RebuildMenu
//----------------------------------------------------------------------------------------
void
CZPAddLayerToAssignmentDynamicMenu::RebuildMenu(
	ActionID					dynamicActionID,
	IPMUnknown*					widget)
{
	do
	{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
		InterfacePtr<IActiveContext> ac(gSession->GetActiveContext(), UseDefaultIID());
		if(!ac)
			break;

		switch (dynamicActionID.Get())
		{
			case kSLAddLayerToAssignmentDynamicActionID:
			{
				InterfacePtr<IDynamicMenu> orgImpl(this , IID_IZPDYNAMICMENUDELEGATE );
				orgImpl->RebuildMenu( dynamicActionID, widget );
				break;
			}
			case kIZPAddLayerToAssignmentActionID:
			{
				InterfacePtr<IDynamicMenu> orgImpl(this , IID_IZPDYNAMICMENUDELEGATE );
				orgImpl->RebuildMenu( kSLAddLayerToAssignmentDynamicActionID, widget );
				this->RebuildShowColumnsMenu( dynamicActionID, widget );
				break;
			}
		}
	}while( false );
}

//----------------------------------------------------------------------------------------
// RebuildShowColumnsMenu
//----------------------------------------------------------------------------------------
void
CZPAddLayerToAssignmentDynamicMenu::RebuildShowColumnsMenu(
	ActionID					dynamicActionID,
	IPMUnknown*					widget)
{
	do
	{
		// Get the menu and action managers
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
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

		InterfacePtr<const IActionComponent> selfActionComponent( this, UseDefaultIID() );
		ASSERT( selfActionComponent );

		if( !widget )	//Coming from somewhere else than from our code.
			break;

		const ActionID startActionID = kSLAddLayerToAssignmentDynamicActionID + 1;
		const ActionID endActionID = kSLAddLayerToAssignmentDynamicEndActionID;

		//Remove all items from the menu manage and action mgr.
		this->ClearExistingItems( actionMgr, menuMgr, "AssignmentPanelPopup:Add &Layer to Assignment", startActionID, endActionID );

		if( actionMgr->IsValidAction( kSLAddLayerToNewAssignmentActionID ) )
			menuMgr->RemoveMenuItem("AssignmentPanelPopup:Add &Layer to Assignment", kSLAddLayerToNewAssignmentActionID);

		menuMgr->AddMenuItem(kSLAddLayerToNewAssignmentActionID,			// Action ID
							 "AssignmentPanelPopup:Add &Layer to Assignment",	// Menu path
							 1,			// Menu position
							 kSDKDefIsNotDynamicMenuFlag);	// Note we say it's not dynamic here.

		menuMgr->RemoveMenuItem("AssignmentPanelPopup:Add &Layer to Assignment:-", kIZPSeparator1ActionID);

		menuMgr->AddMenuItem(kIZPSeparator1ActionID,			// Action ID
			"AssignmentPanelPopup:Add &Layer to Assignment:-",	// Menu path
							 2,			// Menu position
							 kSDKDefIsNotDynamicMenuFlag);	// Note we say it's not dynamic here.

		ActionID currActionID = startActionID;
		for( ; currActionID < endActionID; ++currActionID )
		{
			if( actionMgr->IsValidAction( currActionID ) )
			{
				menuMgr->AddMenuItem(currActionID,			// Action ID
									 "AssignmentPanelPopup:Add &Layer to Assignment",	// Menu path
									 currActionID.Get(),//newMenuPos,			// Menu position
									 kSDKDefIsNotDynamicMenuFlag);	// Note we say it's not dynamic here.
			}
		}
	}while(kFalse);
}

//----------------------------------------------------------------------------------------
// ClearExistingItems
//----------------------------------------------------------------------------------------
void
CZPAddLayerToAssignmentDynamicMenu::ClearExistingItems(
	IActionManager *			inActionMgr,
	IMenuManager *				inMenuMgr,
	const PMString				inTargetMenuPath,
	ActionID					inStartActionID,
	ActionID					inEndActionID)
{
	ActionID currActionID = inStartActionID;
	for( ; currActionID < inEndActionID; ++currActionID )
	{
		if( inActionMgr->IsValidAction( currActionID ) )
		{
			// Remove the entry from the menu manager.  Menu path is same as used in menu def resource
			inMenuMgr->RemoveMenuItem(inTargetMenuPath, currActionID);
			
			// Remove entry from action manager
			//inActionMgr->RemoveAction(currActionID);
		}
	}
}
