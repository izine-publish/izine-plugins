//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Plugin/IZPUI_ActionID.h $
//	$Revision: 3651 $
//	$Date: 2011-11-15 13:45:59 +0100 (Tue, 15 Nov 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 22-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPUI_ActionID_
#define _h_IZPUI_ActionID_
#pragma once

// Actions handled by App Action component
DECLARE_PMID(kActionIDSpace, kIZPLoginActionID,																kIZPUIPrefix + 0 )
DECLARE_PMID(kActionIDSpace, kIZPLogoffActionID,															kIZPUIPrefix + 1 )
DECLARE_PMID(kActionIDSpace, kIZPPreferencesPanelSep1ActionID,												kIZPUIPrefix + 2 )
DECLARE_PMID(kActionIDSpace, kIZPPreferencesPanelActionID,													kIZPUIPrefix + 3 )
DECLARE_PMID(kActionIDSpace, kIZPAboutActionID,																kIZPUIPrefix + 4 )
DECLARE_PMID(kActionIDSpace, kIZPPopupAboutThisActionID,													kIZPUIPrefix + 6 )
	//Handled by kZPUITasksActionComponentBoss
DECLARE_PMID(kActionIDSpace, kIZPCreateTaskActionID,														kIZPUIPrefix + 10 )
DECLARE_PMID(kActionIDSpace, kIZPDuplicateTaskActionID,														kIZPUIPrefix + 11 )
DECLARE_PMID(kActionIDSpace, kIZPDeleteTaskActionID,														kIZPUIPrefix + 12 )
DECLARE_PMID(kActionIDSpace, kIZPOpenTaskDocumentActionID,													kIZPUIPrefix + 13 )
	//Handled by kZPUIAMActionComponentBoss
DECLARE_PMID(kActionIDSpace, kIZPUpdatedSelectedAssetStatusActionID,										kIZPUIPrefix + 20 )
DECLARE_PMID(kActionIDSpace, kIZPCheckoutAllAssignmentStoriesActionID,										kIZPUIPrefix + 21 )
	
	//Debugging menu
DECLARE_PMID(kActionIDSpace, kIZPDebugMenuActionID,															kIZPUIPrefix + 30 )
DECLARE_PMID(kActionIDSpace, kIZPDbgMenuWSUploadVersionActionID,											kIZPUIPrefix + 31 )
DECLARE_PMID(kActionIDSpace, kIZPDbgMenuWSDownloadVersionActionID,											kIZPUIPrefix + 32 )
DECLARE_PMID(kActionIDSpace, kIZPDbgMenuWSUpdateStatusActionID,												kIZPUIPrefix + 33 )
		//Logs
DECLARE_PMID(kActionIDSpace, kIZPDbgMenuLogsDisableAllActionID,												kIZPUIPrefix + 34 )
DECLARE_PMID(kActionIDSpace, kIZPDbgMenuLogsEnableAllActionID,												kIZPUIPrefix + 35 )
		//Logs General
DECLARE_PMID(kActionIDSpace, kIZPDbgMenuLogsSeparator1ActionID,												kIZPUIPrefix + 36 )
DECLARE_PMID(kActionIDSpace, kIZPDbgMenuLogsEnableLMAAppActionID,											kIZPUIPrefix + 37 )
DECLARE_PMID(kActionIDSpace, kIZPDbgMenuLogsEnableLMAIdleActionID,											kIZPUIPrefix + 38 )
DECLARE_PMID(kActionIDSpace, kIZPDbgMenuLogsEnableLMAScriptingActionID,										kIZPUIPrefix + 39 )
DECLARE_PMID(kActionIDSpace, kIZPDbgMenuLogsEnableLMACommandsActionID,										kIZPUIPrefix + 40 )
DECLARE_PMID(kActionIDSpace, kIZPDbgMenuLogsEnableLMAResponderActionID,										kIZPUIPrefix + 41 )
DECLARE_PMID(kActionIDSpace, kIZPDbgMenuLogsEnableLMAWSActionID,											kIZPUIPrefix + 42 )
DECLARE_PMID(kActionIDSpace, kIZPDbgMenuLogsEnableLMADBObjectsActionID,										kIZPUIPrefix + 43 )
		//Logs Asset Management
DECLARE_PMID(kActionIDSpace, kIZPDbgMenuLogsSeparator2ActionID,												kIZPUIPrefix + 44 )
DECLARE_PMID(kActionIDSpace, kIZPDbgMenuLogsEnableLMAAMSysPActionID,										kIZPUIPrefix + 45 )
DECLARE_PMID(kActionIDSpace, kIZPDbgMenuLogsEnableLMAAMSrvPActionID,										kIZPUIPrefix + 46 )
DECLARE_PMID(kActionIDSpace, kIZPDbgMenuLogsEnableLMAAMUISrvPActionID,										kIZPUIPrefix + 47 )
DECLARE_PMID(kActionIDSpace, kIZPDbgMenuLogsEnableLMAAMServiceActionID,										kIZPUIPrefix + 48 )
DECLARE_PMID(kActionIDSpace, kIZPDbgMenuLogsEnableLMAAMUIServiceActionID,									kIZPUIPrefix + 49 )
DECLARE_PMID(kActionIDSpace, kIZPDbgMenuLogsEnableLMAAssetMonitorActionID,									kIZPUIPrefix + 50 )
DECLARE_PMID(kActionIDSpace, kIZPDbgMenuLogsEnableLMAAMActionID,											kIZPUIPrefix + 51 )
		//Logs Utils
DECLARE_PMID(kActionIDSpace, kIZPDbgMenuLogsSeparator3ActionID,												kIZPUIPrefix + 52 )
DECLARE_PMID(kActionIDSpace, kIZPDbgMenuLogsEnableLMAICExportActionID,										kIZPUIPrefix + 53 )
DECLARE_PMID(kActionIDSpace, kIZPDbgMenuLogsEnableLMAUtilsActionID,											kIZPUIPrefix + 54 )
DECLARE_PMID(kActionIDSpace, kIZPDbgMenuLogsEnableLMAStoryActionID,											kIZPUIPrefix + 55 )
		//Logs UI
DECLARE_PMID(kActionIDSpace, kIZPDbgMenuLogsSeparator4ActionID,												kIZPUIPrefix + 56 )
DECLARE_PMID(kActionIDSpace, kIZPDbgMenuLogsEnableLMAActionComponentActionID,								kIZPUIPrefix + 57 )
DECLARE_PMID(kActionIDSpace, kIZPDbgMenuLogsEnableLMADialogsActionID,										kIZPUIPrefix + 58 )
DECLARE_PMID(kActionIDSpace, kIZPDbgMenuLogsEnableLMAUIActionID,											kIZPUIPrefix + 59 )
DECLARE_PMID(kActionIDSpace, kIZPDbgMenuLogsEnableLMADnDActionID,											kIZPUIPrefix + 60 )
DECLARE_PMID(kActionIDSpace, kIZPDbgMenuLogsEnableLMAPanelAssetsActionID,									kIZPUIPrefix + 61 )
DECLARE_PMID(kActionIDSpace, kIZPDbgMenuLogsEnableLMAPanelTasksActionID,									kIZPUIPrefix + 62 )
DECLARE_PMID(kActionIDSpace, kIZPDbgMenuLogsEnableLMAAssetsTVActionID,										kIZPUIPrefix + 63 )
DECLARE_PMID(kActionIDSpace, kIZPDbgMenuLogsEnableLMATasksTVActionID,										kIZPUIPrefix + 64 )
DECLARE_PMID(kActionIDSpace, kIZPDbgMenuLogsEnableLMATVHeaderActionID,										kIZPUIPrefix + 65 )
//Gap till 75 for logs on/off
#define kIZPLogsFirstActionID	kIZPDbgMenuLogsEnableLMAAppActionID
#define kIZPLogsLastActionID	kIZPDbgMenuLogsEnableLMATVHeaderActionID


//DECLARE_PMID(kActionIDSpace, kIZPSaveCopyActionID,															kIZPUIPrefix + 60 )
//DECLARE_PMID(kActionIDSpace, kIZPSaveVersionActionID,														kIZPUIPrefix + 61 )
//DECLARE_PMID(kActionIDSpace, kIZPApplyTagsActionID,															kIZPUIPrefix + 62 )
//DECLARE_PMID(kActionIDSpace, kIZPCheckOutAllActionID,														kIZPUIPrefix + 63 )

DECLARE_PMID(kActionIDSpace, kIZPSeparatorActionID,															kIZPUIPrefix + 80 )
DECLARE_PMID(kActionIDSpace, kIZPSeparator1ActionID,														kIZPUIPrefix + 81 )
DECLARE_PMID(kActionIDSpace, kIZPSeparator2ActionID,														kIZPUIPrefix + 82 )

////DECLARE_PMID(kActionIDSpace, kIZPOpenFromRepositoryDialogActionID,										kIZPUIPrefix + 91 )
//DECLARE_PMID(kActionIDSpace, kIZPFileSeparator1ActionID,													kIZPUIPrefix + 92 )
//DECLARE_PMID(kActionIDSpace, kIZPFileSeparator2ActionID,													kIZPUIPrefix + 93 )

// gap of 10
	//Panel
//DECLARE_PMID(kActionIDSpace, kIZPDocumentsPanelWidgetActionID,												kIZPUIPrefix + 120 )
//DECLARE_PMID(kActionIDSpace, kIZPHistoryPanelWidgetActionID,												kIZPUIPrefix + 121 )
DECLARE_PMID(kActionIDSpace, kIZPAssetsPanelWidgetActionID,													kIZPUIPrefix + 122 )
//DECLARE_PMID(kActionIDSpace, kIZPAssignmentsPanelWidgetActionID,											kIZPUIPrefix + 123 )
DECLARE_PMID(kActionIDSpace, kIZPTasksPanelWidgetActionID,													kIZPUIPrefix + 124 )
	//TreeView Header dynamic menu
DECLARE_PMID(kActionIDSpace, kIZPTVHeaderShowColumnsActionID,												kIZPUIPrefix + 150 )
DECLARE_PMID(kActionIDSpace, kIZPTVHeaderShowColumnsStartActionID,											kIZPUIPrefix + 151 )
	//Keep Gap of 20 
DECLARE_PMID(kActionIDSpace, kIZPTVHeaderShowColumnsEndActionID,											kIZPUIPrefix + 170 )
	//Assets TreeView dynamic menu
DECLARE_PMID(kActionIDSpace, kIZPRenameAssetActionID,														kIZPUIPrefix + 180 )
	//Assignment Panel menu
DECLARE_PMID(kActionIDSpace, kIZPAddLayerToAssignmentActionID,												kIZPUIPrefix + 200 )

// Menu item positions:
#define	kIZPSeparator1MenuItemPosition				10.0
#define kIZPAboutThisMenuItemPosition				11.0
#define kIZPFileSeparator1MenuItemPosition			999
#define kIZPOpenFromRepositoryMenuItemPosition		1004
#define kIZPFileSeparator2MenuItemPosition			1002
#define kIZPPreferencesMenuPosition					1540.0
#define kIZPPreferencesPanelSepMenuPosition			1530.0

#define kIZPLoginMenuItemPosition					1001
#define kIZPLogoffMenuItemPosition					1002
#define	kIZPMainMenuSeparator1ItemPosition			2000
#define kIZPSaveVersionMenuItemPosition				2001
#define kIZPSaveCopyMenuItemPosition				2002
#define	kIZPMainMenuSeparator2ItemPosition			3000
#define kIZPMainMenuRefreshMenuItemPosition			3001
#define	kIZPMainMenuSeparator3ItemPosition			4000
#define kIZPMainMenuExportMenuItemPosition			4001
#define kIZPMainMenuCheckInMenuItemPosition			4002
#define kIZPMainMenuCheckOutMenuItemPosition		4003
#define kIZPMainMenuUpdateMenuItemPosition			4004
#define kIZPMainMenuSaveMenuItemPosition			4005
#define kIZPMainMenuCancelCheckOutMenuItemPosition	4006
#define kIZPMainMenuUnlinkMenuItemPosition			4007

#define	kIZPMainMenuSeparator4ItemPosition			5000
#define kIZPMainMenuExportAllMenuItemPosition		5001
#define	kIZPMainMenuSeparator5ItemPosition			6000

#define kIZPCheckoutAllAssignmentStoriesMenuItemPosition				(kSLICBridgeEditInMenuPosition + 1)
#define kIZPCheckoutAllAssignmentStoriesEditMenuItemPosition			701	//After kICBridgeEditInPlaceSelActionID menu item
#define kIZPCheckoutAllAssignmentStoriesRtMouseTextMenuItemPosition		(kContextTextInCopySubmitMenuPosition)
#define kIZPCheckoutAllAssignmentStoriesRtMouseLayoutMenuItemPosition	(kContextLayoutInCopySubmitMenuPosition)
#define kIZPCheckinAllAssignmentStoriesRtMouseTextMenuItemPosition		(kContextTextInCopySubmitMenuPosition + 1)
#define kIZPCheckinAllAssignmentStoriesRtMouseLayoutMenuItemPosition	(kContextLayoutInCopySubmitMenuPosition + 1)

#define kIZPMainMenuDbgMenuItemPosition				10000
#define kIZPDbgMenuWSUploadVersionMenuItemPosition	10001

// "Plug-ins" sub-menu item position for dialog:
#define kIZPDialogMenuItemPosition	12.0

#endif //_h_IZPUI_ActionID_
