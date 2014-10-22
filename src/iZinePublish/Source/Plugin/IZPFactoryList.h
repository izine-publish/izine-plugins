//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Plugin/IZPFactoryList.h $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 2010 Mar 19
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description: 
//========================================================================================
#ifndef _h_IZPFactoryList_
#define _h_IZPFactoryList_
#pragma once

#if defined(InDnCS5) || defined(InDnCS5_5)	//TODO: CS5 Porting
#include "IZPFactoryListCS5_Auto.h"	
#else
#include "IZPFactoryList_Auto.h"	//This file is automatically updated by a macro "UpdateIntefaceFactories"
#endif

REGISTER_PMINTERFACE(CZPStringListData_<IStringListData>, kZPStringListDataImpl)

//REGISTER_PMINTERFACE(IZPActionComponent,							kIZPActionComponentImpl)
//REGISTER_PMINTERFACE(IZPScriptProvider,								kIZPScriptProviderImpl)
//REGISTER_PMINTERFACE(IZPAMSystemProvider,							kIZPAMSystemProviderImpl)
//REGISTER_PMINTERFACE(IZPAMServiceProvider,							kIZPAMServiceProviderImpl)
//REGISTER_PMINTERFACE(IZPAMUIServiceProvider,						kIZPAMUIServiceProviderImpl)
//REGISTER_PMINTERFACE(IZPAMCheckInService,							kIZPAMCheckInServiceImpl)
//REGISTER_PMINTERFACE(IZPAMCheckOutService,							kIZPAMCheckOutServiceImpl)
//REGISTER_PMINTERFACE(IZPAMCancelCheckOutService,					kIZPAMCancelCheckOutServiceImpl)
//REGISTER_PMINTERFACE(IZPAMGetUniqueAssetLocationService,			kIZPAMGetUniqueAssetLocationServiceImpl)
//REGISTER_PMINTERFACE(IZPAMInitializeService,						kIZPAMInitializeServiceImpl)
//REGISTER_PMINTERFACE(IZPAMUpdateStoredAssetReferenceService,		kIZPAMUpdateStoredAssetReferenceServiceImpl)
//REGISTER_PMINTERFACE(IZPAssetMonitor,								kIZPAssetMonitorImpl)
//REGISTER_PMINTERFACE(IZPSaveAsDialogService,						kIZPAMUISaveAsDialogServiceImpl)
//REGISTER_PMINTERFACE(IZPSaveAlertService,							kIZPAMUISaveAlertServiceImpl)
//REGISTER_PMINTERFACE(IZPPrefsPanelCreator,							kIZPPrefsPanelCreatorImpl)
//REGISTER_PMINTERFACE(IZPPrefsDialogObserver,						kIZPPrefsDialogObserverImpl)
//REGISTER_PMINTERFACE(IZPPrefsDialogController,						kIZPPrefsDialogControllerImpl)
//REGISTER_PMINTERFACE(IZPPrefsDataPersist,							kIZPPrefsDataPersistImpl)
//REGISTER_PMINTERFACE(IZPPrefsData,									kIZPPrefsDataImpl)
//REGISTER_PMINTERFACE(IZPPrefsDataCmd,								kIZPPrefsDataCmdImpl)
//REGISTER_PMINTERFACE(IZPSaveToRepDialogController,					kIZPSaveToRepDialogControllerImpl)
//REGISTER_PMINTERFACE(IZPSaveToRepDialogObserver,					kIZPSaveToRepDialogObserverImpl)
//REGISTER_PMINTERFACE(IZPOpenFromRepDialogController,				kIZPOpenFromRepDialogControllerImpl)
//REGISTER_PMINTERFACE(IZPOpenFromRepDialogObserver,					kIZPOpenFromRepDialogObserverImpl)
//REGISTER_PMINTERFACE(IZPTVDataModel,								kIZPTVDataModelImpl)
//REGISTER_PMINTERFACE(IZPTVObserver,									kIZPTVObserverImpl)
//REGISTER_PMINTERFACE(IZPTVHierarchyAdapter,							kIZPTVHierarchyAdapterImpl)
//REGISTER_PMINTERFACE(IZPTVWidgetMgr,								kIZPTVWidgetMgrImpl)
//REGISTER_PMINTERFACE(DocDataPersist,								kDocDataPersistImpl)
//REGISTER_PMINTERFACE(AssetData,										kAssetDataImpl)
//REGISTER_PMINTERFACE(IZPDocServiceProvider,							kIZPDocServiceProviderImpl)
//REGISTER_PMINTERFACE(IZPAMUIEnablementRules,						kIZPAMUIEnablementRulesImpl)
//REGISTER_PMINTERFACE(IZPOpenDialogData,								kIZPOpenDialogDataImpl)
//REGISTER_PMINTERFACE(IZPSaveDialogData,								kIZPSaveDialogDataImpl)
//REGISTER_PMINTERFACE(IZPAMAfterSaveService,							kIZPAMAfterSaveServiceImpl)
//REGISTER_PMINTERFACE(IZPAMAfterSaveAsService,						kIZPAMAfterSaveAsServiceImpl)
//REGISTER_PMINTERFACE(IZPAMUpdateProxyVersionNumberService,			kIZPAMUpdateProxyVersionNumberServiceImpl)
//REGISTER_PMINTERFACE(IZPAMInvalidateProxyVersionNumberService,		kIZPAMInvalidateProxyVersionNumberServiceImpl)
//REGISTER_PMINTERFACE(IZPAMBatchCheckInService,						kIZPAMBatchCheckInServiceImpl)
//REGISTER_PMINTERFACE(IZPAMAfterCheckInService,						kIZPAMAfterCheckInServiceImpl)
//REGISTER_PMINTERFACE(IZPAMBeforeCheckOutService,					kIZPAMBeforeCheckOutServiceImpl)
//REGISTER_PMINTERFACE(IZPAMAfterCancelCheckOutService,				kIZPAMAfterCancelCheckOutServiceImpl)
//REGISTER_PMINTERFACE(IZPAMUpdateLinkLocationService,				kIZPAMUpdateLinkLocationServiceImpl)
//REGISTER_PMINTERFACE(IZPAMRelinkService,							kIZPAMRelinkServiceImpl)
//REGISTER_PMINTERFACE(IZPAMUnembedService,							kIZPAMUnembedServiceImpl)
//REGISTER_PMINTERFACE(IZPAMRevertService,							kIZPAMRevertServiceImpl)
//REGISTER_PMINTERFACE(IZPAMUIGetVersionCommentsService,				kIZPAMUIGetVersionCommentsServiceImpl)
//REGISTER_PMINTERFACE(IZPAMEnsureLatestVersionService,				kIZPAMEnsureLatestVersionServiceImpl)
//REGISTER_PMINTERFACE(IZPUtils,										kIZPUtilsImpl)
//REGISTER_PMINTERFACE(IZPAMDebugHelper,								kIZPAMDebugHelperImpl)
//REGISTER_PMINTERFACE(IZPLoginDialogObserver,						kIZPLoginDialogObserverImpl)
//REGISTER_PMINTERFACE(IZPLoginDialogController,						kIZPLoginDialogControllerImpl)
//REGISTER_PMINTERFACE(IZPCommentsDialogController,					kIZPCommentsDialogControllerImpl)
//REGISTER_PMINTERFACE(IZPCommentsDialogObserver,						kIZPCommentsDialogObserverImpl)
//REGISTER_PMINTERFACE(IZPDocListObserver,							kIZPDocListObserverImpl)
//REGISTER_PMINTERFACE(IZPHistoryDocListObserver,						kIZPHistoryDocListObserverImpl)
//REGISTER_PMINTERFACE(IZPHistoryPanelObserver,						kIZPHistoryPanelObserverImpl)
//REGISTER_PMINTERFACE(IZPCommentsDialogData,							kIZPCommentsDialogDataImpl)
//REGISTER_PMINTERFACE(IZPSessionData,								kIZPSessionDataImpl)
//REGISTER_PMINTERFACE(IZPAssetsPanelObserver,						kIZPAssetsPanelObserverImpl)
//REGISTER_PMINTERFACE(IZPAssetsPanelDocListObserver,					kIZPAssetsPanelDocListObserverImpl)
//REGISTER_PMINTERFACE(IZPDataLink,									kIZPDataLinkImpl)
//REGISTER_PMINTERFACE(IZPLinkedStream,								kIZPLinkedStreamImpl)
//REGISTER_PMINTERFACE(IZPDataLinkAction,								kIZPDataLinkActionImpl)
//REGISTER_PMINTERFACE(IZPUpdateLinkService,							kIZPUpdateLinkServiceImpl)
//REGISTER_PMINTERFACE(IZPDragDropSource,								kIZPDragDropSourceImpl)
//REGISTER_PMINTERFACE(IZPDragTargetFlavorHelper,						kIZPDragTargetFlavorHelperImpl)
//REGISTER_PMINTERFACE(IZPDataExchangeHandler,						kIZPDataExchangeHandlerImpl)
//REGISTER_PMINTERFACE(IZPDataLinkFacade,								kIZPDataLinkFacadeImpl)
//REGISTER_PMINTERFACE(IZPAssetsPanelUtils,							kIZPAssetsPanelUtilsImpl)
//
//REGISTER_PMINTERFACE(IZPAssetsPanelTreeViewHierarchyAdapter,		kIZPAssetsPanelTreeViewHierarchyAdapterImpl)
//REGISTER_PMINTERFACE(IZPAssetsPanelTreeViewWidgetManager,			kIZPAssetsPanelTreeViewWidgetManagerImpl)
//REGISTER_PMINTERFACE(IZPAssetsPanelTreeNodeEventHandler,			kIZPAssetsPanelTreeNodeEventHandlerImpl)
//REGISTER_PMINTERFACE(IZPAssetsPanelTreeViewDataModel,				kIZPAssetsPanelTreeViewDataModelImpl)
//REGISTER_PMINTERFACE(IZPAssetsPanelTreeViewObserver,				kIZPAssetsPanelTreeViewObserverImpl)
//REGISTER_PMINTERFACE(IZPAssetsPanelTreeNodeDragDropSource,			kIZPAssetsPanelTreeNodeDragDropSourceImpl)
//
//REGISTER_PMINTERFACE(IZPLayoutSelSuiteASB,							kIZPLayoutSelSuiteASBImpl)
//REGISTER_PMINTERFACE(IZPLayoutSelSuiteCSB,							kIZPLayoutSelSuiteCSBImpl)
//REGISTER_PMINTERFACE(IZPTextSelSuiteCSB,							kIZPTextSelSuiteCSBImpl)
//
//REGISTER_PMINTERFACE(IZPDocResponder,								kIZPDocResponderImpl)
//
//REGISTER_PMINTERFACE(IZPStartupShutdown,							kIZPStartupShutdownImpl)
//
//REGISTER_PMINTERFACE(IZPExportAssetDialogObserver,					kIZPExportAssetDialogObserverImpl)
//REGISTER_PMINTERFACE(IZPExportAssetDialogController,				kIZPExportAssetDialogControllerImpl)
//REGISTER_PMINTERFACE(IZPExportAssetDialogData,						kIZPExportAssetDialogDataImpl)
//
//REGISTER_PMINTERFACE(IZPAssetsPanelTreeViewNodeDeleteRollOverIconButtonObserver,	kIZPAssetsPanelTreeViewNodeDeleteRollOverIconButtonObserverImpl)
//
//REGISTER_PMINTERFACE(IZPFacade,										kIZPFacadeImpl)
//REGISTER_PMINTERFACE(IZPUIBrowseForLink,							kIZPUIBrowseForLinkImpl)
//
//REGISTER_PMINTERFACE(IZPStoryLock,									kIZPStoryLockImpl)
//REGISTER_PMINTERFACE(IZPManagedStatus,								kIZPManagedStatusImpl)
//
////From InCopy Export sample pln
//REGISTER_PMINTERFACE(InCopyStoryExportProvider,						kInCopyStoryExportProviderImpl)
//REGISTER_PMINTERFACE(InCopyStoryExportSuiteASB,						kICExportSuiteASBImpl)
//REGISTER_PMINTERFACE(InCopyStoryTextExportSuite,					kICExportSuiteTextImpl)
//REGISTER_PMINTERFACE(InCopyStoryLayoutExportSuite,					kICExportSuiteLayoutImpl)
//REGISTER_PMINTERFACE(InCopyStoryExportDefaultSuite,					kICExportSuiteDefaultImpl)
//REGISTER_PMINTERFACE(ICExportPrefsScriptProvider,					kICExportPrefsScriptProviderImpl)
//
////From InCopy Export UI sample pln
//REGISTER_PMINTERFACE(ICExportDialogController,						kICExportDialogControllerImpl)
//REGISTER_PMINTERFACE(InCopyStoryExportLayer,						kICExportLayerStoriesImpl)
//REGISTER_PMINTERFACE(InCopyStoryExportAllSuite,						kICExportAllSuiteImpl)
//REGISTER_PMINTERFACE(InCopyStoryExportAllStoriesSuite,				kICExportAllStoriesSuiteImpl)
//REGISTER_PMINTERFACE(InCopyStoryExportAllGraphicsSuite,				kICExportAllGraphicsSuiteImpl)
//REGISTER_PMINTERFACE(InCopyExportMenuComponent,						kInCopyExportMenuComponentImpl)
//REGISTER_PMINTERFACE(InCopyExportDialogService,						kInCopyExportDialogServiceImpl)
//REGISTER_PMINTERFACE(InCopyExportDialog,							kInCopyExportDialogImpl)
//
////REGISTER_PMINTERFACE(IZPLinkResponderServiceProvider,				kIZPLinkResponderServiceProviderImpl)
////REGISTER_PMINTERFACE(IZPLinkResponder,							kIZPLinkResponderImpl)
////REGISTER_PMINTERFACE(IZPExportData,								kIZPExportDataImpl)
//
//REGISTER_PMINTERFACE(IZPDocumentsPanelAMStatusListener,				kIZPDocumentsPanelAMStatusListenerImpl)
//REGISTER_PMINTERFACE(IZPAMTerminateService,							kIZPAMTerminateServiceImpl)
//REGISTER_PMINTERFACE(IZPActionFilter,								kIZPActionFilterImpl)
//
//REGISTER_PMINTERFACE(IZPAMDocumentSystemProvider,					kIZPAMDocumentSystemProviderImpl)
//REGISTER_PMINTERFACE(IZPAMDocumentServiceProvider,					kIZPAMDocumentServiceProviderImpl)
//REGISTER_PMINTERFACE(IZPAMUIDocumentServiceProvider,				kIZPAMUIDocumentServiceProviderImpl)
//REGISTER_PMINTERFACE(IZPAMDocumentInitializeService,				kIZPAMDocumentInitializeServiceImpl)
//REGISTER_PMINTERFACE(IZPAMDocumentCheckInService,					kIZPAMDocumentCheckInServiceImpl)
//REGISTER_PMINTERFACE(IZPAMDocumentCheckOutService,					kIZPAMDocumentCheckOutServiceImpl)
//REGISTER_PMINTERFACE(IZPAMDocumentCancelCheckOutService,			kIZPAMDocumentCancelCheckOutServiceImpl)
//REGISTER_PMINTERFACE(IZPAMDocumentEnsureLatestVersionService,		kIZPAMDocumentEnsureLatestVersionServiceImpl)
//REGISTER_PMINTERFACE(IZPAMUIDocumentEnablementRules,				kIZPAMUIDocumentEnablementRulesImpl)
//REGISTER_PMINTERFACE(IZPAssetsPanelUpdateCmd,						kIZPAssetsPanelUpdateCmdImpl)
//REGISTER_PMINTERFACE(IZPUpdateAssetDataCmd,							kIZPUpdateAssetDataCmdImpl)
//REGISTER_PMINTERFACE(IZPUpdateAssignmentDataCmd,					kIZPUpdateAssignmentDataCmdImpl)
//REGISTER_PMINTERFACE(IZPAssetsPanelTreeViewNodeSaveRollOverIconButtonObserver,	kIZPAssetsPanelTreeViewNodeSaveRollOverIconButtonObserverImpl)
//REGISTER_PMINTERFACE(IZPAssignmentDialogObserver,					kIZPAssignmentDialogObserverImpl)
//REGISTER_PMINTERFACE(IZPAssignmentDialogController,					kIZPAssignmentDialogControllerImpl)
//REGISTER_PMINTERFACE(IZPDataLinkScript,								kIZPDataLinkScriptImpl)
//REGISTER_PMINTERFACE(IZPSaveDialogUtils,							kIZPSaveDialogUtilsImpl)
//REGISTER_PMINTERFACE(IZPAssignmentDialogData,						kIZPAssignmentDialogDataImpl)
//REGISTER_PMINTERFACE(IZPAssignmentDialogUtils,						kIZPAssignmentDialogUtilsImpl)
//REGISTER_PMINTERFACE(IZPAssignmentData,								kIZPAssignmentDataImpl)
//REGISTER_PMINTERFACE(IZPAssignmentData,								kIZPAssignmentDataPersistImpl)
//REGISTER_PMINTERFACE(IZPHistoryPanelTreeViewAdapter,				kIZPHistoryPanelTreeViewAdapterImpl )
//REGISTER_PMINTERFACE(IZPHistoryPanelTreeViewWidgetMgr,				kIZPHistoryPanelTreeViewWidgetMgrImpl)
//REGISTER_PMINTERFACE(IZPHistoryPanelTreeViewDataModel,				kIZPHistoryPanelTreeViewDataModelImpl)
//REGISTER_PMINTERFACE(IZPAssignmentsPanelObserver,					kIZPAssignmentsPanelObserverImpl)
//REGISTER_PMINTERFACE(IZPIdleTask,									kIZPIdleTaskImpl)
//REGISTER_PMINTERFACE(IZPLoginIdleTask,								kIZPLoginIdleTaskImpl)
//REGISTER_PMINTERFACE(IZPAssetsPanelControlView,						kIZPAssetsPanelControlViewImpl)
//REGISTER_PMINTERFACE(IZPAMUIConflictAlertService,					kIZPAMUIConflictAlertServiceImpl)
//REGISTER_PMINTERFACE(IZPAMUILiveEditConflictAlertService,			kIZPAMUILiveEditConflictAlertServiceImpl)
//
//REGISTER_PMINTERFACE(IZPPanelTreeHeaderSettings,					kIZPPanelTreeHeaderSettingsImpl)
//REGISTER_PMINTERFACE(IZPTreeHeaderEventHandler,						kIZPTreeHeaderEHImpl)
//REGISTER_PMINTERFACE(IZPSplitterPanelObserver,						kIZPSplitterPanelObserverImpl)
//REGISTER_PMINTERFACE(IZPHistoryPanelControlView,					kIZPHistoryPanelControlViewImpl)
//REGISTER_PMINTERFACE(IZPDocumentsPanelPreviewPanel,					kIZPDocumentsPanelPreviewPanelImpl)
//REGISTER_PMINTERFACE( IZPCmdInterceptor,							kIZPCommandInterceptorImpl )
//
//// REGISTER_PMINTERFACE(IZPAMAssignmentServiceProvider,				kIZPAMAssignmentServiceProviderImpl)
//REGISTER_PMINTERFACE(IZPAMAssignmentInitializeService,				kIZPAMAssignmentInitializeServiceImpl)
//REGISTER_PMINTERFACE(IZPAMAssignmentCheckInService,					kIZPAMAssignmentCheckInServiceImpl)
//REGISTER_PMINTERFACE(IZPAMAssignmentSystemProvider,					kIZPAMAssignmentSystemProviderImpl)
//REGISTER_PMINTERFACE(IZPConversionProvider,							kIZPConversionProviderImpl)
//REGISTER_PMINTERFACE(IZPDynamicMenu,								kIZPDynamicMenuImpl)
//REGISTER_PMINTERFACE(IZPSuppressedUI,								kIZPSuppressedUIImpl)
//// REGISTER_PMINTERFACE(IZPWorkgroupAccess,							kIZPWorkgroupAccessImpl)
//REGISTER_PMINTERFACE(IZPBridgeCmdSuite,								kIZPBridgeCmdSuiteImpl)
//REGISTER_PMINTERFACE(IZPWorkgroupAccessServiceProvider,				kIZPWorkgroupAccessServiceProviderImpl)
//
////REGISTER_PMINTERFACE(IZPAMAssignmentServiceProvider,				kIZPAMAssignmentServiceProviderImpl)
//

#endif
