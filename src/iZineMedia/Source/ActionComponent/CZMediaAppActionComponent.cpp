//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/ActionComponent/CZMediaAppActionComponent.cpp $
//	$Revision: 2745 $
//	$Date: 2011-04-08 09:05:28 +0200 (Fri, 08 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 12-11-2010
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IActionStateList.h"
#include "IActiveContext.h"

//SDK General includes
#include "CActionComponent.h"
#include "CAlert.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "IZMediaID.h"

//IZP Interfaces
#include "IZMUIDialogFacade.h"

//IZP General includes
//#include "IZPLog.h"

#pragma mark -

#define LogFunctionEnterExit	//IZPLog_func( enLMA_ActionComponent )

/** Implements IActionComponent; performs the actions that are executed when the plug-in's
	menu items are selected.

	@ingroup iZineExport
*/
#define inheritClass CActionComponent
class CZMAppActionComponent : public inheritClass
{
public:
						CZMAppActionComponent(IPMUnknown* boss);
						~CZMAppActionComponent();

	void				DoAction(
							IActiveContext*				ac,
							ActionID					actionID,
							GSysPoint					mousePoint,
							IPMUnknown*					widget);
	void				UpdateActionStates(
							IActiveContext*				ac,
							IActionStateList *			listToUpdate,
							GSysPoint					mousePoint,
							IPMUnknown*					widget);
private:
	void DoAbout();
	
private:

private:
};

CREATE_PMINTERFACE(CZMAppActionComponent, kZMActionComponentImpl)

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZMAppActionComponent::CZMAppActionComponent(IPMUnknown* boss)
: inheritClass(boss)
{
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZMAppActionComponent::~CZMAppActionComponent()
{

}

//----------------------------------------------------------------------------------------
// UpdateActionStates
//----------------------------------------------------------------------------------------
void
CZMAppActionComponent::UpdateActionStates(
	IActiveContext*				ac,
	IActionStateList *			listToUpdate,
	GSysPoint					mousePoint,
	IPMUnknown*					widget)
{
	LogFunctionEnterExit;
	
	do {
		for(int i=0; i < listToUpdate->Length(); i++) {
			ActionID actionID = listToUpdate->GetNthAction(i);
			int16 state = listToUpdate->GetNthActionState(i);

			switch (actionID.Get())
			{
				default:
					break;
			}
			listToUpdate->SetNthActionState(i, state);
		}
	} while (kFalse);
}

//----------------------------------------------------------------------------------------
// DoAction
//----------------------------------------------------------------------------------------
void
CZMAppActionComponent::DoAction(
	IActiveContext*				ac,
	ActionID					actionID,
	GSysPoint					mousePoint,
	IPMUnknown*					widget)
{
	LogFunctionEnterExit;
	
	switch (actionID.Get())
	{
		case kIZMediaAboutActionID:
		{
			this->DoAbout();
			break;
		}
		case kIZMPreferencesPanelActionID:
			Utils<IZMUIDialogFacade>()->ShowPrefs();
			break;
		default:
			break;
	}
}

//----------------------------------------------------------------------------------------
// DoAbout
//----------------------------------------------------------------------------------------
void
CZMAppActionComponent::DoAbout()
{
	LogFunctionEnterExit;
	
	CAlert::ModalAlert
	(
		kIZMediaAboutBoxStringKey,		// Alert string
		kOKString, 						// OK button
		kNullString, 					// No second button
		kNullString, 					// No third button
		1,								// Set OK button to default
		CAlert::eInformationIcon		// Information icon.
	);
}

