//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/SDK/Public/iZMediaUI_ClassDefID.h $
//	$Revision: 2799 $
//	$Date: 2011-04-12 12:20:30 +0200 (Tue, 12 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 30-3-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_iZMediaUI_ClassDefID_
#define _h_iZMediaUI_ClassDefID_
#pragma once

// ClassIDs:
	//General
DECLARE_PMID(kClassIDSpace, kZMAppActionComponentBoss,														kIZMediaUIPrefix + 1 )
DECLARE_PMID(kClassIDSpace, kZMTVHeaderActionComponentBoss,													kIZMediaUIPrefix + 2 )

	//Dialogs
DECLARE_PMID(kClassIDSpace, kZMAlertDialogBoss,																kIZMediaUIPrefix + 50 )
DECLARE_PMID(kClassIDSpace, kZMPrefsDialogBoss,																kIZMediaUIPrefix + 51 )

	//Panels
DECLARE_PMID(kClassIDSpace, kZMUIAdsPanelWidgetBoss,														kIZMediaUIPrefix + 80 )

	//Widgets
DECLARE_PMID(kClassIDSpace, kZMUIDataObjPopupBoss,															kIZMediaUIPrefix + 100 )
DECLARE_PMID(kClassIDSpace, kZMUIDataObjPopupWithFixedValuesBoss,												kIZMediaUIPrefix + 101 )
DECLARE_PMID(kClassIDSpace, kZMUIPopTitleListBoss,															kIZMediaUIPrefix + 102 )
DECLARE_PMID(kClassIDSpace, kZMUIPopEditionsListBoss,														kIZMediaUIPrefix + 103 )
DECLARE_PMID(kClassIDSpace, kZMUIPopEditionWithAllListBoss,													kIZMediaUIPrefix + 104 )

DECLARE_PMID(kClassIDSpace, kZMUIDataObjTVNodeWidgetBoss,													kIZMediaUIPrefix + 110 )
DECLARE_PMID(kClassIDSpace, kZMTVHeaderWidgetBoss,															kIZMediaUIPrefix + 111 )
	//TV column dynamic widgets
DECLARE_PMID(kClassIDSpace, kZMTVColumnRollOverIconButtonWidgetBoss,										kIZMediaUIPrefix + 112 )

	//Ads Panel Controls
DECLARE_PMID(kClassIDSpace, kZMAdsTreeViewWidgetBoss,														kIZMediaUIPrefix + 121 )
DECLARE_PMID(kClassIDSpace, kZMAdsTVAdNodeWidgetBoss,														kIZMediaUIPrefix + 122 )
DECLARE_PMID(kClassIDSpace, kZMAdsTVAdFileNodeWidgetBoss,													kIZMediaUIPrefix + 123 )

	//Dialogs

	//Panels
	//Drag n Drop

#pragma mark -
// InterfaceIDs:
	//General
DECLARE_PMID(kInterfaceIDSpace, IID_IZMUIDIALOGFACADE,														kIZMediaUIPrefix + 0 )

	//Widgets
DECLARE_PMID(kInterfaceIDSpace, IID_IZMOLDEVENTHANDLER,														kIZMediaUIPrefix + 20 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZMADSTVDATAMODEL,														kIZMediaUIPrefix + 21 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZMUISELECTEDID,														kIZMediaUIPrefix + 22 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZMDATAOBJPOPUPCONTROLLER,												kIZMediaUIPrefix + 23 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZMUIOBJIDSSTRINGLISTDATA,												kIZMediaUIPrefix + 24 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZMTITLEIDSTRINGDATA,													kIZMediaUIPrefix + 25 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZMPOPFIXEDITEMSTRINGDATA,												kIZMediaUIPrefix + 26 )

DECLARE_PMID(kInterfaceIDSpace, IID_IZMUITVNODEDATA,														kIZMediaUIPrefix + 30 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZMUITVNODEWIDGETMGR,													kIZMediaUIPrefix + 31 )

DECLARE_PMID(kInterfaceIDSpace, IID_IZMTVHEADERCONTROLLER,													kIZMediaUIPrefix + 40 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZMTVCOLUMNSINFO,														kIZMediaUIPrefix + 41 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZMTVCOLUMNSPREF,														kIZMediaUIPrefix + 42 )
DECLARE_PMID(kInterfaceIDSpace, IID_IZMINTDATA,																kIZMediaUIPrefix + 43 )

DECLARE_PMID(kInterfaceIDSpace, IID_IZMADSPANELHELPER,														kIZMediaUIPrefix + 71 )
#pragma mark -
// ImplementationIDs:
	//General
DECLARE_PMID(kImplementationIDSpace, kZMActionComponentImpl,												kIZMediaUIPrefix + 1 )
DECLARE_PMID(kImplementationIDSpace, kZMTVHeaderActionComponentImpl,										kIZMediaUIPrefix + 2 )

//	Export Providers
	//Utils
DECLARE_PMID(kImplementationIDSpace, kZMUIDialogFacadeImpl,													kIZMediaUIPrefix + 20 )

//UI
//Dialogs
	//Alert Dialog
DECLARE_PMID(kImplementationIDSpace, kZMUIAlertDialogControllerImpl,										kIZMediaUIPrefix + 50 )
DECLARE_PMID(kImplementationIDSpace, kZMUIAlertDialogObserverImpl,											kIZMediaUIPrefix + 51 )
	//Prefs Dialog
DECLARE_PMID(kImplementationIDSpace, kZMUIPrefsPanelCreatorImpl,											kIZMediaUIPrefix + 55 )
DECLARE_PMID(kImplementationIDSpace, kZMUIPrefsDialogControllerImpl,										kIZMediaUIPrefix + 56 )
DECLARE_PMID(kImplementationIDSpace, kZMUIPrefsDialogObserverImpl,											kIZMediaUIPrefix + 57 )
//Panels
	//Ads
DECLARE_PMID(kImplementationIDSpace, kZMAdsPanelObserverImpl,												kIZMediaUIPrefix + 80 )
DECLARE_PMID(kImplementationIDSpace, kZMAdsPanelControlViewImpl,											kIZMediaUIPrefix + 81 )
DECLARE_PMID(kImplementationIDSpace, kZMAdsPanelHelperImpl,													kIZMediaUIPrefix + 82 )
//Widgets
	//Popups
DECLARE_PMID(kImplementationIDSpace, kZMTitlePopupControllerImpl,											kIZMediaUIPrefix + 100 )
DECLARE_PMID(kImplementationIDSpace, kZMTitlePopupObserverImpl,												kIZMediaUIPrefix + 101 )
DECLARE_PMID(kImplementationIDSpace, kZMEditionPopupObserverImpl,											kIZMediaUIPrefix + 102 )
DECLARE_PMID(kImplementationIDSpace, kZMEditionPopupControllerImpl,											kIZMediaUIPrefix + 103 )
DECLARE_PMID(kImplementationIDSpace, kZMEditionCustomPopupObserverImpl,										kIZMediaUIPrefix + 104 )
DECLARE_PMID(kImplementationIDSpace, kZMEditionCustomPopupControllerImpl,									kIZMediaUIPrefix + 105 )
	//Ads TV
DECLARE_PMID(kImplementationIDSpace, kZMAdsTVDataModelImpl,													kIZMediaUIPrefix + 140 )
DECLARE_PMID(kImplementationIDSpace, kZMAdsTVHierarchyAdapterImpl,											kIZMediaUIPrefix + 141 )
DECLARE_PMID(kImplementationIDSpace, kZMAdsTVWidgetManagerImpl,												kIZMediaUIPrefix + 142 )
DECLARE_PMID(kImplementationIDSpace, kZMAdsTVObserverImpl,													kIZMediaUIPrefix + 143 )
	//Ad File Node
DECLARE_PMID(kImplementationIDSpace, kZMAdsTVAdFileNodeDataImpl,											kIZMediaUIPrefix + 150 )
DECLARE_PMID(kImplementationIDSpace, kZMAdsTVAdFileNodeWidgetMgrImpl,										kIZMediaUIPrefix + 151 )
DECLARE_PMID(kImplementationIDSpace, kZMAdsTVAdFileNodeEHImpl,												kIZMediaUIPrefix + 152 )
	//Ad Node
DECLARE_PMID(kImplementationIDSpace, kZMAdsTVAdNodeDataImpl,												kIZMediaUIPrefix + 160 )
DECLARE_PMID(kImplementationIDSpace, kZMAdsTVAdNodeWidgetMgrImpl,											kIZMediaUIPrefix + 161 )
DECLARE_PMID(kImplementationIDSpace, kZMAdsTVAdNodeObserverImpl,											kIZMediaUIPrefix + 162 )
DECLARE_PMID(kImplementationIDSpace, kZMAdsTVAdNodeEHImpl,													kIZMediaUIPrefix + 163 )
DECLARE_PMID(kImplementationIDSpace, kZMAdsTVAdNodeDragDropSourceImpl,										kIZMediaUIPrefix + 164 )
	//TreeView Common impls
DECLARE_PMID(kImplementationIDSpace, kZMTreeViewMgrImpl,													kIZMediaUIPrefix + 200 )
	//TreeView Header
DECLARE_PMID(kImplementationIDSpace, kZMTVHeaderControllerImpl,												kIZMediaUIPrefix + 205 )
DECLARE_PMID(kImplementationIDSpace, kZMTVColumnsInfoImpl,													kIZMediaUIPrefix + 206 )
DECLARE_PMID(kImplementationIDSpace, kZMTVColumnsPrefPersistImpl,											kIZMediaUIPrefix + 207 )
DECLARE_PMID(kImplementationIDSpace, kZMTVHeaderObserverImpl,												kIZMediaUIPrefix + 208 )
DECLARE_PMID(kImplementationIDSpace, kZMTVHeaderEHImpl,														kIZMediaUIPrefix + 209 )
DECLARE_PMID(kImplementationIDSpace, kZMTVHeaderDynamicMenuImpl,											kIZMediaUIPrefix + 210 )

DECLARE_PMID(kImplementationIDSpace, kZMTVColumnRollOverIconButtonObserverImpl,								kIZMediaUIPrefix + 215 )
	//Alias
DECLARE_PMID(kImplementationIDSpace, kZMDataIDStringData1Impl,												kIZMediaUIPrefix + 230 )
DECLARE_PMID(kImplementationIDSpace, kZMStringListDataImpl,													kIZMediaUIPrefix + 231 )
DECLARE_PMID(kImplementationIDSpace, kZMStringListControlDataImpl,											kIZMediaUIPrefix + 232 )

#endif //_h_iZMediaUI_ClassDefID_
