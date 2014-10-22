//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/ActionComponent/CZPTasksActionComponent.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 25-10-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IActionStateList.h"
#include "IApplication.h"
#include "IControlView.h"
#include "IPanelMgr.h"
#include "IStringData.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes
#include "CActionComponent.h"
#include "CAlert.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPID.h"
#include "IZPUIID.h"

//IZP Interfaces
#include "IZPAMHelper.h"
#include "IZPApp.h"
#include "IZPAppController.h"
#include "IZPAssetUtils.h"
#include "IZPDefaultSession.h"
#include "IZPEditionMgr.h"
#include "IZPLoginController.h"
#include "IZPTaskInfo.h"
#include "IZPTasksPanelHelper.h"
#include "IZPUIDialogFacade.h"
#include "IZPWSUtils.h"

//IZP General includes
#include "IZPLog.h"
#include "ZPDataHelper.h"

#pragma mark -

#define thisFileLA	enLMA_ActionComponent
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

/** Implements IActionComponent; performs the actions that are executed when the plug-in's
	menu items are selected.

	@ingroup izinepublish

*/
class CZPTasksActionComponent : public CActionComponent
{
public:
						CZPTasksActionComponent(IPMUnknown* boss);
						~CZPTasksActionComponent();

		void			DoAction(IActiveContext* ac, ActionID actionID, GSysPoint mousePoint, IPMUnknown* widget);
		void			UpdateActionStates(IActiveContext* ac, IActionStateList *listToUpdate, GSysPoint mousePoint, IPMUnknown* widget);
private:
	void				DoShowCreateTaskDialog();

	void				DoDeleteTask();
	void				DoDuplicateTask();
	void				DoOpenTaskDocument();

	bool				CanOpenTaskDocument();

	const IStringData *	GetSelectedTask() const;

	const PMString  	GetSelectedEdition() const;

	const IZPTasksPanelHelper *	GetTaskPanelHelper() const;

private:

private:
};

CREATE_PMINTERFACE(CZPTasksActionComponent, kZPTasksActionComponentImpl)

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPTasksActionComponent::CZPTasksActionComponent(IPMUnknown* boss)
: CActionComponent(boss)
{
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPTasksActionComponent::~CZPTasksActionComponent()
{

}


//----------------------------------------------------------------------------------------
// UpdateActionStates
//----------------------------------------------------------------------------------------
void
CZPTasksActionComponent::UpdateActionStates(
	IActiveContext* ac, IActionStateList *listToUpdate, GSysPoint mousePoint, IPMUnknown* widget)
{
	LogFunctionEnterExit;
	
	do {
		for(int i=0; i < listToUpdate->Length(); i++) {
			ActionID actionID = listToUpdate->GetNthAction(i);
			int16 state = listToUpdate->GetNthActionState(i);

			bool isLoggedIn = (gZPApp->GetAppState() == eAppState_LoggedIn);
			switch (actionID.Get())
			{
				case kIZPCreateTaskActionID:
				{
					if( isLoggedIn )
						state = kEnabledAction;
					else
						state = kDisabled_Unselected;
					break;
				}
				case kIZPDuplicateTaskActionID:
				case kIZPDeleteTaskActionID:
				{
					if( isLoggedIn && this->GetSelectedTask() )
						state = kEnabledAction;
					else
						state = kDisabled_Unselected;
					break;
				}
				case kIZPOpenTaskDocumentActionID:
				{
					if( isLoggedIn && this->CanOpenTaskDocument() )
						state = kEnabledAction;
					else
						state = kDisabled_Unselected;
					break;
				}
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
CZPTasksActionComponent::DoAction(
	IActiveContext* ac, ActionID actionID, GSysPoint mousePoint, IPMUnknown* widget)
{
	LogFunctionEnterExit;
	
	switch (actionID.Get())
	{
		case kIZPCreateTaskActionID:
			this->DoShowCreateTaskDialog();
			break;
		case kIZPDuplicateTaskActionID:
			this->DoDuplicateTask();
			break;
		case kIZPDeleteTaskActionID:
			this->DoDeleteTask();
			break;
		case kIZPOpenTaskDocumentActionID:
			this->DoOpenTaskDocument();
			break;
		default:
			break;
	}
}

//----------------------------------------------------------------------------------------
// DoShowCreateTaskDialog
//----------------------------------------------------------------------------------------
void
CZPTasksActionComponent::DoShowCreateTaskDialog()
{
	LogFunctionEnterExit;
	
	do
	{
		const PMString edition = this->GetSelectedEdition();
		Utils<IZPUIDialogFacade>()->ShowCreateTaskDialog(nil, edition);
	} while (kFalse);
}

//----------------------------------------------------------------------------------------
// DoDeleteTask
//----------------------------------------------------------------------------------------
void
CZPTasksActionComponent::DoDeleteTask()
{
	LogFunctionEnterExit;
	
	const IStringData * taskToDelete = this->GetSelectedTask();
	if( !taskToDelete )
		return;

	Utils<IZPWSUtils>()->DeleteTask( taskToDelete->Get() );
}

//----------------------------------------------------------------------------------------
// DoDuplicateTask
//----------------------------------------------------------------------------------------
void
CZPTasksActionComponent::DoDuplicateTask()
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif	
	const IStringData * dbTaskToDuplicate = this->GetSelectedTask();
	if( !dbTaskToDuplicate )
		return;

	InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<IZPEditionMgr> editionMgr( zpApp->GetEditionMgr(), UseDefaultIID() );
	ASSERT(editionMgr);

	const IStringData * dbEditionID = editionMgr->GetEditionForTaskID( dbTaskToDuplicate->Get() );
	ASSERT( dbEditionID );

	Utils<IZPUIDialogFacade>()->ShowCreateTaskDialog( dbTaskToDuplicate, dbEditionID->Get(), true );
}

//----------------------------------------------------------------------------------------
// DoOpenTaskDocument
//----------------------------------------------------------------------------------------
void
CZPTasksActionComponent::DoOpenTaskDocument()
{
	LogFunctionEnterExit;

	do
	{
		const IStringData * dbTaskIDToOpen = this->GetSelectedTask();
		if( !dbTaskIDToOpen )
			break;

		InterfacePtr<const IZPTaskInfo> dbTaskInfo( dbTaskIDToOpen, UseDefaultIID() );
		ASSERT( dbTaskInfo );
		
		const PMString & theAssetIDStr = dbTaskInfo->GetDocumentID();
		
		if( theAssetIDStr.IsEmpty() || false == Utils<IZPAssetUtils>()->CanOpenAsset( theAssetIDStr ) )
			break;

		const IStringData * dbAssetID = ZPDataHelper::GetAssetByID( theAssetIDStr );
		if( dbAssetID )
			Utils<IZPAMHelper>()->ProcessAssetForOpening( dbAssetID , false);
	}while(kFalse);
}

//----------------------------------------------------------------------------------------
// CanOpenTaskDocument
//----------------------------------------------------------------------------------------
bool
CZPTasksActionComponent::CanOpenTaskDocument()
{
	bool toReturn = false;
	do
	{
		const IStringData * dbTaskIDToOpen = this->GetSelectedTask();
		if( !dbTaskIDToOpen )
			break;

		InterfacePtr<const IZPTaskInfo> dbTaskInfo( dbTaskIDToOpen, UseDefaultIID() );
		ASSERT( dbTaskInfo );
		
		const PMString & theAssetIDStr = dbTaskInfo->GetDocumentID();
		
		if( theAssetIDStr.IsEmpty() || false == Utils<IZPAssetUtils>()->CanOpenAsset( theAssetIDStr ) )
			break;

		toReturn = true;

	}while(kFalse);

	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetSelectedTask
//----------------------------------------------------------------------------------------
const IStringData *
CZPTasksActionComponent::GetSelectedTask()const
{
	const IStringData * toReturn = nil;
	do
	{
		const IZPTasksPanelHelper * taskPanelHelper = this->GetTaskPanelHelper();

		if (taskPanelHelper)
			toReturn = taskPanelHelper->GetSelectedTask();

	}while(kFalse);
	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetSelectedEdition
//----------------------------------------------------------------------------------------
const PMString
CZPTasksActionComponent::GetSelectedEdition()const
{
	PMString toReturn;
	do
	{
		const IZPTasksPanelHelper * taskPanelHelper = this->GetTaskPanelHelper();

		if (taskPanelHelper)
			toReturn = taskPanelHelper->GetSelectedEditionID();

	}while(kFalse);
	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetTaskPanelHelper
//----------------------------------------------------------------------------------------
const IZPTasksPanelHelper *
CZPTasksActionComponent::GetTaskPanelHelper()const
{
	const IZPTasksPanelHelper *toReturn = nil;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	do
	{
		InterfacePtr<IApplication> application(gSession->QueryApplication());
		ASSERT(application);
		
		InterfacePtr<IPanelMgr> panelMgr(application->QueryPanelManager());
		if( !panelMgr )
			break;
		
		if( !panelMgr->IsPanelWithWidgetIDShown( kZPUITasksPanelWidgetID ) )
			break;

		IControlView* taskPanelView = panelMgr->GetPanelFromWidgetID( kZPUITasksPanelWidgetID );
		if( !taskPanelView )
			break;

		InterfacePtr<const IZPTasksPanelHelper> taskPanelHelper( taskPanelView, UseDefaultIID() );
		ASSERT( taskPanelHelper );

		toReturn = taskPanelHelper;

	}while(kFalse);
	return toReturn;
}
