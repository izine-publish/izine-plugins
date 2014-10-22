//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/TreeViewHeader/TVHeaderDynamicMenu/CZPTVHeaderActionComponent.cpp $
//	$Revision: 2336 $
//	$Date: 2010-10-18 12:58:13 +0200 (Mon, 18 Oct 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 18-10-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IActionStateList.h"

//SDK General includes
#include "CActionComponent.h"

//OS / ThirdParty includes

#include "IZPUIID.h"

//IZP Interfaces
#include "IZPTVHeaderController.h"
#include "IZPTVColumnsInfo.h"
#include "IZPTVColumnsPref.h"

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


CREATE_PMINTERFACE(CZPTVHeaderActionComponent, kZPTVHeaderActionComponentImpl)

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
		InterfacePtr<const IZPTVColumnsInfo> headerColInfo( widget, UseDefaultIID() );
		ASSERT( headerColInfo );
		if( !headerColInfo )
			break;

		InterfacePtr<const IZPTVColumnsPref> headerColPrefs( headerColInfo, UseDefaultIID() );
		ASSERT( headerColPrefs );
		if( !headerColPrefs )
			break;

		int actionIDListLen = listToUpdate->Length();
		for(int i=0; i < actionIDListLen; ++i)
		{
			ActionID actionID = listToUpdate->GetNthAction(i);
			int16 state = listToUpdate->GetNthActionState(i);

			if( actionID >= kIZPTVHeaderShowColumnsStartActionID && actionID <= kIZPTVHeaderShowColumnsEndActionID )
			{
				int colID = actionID - kIZPTVHeaderShowColumnsStartActionID;
				//const ZPTVColumnInfo & currActionColInfo = headerColInfo->GetColumnInfoForColID( colID );
				state = kEnabledAction;

				//Check if this column is currently visible.
				const ZPTVColumnPref & currActionColPref = headerColPrefs->GetColumnPrefByColID( colID );
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
	if( actionID >= kIZPTVHeaderShowColumnsStartActionID && actionID <= kIZPTVHeaderShowColumnsEndActionID )
	{
		InterfacePtr<IZPTVHeaderController> headerController( widget, UseDefaultIID() );
		ASSERT( headerController );
		int colID = actionID - kIZPTVHeaderShowColumnsStartActionID;
		if( headerController )
			headerController->ToggleColumnVisibility( colID );
	}
}
