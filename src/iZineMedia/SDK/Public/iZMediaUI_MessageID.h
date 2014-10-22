//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/SDK/Public/iZMediaUI_MessageID.h $
//	$Revision: 2755 $
//	$Date: 2011-04-08 12:53:56 +0200 (Fri, 08 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 30-3-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_iZMediaUI_MessageID_
#define _h_iZMediaUI_MessageID_
#pragma once

DECLARE_PMID(kMessageIDSpace, kZMUIDataObjDropDown_ListLengthChangedMsg,									kIZMediaUIPrefix + 0 )
DECLARE_PMID(kMessageIDSpace, kZMUIDataObjDropDown_SelectionChangedMsg,										kIZMediaUIPrefix + 1 )

	//Sent by button widget that is contained in a TV node, as a column
	//interface ITriStateControlData
DECLARE_PMID(kMessageIDSpace, kZMUITVNodeColumn_BtnPressedMsg,												kIZMediaUIPrefix + 10 )

DECLARE_PMID(kMessageIDSpace, kZMUIAdsTV_ModelChangedMsg,													kIZMediaUIPrefix + 20 )
	//Sent by Ad File node, when it is double clicked
	//Interface IEventHandler
	//changed by will contain the event. status id can be obtained from node's subject.
DECLARE_PMID(kMessageIDSpace, kZMUIAdsTV_AdFileNodeDoubleClickPressedMsg,									kIZMediaUIPrefix + 21 )
	//Sent by Ad node, when it is double clicked
	//Interface IEventHandler
	//changed by will contain the event. Ad id can be obtained from node's subject.
DECLARE_PMID(kMessageIDSpace, kZMUIAdsTV_AdNodeDoubleClickPressedMsg,										kIZMediaUIPrefix + 22 )
	//Sent by Ads TV observer when selection changes.
	//Interface ITreeViewController
	//void * contains the controlview of current selected node. Can be nil, when multi or none selected
DECLARE_PMID(kMessageIDSpace, kZMUIAdsTV_SelectionChangedMsg,												kIZMediaUIPrefix + 30 )
DECLARE_PMID(kMessageIDSpace, kZMUIAdsTVNodeDataWillChangeMsg,												kIZMediaUIPrefix + 31 )
DECLARE_PMID(kMessageIDSpace, kZMUIAdsTVNodeDataChangedMsg,													kIZMediaUIPrefix + 32 )

	//TreeView Header messages
DECLARE_PMID(kMessageIDSpace, kZMUITVHeaderColPositionsChangedMsg,											kIZMediaUIPrefix + 41 )
#endif //_h_iZMediaUI_MessageID_
