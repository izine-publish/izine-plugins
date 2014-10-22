//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Controls/TreeViewHeader/TVHeaderDynamicMenu/CZMTVHeaderDynamicMenu.cpp $
//	$Revision: 2799 $
//	$Date: 2011-04-12 12:20:30 +0200 (Tue, 12 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 12-4-2011
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

//SDK General includes

//OS / ThirdParty includes

#include "IZMediaID.h"

//IZP Interfaces
#include "IZMTVColumnsInfo.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_ActionComponent
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IDynamicMenu>
class CZPTVHeaderDynamicMenu : public inheritClass
{
public:
						CZPTVHeaderDynamicMenu(IPMUnknown * boss);
	virtual				~CZPTVHeaderDynamicMenu();

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


CREATE_PMINTERFACE(CZPTVHeaderDynamicMenu, kZMTVHeaderDynamicMenuImpl)

CZPTVHeaderDynamicMenu::CZPTVHeaderDynamicMenu(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPTVHeaderDynamicMenu::~CZPTVHeaderDynamicMenu()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// RebuildMenu
//----------------------------------------------------------------------------------------
void
CZPTVHeaderDynamicMenu::RebuildMenu(
	ActionID					dynamicActionID,
	IPMUnknown*					widget)
{

	do
	{
		InterfacePtr<IActiveContext> ac(gSession->GetActiveContext(), UseDefaultIID());
		if(!ac)
			break;

		switch (dynamicActionID.Get())
		{
			case kIZMTVHeaderShowColumnsActionID:
				this->RebuildShowColumnsMenu( dynamicActionID, widget );
				break;
		}
	}while( false );
}

//----------------------------------------------------------------------------------------
// RebuildShowColumnsMenu
//----------------------------------------------------------------------------------------
void
CZPTVHeaderDynamicMenu::RebuildShowColumnsMenu(
	ActionID					dynamicActionID,
	IPMUnknown*					widget)
{
	do
	{
		// Get the menu and action managers
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

		InterfacePtr<const IZMTVColumnsInfo> headerColInfo( widget, UseDefaultIID() );
		ASSERT( headerColInfo );
		if( !headerColInfo )
			break;

		const ActionID startActionID = kIZMTVHeaderShowColumnsStartActionID;
		const ActionID endActionID = kIZMTVHeaderShowColumnsEndActionID;

		//Remove all items from the menu manage and action mgr.
		this->ClearExistingItems( actionMgr, menuMgr, kIZMTVHeaderMenuPath, startActionID, endActionID );

		//Add menu items and actionIDs for the column info.
		int colCount = headerColInfo->GetColumnCount();

		for( int i = 0; i < colCount ; ++i )
		{
			const ZMTVColumnInfo & currColInfo = headerColInfo->GetNthColumnInfo( i );
			ASSERT( currColInfo.mColID >= 0 );
			if( currColInfo.mCanHide )
			{	//Add menu item for this column
				// Compose menu entry string. Note that since it is dynamic, it is made non-translatable.
				ActionID newActionID(startActionID + currColInfo.mColID);
				ASSERT( newActionID >= startActionID && newActionID <= endActionID );
				// Calculate new menu position
				PMReal newMenuPos(300 + i);
				
				PMString dispName = currColInfo.mContextMenuDispName;
				if( currColInfo.mContextMenuDispName.IsEmpty() )
					dispName = currColInfo.mColDispName;
				// Add to menu and action managers
				actionMgr->AddAction( selfActionComponent,		// ptr to IActionComponent to field menu hit
									newActionID,				// Action ID
									dispName,					// Sub-menu string
									kMiscellaneousArea,			// Action area
									kNormalAction,				// Action type
									kCustomEnabling,			// Enabling type
									kInvalidInterfaceID,		// Selection IID
									kFalse						// User editability
									);
				
				menuMgr->AddMenuItem(newActionID,			// Action ID
									 kIZMTVHeaderMenuPath,	// Menu path
									 newMenuPos,			// Menu position
									 kSDKDefIsNotDynamicMenuFlag);	// Note we say it's not dynamic here.
				IZPLog_Str_( thisFileLA, enLT_Details, "Added menu item for columnID %d", currColInfo.mColID );
			}
		}
	}while(kFalse);
}

//----------------------------------------------------------------------------------------
// ClearExistingItems
//----------------------------------------------------------------------------------------
void
CZPTVHeaderDynamicMenu::ClearExistingItems(
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
			inActionMgr->RemoveAction(currActionID);
		}
	}
}
