//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Plugin/IZPUI_ClassDefID.h $
//	$Revision: 3510 $
//	$Date: 2011-07-19 12:20:33 +0200 (Tue, 19 Jul 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 2010 Mar 19
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description: 
//========================================================================================

#ifndef __IZPIDUI_ClassDefID_h__
#define __IZPIDUI_ClassDefID_h__

// ClassIDs:
	//General
DECLARE_PMID(kClassIDSpace, kZPUIStartupShutdownBoss,														kIZPUIPrefix + 0 )
DECLARE_PMID(kClassIDSpace, kZPUIAppActionComponentBoss,													kIZPUIPrefix + 1 )
DECLARE_PMID(kClassIDSpace, kZPUIAMActionComponentBoss,														kIZPUIPrefix + 2 )
DECLARE_PMID(kClassIDSpace, kZPUIActionFilterBoss,															kIZPUIPrefix + 3 )
DECLARE_PMID(kClassIDSpace, kZPUILoginDlgServiceBoss,														kIZPUIPrefix + 4 )
DECLARE_PMID(kClassIDSpace, kZPUIIteratorRegisterBoss,														kIZPUIPrefix + 5 )
DECLARE_PMID(kClassIDSpace, kZPUIAMAddLayerToAssignmentActionComponentBoss,									kIZPUIPrefix + 6 )
DECLARE_PMID(kClassIDSpace, kZPUIAMAddAllStoriesToAssignmentActionComponentBoss,							kIZPUIPrefix + 7 )
DECLARE_PMID(kClassIDSpace, kZPUIAMAddSelToAssignmentActionComponentBoss,									kIZPUIPrefix + 8 )
DECLARE_PMID(kClassIDSpace, kZPUIAMAddSelToAssignmentLayoutContextActionComponentBoss,						kIZPUIPrefix + 9 )
DECLARE_PMID(kClassIDSpace, kZPUIAMAddToAssignmentActionComponentBoss,										kIZPUIPrefix + 10 )
DECLARE_PMID(kClassIDSpace, kZPTVHeaderActionComponentBoss,													kIZPUIPrefix + 11 )
DECLARE_PMID(kClassIDSpace, kZPUITasksActionComponentBoss,													kIZPUIPrefix + 12 )
DECLARE_PMID(kClassIDSpace, kZPUIDebugActionComponentBoss,													kIZPUIPrefix + 13 )
DECLARE_PMID(kClassIDSpace, kZPUISuppressActionsBoss,														kIZPUIPrefix + 14 )
DECLARE_PMID(kClassIDSpace, kZPAssetsTVActionComponentBoss,													kIZPUIPrefix + 15 )

DECLARE_PMID(kClassIDSpace, kZPUIConversionProviderBoss,													kIZPUIPrefix + 20 )

	//Asset Management
DECLARE_PMID(kClassIDSpace, kZPAMUIServiceBoss,																kIZPUIPrefix + 31 )
DECLARE_PMID(kClassIDSpace, kZPAMUISaveAlertServiceBoss,													kIZPUIPrefix + 32 )
DECLARE_PMID(kClassIDSpace, kZPAMUISaveAsDialogServiceBoss,													kIZPUIPrefix + 33 )
DECLARE_PMID(kClassIDSpace, kZPAMUIGetVersionCommentsServiceBoss,											kIZPUIPrefix + 34 )
DECLARE_PMID(kClassIDSpace, kZPAMUIRevertServiceBoss,														kIZPUIPrefix + 35 )

DECLARE_PMID(kClassIDSpace, kZPAMUINewAssignmentServiceBoss,												kIZPUIPrefix + 40 )
DECLARE_PMID(kClassIDSpace, kZPAMUIUpdateVersionStatusServiceBoss,											kIZPUIPrefix + 41 )

	//Dialogs
DECLARE_PMID(kClassIDSpace, kZPUILoginDialogBoss,															kIZPUIPrefix + 70 )
DECLARE_PMID(kClassIDSpace, kZPSaveToRepositoryDialogBoss,													kIZPUIPrefix + 71 )
DECLARE_PMID(kClassIDSpace, kZPAlertDialogBoss,																kIZPUIPrefix + 72 )
DECLARE_PMID(kClassIDSpace, kZPPrefsDialogBoss,																kIZPUIPrefix + 73 )
DECLARE_PMID(kClassIDSpace, kZPCreateTaskDialogBoss,														kIZPUIPrefix + 74 )
DECLARE_PMID(kClassIDSpace, kZPUpdateVersionStatusDialogBoss,												kIZPUIPrefix + 75 )
DECLARE_PMID(kClassIDSpace, kZPRenameAssetDialogBoss,														kIZPUIPrefix + 76 )

	//Panels
DECLARE_PMID(kClassIDSpace, kZPUIAssetsPanelWidgetBoss,														kIZPUIPrefix + 80 )
DECLARE_PMID(kClassIDSpace, kZPUITasksPanelWidgetBoss,														kIZPUIPrefix + 81 )
DECLARE_PMID(kClassIDSpace, kZPUITasksHistoryPanelBoss,														kIZPUIPrefix + 82 )

	//Widgets
DECLARE_PMID(kClassIDSpace, kZPUIDBObjPopupBoss,															kIZPUIPrefix + 100 )
DECLARE_PMID(kClassIDSpace, kZPUIPopTitleListBoss,															kIZPUIPrefix + 101 )
DECLARE_PMID(kClassIDSpace, kZPUIPopEditionsListBoss,														kIZPUIPrefix + 102 )
DECLARE_PMID(kClassIDSpace, kZPUIPopTitleStatusListBoss,													kIZPUIPrefix + 103 )
DECLARE_PMID(kClassIDSpace, kZPUIPopTitleUsersListBoss,														kIZPUIPrefix + 104 )
DECLARE_PMID(kClassIDSpace, kZPUIPopEditionAssetsListBoss,													kIZPUIPrefix + 105 )
DECLARE_PMID(kClassIDSpace, kZPUIPopTaskStatusListBoss,														kIZPUIPrefix + 106 )
DECLARE_PMID(kClassIDSpace, kZPUIPopTaskCategoryListBoss,													kIZPUIPrefix + 107 )
DECLARE_PMID(kClassIDSpace, kZPUIDBObjPopupWithFixedValuesBoss,												kIZPUIPrefix + 108 )

DECLARE_PMID(kClassIDSpace, kZPUIDBObjTVNodeWidgetBoss,														kIZPUIPrefix + 110 )
DECLARE_PMID(kClassIDSpace, kZPTVHeaderWidgetBoss,															kIZPUIPrefix + 111 )
	//TV column dynamic widgets
DECLARE_PMID(kClassIDSpace, kZPTVColumnRollOverIconButtonWidgetBoss,										kIZPUIPrefix + 112 )

	//Asset Panel Controls
DECLARE_PMID(kClassIDSpace, kZPDocumentPreviewWidgetBoss,													kIZPUIPrefix + 120 )
DECLARE_PMID(kClassIDSpace, kZPAssetsTreeViewWidgetBoss,													kIZPUIPrefix + 121 )
DECLARE_PMID(kClassIDSpace, kZPAssetsTVStatusNodeWidgetBoss,												kIZPUIPrefix + 122 )
DECLARE_PMID(kClassIDSpace, kZPAssetsTVAssetNodeWidgetBoss,													kIZPUIPrefix + 123 )
//DECLARE_PMID(kClassIDSpace, kZPAssetsTVAssetNodeDeleteRollOverIconBtnWidgetBoss,							kIZPUIPrefix + 124 )
DECLARE_PMID(kClassIDSpace, kZPAssetsTVAssetNodeSaveRollOverIconBtnWidgetBoss,								kIZPUIPrefix + 125 )

	//Tasks Panel Controls
DECLARE_PMID(kClassIDSpace, kZPTasksListTreeViewWidgetBoss,													kIZPUIPrefix + 135 )
DECLARE_PMID(kClassIDSpace, kZPTasksTVNodeWidgetBoss,														kIZPUIPrefix + 136 )

	//Other common controls
DECLARE_PMID(kClassIDSpace, kZPExpanderRollOverIconBtnWidgetBoss,											kIZPUIPrefix + 137 )
	//Dialogs

	//Panels
	//Drag n Drop
DECLARE_PMID(kClassIDSpace, kZPDnDCustomFlavorHelperBoss,													kIZPUIPrefix + 140 )
DECLARE_PMID(kClassIDSpace, kZPDnDCustomFlavorHandlerBoss,													kIZPUIPrefix + 141 )

#pragma mark -
// InterfaceIDs:
	//General
DECLARE_PMID(kInterfaceIDSpace, IID_IZPUIDIALOGFACADE,														kIZPUIPrefix + 0 )

DECLARE_PMID(kInterfaceIDSpace, IID_IZPUICURRDOCSELCHANGEOBSERVER,											kIZPUIPrefix + 10 )

	//Widgets
DECLARE_PMID(kInterfaceIDSpace, IID_IZPOLDEVENTHANDLER,														kIZPUIPrefix + 20 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPASSETSTVDATAMODEL,													kIZPUIPrefix + 21 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPUISELECTEDID,														kIZPUIPrefix + 22 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPDBOBJPOPUPCONTROLLER,												kIZPUIPrefix + 23 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPUIOBJIDSSTRINGLISTDATA,												kIZPUIPrefix + 24 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPTITLEIDSTRINGDATA,													kIZPUIPrefix + 25 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPUITVNODEDATA,														kIZPUIPrefix + 26 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPUITVNODEWIDGETMGR,													kIZPUIPrefix + 27 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPUIACTIONFILTERDATA,													kIZPUIPrefix + 28 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPISNEWVERSION,														kIZPUIPrefix + 29 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPUIINTDATA,															kIZPUIPrefix + 30 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPTASKSTVDATAMODEL,													kIZPUIPrefix + 31 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPEDITIONIDSTRINGDATA,													kIZPUIPrefix + 32 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPPOPFIXEDITEMSTRINGDATA,												kIZPUIPrefix + 33 )

DECLARE_PMID(kInterfaceIDSpace, IID_IZPTVHEADERCONTROLLER,													kIZPUIPrefix + 40 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPTVCOLUMNSINFO,														kIZPUIPrefix + 41 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPTVCOLUMNSPREF,														kIZPUIPrefix + 42 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPINTDATA,																kIZPUIPrefix + 43 )

DECLARE_PMID(kInterfaceIDSpace, IID_IZPACTIONCOMPONENTDELEGATE,												kIZPUIPrefix + 50 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPDYNAMICMENUDELEGATE,													kIZPUIPrefix + 51 )

DECLARE_PMID(kInterfaceIDSpace, IID_IZPTASKSPANELHELPER,													kIZPUIPrefix + 70 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPASSETSPANELHELPER,													kIZPUIPrefix + 71 )

DECLARE_PMID(kInterfaceIDSpace, IID_IZPUIHISTORYPANELCONTROLLER,											kIZPUIPrefix + 80 )

DECLARE_PMID(kInterfaceIDSpace, IID_IZPDISCLOSURETRIANGLEOPTIONS,											kIZPUIPrefix + 90 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZPISEXPANDED,															kIZPUIPrefix + 91 )

	//Dnd
DECLARE_PMID(kInterfaceIDSpace, IID_IZPUIASSETDNDOBSERVER,													kIZPUIPrefix + 100 )

	//Patches
DECLARE_PMID(kInterfaceIDSpace, IID_IZPUISTORYCHECKOUTEVENTWATCHER,											kIZPUIPrefix + 120 )

#pragma mark -
// ImplementationIDs:
	//General
DECLARE_PMID(kImplementationIDSpace, kZPUIStartupShutdownImpl,												kIZPUIPrefix + 0 )
DECLARE_PMID(kImplementationIDSpace, kZPAppActionComponentImpl,												kIZPUIPrefix + 1 )
DECLARE_PMID(kImplementationIDSpace, kZPAMActionComponentImpl,												kIZPUIPrefix + 2 )
DECLARE_PMID(kImplementationIDSpace, kZPUIActionFilterImpl,													kIZPUIPrefix + 3 )
DECLARE_PMID(kImplementationIDSpace, kZPUIActionFilterDataImpl,												kIZPUIPrefix + 4 )
DECLARE_PMID(kImplementationIDSpace, kZPUIContentIteratorRegisterImpl,										kIZPUIPrefix + 5 )
DECLARE_PMID(kImplementationIDSpace, kZPAMAddLayerToAssignmentActionComponentImpl,							kIZPUIPrefix + 6 )
DECLARE_PMID(kImplementationIDSpace, kZPAMAddAllStoriesToAssignmentActionComponentImpl,						kIZPUIPrefix + 7 )
DECLARE_PMID(kImplementationIDSpace, kZPAMAddSelToAssignmentActionComponentImpl,							kIZPUIPrefix + 8 )
DECLARE_PMID(kImplementationIDSpace, kZPTVHeaderActionComponentImpl,										kIZPUIPrefix + 9 )
DECLARE_PMID(kImplementationIDSpace, kZPTasksActionComponentImpl,											kIZPUIPrefix + 10 )
DECLARE_PMID(kImplementationIDSpace, kZPDebugActionComponentImpl,											kIZPUIPrefix + 11 )
DECLARE_PMID(kImplementationIDSpace, kZPUISuppressedActionsImpl,											kIZPUIPrefix + 12 )
DECLARE_PMID(kImplementationIDSpace, kZPAddLayerToAssignmentDynamicMenuImpl,								kIZPUIPrefix + 13 )

//	Export Providers
DECLARE_PMID(kImplementationIDSpace, kZPICExportLayerStoriesImpl,											kIZPUIPrefix + 15 )
DECLARE_PMID(kImplementationIDSpace, kZPICExportAllStoriesImpl,												kIZPUIPrefix + 16 )
DECLARE_PMID(kImplementationIDSpace, kZPICExportSelectionStoriesImpl,										kIZPUIPrefix + 17 )

	//Utils
DECLARE_PMID(kImplementationIDSpace, kZPUIDialogFacadeImpl,													kIZPUIPrefix + 20 )

	//Asset Management - UI providers
DECLARE_PMID(kImplementationIDSpace, kZPAMUIDocumentServiceProviderImpl,									kIZPUIPrefix + 30 )
DECLARE_PMID(kImplementationIDSpace, kZPAMUIAssignmentServiceProviderImpl,									kIZPUIPrefix + 31 )
DECLARE_PMID(kImplementationIDSpace, kZPAMUILinkedStoryServiceProviderImpl,									kIZPUIPrefix + 32 )
	//Asset Management - UI services
DECLARE_PMID(kImplementationIDSpace, kZPAMUISaveAlertServiceImpl,											kIZPUIPrefix + 33 )
DECLARE_PMID(kImplementationIDSpace, kZPAMUISaveAsDialogServiceImpl,										kIZPUIPrefix + 34 )
DECLARE_PMID(kImplementationIDSpace, kZPAMUIGetVersionCommentsServiceImpl,									kIZPUIPrefix + 35 )
DECLARE_PMID(kImplementationIDSpace, kZPAMUIRevertServiceImpl,												kIZPUIPrefix + 36 )

DECLARE_PMID(kImplementationIDSpace, kZPAMUINewAssignmentServiceImpl,										kIZPUIPrefix + 40 )
DECLARE_PMID(kImplementationIDSpace, kZPAMUIUpdateVersionStatusServiceImpl,									kIZPUIPrefix + 41 )
//UI
//Dialogs
	//Login
DECLARE_PMID(kImplementationIDSpace, kZPUILoginDialogControllerImpl,										kIZPUIPrefix + 50 )
DECLARE_PMID(kImplementationIDSpace, kZPUILoginDialogObserverImpl,											kIZPUIPrefix + 51 )
DECLARE_PMID(kImplementationIDSpace, kZPLoginDlgCreatorImpl,												kIZPUIPrefix + 52 )
	//Save to Repository
DECLARE_PMID(kImplementationIDSpace, kZPUISaveToRepDialogControllerImpl,									kIZPUIPrefix + 53 )
DECLARE_PMID(kImplementationIDSpace, kZPUISaveToRepDialogObserverImpl,										kIZPUIPrefix + 54 )
	//Alert Dialog
DECLARE_PMID(kImplementationIDSpace, kZPUIAlertDialogControllerImpl,										kIZPUIPrefix + 55 )
DECLARE_PMID(kImplementationIDSpace, kZPUIAlertDialogObserverImpl,											kIZPUIPrefix + 56 )
	//Prefs Dialog
DECLARE_PMID(kImplementationIDSpace, kZPUIPrefsPanelCreatorImpl,											kIZPUIPrefix + 59 )
DECLARE_PMID(kImplementationIDSpace, kZPUIPrefsDialogControllerImpl,										kIZPUIPrefix + 60 )
	//Tasks Dialog
DECLARE_PMID(kImplementationIDSpace, kZPCreateTaskDialogControllerImpl,										kIZPUIPrefix + 65 )
DECLARE_PMID(kImplementationIDSpace, kZPCreateTaskDialogObserverImpl,										kIZPUIPrefix + 66 )
	//Status update Dialog
DECLARE_PMID(kImplementationIDSpace, kZPUpdateVersionStatusDialogControllerImpl,							kIZPUIPrefix + 70 )
DECLARE_PMID(kImplementationIDSpace, kZPUpdateVersionStatusDialogObserverImpl,								kIZPUIPrefix + 71 )
//Panels
	//Assets
DECLARE_PMID(kImplementationIDSpace, kZPAssetsPanelObserverImpl,											kIZPUIPrefix + 80 )
DECLARE_PMID(kImplementationIDSpace, kZPAssetsPanelControlViewImpl,											kIZPUIPrefix + 81 )
DECLARE_PMID(kImplementationIDSpace, kZPAssetsPanelHelperImpl,												kIZPUIPrefix + 82 )
	//Tasks
DECLARE_PMID(kImplementationIDSpace, kZPTasksPanelControlViewImpl,											kIZPUIPrefix + 85 )
DECLARE_PMID(kImplementationIDSpace, kZPTasksPanelObserverImpl,												kIZPUIPrefix + 86 )
DECLARE_PMID(kImplementationIDSpace, kZPTasksPanelHelperImpl,												kIZPUIPrefix + 87 )
	//Task History
DECLARE_PMID(kImplementationIDSpace, kZPUITaskHistoryPanelControllerImpl,									kIZPUIPrefix + 95 )
DECLARE_PMID(kImplementationIDSpace, kZPUITaskHistoryPanelObserverImpl,										kIZPUIPrefix + 96 )
	//Rename Asset
DECLARE_PMID(kImplementationIDSpace, kZPRenameAssetDialogControllerImpl,									kIZPUIPrefix + 100 )
DECLARE_PMID(kImplementationIDSpace, kZPRenameAssetDialogObserverImpl,										kIZPUIPrefix + 101 )
//Widgets
	//Popups
DECLARE_PMID(kImplementationIDSpace, kZPEditionPopupControllerImpl,											kIZPUIPrefix + 150 )
DECLARE_PMID(kImplementationIDSpace, kZPEditionPopupObserverImpl,											kIZPUIPrefix + 151 )
DECLARE_PMID(kImplementationIDSpace, kZPTitlePopupControllerImpl,											kIZPUIPrefix + 152 )
DECLARE_PMID(kImplementationIDSpace, kZPTitlePopupObserverImpl,												kIZPUIPrefix + 153 )
DECLARE_PMID(kImplementationIDSpace, kZPTitleStatusPopupControllerImpl,										kIZPUIPrefix + 154 )
DECLARE_PMID(kImplementationIDSpace, kZPTitleStatusPopupObserverImpl,										kIZPUIPrefix + 155 )
DECLARE_PMID(kImplementationIDSpace, kZPTitleUsersPopupObserverImpl,										kIZPUIPrefix + 156 )
DECLARE_PMID(kImplementationIDSpace, kZPTitleUsersPopupControllerImpl,										kIZPUIPrefix + 157 )
DECLARE_PMID(kImplementationIDSpace, kZPEditionAssetsPopupObserverImpl,										kIZPUIPrefix + 158 )
DECLARE_PMID(kImplementationIDSpace, kZPEditionAssetsPopupControllerImpl,									kIZPUIPrefix + 159 )
DECLARE_PMID(kImplementationIDSpace, kZPTaskStatusPopupObserverImpl,										kIZPUIPrefix + 160 )
DECLARE_PMID(kImplementationIDSpace, kZPTaskStatusPopupControllerImpl,										kIZPUIPrefix + 161 )
DECLARE_PMID(kImplementationIDSpace, kZPTaskCategoryPopupObserverImpl,										kIZPUIPrefix + 162 )
DECLARE_PMID(kImplementationIDSpace, kZPTaskCategoryPopupControllerImpl,									kIZPUIPrefix + 163 )
	//Assets TV
DECLARE_PMID(kImplementationIDSpace, kZPAssetsTVDataModelImpl,												kIZPUIPrefix + 170 )
DECLARE_PMID(kImplementationIDSpace, kZPAssetsTVHierarchyAdapterImpl,										kIZPUIPrefix + 171 )
DECLARE_PMID(kImplementationIDSpace, kZPAssetsTVWidgetManagerImpl,											kIZPUIPrefix + 172 )
DECLARE_PMID(kImplementationIDSpace, kZPAssetsTVObserverImpl,												kIZPUIPrefix + 173 )
DECLARE_PMID(kImplementationIDSpace, kZPAssetsTVActionComponentImpl,										kIZPUIPrefix + 174 )
		//Asset Node
DECLARE_PMID(kImplementationIDSpace, kZPAssetsTVAssetNodeDataImpl,											kIZPUIPrefix + 180 )
DECLARE_PMID(kImplementationIDSpace, kZPAssetsTVAssetNodeWidgetMgrImpl,										kIZPUIPrefix + 181 )
DECLARE_PMID(kImplementationIDSpace, kZPAssetsTVAssetNodeObserverImpl,										kIZPUIPrefix + 182 )
DECLARE_PMID(kImplementationIDSpace, kZPAssetsTVAssetNodeEHImpl,											kIZPUIPrefix + 183 )
DECLARE_PMID(kImplementationIDSpace, kZPAssetsTVAssetNodeDragDropSourceImpl,								kIZPUIPrefix + 184 )
DECLARE_PMID(kImplementationIDSpace, kZPAssetsTVNodeDeleteBtnObserverImpl,									kIZPUIPrefix + 185 )
DECLARE_PMID(kImplementationIDSpace, kZPAssetsTVNodeSaveBtnObserverImpl,									kIZPUIPrefix + 186 )
		//Status Node
DECLARE_PMID(kImplementationIDSpace, kZPAssetsTVStatusNodeWidgetMgrImpl,									kIZPUIPrefix + 190 )
DECLARE_PMID(kImplementationIDSpace, kZPAssetsTVStatusNodeDataImpl,											kIZPUIPrefix + 191 )
DECLARE_PMID(kImplementationIDSpace, kZPAssetsTVStatusNodeEHImpl,											kIZPUIPrefix + 192 )
DECLARE_PMID(kImplementationIDSpace, kZPAssetsTVStatusDropTargetImpl,										kIZPUIPrefix + 193 )
	//Tasks List TV
DECLARE_PMID(kImplementationIDSpace, kZPTasksTVHierarchyAdapterImpl,										kIZPUIPrefix + 195 )
DECLARE_PMID(kImplementationIDSpace, kZPTasksTVWidgetManagerImpl,											kIZPUIPrefix + 196 )
DECLARE_PMID(kImplementationIDSpace, kZPTasksTVObserverImpl,												kIZPUIPrefix + 197 )
DECLARE_PMID(kImplementationIDSpace, kZPTasksTVDataModelImpl,												kIZPUIPrefix + 198 )
		//Task Node
DECLARE_PMID(kImplementationIDSpace, kZPTasksTVNodeDataImpl,												kIZPUIPrefix + 199 )
DECLARE_PMID(kImplementationIDSpace, kZPTasksTVNodeWidgetMgrImpl,											kIZPUIPrefix + 200 )
DECLARE_PMID(kImplementationIDSpace, kZPTasksTVNodeEHImpl,													kIZPUIPrefix + 201 )
	//TreeView Common impls
DECLARE_PMID(kImplementationIDSpace, kZPTreeViewMgrImpl,													kIZPUIPrefix + 202 )
	//TreeView Header
DECLARE_PMID(kImplementationIDSpace, kZPTVHeaderControllerImpl,												kIZPUIPrefix + 205 )
DECLARE_PMID(kImplementationIDSpace, kZPTVColumnsInfoImpl,													kIZPUIPrefix + 206 )
DECLARE_PMID(kImplementationIDSpace, kZPTVColumnsPrefPersistImpl,											kIZPUIPrefix + 207 )
DECLARE_PMID(kImplementationIDSpace, kZPTVHeaderObserverImpl,												kIZPUIPrefix + 208 )
DECLARE_PMID(kImplementationIDSpace, kZPTVHeaderEHImpl,														kIZPUIPrefix + 209 )

DECLARE_PMID(kImplementationIDSpace, kZPTVColumnRollOverIconButtonObserverImpl,								kIZPUIPrefix + 215 )
	//Document Preview
DECLARE_PMID(kImplementationIDSpace, kZPDocumentPreviewControlViewImpl,										kIZPUIPrefix + 220 )
	//Expander button, disclosure TriAngle
DECLARE_PMID(kImplementationIDSpace, kZPDisclosureTriAngleOptionsImpl,										kIZPUIPrefix + 221 )
DECLARE_PMID(kImplementationIDSpace, kZPDisclosureTriAngleObserverImpl,										kIZPUIPrefix + 222 )

//Others
DECLARE_PMID(kImplementationIDSpace, kZPDynamicMenuImpl,													kIZPUIPrefix + 225 )
DECLARE_PMID(kImplementationIDSpace, kZPTVHeaderDynamicMenuImpl,											kIZPUIPrefix + 226 )
DECLARE_PMID(kImplementationIDSpace, kZPDnDCustomFlavorHelperImpl,											kIZPUIPrefix + 227 )
DECLARE_PMID(kImplementationIDSpace, kZPDnDDataExchangeHandlerImpl,											kIZPUIPrefix + 228 )
DECLARE_PMID(kImplementationIDSpace, kZPUIAssetDnDObserverImpl,												kIZPUIPrefix + 229 )
DECLARE_PMID(kImplementationIDSpace, kZPUICurrDocSelChangeObserverImpl,										kIZPUIPrefix + 230 )
DECLARE_PMID(kImplementationIDSpace, kZPUIStoryCheckoutEventWatcherImpl,									kIZPUIPrefix + 231 )
//Alias
DECLARE_PMID(kImplementationIDSpace, kZPDataIDStringData1Impl,												kIZPUIPrefix + 240 )
DECLARE_PMID(kImplementationIDSpace, kZPStringListControlDataImpl,											kIZPUIPrefix + 241 )

#endif // __IZPUIID_h__

