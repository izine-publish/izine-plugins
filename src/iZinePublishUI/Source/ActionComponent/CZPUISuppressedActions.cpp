//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/ActionComponent/CZPUISuppressedActions.cpp $
//	$Revision: 3650 $
//	$Date: 2011-11-15 06:52:23 +0100 (Tue, 15 Nov 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 24-6-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "ISuppressedUI.h"

//SDK General includes
#include "AssignmentUIID.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPUIID.h"
#include "IZPID.h"

//IZP Interfaces
#include "IZPInDnUtils.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_ActionComponent
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<ISuppressedUI>
class CZPUISuppressedActions : public inheritClass
{
public:
						CZPUISuppressedActions(IPMUnknown * boss);
	virtual				~CZPUISuppressedActions();

	// -----------------------------------------------------------
	/**@name Manage workspaces in standard locations */
	//@{

	/** 
		Called by UI code to determine if the specified widget should be disabled
		@param widget the widget in question
		@return kTrue to disable the widget, kFalse if you do not wish to disable the widget
	*/
	bool16			IsWidgetDisabled( const IControlView* widget ) const;

	/** 
		Called by UI code to determine if the specified widget should be hidden
		@param widget the widget in question
		@return kTrue to hide the widget, kFalse if you do not wish to hide the widget
	*/
	bool16			IsWidgetHidden( const IControlView* widget ) const;

	/** 
		Called by UI code to determine if the specified widget should accept a drop.  data and source contain more information about this particular drop.
		You can choose to accept some drops and disable others
		@param target the widget in question
		@param data contains information about the dragdrop action in question
		@param source contains the source of the dragdrop operation
		@return kTrue to disable the drop to this widget, kFalse if you do not wish to disable the drop to this widget
	*/
	bool16			IsDragDropDisabled( const IDragDropTarget* target, DataObjectIterator* data, const IDragDropSource* source ) const;

	/** 
		Called by ActionManager to determine if the specified action should be disabled.  Disabled actions will show up in menus, but will be disabled.
		@param action the action
		@return kTrue to disable the action, kFalse if you do not wish to disable the action
	*/
	bool16			IsActionDisabled( ActionID action ) const;

	/** 
		Called by ActionManager to determine if the specified action should be hidden.  Hidden actions will NOT show up in menus.
		@param action the action
		@return kTrue to hide the action, kFalse if you do not wish to hide the action
	*/
	bool16			IsActionHidden( ActionID action ) const;

	/** 
		Called by MenuManager to determine if the specified menu should be disabled.  This refers to submenus which are menu items that contain a menu
		beneath them.  They have no actionID and are specified by Menu path.  e.g. File > New is a submenu.
		@param untranslatedSubMenuName the This should contain the full menu path in it's untranslated form.  This is the same format used to add submenus
		in MenuDef resource.  e.g. "Main:&Window:PW_Workspace:" is a submenu path for Palette Workspaces
		@return kTrue to disable the submenu, kFalse if you do not wish to disable the submenu
	*/
	bool16			IsSubMenuDisabled( const PMString& untranslatedSubMenuName ) const;

	/** 
		Called by MenuManager to determine if the specified menu should be hidden.  This refers to submenus which are menu items that contain a menu
		beneath them.  They have no actionID and are specified by Menu path.  e.g. File > New is a submenu.
		@param untranslatedSubMenuName the This should contain the full menu path in it's untranslated form.  This is the same format used to add submenus
		in MenuDef resource.  e.g. "Main:&Window:PW_Workspace:" is a submenu path for Palette Workspaces
		@return kTrue to hide the submenu, kFalse if you do not wish to hide the submenu
	*/
	bool16			IsSubMenuHidden( const PMString& untranslatedSubMenuName ) const;

	/** 
		The different variations of Open and Save dialogs in InDesign do not contain InDesign widgets and therefore can't be handled in the same way.
		These are handled specifically by an identifier found in SuppressedUIXMLDefs.h.  You can either Hide all the custom controls or disable individual 
		controls.  Suppressing all controls will not cause the dialog to not show the custom control area.  Suppressing only individual controls will disable them.
		At this point, you cannot hide individual controls.  This method is called by the dialog to determine if it should hide or disable it's custom controls
		@param platformDialogIdentifier an identifier which identifies the dialog asking whether it's custom controls should be hidden
		@return kTrue to hide the custom controls, kFalse if you do not wish to hide the custom controls
	*/
	bool16			IsPlatformDialogControlSuppressed( const PMString& platformDialogIdentifier ) const;

	/** 
		Call Reset() to force the implementations of this interface to recreate a list of items that it would like to suppress.  Since the calls above occur very frequently,
		the implementation needs to respond very quickly and should keep a cache of suppressed items.  When Reset() is called, it should recreate it's
		cache.  For instance, the implementation that reads suppressed items from an XML file will reread the xml file when Reset() is called.
	*/
	void			Reset( ); // or Initialize()
protected:
private:
};


CREATE_PMINTERFACE(CZPUISuppressedActions, kZPUISuppressedActionsImpl)

CZPUISuppressedActions::CZPUISuppressedActions(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPUISuppressedActions::~CZPUISuppressedActions()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// IsWidgetDisabled
//----------------------------------------------------------------------------------------
bool16
CZPUISuppressedActions::IsWidgetDisabled(
	const IControlView* widget) const
{
	return kFalse;
}


//----------------------------------------------------------------------------------------
// IsWidgetHidden
//----------------------------------------------------------------------------------------
bool16
CZPUISuppressedActions::IsWidgetHidden(
	const IControlView* widget) const
{
	return kFalse;
}

//----------------------------------------------------------------------------------------
// IsDragDropDisabled
//----------------------------------------------------------------------------------------
bool16
CZPUISuppressedActions::IsDragDropDisabled(
	const IDragDropTarget* target, DataObjectIterator* data, const IDragDropSource* source) const
{
	return kFalse;
}

//----------------------------------------------------------------------------------------
// IsActionDisabled
//----------------------------------------------------------------------------------------
bool16
CZPUISuppressedActions::IsActionDisabled(
	ActionID action) const
{
	return kFalse;
}

//----------------------------------------------------------------------------------------
// IsActionHidden
//----------------------------------------------------------------------------------------
bool16
CZPUISuppressedActions::IsActionHidden(
	ActionID action) const
{
	switch( action.Get() )
	{
		case kSLRelinkAssignmentActionID:					//Change assignment location.
		case kSLAssignmentOptionsActionID:					//Hide assignment options
		case kPackageAssignmentActionID:					//All package relation actions
		case kPackageAndEmailAssignmentActionID:
		case kForwardPackagedAssignmentActionID:
		case kForwardAndEmailPackagedAssignmentActionID:
		case kReturnPackagedAssignmentActionID:
		case kReturnAndEmailPackagedAssignmentActionID:
		case kOpenPackagedAssignmentActionID:
		case kCancelPackagedAssignmentActionID:
			return kTrue;
	}
	return kFalse;
}

//----------------------------------------------------------------------------------------
// IsSubMenuDisabled
//----------------------------------------------------------------------------------------
bool16
CZPUISuppressedActions::IsSubMenuDisabled(
	const PMString& untranslatedSubMenuName) const
{
	return kFalse;
}

//----------------------------------------------------------------------------------------
// IsSubMenuHidden
//----------------------------------------------------------------------------------------
bool16
CZPUISuppressedActions::IsSubMenuHidden(
	const PMString& untranslatedSubMenuName) const
{
	//IZPLog_Str_( thisFileLA, enLT_Details, "%s", untranslatedSubMenuName.GrabCString() );
	bool toReturn = kFalse;
	do
	{
		//Copied from old plugin's file IZPSuppressedUI.cpp
		if( untranslatedSubMenuName.IsEqual("Main:&Edit:kInCopyMenu:Add All &Graphics to Assignment") )
			toReturn = kTrue;
		else if( untranslatedSubMenuName.IsEqual("Main:&Edit:kInCopyMenu:Add All Sto&ries to Assignment") )
			toReturn = kTrue;
		else if( untranslatedSubMenuName.IsEqual("Main:&File:ICUser_MenuName") )
			toReturn = kTrue;
		else if( untranslatedSubMenuName.IsEqual("Main:&File:Packa&ge...") )
			toReturn = kTrue;//Utils<IZPInDnUtils>()->IsHostAppInCopy();
	}while( false );

	return toReturn;
}

//----------------------------------------------------------------------------------------
// IsPlatformDialogControlSuppressed
//----------------------------------------------------------------------------------------
bool16
CZPUISuppressedActions::IsPlatformDialogControlSuppressed(
	const PMString& platformDialogIdentifier) const
{
	return kFalse;
}

//----------------------------------------------------------------------------------------
// Reset
//----------------------------------------------------------------------------------------
void
CZPUISuppressedActions::Reset()
{

}
