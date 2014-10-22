//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Plugin/IZPUIID.h $
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

#ifndef __IZPIDUI_h__
#define __IZPIDUI_h__

#include "SDKDef.h"
#ifndef _iZineUIVersion_h_
#include "iZineUIVersion.h"
#endif //_iZineUIVersion_h_

// Company:
#define kIZPUICompanyKey		"kIZPPlugInCompanyKey"		// Company name used internally for menu paths and the like. Must be globally unique, only A-Z, 0-9, space and "_".
#define kIZPUICompanyValue		"iZine Publish"				// Company name displayed externally.

// Plug-in:
#define kIZPUIPluginName			"iZine UI"					// Name of this plug-in.
#define kIZPUIPluginFileName		"izine.ui"
#ifndef kIZPProductName
#define kIZPProductName				"iZine Publish"
#endif

//#define kIZPPrefixNumber	0x130700	// iZine Publish prefix number
//#define kIZPOldPrefix		0x61000		//Un registered ID,used before 2010 Feb 01.
#define kIZPUIPrefixNumber	0x14d500	// prefix number for iZine Publish UI
//0x61000	//Unregistered ID
//0x130700	//iZine Publish
//0x14d500	//iZine InDesign UI
//0x14d600	//iZine InDesign Server
//0xe9400

#define kIZPUIVersion kIZPUIPluginVersionString
#define kIZPUIAuthor		"iZine Publish"					// Author of this plug-in (for the About Box).
//// #define kIZPAuthor		"izine-publish.net"

// Plug-in Prefix: (please change kIZPPrefixNumber above to modify the prefix.)
#define kIZPUIPrefix		RezLong(kIZPUIPrefixNumber)			// Prefix for for object model IDs for iZine Publish UI
#define kIZPUIStringPrefix	IZPMAKESTR(kIZPUIPrefixNumber)

// Missing plug-in: (see ExtraPluginInfo resource)
#define kIZPUIMissingPluginURLValue		"http://www.izine-publish.net/" // URL displayed in Missing Plug-in dialog
#define kIZPUIMissingPluginAlertValue		"One of iZine Publish plugins is missing. Please add it to open the documents." // Message displayed in Missing Plug-in dialog - provide a string that instructs user how to solve their missing plug-in problem

#define kIZPUICopyrightShortValue	"Copyright 2008-2010 iZine Solutions."
#define kIZPUICopyrightValue	kIZPUICopyrightShortValue " All Rights Reserved."
#define kIZPUISupportURL		"http://www.izine-publish.com/"


// PluginID:
DECLARE_PMID(kPlugInIDSpace, kIZPUIPluginID, kIZPUIPrefix + 0)

// Resources
//Note: Resources that are localised must have IDs apart by 50.
//i.e max value defined in pmlocaleids.h for index_xxxx values.
//currently this max value is 0x0024 i.e 36.
	//Panels
#define kZPUIPrefsPanelResourceID		10000
#define	kIZPHistoryPanelResourceID		10050
#define kZPUIAssetsPanelResourceID		10100
#define kIZPAssignmentsPanelResourceID	10150
#define kIZPDocumentsPanelResourceID	10200
#define kZPUITasksPanelResourceID		10250
	//Dialogs
#define kZPUISaveToRepDialogResourceID	11000
#define kIZPOpenFromRepDialogResourceID 11050
#define kZPUILoginDialogResourceID		11100
#define kIZPExportAssetDialogResourceID	11150
#define kIZPAssignmentDialogResourceID	11200
#define kIZPCommentDlgRsrcID			11250
#define kZPUIAlertDialogResourceID		11300
#define kZPUICreateTaskDialogResourceID	11350
#define kZPUIUpdateVersionStatusDialogResourceID	11400
#define kZPUIRenameAssetDialogResourceID	11450
	//Widgets
#define kIZPTVNodeWidgetRsrcID							12000
#define kIZPHistoryPanelTreeViewNodeWidgetRsrcID		12001
#define kIZPDocumentsPanelTreeViewNodeWidgetRsrcID		12002
#define kIZPAssetsTVAssetNodeWidgetRsrcID	12003
#define kIZPAssetsTVStatusNodeWidgetRsrcID	12004
#define kIZPStoriesListElementRsrcID					12005
#define kIZPTasksTVTaskNodeWidgetRsrcID					12400
#define kZPUITVHeaderColumnWidgetRsrcID					12450
#define kZPUITVColumnTxtWidgetRsrcID					12500
#define kZPUITVColumnIconWidgetRsrcID					12550
#define kZPUITVColumnRollOverIconButtonWidgetRsrcID		12600

	//Icons
#define kIZPStoryIconRsrcID				11000
#define kIZPAscIconRsrcID				12000
#define kIZPDescIconRsrcID				12001
#define kIZPBlankIconRsrcID				12002	//Not Used

// ClassID/Interface/Impl
#include "IZPUI_ClassDefID.h"
// ActionIDs & Menu positions
#include "IZPUI_ActionID.h"
// WidgetIDs:
#include "IZPUI_WidgetID.h"

DECLARE_PMID(kMessageIDSpace, kZPUIDBObjDropDown_ListLengthChangedMsg,										kIZPUIPrefix + 0 )
DECLARE_PMID(kMessageIDSpace, kZPUIDBObjDropDown_SelectionChangedMsg,										kIZPUIPrefix + 1 )
DECLARE_PMID(kMessageIDSpace, kZPUIAssetTV_ModelChangedMsg,													kIZPUIPrefix + 2 )
	//Sent by button widget that is contained in a TV node, as a column
	//interface ITriStateControlData
DECLARE_PMID(kMessageIDSpace, kZPUITVNodeColumn_BtnPressedMsg,												kIZPUIPrefix + 3 )
	//Sent by Asset node, when any button in it is pressed.
	//Interface IObserver
DECLARE_PMID(kMessageIDSpace, kZPUIAssetTV_AssetNodeSaveBtnPressedMsg,										kIZPUIPrefix + 4 )
DECLARE_PMID(kMessageIDSpace, kZPUIAssetTV_AssetNodeDeleteBtnPressedMsg,									kIZPUIPrefix + 5 )
	//Sent by Asset node, when it is double clicked
	//Interface IEventHandler
	//changed by will contain the event. Asset id can be obtained from node's subject.
DECLARE_PMID(kMessageIDSpace, kZPUIAssetTV_AssetNodeDoubleClickPressedMsg,									kIZPUIPrefix + 6 )
	//Sent by Status node, when it is double clicked
	//Interface IEventHandler
	//changed by will contain the event. status id can be obtained from node's subject.
DECLARE_PMID(kMessageIDSpace, kZPUIAssetTV_StatusNodeDoubleClickPressedMsg,									kIZPUIPrefix + 7 )
	//Sent by Assets TV observer when selection changes.
	//Interface ITreeViewController
	//void * contains the controlview of current selected node. Can be nil, when multi or none selected
DECLARE_PMID(kMessageIDSpace, kZPUIAssetTV_SelectionChangedMsg,												kIZPUIPrefix + 8 )
DECLARE_PMID(kMessageIDSpace, kZPUIAssetTVNodeDataWillChangeMsg,											kIZPUIPrefix + 9 )
DECLARE_PMID(kMessageIDSpace, kZPUIAssetTVNodeDataChangedMsg,												kIZPUIPrefix + 10 )

	//Sent by Tasks TV observer when selection changes.
	//Interface ITreeViewController
	//void * contains the controlview of current selected node. Can be nil, when multi or none selected
DECLARE_PMID(kMessageIDSpace, kZPUITasksTV_SelectionChangedMsg,												kIZPUIPrefix + 15 )
DECLARE_PMID(kMessageIDSpace, kZPUITasksTVNodeDataWillChangeMsg,											kIZPUIPrefix + 16 )
DECLARE_PMID(kMessageIDSpace, kZPUITasksTVNodeDataChangedMsg,												kIZPUIPrefix + 17 )

DECLARE_PMID(kMessageIDSpace, kZPUITaskTV_ModelChangedMsg,													kIZPUIPrefix + 18 )
	//Sent by Task node, when it is double clicked
	//Interface IEventHandler
	//changed by will contain the event. Task id can be obtained from node's subject.
DECLARE_PMID(kMessageIDSpace, kZPUITaskTV_TaskNodeDoubleClickPressedMsg,									kIZPUIPrefix + 19 )

	//TreeView Header messages
DECLARE_PMID(kMessageIDSpace, kZPUITVHeaderColPositionsChangedMsg,											kIZPUIPrefix + 31 )

	//Sent by disclosure triangle, when it is clicked
	//Interface ITriStateControlData::kDefaultIID
DECLARE_PMID(kMessageIDSpace, kZPUIDisclosureBtnStateChangedMsg,											kIZPUIPrefix + 33 )

//Icon IDs
#define kIZPAvailableIconRsrcID			1500
#define kIZPCheckedOutIconRsrcID		1501
#define kIZPDownloadableIconRsrcID		1502
#define kIZPEditedLocallyIconRsrcID		1503
#define kIZPEditElsewhereIconRsrcID		1504
#define kIZPLockedIconRsrcID			1505
#define kIZPProxyUpdateIconRsrcID		1506
#define kIZPInCopyIconRsrcID			1507
#define kIZPInDesignIconRsrcID			1508
#define kIZPLogoIconRsrcID				1509

#define kIZPAssetType_ICCS3DocumentIconRsrcID	1550
#define kIZPAssetType_ICCS3TemplateIconRsrcID	1551
#define kIZPAssetType_IDCS3AssignmentIconRsrcID	1552
#define kIZPAssetType_IDCS3DocumentIconRsrcID	1553
#define kIZPAssetType_IDCS3StoryIconRsrcID		1554
#define kIZPAssetType_IDCS3TemplateIconRsrcID	1555



#define kIZPAssetIDDragDropCustomFlavor			PMFlavor('Zaid')

#include "IZPUI_StringKeys.h"

// Initial data format version numbers
#define kIZPUIFirstMajorFormatNumber	RezLong(1)
#define kIZPUIFirstMinorFormatNumber	RezLong(0)

#define kIZPUICurrentMajorFormatNumber kIZPUIFirstMajorFormatNumber
#define kIZPUICurrentMinorFormatNumber kIZPUIFirstMinorFormatNumber

#endif // __IZPUIID_h__

