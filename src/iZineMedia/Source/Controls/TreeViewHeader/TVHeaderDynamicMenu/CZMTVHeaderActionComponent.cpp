//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Controls/TreeViewHeader/TVHeaderDynamicMenu/CZMTVHeaderActionComponent.cpp $
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
#include "IActionStateList.h"

//SDK General includes
#include "CActionComponent.h"

//OS / ThirdParty includes

#include "IZMediaID.h"

//IZP Interfaces
#include "IZMTVHeaderController.h"
#include "IZMTVColumnsInfo.h"
#include "IZMTVColumnsPref.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_ActionComponent
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CActionComponent
class CZPTVHeaderActionComponent : public inheritClass
{
public:
						CZPTVHeaderActionComponent(IPMUnknown * boss);
	virtual				~CZPTVHeaderActionComponent();

	void				DoAction(
							IActiveContext* ac, ActionID actionID, GSysPoint mousePoint, IPMUnknown* widget);
	void				UpdateActionStates(
							IActiveContext* ac, IActionStateList *listToUpdate, GSysPoint mousePoint, IPMUnknown* widget);
protected:
private:
};


CREATE_PMINTERFACE(CZPTVHeaderActionComponent, kZMTVHeaderActionComponentImpl)

CZPTVHeaderActionComponent::CZPTVHeaderActionComponent(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPTVHeaderActionComponent::~CZPTVHeaderActionComponent()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// UpdateActionStates
//----------------------------------------------------------------------------------------
void
CZPTVHeaderActionComponent::UpdateActionStates(
	IActiveContext* ac, IActionStateList *listToUpdate, GSysPoint mousePoint, IPMUnknown* widget)
{
	do
	{
		InterfacePtr<const IZMTVColumnsInfo> headerColInfo( widget, UseDefaultIID() );
		ASSERT( headerColInfo );
		if( !headerColInfo )
			break;

		InterfacePtr<const IZMTVColumnsPref> headerColPrefs( headerColInfo, UseDefaultIID() );
		ASSERT( headerColPrefs );
		if( !headerColPrefs )
			break;

		int actionIDListLen = listToUpdate->Length();
		for(int i=0; i < actionIDListLen; ++i)
		{
			ActionID actionID = listToUpdate->GetNthAction(i);
			int16 state = listToUpdate->GetNthActionState(i);

			if( actionID >= kIZMTVHeaderShowColumnsStartActionID && actionID <= kIZMTVHeaderShowColumnsEndActionID )
			{
				int colID = actionID - kIZMTVHeaderShowColumnsStartActionID;
				//const ZPTVColumnInfo & currActionColInfo = headerColInfo->GetColumnInfoForColID( colID );
				state = kEnabledAction;

				//Check if this column is currently visible.
				const ZMTVColumnPref & currActionColPref = headerColPrefs->GetColumnPrefByColID( colID );
				if( currActionColPref.mIsVisible )
					state = state | kSelectedAction;
			}
			listToUpdate->SetNthActionState(i, state);
		}
	}while(kFalse);
}

//----------------------------------------------------------------------------------------
// DoAction
//----------------------------------------------------------------------------------------
void
CZPTVHeaderActionComponent::DoAction(
	IActiveContext* ac, ActionID actionID, GSysPoint mousePoint, IPMUnknown* widget)
{
	if( actionID >= kIZMTVHeaderShowColumnsStartActionID && actionID <= kIZMTVHeaderShowColumnsEndActionID )
	{
		InterfacePtr<IZMTVHeaderController> headerController( widget, UseDefaultIID() );
		ASSERT( headerController );
		int colID = actionID - kIZMTVHeaderShowColumnsStartActionID;
		if( headerController )
			headerController->ToggleColumnVisibility( colID );
	}
}
