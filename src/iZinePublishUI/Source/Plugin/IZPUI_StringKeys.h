//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Plugin/IZPUI_StringKeys.h $
//	$Revision: 4096 $
//	$Date: 2013-04-01 09:20:00 +0200 (Mon, 01 Apr 2013) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 12-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPUI_StringKeys_
#define _h_IZPUI_StringKeys_
#pragma once

//KBSE areas

#define kZPUIActionArea					"KBSCE iZine"
#define kZPUITasksActionArea			kZPUIActionArea ": Tasks: "
#define kZPUIAssetsActionArea			kZPUIActionArea ": Assets: "

//Menu Path and Item Name Key
	// "About Plug-ins" sub-menu:
#define kIZPUIAboutMenuKey				kIZPUIStringPrefix "kIZPAboutMenuKey"
#define kIZPUIAboutMenuPath				kSDKDefStandardAboutMenuPath kIZPCompanyKey

	// "Plug-ins" sub-menu:
#define kIZPPreferencesPanelSepMenuPath	kSDKDefPreferencesStandardMenuPath kSDKDefDelimiterAndSeparatorPath

//////#define kIZPFileMenuPath				"Main:&File"
//////#define kIZPFileSepMenuPath				kIZPFileMenuPath kSDKDefDelimiterAndSeparatorPath
#define kIZPUIPluginsMenuKey 			kIZPUIStringPrefix "kIZPPluginsMenuKey"
//////#define kIZPPluginsMenuPath				kSDKDefPlugInsStandardMenuPath kIZPCompanyKey kSDKDefDelimitMenuPath kIZPPluginsMenuKey

#define kIZPMainMenuPath				"Main" kSDKDefDelimitMenuPath kIZPUIPluginsMenuKey
//////#define kIZPMouseTextMenuPath			"RtMouseText:" kIZPUIStringPrefix "kIZPPluginsMenuKey"
//////#define kIZPMouseLayoutMenuPath			"RtMouseLayout:" kIZPUIStringPrefix "kIZPPluginsMenuKey"
//////#define kIZPMainMenuSeparatorPath		kIZPMainMenuPath kSDKDefDelimiterAndSeparatorPath
#define kIZPAboutMenuKey				kIZPUIStringPrefix "kIZPAboutMenuKey"

	// Menu item keys:
#define kIZPLoginMenuKey				kIZPUIStringPrefix "kIZPLoginMenuKey"
#define kIZPLogoffMenuKey				kIZPUIStringPrefix "kIZPLogoffMenuKey"
#define kIZPMainMenuRefreshMenuKey		kIZPUIStringPrefix "kIZPMainMenuRefreshMenuKey"
#define kIZPCreateTaskMenuKey			kIZPUIStringPrefix "kIZPCreateTaskMenuKey"

	// Assets Panel menu item keys
#define kIZPUpdatedSelectedAssetStatusMenuKey	kIZPUIStringPrefix "kIZPUpdatedSelectedAssetStatusMenuKey"

	// Task panel menu item keys
#define kIZPDuplicateTaskMenuKey				kIZPUIStringPrefix "kIZPDuplicateTaskMenuKey"
#define kIZPDeleteTaskMenuKey					kIZPUIStringPrefix "kIZPDeleteTaskMenuKey"
#define kIZPOpenTaskDocumentMenuKey				kIZPUIStringPrefix "kIZPOpenTaskDocumentMenuKey"

	// Assignment Panel menu item keys
#define kIZPCheckoutAllAssignmentStoriesMenuKey kIZPUIStringPrefix "kIZPCheckoutAllAssignmentStoriesMenuKey"
#define kIZPAddLayerToAssignmentMenuKey			kIZPUIStringPrefix "kIZPAddLayerToAssignmentMenuKey"

	// Debug menu item keys:
#define kIZPDebugMenuKey						kIZPUIStringPrefix "kIZPDebugMenuKey"
#define kIZPDbgMenuPath							kIZPMainMenuPath kSDKDefDelimitMenuPath kIZPDebugMenuKey
#define kIZPDbgMenuWSUploadVersionMenuKey		kIZPUIStringPrefix "kIZPDbgMenuWSUploadVersionMenuKey"
#define kIZPDbgMenuWSDownloadVersionMenuKey		kIZPUIStringPrefix "kIZPDbgMenuWSDownloadVersionMenuKey"
#define kIZPDbgMenuWSUpdateStatusMenuKey		kIZPUIStringPrefix "kIZPDbgMenuWSUpdateStatusMenuKey"
#define kIZPDbgLogsMenuKey						kIZPUIStringPrefix "kIZPDbgLogsMenuKey"
#define kIZPDbgLogsMenuPath						kIZPDbgMenuPath kSDKDefDelimitMenuPath kIZPDbgLogsMenuKey
		//Logs
#define kIZPDbgMenuLogsDisableAllMenuKey				kIZPUIStringPrefix "kIZPDbgMenuLogsDisableAllMenuKey"
#define kIZPDbgMenuLogsEnableAllMenuKey					kIZPUIStringPrefix "kIZPDbgMenuLogsEnableAllMenuKey"
		//Logs General
#define kIZPDbgMenuLogsSeparator1MenuKey				kIZPUIStringPrefix "kIZPDbgMenuLogsSeparator1MenuKey"
#define kIZPDbgMenuLogsEnableLMAAppMenuKey				kIZPUIStringPrefix "kIZPDbgMenuLogsEnableLMAAppMenuKey"
#define kIZPDbgMenuLogsEnableLMAIdleMenuKey				kIZPUIStringPrefix "kIZPDbgMenuLogsEnableLMAIdleMenuKey"
#define kIZPDbgMenuLogsEnableLMAScriptingMenuKey		kIZPUIStringPrefix "kIZPDbgMenuLogsEnableLMAScriptingMenuKey"
#define kIZPDbgMenuLogsEnableLMACommandsMenuKey			kIZPUIStringPrefix "kIZPDbgMenuLogsEnableLMACommandsMenuKey"
#define kIZPDbgMenuLogsEnableLMAResponderMenuKey		kIZPUIStringPrefix "kIZPDbgMenuLogsEnableLMAResponderMenuKey"
#define kIZPDbgMenuLogsEnableLMAWSMenuKey				kIZPUIStringPrefix "kIZPDbgMenuLogsEnableLMAWSMenuKey"
#define kIZPDbgMenuLogsEnableLMADBObjectsMenuKey		kIZPUIStringPrefix "kIZPDbgMenuLogsEnableLMADBObjectsMenuKey"
		//Logs Asset Management
#define kIZPDbgMenuLogsSeparator2MenuKey				kIZPDbgMenuLogsSeparator1MenuKey
#define kIZPDbgMenuLogsEnableLMAAMSysPMenuKey			kIZPUIStringPrefix "kIZPDbgMenuLogsEnableLMAAMSysPMenuKey"
#define kIZPDbgMenuLogsEnableLMAAMSrvPMenuKey			kIZPUIStringPrefix "kIZPDbgMenuLogsEnableLMAAMSrvPMenuKey"
#define kIZPDbgMenuLogsEnableLMAAMUISrvPMenuKey			kIZPUIStringPrefix "kIZPDbgMenuLogsEnableLMAAMUISrvPMenuKey"
#define kIZPDbgMenuLogsEnableLMAAMServiceMenuKey		kIZPUIStringPrefix "kIZPDbgMenuLogsEnableLMAAMServiceMenuKey"
#define kIZPDbgMenuLogsEnableLMAAMUIServiceMenuKey		kIZPUIStringPrefix "kIZPDbgMenuLogsEnableLMAAMUIServiceMenuKey"
#define kIZPDbgMenuLogsEnableLMAAssetMonitorMenuKey		kIZPUIStringPrefix "kIZPDbgMenuLogsEnableLMAAssetMonitorMenuKey"
#define kIZPDbgMenuLogsEnableLMAAMMenuKey				kIZPUIStringPrefix "kIZPDbgMenuLogsEnableLMAAMMenuKey"
		//Logs Utils
#define kIZPDbgMenuLogsSeparator3MenuKey				kIZPDbgMenuLogsSeparator1MenuKey
#define kIZPDbgMenuLogsEnableLMAICExportMenuKey			kIZPUIStringPrefix "kIZPDbgMenuLogsEnableLMAICExportMenuKey"
#define kIZPDbgMenuLogsEnableLMAUtilsMenuKey			kIZPUIStringPrefix "kIZPDbgMenuLogsEnableLMAUtilsMenuKey"
#define kIZPDbgMenuLogsEnableLMAStoryMenuKey			kIZPUIStringPrefix "kIZPDbgMenuLogsEnableLMAStoryMenuKey"
		//Logs UI
#define kIZPDbgMenuLogsSeparator4MenuKey				kIZPDbgMenuLogsSeparator1MenuKey
#define kIZPDbgMenuLogsEnableLMAActionComponentMenuKey	kIZPUIStringPrefix "kIZPDbgMenuLogsEnableLMAActionComponentMenuKey"
#define kIZPDbgMenuLogsEnableLMADialogsMenuKey			kIZPUIStringPrefix "kIZPDbgMenuLogsEnableLMADialogsMenuKey"
#define kIZPDbgMenuLogsEnableLMAUIMenuKey				kIZPUIStringPrefix "kIZPDbgMenuLogsEnableLMAUIMenuKey"
#define kIZPDbgMenuLogsEnableLMADnDMenuKey				kIZPUIStringPrefix "kIZPDbgMenuLogsEnableLMADnDMenuKey"
#define kIZPDbgMenuLogsEnableLMAPanelAssetsMenuKey		kIZPUIStringPrefix "kIZPDbgMenuLogsEnableLMAPanelAssetsMenuKey"
#define kIZPDbgMenuLogsEnableLMAPanelTasksMenuKey		kIZPUIStringPrefix "kIZPDbgMenuLogsEnableLMAPanelTasksMenuKey"
#define kIZPDbgMenuLogsEnableLMAAssetsTVMenuKey			kIZPUIStringPrefix "kIZPDbgMenuLogsEnableLMAAssetsTVMenuKey"
#define kIZPDbgMenuLogsEnableLMATasksTVMenuKey			kIZPUIStringPrefix "kIZPDbgMenuLogsEnableLMATasksTVMenuKey"
#define kIZPDbgMenuLogsEnableLMATVHeaderMenuKey			kIZPUIStringPrefix "kIZPDbgMenuLogsEnableLMATVHeaderMenuKey"


#define kIZPTVHeaderMenuPath					"ZPTVHdrMenu"
#define kIZPTVHdrVisibleColumnsMenuKey			kIZPUIStringPrefix "kIZPTVHdrVisibleColumnsMenuKey"

#define kIZPAssetsTVMenuPath					"ZPAssetTVMenu"
#define kIZPRenameAssetMenuKey					kIZPUIStringPrefix "kIZPRenameAssetMenuKey"

#define kIZPAssetsPanelRefreshButtonKey kIZPUIStringPrefix "kIZPAssetsPanelRefreshButtonKey"
#define kIZPAssetsPanelHistoryButtonKey			kIZPUIStringPrefix "kIZPAssetsPanelHistoryButtonKey"

	// Other StringKeys:
#define kIZPAboutBoxStringKey			kIZPUIStringPrefix "kIZPAboutBoxStringKey"
//////#define kIZPStaticTextKey				kIZPUIStringPrefix	"kIZPStaticTextKey"
#define kZPUIAssetsPanelPopupMenuNameKey	kIZPUIStringPrefix	"kIZPAssetsPanelPopupMenuNameKey"
#define kZPUITasksPanelPopupMenuNameKey		kIZPUIStringPrefix	"kZPUITasksPanelPopupMenuNameKey"
//////#define kIZPHistoryPanelInternalPopupMenuNameKey kIZPUIStringPrefix	"kIZPHistoryPanelInternalPopupMenuNameKey"

////#define kIZPTargetSeparatorPath			kIZPTargetMenuPath kSDKDefDelimiterAndSeparatorPath
#define kIZPAssetsPanelInternalMenuPath				kZPUIAssetsPanelPopupMenuNameKey
#define kIZPTasksPanelInternalMenuPath				kZPUITasksPanelPopupMenuNameKey


#pragma mark -
// Translatable Strings
	// Save to Repository Dialog
#define kZPUISaveToRepDialogTitleKey						kIZPUIStringPrefix "kZPSaveToRepDialogTitleKey"
#define kZPUISaveToRepNameKey								kIZPUIStringPrefix "kZPSaveToRepNameKey"
#define kZPUISaveToRepDescriptionKey						kIZPUIStringPrefix "kZPSaveToRepDescriptionKey"
#define kZPUISaveToRepUploadKey								kIZPUIStringPrefix "kZPSaveToRepUploadKey"
#define kZPUISaveToRepPublicationKey						kIZPUIStringPrefix "kZPSaveToRepPublicationKey"
#define kZPUISaveToRepEditionKey							kIZPUIStringPrefix "kZPSaveToRepEditionKey"
#define kZPUISaveToRepStoriesKey							kIZPUIStringPrefix "kZPSaveToRepStoriesKey"
#define kZPUISaveToRepTemplateKey							kIZPUIStringPrefix "kZPSaveDialogTemplateKey"
#define kZPUISaveToRepStatusKey								kIZPUIStringPrefix "kZPSaveDialogStatusKey"
#define kZPUISaveToRepDialogTitle_NoneKey					kIZPUIStringPrefix "kZPUISaveToRepDialogTitle_NoneKey"
#define kZPUISaveToRepDialogTitle_IDDocumentKey				kIZPUIStringPrefix "kZPUISaveToRepDialogTitle_IDDocumentKey"
#define kZPUISaveToRepDialogTitle_IDTemplateKey				kIZPUIStringPrefix "kZPUISaveToRepDialogTitle_IDTemplateKey"
#define kZPUISaveToRepDialogTitle_ICTemplateKey				kIZPUIStringPrefix "kZPUISaveToRepDialogTitle_ICTemplateKey"
#define kZPUISaveToRepDialogTitle_ICDocumentKey				kIZPUIStringPrefix "kZPUISaveToRepDialogTitle_ICDocumentKey"
#define kZPUISaveToRepDialogTitle_ICAssignmentKey			kIZPUIStringPrefix "kZPUISaveToRepDialogTitle_ICAssignmentKey"
#define kZPUISaveToRepDialogTitle_IDStoryKey				kIZPUIStringPrefix "kZPUISaveToRepDialogTitle_IDStoryKey"

	// Open from Repository Dialog
//////#define kIZPOpenFromRepDialogTitleKey					kIZPUIStringPrefix "kIZPOpenFromRepDialogTitleKey"
//////#define kIZPOpenFromRepOpenKey							kIZPUIStringPrefix "kIZPOpenFromRepOpenKey"

	// Prefs Panel
#define kZPPrefsPanelPanelTitleKey							kIZPUIStringPrefix "kZPPrefsPanelPanelTitleKey"
#define kZPPrefsAssetMonitorRefreshDurationTitleKey			kIZPUIStringPrefix "kZPPrefsAssetMonitorRefreshDurationTitleKey"
#define kZPPrefsAutoUpdateLinksOnOpeningDocumentKey			kIZPUIStringPrefix "kZPPrefsAutoUpdateLinksOnOpeningDocumentKey"
#define kZPPrefsAutoUnlockStoriesKey						kIZPUIStringPrefix "kZPPrefsAutoUnlockStoriesKey"
#define kZPPrefsCreateAssignmentOnStoryImportKey			kIZPUIStringPrefix "kZPPrefsCreateAssignmentOnStoryImportKey"
#define kZPPrefsIncludeAllAssignmentsOnDocumentCheckinKey	kIZPUIStringPrefix "kZPPrefsIncludeAllAssignmentsOnDocumentCheckinKey"
#define kZPPrefsGrpDocumentOptionsKey						kIZPUIStringPrefix "kZPPrefsGrpDocumentOptionsKey"
#define kZPPrefsGrpServerCredentialsKey						kIZPUIStringPrefix "kZPPrefsGrpServerCredentialsKey"
#define kZPPrefsRemoveStylesKey								kIZPUIStringPrefix "kZPPrefsRemoveStylesKey"
//////#define kIZPPrefsWebServicelTitleKey					kIZPUIStringPrefix "kIZPPrefsWebServicelTitleKey"
//////#define kIZPPrefsWebServiceURLKey						kIZPUIStringPrefix "kIZPPrefsWebServiceURLKey"
//////#define kIZPPrefsUserNameKey							kIZPUIStringPrefix "kIZPPrefsUserNameKey"
//////#define kIZPPrefsPasswordKey							kIZPUIStringPrefix "kIZPPrefsPasswordKey"
//////#define kIZPPrefsDocumentPathKey						kIZPUIStringPrefix "kIZPPrefsDocumentPathKey"
//////#define kIZPPrefsProjectPathTextWidgetKey				kIZPUIStringPrefix "kIZPPrefsProjectPathTextWidgetKey"
//////#define kIZPPrefsProjectFolderTextBoxKey				kIZPUIStringPrefix "kIZPPrefsProjectFolderTextBoxKey"
//////#define kIZPPrefsBrowseButtonTextBoxKey					kIZPUIStringPrefix "kIZPPrefsBrowseButtonTextBoxKey"
//////#define kIZPPrefsProjectPathKey							kIZPUIStringPrefix "kIZPPrefsProjectPathKey"
	// Comments Dialog
//////#define kIZPEnterCommentTitleKey						kIZPUIStringPrefix "kIZPEnterCommentTitleKey"
//////#define kIZPCommentsDialogTitleKey						kIZPUIStringPrefix "kIZPCommentsDialogTitleKey"
//////#define kIZPCommentsDialogStatusKey						kIZPUIStringPrefix "kIZPCommentsDialogStatusKey"
//////#define kIZPCommentsDialogDescriptionKey				kIZPUIStringPrefix "kIZPCommentsDialogDescriptionKey"
//////#define kIZPExportAssetDialogTitleKey					kIZPUIStringPrefix "kIZPExportAssetDialogTitleKey"

	// "Plug-ins" sub-menu item key for dialog:
//////#define kIZPDialogMenuItemKey							kIZPUIStringPrefix "kIZPDialogMenuItemKey"
//////#define kIZPOpenFromRepositoryMenuItemKey				kIZPUIStringPrefix "kIZPOpenFromRepositoryMenuItemKey"
#define kIZPPreferencesPanelMenuItemKey					kIZPUIStringPrefix "kIZPPreferencesPanelMenuItemKey"
//////#define kIZPSaveVersionMenuItemKey						kIZPUIStringPrefix "kIZPSaveVersionMenuItemKey"
//////#define kIZPSaveCopyMenuItemKey							kIZPUIStringPrefix "kIZPSaveCopyMenuItemKey"
//////#define kIZPApplyTagsMenuItemKey						kIZPUIStringPrefix "kIZPApplyTagsMenuItemKey"

	// main menu
//////#define kIZPMainMenuExportMenuItemKey					kIZPUIStringPrefix "kIZPMainMenuExportMenuItemKey"
//////#define kIZPMainMenuCheckInMenuItemKey					kIZPUIStringPrefix "kIZPMainMenuCheckInMenuItemKey"
//////#define kIZPMainMenuCheckOutMenuItemKey					kIZPUIStringPrefix "kIZPMainMenuCheckOutMenuItemKey"
//////#define kIZPMainMenuCancelCheckOutMenuItemKey			kIZPUIStringPrefix "kIZPMainMenuCancelCheckOutMenuItemKey"
//////#define kIZPMainMenuUnlinkMenuItemKey					kIZPUIStringPrefix "kIZPMainMenuUnlinkMenuItemKey"
//////#define kIZPMainMenuUpdateMenuItemKey					kIZPUIStringPrefix "kIZPMainMenuUpdateMenuItemKey"
//////#define kIZPMainMenuSaveMenuItemKey						kIZPUIStringPrefix "kIZPMainMenuSaveMenuItemKey"
//////#define kIZPMainMenuExportAllMenuItemKey				kIZPUIStringPrefix "kIZPMainMenuExportAllMenuItemKey"
//////#define kIZPCheckOutAllMenuItemKey						kIZPUIStringPrefix "kIZPCheckOutAllMenuItemKey"

	// History Panel
//#define kIZPHistoryPanelTitleKey						kIZPUIStringPrefix "kIZPHistoryPanelTitleKey"
//////#define kIZPHistoryPanelUpdateButtonKey					kIZPUIStringPrefix "kIZPHistoryPanelUpdateButtonKey"
//////#define kIZPHistoryCol_DateKey							kIZPUIStringPrefix "kIZPHistoryCol_DateKey"
//////#define kIZPHistoryCol_UserKey							kIZPUIStringPrefix "kIZPHistoryCol_UserKey"
//////#define kIZPHistoryCol_DocumentKey						kIZPUIStringPrefix "kIZPHistoryCol_DocumentKey"
//////#define kIZPHistoryCol_CommentKey						kIZPUIStringPrefix "kIZPHistoryCol_CommentKey"

	// Document Info Panel
//////#define kIZPDocumentsPanelIDStaticTextKey				kIZPUIStringPrefix "kIZPDocumentsPanelIDStaticTextKey"
//////#define kIZPDocumentsPanelPublicationStaticTextKey		kIZPUIStringPrefix "kIZPDocumentsPanelPublicationStaticTextKey"
//////#define kIZPDocumentsPanelNameStaticTextKey				kIZPUIStringPrefix "kIZPDocumentsPanelNameStaticTextKey"
#define kIZPDocumentsPanelTitleKey						kIZPUIStringPrefix "kIZPDocumentsPanelTitleKey"
//////#define kIZPDocumentsPanelUpdateButtonKey				kIZPUIStringPrefix "kIZPDocumentsPanelUpdateButtonKey"

	// Asset Info Panel
#define kIZPAssetsPanelIDStaticTextKey					kIZPUIStringPrefix "kIZPAssetsPanelIDStaticTextKey"
#define kIZpAssetsPanelPublicationStaticTextKey			kIZPUIStringPrefix "kIZpAssetsPanelPublicationStaticTextKey"
#define kIZPAssetsPanelNameStaticTextKey				kIZPUIStringPrefix "kIZPAssetsPanelNameStaticTextKey"
#define kIZPAssetsPanelTitleKey							kIZPUIStringPrefix "kIZPAssetsPanelTitleKey"
#define kIZPAssetsPanelUpdateButtonKey					kIZPUIStringPrefix "kIZPAssetsPanelUpdateButtonKey"
#define kIZPAssetsPanelUserKey							kIZPUIStringPrefix "kIZPAssetsPanelUserKey"

	// Login dialog
#define kIZPLoginDialogTitleKey							kIZPUIStringPrefix "kIZPLoginDialogTitleKey"
#define kIZPLoginButtonKey								kIZPUIStringPrefix "kIZPLoginButtonKey"
#define kIZPLoginDialogWebServiceURLKey					kIZPUIStringPrefix "kIZPLoginDialogWebServiceURLKey"
#define kIZPLoginDialogPasswordKey						kIZPUIStringPrefix "kIZPLoginDialogPasswordKey"
#define kIZPLoginDialogUsernameKey						kIZPUIStringPrefix "kIZPLoginDialogUsernameKey"
#define kIZPLoginDialogErrorMessageKey					kIZPUIStringPrefix "kIZPLoginDialogErrorMessageKey"
#define kIZPLoginDialogUseProxyServerCheckBoxKey		kIZPUIStringPrefix "kIZPLoginDialogUseProxyServerCheckBoxKey"
#define kIZPLoginDialogProxyServerKey					kIZPUIStringPrefix "kIZPLoginDialogProxyServerKey"
#define kIZPLoginDialogProxyPortKey						kIZPUIStringPrefix "kIZPLoginDialogProxyPortKey"
#define kIZPLoginDialogAdvancedSettingKey				kIZPUIStringPrefix "kIZPLoginDialogAdvancedSettingKey"

	// Create Task dialog
#define kIZPCreateTaskDialogTitleKey					kIZPUIStringPrefix "kIZPCreateTaskDialogTitleKey"
#define kIZPUpdateTaskDialogTitleKey					kIZPUIStringPrefix "kIZPUpdateTaskDialogTitleKey"
#define kIZPCreateTaskDlgCreateBtnKey					kIZPUIStringPrefix "kIZPCreateTaskDlgCreateBtnKey"
#define kIZPCreateTaskDlgUpdateBtnKey					kIZPUIStringPrefix "kIZPCreateTaskDlgUpdateBtnKey"
#define kIZPCTaskDlgLblSubjectKey						kIZPUIStringPrefix "kIZPCTaskDlgLblSubjectKey"
#define kIZPCTaskDlgLblDescriptionKey					kIZPUIStringPrefix "kIZPCTaskDlgLblDescriptionKey"
#define kIZPCTaskDlgLblAsgnToDocKey						kIZPUIStringPrefix "kIZPCTaskDlgLblAsgnToDocKey"
#define kIZPCTaskDlgLblSpreadKey						kIZPUIStringPrefix "kIZPCTaskDlgLblSpreadKey"
#define kIZPCTaskDlgLblPageKey							kIZPUIStringPrefix "kIZPCTaskDlgLblPageKey"
#define kIZPCTaskDlgLblStatusKey						kIZPUIStringPrefix "kIZPCTaskDlgLblStatusKey"
#define kIZPCTaskDlgLblCategoryKey						kIZPUIStringPrefix "kIZPCTaskDlgLblCategoryKey"
#define kIZPCTaskDlgLblAsgnToUsrKey						kIZPUIStringPrefix "kIZPCTaskDlgLblAsgnToUsrKey"
#define kIZPCTaskDlgLblCmntKey							kIZPUIStringPrefix "kIZPCTaskDlgLblCmntKey"

	// Status Update dialog
#define kIZPUpdateVersionStatusDialogTitleKey			kIZPUIStringPrefix "kIZPUpdateVersionStatusDialogTitleKey"
#define kIZPUpdateVersionStatusDlgLblDescriptionKey		kIZPUIStringPrefix "kIZPUpdateVersionStatusDlgLblDescriptionKey"
#define kIZPUpdateVersionStatusDlgLblStatusKey			kIZPUIStringPrefix "kIZPUpdateVersionStatusDlgLblStatusKey"
#define kIZPUpdateVersionStatusDlgOkBtnKey				kIZPUIStringPrefix "kIZPUpdateVersionStatusDlgOkBtnKey"

	//Rename Asset dialog
#define kIZPRenameAssetDialogTitleKey					kIZPUIStringPrefix "kIZPRenameAssetDialogTitleKey"
#define kIZPRenameAssetDlgLblNewNameKey					kIZPUIStringPrefix "kIZPRenameAssetDlgLblNewNameKey"

	// Tasks Panel
#define kIZPTasksPanelTitleKey							kIZPUIStringPrefix "kIZPTasksPanelTitleKey"
#define kIZPTasksPanelLblDescriptionKey					kIZPCTaskDlgLblDescriptionKey
#define kIZPTasksPanelLblSpreadKey						kIZPCTaskDlgLblSpreadKey
#define kIZPTasksPanelLblPageKey						kIZPCTaskDlgLblPageKey
#define kIZPTasksPanelLblStatusKey						kIZPCTaskDlgLblStatusKey
#define kIZPTasksPanelLblCategoryKey					kIZPCTaskDlgLblCategoryKey
#define kIZPTasksPanelLblAsgnToUsrKey					kIZPCTaskDlgLblAsgnToUsrKey

	// Tasks List Column Headers
#define kIZPTasksColHeaderSubjectKey					kIZPUIStringPrefix"kIZPTasksColHeaderSubjectKey"
#define kIZPTasksColHeaderStatusKey						kIZPUIStringPrefix"kIZPTasksColHeaderStatusKey"
#define kIZPTasksColHeaderAssignedToKey					kIZPUIStringPrefix"kIZPTasksColHeaderAssignedToKey"
#define kIZPTasksColHeaderDocumentKey					kIZPUIStringPrefix"kIZPTasksColHeaderDocumentKey"
#define kIZPTasksColHeaderCategoryKey					kIZPUIStringPrefix"kIZPTasksColHeaderCategoryKey"
#define kIZPTasksColHeaderSpreadKey						kIZPUIStringPrefix"kIZPTasksColHeaderSpreadKey"
#define kIZPTasksColHeaderPageKey						kIZPUIStringPrefix"kIZPTasksColHeaderPageKey"
#define kIZPTasksColHeaderUpdatedByKey					kIZPUIStringPrefix"kIZPTasksColHeaderUpdatedByKey"
#define kIZPTasksColHeaderUpdatedOnKey					kIZPUIStringPrefix"kIZPTasksColHeaderUpdatedOnKey"
#define kIZPTasksColHeaderCreatorKey					kIZPUIStringPrefix"kIZPTasksColHeaderCreatorKey"
#define kIZPTasksColHeaderCreationDateKey				kIZPUIStringPrefix"kIZPTasksColHeaderCreationDateKey"

	// Assets list column header
#define kIZPAssetsColHeaderLinkTypeIconKey				kIZPUIStringPrefix"kIZPAssetsColHeaderLinkTypeIconKey"
#define kIZpAssetsColHeaderLockIconKey					kIZPUIStringPrefix"kIZpAssetsColHeaderLockIconKey"
#define kIZPAssetsColHeaderSaveKey						kIZPUIStringPrefix"kIZPAssetsColHeaderSaveKey"
#define kIZPAssetsColHeaderDeleteKey					kIZPUIStringPrefix"kIZPAssetsColHeaderDeleteKey"
#define kIZPAssetsColHeaderAssetNameKey					kIZPUIStringPrefix"kIZPAssetsColHeaderAssetNameKey"
#define kIZPAssetsColHeaderDocumentKey					kIZPUIStringPrefix"kIZPAssetsColHeaderDocumentKey"
#define kIZPAssetsColHeaderVersionNumKey				kIZPUIStringPrefix"kIZPAssetsColHeaderVersionNumKey"
#define kIZpAssetsColHeaderVersionAuthorKey				kIZPUIStringPrefix"kIZpAssetsColHeaderVersionAuthorKey"
#define kIZPAssetsColHeaderVersionDateKey				kIZPUIStringPrefix"kIZPAssetsColHeaderVersionDateKey"
#define kIZPAssetsColHeaderLocalStateKey				kIZPUIStringPrefix"kIZPAssetsColHeaderLocalStateKey"
#define kIZPAssetsColHeaderDescriptionKey				kIZPUIStringPrefix"kIZPAssetsColHeaderDescriptionKey"
	// Assets list column header context
#define kIZpAssetsColHeaderMenuLinkTypeIconKey			kIZPUIStringPrefix"kIZpAssetsColHeaderMenuLinkTypeIconKey"
#define kIZpAssetsColHeaderMenuLockIconKey				kIZPUIStringPrefix"kIZpAssetsColHeaderMenuLockIconKey"
#define kIZpAssetsColHeaderMenuSaveIconKey				kIZPUIStringPrefix"kIZpAssetsColHeaderMenuSaveIconKey"
#define kIZpAssetsColHeaderMenuDeleteIconKey			kIZPUIStringPrefix"kIZpAssetsColHeaderMenuDeleteIconKey"
#define kIZpAssetsColHeaderMenuVersionNumberIconKey		kIZPUIStringPrefix"kIZpAssetsColHeaderMenuVersionNumberIconKey"
#define kIZpAssetsLocalStateNotFetchedKey				kIZPUIStringPrefix"kIZpAssetsLocalStateNotFetchedKey"
#define kIZpAssetsLocalStateLocallyModifiedKey			kIZPUIStringPrefix"kIZpAssetsLocalStateLocallyModifiedKey"
#define kIZpAssetsLocalStateUptoDateKey					kIZPUIStringPrefix"kIZpAssetsLocalStateUptoDateKey"
#define kIZpAssetsLocalStateOutOfDateKey				kIZPUIStringPrefix"kIZpAssetsLocalStateOutOfDateKey"

//////#define kIZPExportAssetDialogStatusKey					kIZPUIStringPrefix "kIZPExportAssetDialogStatusKey"

	// Assignment Dialog
//////#define kIZPAssignmentDialogCaptionKey					kIZPUIStringPrefix "kIZPAssignmentDialogCaptionKey"
//////#define kIZPAssignmentDialogGroupTitleKey				kIZPUIStringPrefix "kIZPAssignmentDialogGroupTitleKey"
//////#define kIZPAssignmentDialogNameKey						kIZPUIStringPrefix "kIZPAssignmentDialogNameKey"
//////#define kIZPAssignmentDialogAssignedToKey				kIZPUIStringPrefix "kIZPAssignmentDialogAssignedToKey"
//////#define kIZPAssignmentDialogColorKey					kIZPUIStringPrefix "kIZPAssignmentDialogColorKey"
//////#define kIZPAssignmentDialogStatusKey					kIZPUIStringPrefix "kIZPAssignmentDialogStatusKey"
//////#define kIZPAssignmentDialogShelveKey					kIZPUIStringPrefix "kIZPAssignmentDialogShelveKey"
//////#define kIZPAssignmentDialogTitleKey					kIZPUIStringPrefix "kIZPAssignmentDialogTitleKey"

	// Error Strings
//////#define kIZPAssetErrorDataKey							kIZPUIStringPrefix "kIZPAssetErrorDataKey"
//////#define kIZPLockInfoErrorKey							kIZPUIStringPrefix "kIZPLockInfoErrorKey"
//////#define kIZPLoginErrorKey								kIZPUIStringPrefix "kIZPLoginErrorKey"

	// Alerts
#define kIZPSaveAlertKey								kIZPUIStringPrefix "kIZPSaveAlertKey"
#define kIZPRevertAlertKey								kIZPUIStringPrefix "kIZPRevertAlertKey"
#define kIZPManualDeleteAssetAlertKey					kIZPUIStringPrefix "kIZPManualDeleteAssetAlertKey"
//////#define kIZPConflictAlertKey							kIZPUIStringPrefix "kIZPConflictAlertKey"
//////#define kIZPConflictAlertContinuationKey				kIZPUIStringPrefix "kIZPConflictAlertContinuationKey"
//////#define kIZPLiveEditConflictAlertKey					kIZPUIStringPrefix "kIZPLiveEditConflictAlertKey"
//////#define kIZPLiveEditConflictAlertContinuationKey		kIZPUIStringPrefix "kIZPLiveEditConflictAlertContinuationKey"

//#define kIZPAssignmentsPanelTitleKey					kIZPUIStringPrefix "kIZPAssignmentsPanelTitleKey"

	// Misc
//////#define kIZPDocServiceProviderKey						kIZPUIStringPrefix "kIZPDocServiceProviderKey"
//////#define kIZPProgressBarOpenDocumentKey					kIZPUIStringPrefix "kIZPProgressBarOpenDocumentKey"
//////#define kIZPProgressBarSavingDocumentKey				kIZPUIStringPrefix "kIZPProgressBarSavingDocumentKey"
//////#define kIZPDocAlreadyExistKey							kIZPUIStringPrefix "kIZPDocAlreadyExistKey"
//////#define kIZPErrorOccuredWhileDownlodingVersionKey		kIZPUIStringPrefix "kIZPErrorOccuredWhileDownlodingVersionKey"
//////#define kIZPErrorOoccuredDuringCheckoutOfStoryKey		kIZPUIStringPrefix "kIZPErrorOoccuredDuringCheckoutOfStoryKey"
//////#define kIZPErrorCouldNotRetrieveAssetPropertiesKey		kIZPUIStringPrefix "kIZPErrorCouldNotRetrieveAssetPropertiesKey"
//////#define kIZPProgressBarUpdateDocumentKey				kIZPUIStringPrefix "kIZPProgressBarUpdateDocumentKey"
//////#define kIZPProgressBarUpdateLinksKey					kIZPUIStringPrefix "kIZPProgressBarUpdateLinksKey"
//////#define kIZPProgressBarUploadDocumentKey				kIZPUIStringPrefix "kIZPProgressBarUploadDocumentKey"
//////#define kIZPProgressBarUpdatePagesKey					kIZPUIStringPrefix "kIZPProgressBarUpdatePagesKey"
//////#define kIZPProgressBarUpdatePdfKey						kIZPUIStringPrefix "kIZPProgressBarUpdatePdfKey"
//////#define kIZPProgressBarCheckFilesKey					kIZPUIStringPrefix "kIZPProgressBarCheckFilesKey"
#define kIZPSaveVersionBtnKey								kIZPUIStringPrefix "kIZPSaveVersionBtnKey"
#define kIZPRevertBtnKey									kIZPUIStringPrefix "kIZPRevertBtnKey"
#define kIZPRevertToVersionBtnKey							kIZPUIStringPrefix "kIZPRevertToVersionBtnKey"

#define kIZPPopupItemAllKey									kIZPUIStringPrefix "kIZPPopupItemAllKey"
#define kIZPPopupItemAnyKey									kIZPUIStringPrefix "kIZPPopupItemAnyKey"
#define kIZPPopupItemNoneKey								kIZPUIStringPrefix "kIZPPopupItemNoneKey"
#define kIZPPopupItemSelfKey								kIZPUIStringPrefix "kIZPPopupItemSelfKey"
#define kIZPPopupItemEmptyKey								kIZPUIStringPrefix "kIZPPopupItemEmptyKey"

#if defined(InDnCS5) || defined(InDnCS5_5)
#define kIZPAssetConversionKey								kIZPUIStringPrefix "kIZPAssetConversionKey"
#define kIZPExportAssignmentKey								kIZPUIStringPrefix "kIZPExportAssignmentKey"
#define kIZPExportStoriesKey								kIZPUIStringPrefix "kIZPExportStoriesKey"
#define kIZPUnAssignedICContent								"UnAssignedIC"
#endif

#endif //_h_IZPUI_StringKeys_
