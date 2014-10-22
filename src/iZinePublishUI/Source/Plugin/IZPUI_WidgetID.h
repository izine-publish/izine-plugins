//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Plugin/IZPUI_WidgetID.h $
//	$Revision: 3535 $
//	$Date: 2011-07-25 09:30:08 +0200 (Mon, 25 Jul 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 22-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPUI_WidgetID_
#define _h_IZPUI_WidgetID_
#pragma once

	//Prefs
DECLARE_PMID(kWidgetIDSpace, kIZPPrefsPanelWidgetID,														kIZPUIPrefix + 0 )
DECLARE_PMID(kWidgetIDSpace, kZPUIPrefsAMRefreshDurationNudgeWidgetID,										kIZPUIPrefix + 1 )
DECLARE_PMID(kWidgetIDSpace, kZPUIPrefsAMRefreshDurationEditBoxWidgetID,									kIZPUIPrefix + 2 )
DECLARE_PMID(kWidgetIDSpace, kZPUIPrefsAutoUpdateLinkedStoriesCheckBoxWidgetID,								kIZPUIPrefix + 3 )
DECLARE_PMID(kWidgetIDSpace, kZPUIPrefsAutoUnlockStoriesCheckBoxWidgetID,									kIZPUIPrefix + 4 )
DECLARE_PMID(kWidgetIDSpace, kZPUIPrefsCreateAssignmentOnStoryImportCheckBoxWidgetID,						kIZPUIPrefix + 5 )
DECLARE_PMID(kWidgetIDSpace, kZPUIPrefsUserNameWidgetID,													kIZPUIPrefix + 6 )
DECLARE_PMID(kWidgetIDSpace, kZPUIPrefsPasswordWidgetID,													kIZPUIPrefix + 7 )
DECLARE_PMID(kWidgetIDSpace, kZPUIPrefsGrpDocumentOptionsWidgetID,											kIZPUIPrefix + 8 )
DECLARE_PMID(kWidgetIDSpace, kZPUIPrefsGrpServerCredentialsWidgetID,										kIZPUIPrefix + 9 )
DECLARE_PMID(kWidgetIDSpace, kZPUIPrefsIncludeAllAssignmentsOnDocumentCheckinCheckBoxWidgetID,				kIZPUIPrefix + 10 )
DECLARE_PMID(kWidgetIDSpace, kZPUIPrefsRemoveStylesCheckBoxWidgetID,										kIZPUIPrefix + 11 )
	//Login Dialog
DECLARE_PMID(kWidgetIDSpace, kZPUILoginDlgWidgetID,															kIZPUIPrefix + 20 )
DECLARE_PMID(kWidgetIDSpace, kZPUILoginDlgWebserviceURLEditBoxWidgetID,										kIZPUIPrefix + 21 )
DECLARE_PMID(kWidgetIDSpace, kZPUILoginDlgUsernameEditBoxWidgetID,											kIZPUIPrefix + 22 )
DECLARE_PMID(kWidgetIDSpace, kZPUILoginDlgPasswordEditBoxWidgetID,											kIZPUIPrefix + 23 )
DECLARE_PMID(kWidgetIDSpace, kZPUILoginDlgErrorStaticTextWidgetID,											kIZPUIPrefix + 24 )
DECLARE_PMID(kWidgetIDSpace, kZPUILoginDlgLogoPictureWidgetID,												kIZPUIPrefix + 25 )
DECLARE_PMID(kWidgetIDSpace, kZPUILoginDlgUseProxyCheckBoxWidgetID,											kIZPUIPrefix + 26 )
DECLARE_PMID(kWidgetIDSpace, kZPUILoginDlgProxyServerEditBoxWidgetID,										kIZPUIPrefix + 27 )
DECLARE_PMID(kWidgetIDSpace, kZPUILoginDlgProxyPortEditBoxWidgetID,											kIZPUIPrefix + 28 )
DECLARE_PMID(kWidgetIDSpace, kZPUILoginDlgProxyGroupWidgetID,												kIZPUIPrefix + 29 )
DECLARE_PMID(kWidgetIDSpace, kZPUILoginDlgProxyExpanderWidgetID,											kIZPUIPrefix + 30 )
DECLARE_PMID(kWidgetIDSpace, kZPUILoginDlgProxySperatorWidgetID,											kIZPUIPrefix + 31 )
	//Assets Panel
DECLARE_PMID(kWidgetIDSpace, kZPUIAssetsPanelWidgetID,														kIZPUIPrefix + 40 )
DECLARE_PMID(kWidgetIDSpace, kZPUIAssetsPanelEditionDropDownWidgetID,										kIZPUIPrefix + 41 )
DECLARE_PMID(kWidgetIDSpace, kZPUIAssetsPanelTitleDropDownWidgetID,											kIZPUIPrefix + 42 )
DECLARE_PMID(kWidgetIDSpace, kZPUIAssetsPanelRefreshButtonWidgetID,											kIZPUIPrefix + 43 )
DECLARE_PMID(kWidgetIDSpace, kZPUIAssetsPanelUsernameWidgetID,												kIZPUIPrefix + 44 )
DECLARE_PMID(kWidgetIDSpace, kZPUIAssetsPanelVersionWidgetID,												kIZPUIPrefix + 45 )
DECLARE_PMID(kWidgetIDSpace, kZPUIAssetsPanelHdrAssetsWidgetID,												kIZPUIPrefix + 46 )
DECLARE_PMID(kWidgetIDSpace, kZPUIAssetsPanelLstAssetsWidgetID,												kIZPUIPrefix + 47 )
DECLARE_PMID(kWidgetIDSpace, kZPUIDocumentsPanelPreviewPanelWidgetID,										kIZPUIPrefix + 48 )

//Assets Tree View control
DECLARE_PMID(kWidgetIDSpace, kZPUIAssetsTVStatusNodeWidgetID,												kIZPUIPrefix + 60 )
DECLARE_PMID(kWidgetIDSpace, kZPUIAssetsTVAssetNodeWidgetID,												kIZPUIPrefix + 61 )
DECLARE_PMID(kWidgetIDSpace, kZPUIAssetsTVNodeIconWidgetID,													kIZPUIPrefix + 62 )
//DECLARE_PMID(kWidgetIDSpace, kZPUIAssetsTVNodeTypeIconWidgetID,											kIZPUIPrefix + 63 )
//DECLARE_PMID(kWidgetIDSpace, kZPUIAssetsTVNodeLockIconWidgetID,											kIZPUIPrefix + 64 )
//DECLARE_PMID(kWidgetIDSpace, kZPUIAssetsTVNodeSaveRollOverIconButtonWidgetID,								kIZPUIPrefix + 65 )
//DECLARE_PMID(kWidgetIDSpace, kZPUIAssetsTVNodeDeleteRollOverIconButtonWidgetID,							kIZPUIPrefix + 66 )
DECLARE_PMID(kWidgetIDSpace, kZPUIAssetsTVNodeNameWidgetID,													kIZPUIPrefix + 67 )
//DECLARE_PMID(kWidgetIDSpace, kZPUIAssetsTVNodeDocumentWidgetID,											kIZPUIPrefix + 68 )
//DECLARE_PMID(kWidgetIDSpace, kZPUIAssetsTVNodeVersionNumberWidgetID,										kIZPUIPrefix + 69 )
//DECLARE_PMID(kWidgetIDSpace, kZPUIAssetsTVNodeVersionAuthorWidgetID,										kIZPUIPrefix + 70 )
//DECLARE_PMID(kWidgetIDSpace, kZPUIAssetsTVNodeVersionDateWidgetID,										kIZPUIPrefix + 71 )
//DECLARE_PMID(kWidgetIDSpace, kZPUIAssetsTVNodeDescriptionWidgetID,										kIZPUIPrefix + 72 )
//Gap till eTVColAssetsCount
#define kZPUIAssetsTVNodeFirstColumnWidgetID kZPUIAssetsTVNodeIconWidgetID

	//Alert Dialog
DECLARE_PMID(kWidgetIDSpace, kZPUIAlertDlgWidgetID,															kIZPUIPrefix + 90 )
DECLARE_PMID(kWidgetIDSpace, kZPUIAlertDlgMessageWidgetID,													kIZPUIPrefix + 91 )
DECLARE_PMID(kWidgetIDSpace, kZPUIAlertDlgMessageScrollWidgetID,											kIZPUIPrefix + 92 )
DECLARE_PMID(kWidgetIDSpace, kZPUIAlertDlgBtn1WidgetID,														kIZPUIPrefix + 93 )
DECLARE_PMID(kWidgetIDSpace, kZPUIAlertDlgBtn2WidgetID,														kIZPUIPrefix + 94 )
DECLARE_PMID(kWidgetIDSpace, kZPUIAlertDlgBtn3WidgetID,														kIZPUIPrefix + 95 )
DECLARE_PMID(kWidgetIDSpace, kZPUIAlertDlgBtn4WidgetID,														kIZPUIPrefix + 96 )

	//Rename asset dialog
DECLARE_PMID(kWidgetIDSpace, kZPUIRenameAssetDlgWidgetID,													kIZPUIPrefix + 110 )
DECLARE_PMID(kWidgetIDSpace, kZPUIRenameAssetDlgTxtNewNameWidgetID,											kIZPUIPrefix + 111 )

	//Status Update dialog
DECLARE_PMID(kWidgetIDSpace, kZPUIStatusUpdateDlgWidgetID,													kIZPUIPrefix + 120 )
DECLARE_PMID(kWidgetIDSpace, kZPUIStatusUpdateDlgTxtDescriptionWidgetID,									kIZPUIPrefix + 121 )
DECLARE_PMID(kWidgetIDSpace, kZPUIStatusUpdateDlgPopStatusWidgetID,											kIZPUIPrefix + 122 )

//SaveToRepository
DECLARE_PMID(kWidgetIDSpace, kZPUISaveToRepDialogWidgetID,													kIZPUIPrefix + 130 )
DECLARE_PMID(kWidgetIDSpace, kZPUISaveToRepNameEditBoxWidgetID,												kIZPUIPrefix + 131 )
DECLARE_PMID(kWidgetIDSpace, kZPUISaveToRepDescriptionEditBoxWidgetID,										kIZPUIPrefix + 132 )
DECLARE_PMID(kWidgetIDSpace, kZPUISaveToRepPublicationDropDownWidgetID,										kIZPUIPrefix + 133 )
DECLARE_PMID(kWidgetIDSpace, kZPUISaveToRepEditionDropDownWidgetID,											kIZPUIPrefix + 134 )
DECLARE_PMID(kWidgetIDSpace, kZPUISaveToRepStatusDropDownWidgetID,											kIZPUIPrefix + 135 )
DECLARE_PMID(kWidgetIDSpace, kZPUISaveToRepIsTemplateCheckBoxWidgetID,										kIZPUIPrefix + 136 )

	//Create Task Dialog
DECLARE_PMID(kWidgetIDSpace, kZPUICreateTaskDlgWidgetID,													kIZPUIPrefix + 145 )
DECLARE_PMID(kWidgetIDSpace, kZPUICreateTaskDlgTxtSubjectWidgetID,											kIZPUIPrefix + 146 )
DECLARE_PMID(kWidgetIDSpace, kZPUICreateTaskDlgTxtDescriptionWidgetID,										kIZPUIPrefix + 147 )
DECLARE_PMID(kWidgetIDSpace, kZPUICreateTaskDlgTxtSpreadWidgetID,											kIZPUIPrefix + 148 )
DECLARE_PMID(kWidgetIDSpace, kZPUICreateTaskDlgTxtPageWidgetID,												kIZPUIPrefix + 149 )
DECLARE_PMID(kWidgetIDSpace, kZPUICreateTaskDlgTxtCommentsWidgetID,											kIZPUIPrefix + 150 )
DECLARE_PMID(kWidgetIDSpace, kZPUICreateTaskDlgPopTitleWidgetID,											kIZPUIPrefix + 151 )
DECLARE_PMID(kWidgetIDSpace, kZPUICreateTaskDlgPopEditionWidgetID,											kIZPUIPrefix + 152 )
DECLARE_PMID(kWidgetIDSpace, kZPUICreateTaskDlgPopAsgnToDocWidgetID,										kIZPUIPrefix + 153 )
DECLARE_PMID(kWidgetIDSpace, kZPUICreateTaskDlgPopStatusWidgetID,											kIZPUIPrefix + 154 )
DECLARE_PMID(kWidgetIDSpace, kZPUICreateTaskDlgPopCategoryWidgetID,											kIZPUIPrefix + 155 )
DECLARE_PMID(kWidgetIDSpace, kZPUICreateTaskDlgPopAsgnToUserWidgetID,										kIZPUIPrefix + 156 )

	// Tasks Panel
DECLARE_PMID(kWidgetIDSpace, kZPUITasksPanelWidgetID,														kIZPUIPrefix + 165 )
DECLARE_PMID(kWidgetIDSpace, kZPUITasksPanelTitleDropDownWidgetID,											kIZPUIPrefix + 166 )
DECLARE_PMID(kWidgetIDSpace, kZPUITasksPanelEditionDropDownWidgetID,										kIZPUIPrefix + 167 )
DECLARE_PMID(kWidgetIDSpace, kZPUITasksPanelRefreshButtonWidgetID,											kIZPUIPrefix + 168 )
DECLARE_PMID(kWidgetIDSpace, kZPUITasksPanelTxtDescriptionWidgetID,											kIZPUIPrefix + 170 )
DECLARE_PMID(kWidgetIDSpace, kZPUITasksPanelTxtSpreadWidgetID,												kIZPUIPrefix + 171 )
DECLARE_PMID(kWidgetIDSpace, kZPUITasksPanelTxtPageWidgetID,												kIZPUIPrefix + 172 )
DECLARE_PMID(kWidgetIDSpace, kZPUITasksPanelPopTitleWidgetID,												kIZPUIPrefix + 174 )
DECLARE_PMID(kWidgetIDSpace, kZPUITasksPanelPopEditionWidgetID,												kIZPUIPrefix + 175 )
DECLARE_PMID(kWidgetIDSpace, kZPUITasksPanelPopAsgnToDocWidgetID,											kIZPUIPrefix + 176 )
DECLARE_PMID(kWidgetIDSpace, kZPUITasksPanelPopStatusWidgetID,												kIZPUIPrefix + 177 )
DECLARE_PMID(kWidgetIDSpace, kZPUITasksPanelPopCategoryWidgetID,											kIZPUIPrefix + 178 )
DECLARE_PMID(kWidgetIDSpace, kZPUITasksPanelPopAsgnToUserWidgetID,											kIZPUIPrefix + 179 )
DECLARE_PMID(kWidgetIDSpace, kZPUITasksPanelLstTasksWidgetID,												kIZPUIPrefix + 180 )
DECLARE_PMID(kWidgetIDSpace, kZPUITasksPanelHdrTasksWidgetID,												kIZPUIPrefix + 181 )
DECLARE_PMID(kWidgetIDSpace, kZPUITasksPanelBtnHistoryWidgetID,												kIZPUIPrefix + 182 )

	//Task TreeView control
DECLARE_PMID(kWidgetIDSpace, kZPUITasksTVNodeWidgetID,														kIZPUIPrefix + 191 )
DECLARE_PMID(kWidgetIDSpace, kZPUITasksTVNodeSubjectWidgetID,												kIZPUIPrefix + 192 )
//GAP of 15, reserved for column headers
DECLARE_PMID(kWidgetIDSpace, kZPUITasksTVNodeEndWidgetID,													kIZPUIPrefix + 206 )

DECLARE_PMID(kWidgetIDSpace, kZPUITVHdrControlStripWidgetID,												kIZPUIPrefix + 210 )
DECLARE_PMID(kWidgetIDSpace, kZPUITVColumnHeaderStartWidgetID,												kIZPUIPrefix + 211)
//GAP of 15, reserved for column headers
DECLARE_PMID(kWidgetIDSpace, kZPUITVColumnHeaderEndWidgetID,												kIZPUIPrefix + 225 )


DECLARE_PMID(kWidgetIDSpace, kZPUITVGroupElementWidgetID,													kIZPUIPrefix + 226 )

//Note: After consuming all the IDs for kIZPUIPrefix, we can use the kIZPPrefix as non-UI plugin will not have any widget space

#endif //_h_IZPUI_WidgetID_
