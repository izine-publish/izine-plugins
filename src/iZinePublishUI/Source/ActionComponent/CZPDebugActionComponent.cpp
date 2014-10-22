//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/ActionComponent/CZPDebugActionComponent.cpp $
//	$Revision: 3464 $
//	$Date: 2011-07-07 11:22:50 +0200 (Thu, 07 Jul 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 7-7-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IActionStateList.h"

//SDK General includes
#include "CActionComponent.h"
#include "CAlert.h"
#include "KeyValuePair.h"
#include "K2Vector.tpp"
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPID.h"
#include "IZPUIID.h"

//IZP Interfaces
#include "IZPApp.h"
#include "IZPAppController.h"
#include "IZPDefaultSession.h"
#include "IZPUIDialogFacade.h"
#include "IZPLoginController.h"

//IZP General includes
#include "../../iZinePublish/Source/WS/CZPTestWSCalling.h"
#include "IZPLog.h"

#pragma mark -

#define thisFileLA	enLMA_ActionComponent
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

/** Implements IActionComponent; performs the actions that are executed only for debug build.
*/
class CZPDebugActionComponent : public CActionComponent
{
public:
						CZPDebugActionComponent(IPMUnknown* boss);
						~CZPDebugActionComponent();

	void				DoAction(IActiveContext* ac, ActionID actionID, GSysPoint mousePoint, IPMUnknown* widget);
	void				UpdateActionStates(IActiveContext* ac, IActionStateList *listToUpdate, GSysPoint mousePoint, IPMUnknown* widget);
private:
	typedef KeyValuePair<ActionID, eIZPMajorLogArea >	LogActionLogAreaPair;
	typedef K2Vector<LogActionLogAreaPair>	LogActionList;
	void				GetLogActionList(
							LogActionList &				oList );
	eIZPMajorLogArea	GetLMAOfLogAction(
							const ActionID &			inActionID,
							const LogActionList &		inList );
};

CREATE_PMINTERFACE(CZPDebugActionComponent, kZPDebugActionComponentImpl)

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPDebugActionComponent::CZPDebugActionComponent(IPMUnknown* boss)
: CActionComponent(boss)
{
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPDebugActionComponent::~CZPDebugActionComponent()
{

}

//----------------------------------------------------------------------------------------
// UpdateActionStates
//----------------------------------------------------------------------------------------
void
CZPDebugActionComponent::UpdateActionStates(
	IActiveContext* ac, IActionStateList *listToUpdate, GSysPoint mousePoint, IPMUnknown* widget)
{
	LogFunctionEnterExit;
	
	do
	{
		int listLen = listToUpdate->Length();

		LogActionList logActionList;
		this->GetLogActionList( logActionList );

		for(int i=0; i < listLen; ++i)
		{
			ActionID actionID = listToUpdate->GetNthAction(i);
			int16 state = listToUpdate->GetNthActionState(i);

			switch (actionID.Get())
			{
				case kIZPDbgMenuWSUploadVersionActionID:
				case kIZPDbgMenuWSDownloadVersionActionID:
				case kIZPDbgMenuWSUpdateStatusActionID:
				{
					if (gZPApp->GetAppState() == eAppState_LoggedIn)
						state = kEnabledAction;
					else
						state = kDisabled_Unselected;
					break;
				}
				case kIZPDbgMenuLogsDisableAllActionID:
				case kIZPDbgMenuLogsEnableAllActionID:
					state = kEnabledAction;
					break;
				default:
					if( actionID >= kIZPLogsFirstActionID && actionID <= kIZPLogsLastActionID )
					{	//It is logs menu action
						eIZPMajorLogArea actionLogArea = this->GetLMAOfLogAction( actionID, logActionList );
						if( actionLogArea != enLMA_None )
						{
							if( IZPLog::IsEnabledLogArea( actionLogArea ) )
								state = kEnabledAction | kSelectedAction;
							else
								state = kEnabledAction;
						}
						else
							state = kDisabled_Unselected;
					}
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
CZPDebugActionComponent::DoAction(
	IActiveContext* ac, ActionID actionID, GSysPoint mousePoint, IPMUnknown* widget)
{

	LogFunctionEnterExit;
	
	switch (actionID.Get())
	{
		case kIZPDbgMenuWSUploadVersionActionID:
			CZPTestWSCalling::TestUploadVersion();
			break;
		case kIZPDbgMenuWSDownloadVersionActionID:
			CZPTestWSCalling::TestDownloadVersion();
			break;
		case kIZPDbgMenuWSUpdateStatusActionID:
			CZPTestWSCalling::TestUpdateStatus();
			break;
		case kIZPDbgMenuLogsDisableAllActionID:
			IZPLog::DisableLogArea( enLMA_ALL );
			break;
		case kIZPDbgMenuLogsEnableAllActionID:
			IZPLog::EnableLogArea( enLMA_ALL );
			break;
		default:
		{
			LogActionList logActionList;
			this->GetLogActionList( logActionList );
			if( actionID >= kIZPLogsFirstActionID && actionID <= kIZPLogsLastActionID )
			{	//It is logs menu action
				eIZPMajorLogArea actionLogArea = this->GetLMAOfLogAction( actionID, logActionList );
				if( actionLogArea != enLMA_None )
				{
					if( IZPLog::IsEnabledLogArea( actionLogArea ) )
						IZPLog::DisableLogArea( actionLogArea );
					else
						IZPLog::EnableLogArea( actionLogArea );
				}
			}
		}
			break;
	}
}

//----------------------------------------------------------------------------------------
// GetLogActionList
//----------------------------------------------------------------------------------------
void
CZPDebugActionComponent::GetLogActionList(
	LogActionList &				oList)
{
	if( oList.size() == 0 )
	{
		oList.push_back( LogActionLogAreaPair( kIZPDbgMenuLogsEnableLMAAppActionID, enLMA_App ) );
		oList.push_back( LogActionLogAreaPair( kIZPDbgMenuLogsEnableLMAIdleActionID, enLMA_Idle ) );
		oList.push_back( LogActionLogAreaPair( kIZPDbgMenuLogsEnableLMAScriptingActionID, enLMA_Scripting ) );
		oList.push_back( LogActionLogAreaPair( kIZPDbgMenuLogsEnableLMACommandsActionID, enLMA_Commands ) );
		oList.push_back( LogActionLogAreaPair( kIZPDbgMenuLogsEnableLMAResponderActionID, enLMA_Responder ) );
		oList.push_back( LogActionLogAreaPair( kIZPDbgMenuLogsEnableLMAWSActionID, enLMA_WS ) );
		oList.push_back( LogActionLogAreaPair( kIZPDbgMenuLogsEnableLMADBObjectsActionID, enLMA_DBObjects ) );
		//Logs Asset Management
		oList.push_back( LogActionLogAreaPair( kIZPDbgMenuLogsEnableLMAAMSysPActionID, enLMA_AMSysP ) );
		oList.push_back( LogActionLogAreaPair( kIZPDbgMenuLogsEnableLMAAMSrvPActionID, enLMA_AMSrvP ) );
		oList.push_back( LogActionLogAreaPair( kIZPDbgMenuLogsEnableLMAAMUISrvPActionID, enLMA_AMUISrvP ) );
		oList.push_back( LogActionLogAreaPair( kIZPDbgMenuLogsEnableLMAAMServiceActionID, enLMA_AMService ) );
		oList.push_back( LogActionLogAreaPair( kIZPDbgMenuLogsEnableLMAAMUIServiceActionID, enLMA_AMUIService ) );
		oList.push_back( LogActionLogAreaPair( kIZPDbgMenuLogsEnableLMAAssetMonitorActionID, enLMA_AssetMonitor ) );
		oList.push_back( LogActionLogAreaPair( kIZPDbgMenuLogsEnableLMAAMActionID, enLMA_AM ) );
		//Logs Utils
		oList.push_back( LogActionLogAreaPair( kIZPDbgMenuLogsEnableLMAICExportActionID, enLMA_ICExport ) );
		oList.push_back( LogActionLogAreaPair( kIZPDbgMenuLogsEnableLMAUtilsActionID, enLMA_Utils ) );
		oList.push_back( LogActionLogAreaPair( kIZPDbgMenuLogsEnableLMAStoryActionID, enLMA_Story ) );
		//Logs UI
		oList.push_back( LogActionLogAreaPair( kIZPDbgMenuLogsEnableLMAActionComponentActionID, enLMA_ActionComponent ) );
		oList.push_back( LogActionLogAreaPair( kIZPDbgMenuLogsEnableLMADialogsActionID, enLMA_Dialogs ) );
		oList.push_back( LogActionLogAreaPair( kIZPDbgMenuLogsEnableLMAUIActionID, enLMA_UI ) );
		oList.push_back( LogActionLogAreaPair( kIZPDbgMenuLogsEnableLMADnDActionID, enLMA_DnD ) );
		oList.push_back( LogActionLogAreaPair( kIZPDbgMenuLogsEnableLMAPanelAssetsActionID, enLMA_PanelAssets ) );
		oList.push_back( LogActionLogAreaPair( kIZPDbgMenuLogsEnableLMAPanelTasksActionID, enLMA_PanelTasks ) );
		oList.push_back( LogActionLogAreaPair( kIZPDbgMenuLogsEnableLMAAssetsTVActionID, enLMA_AssetsTV ) );
		oList.push_back( LogActionLogAreaPair( kIZPDbgMenuLogsEnableLMATasksTVActionID, enLMA_TasksTV ) );
		oList.push_back( LogActionLogAreaPair( kIZPDbgMenuLogsEnableLMATVHeaderActionID, enLMA_TVHeader ) );
	}
}


//----------------------------------------------------------------------------------------
// GetLMAOfLogAction
//----------------------------------------------------------------------------------------
eIZPMajorLogArea
CZPDebugActionComponent::GetLMAOfLogAction(
	const ActionID &			inActionID,
	const LogActionList &		inList)
{
	LogActionList::const_iterator iter = inList.begin();
	LogActionList::const_iterator endIter = inList.end();
	while( iter != endIter )
	{
		const LogActionLogAreaPair & currPair = *iter;
		if( currPair.Key() == inActionID )
			return currPair.Value();
		++iter;
	}
	ASSERT( iter != endIter );
	return enLMA_None;
}
