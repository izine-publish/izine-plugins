//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/TreeViewHeader/TVHeaderDynamicMenu/CZPTVHeaderDynamicMenu.cpp $
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

//OS / ThirdParty includes

#include "IZPUIID.h"

//IZP Interfaces
#include "IZPTVColumnsInfo.h"

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


CREATE_PMINTERFACE(CZPTVHeaderDynamicMenu, kZPTVHeaderDynamicMenuImpl)

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
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	do
	{
		InterfacePtr<IActiveContext> ac(gSession->GetActiveContext(), UseDefaultIID());
		if(!ac)
			break;

		switch (dynamicActionID.Get())
		{
			case kIZPTVHeaderShowColumnsActionID:
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
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
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

		InterfacePtr<const IZPTVColumnsInfo> headerColInfo( widget, UseDefaultIID() );
		ASSERT( headerColInfo );
		if( !headerColInfo )
			break;

		const ActionID startActionID = kIZPTVHeaderShowColumnsStartActionID;
		const ActionID endActionID = kIZPTVHeaderShowColumnsEndActionID;

		//Remove all items from the menu manage and action mgr.
		this->ClearExistingItems( actionMgr, menuMgr, kIZPTVHeaderMenuPath, startActionID, endActionID );

		//Add menu items and actionIDs for the column info.
		int colCount = headerColInfo->GetColumnCount();

		for( int i = 0; i < colCount ; ++i )
		{
			const ZPTVColumnInfo & currColInfo = headerColInfo->GetNthColumnInfo( i );
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
									 kIZPTVHeaderMenuPath,	// Menu path
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
