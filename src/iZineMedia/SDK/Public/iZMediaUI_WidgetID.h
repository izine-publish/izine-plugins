//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/SDK/Public/iZMediaUI_WidgetID.h $
//	$Revision: 2861 $
//	$Date: 2011-04-27 15:19:02 +0200 (Wed, 27 Apr 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 30-3-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_iZMediaUI_WidgetID_
#define _h_iZMediaUI_WidgetID_
#pragma once

	//Prefs
DECLARE_PMID(kWidgetIDSpace, kZMPrefsPanelWidgetID,														kIZMediaUIPrefix + 0 )
DECLARE_PMID(kWidgetIDSpace, kZMPrefsBrowseBtnWidgetID,													kIZMediaUIPrefix + 1 )
DECLARE_PMID(kWidgetIDSpace, kZMPrefsRepositoryPathEditBoxWidgetID,											kIZMediaUIPrefix + 2 )
DECLARE_PMID(kWidgetIDSpace, kZMPrefsAdFileNamePrefixEditBoxWidgetID,										kIZMediaUIPrefix + 3 )

	//Ads Panel
DECLARE_PMID(kWidgetIDSpace, kZPMAdsPanelWidgetID,															kIZMediaUIPrefix + 40 )
DECLARE_PMID(kWidgetIDSpace, kZMUIAdsPanelRefreshBtnWidgetID,												kIZMediaUIPrefix + 41 )
DECLARE_PMID(kWidgetIDSpace, kZMUIAdsPanelVersionWidgetID,													kIZMediaUIPrefix + 42 )
DECLARE_PMID(kWidgetIDSpace, kZMUIAdsPanelTitleDropDownWidgetID,											kIZMediaUIPrefix + 43 )
DECLARE_PMID(kWidgetIDSpace, kZMUIAdsPanelEditionDropDownWidgetID,											kIZMediaUIPrefix + 44 )
DECLARE_PMID(kWidgetIDSpace, kZMUIAdsPanelLstAdsWidgetID,													kIZMediaUIPrefix + 45 )
DECLARE_PMID(kWidgetIDSpace, kZMUIAdsPanelHdrAdsWidgetID,													kIZMediaUIPrefix + 46 )

	//Ads Treeview Control
DECLARE_PMID(kWidgetIDSpace, kZMUIAdsTVAdFileNodeWidgetID,													kIZMediaUIPrefix + 60 )
DECLARE_PMID(kWidgetIDSpace, kZMUIAdsTVAdNodeWidgetID,														kIZMediaUIPrefix + 61 )
DECLARE_PMID(kWidgetIDSpace, kZMUITVGroupElementWidgetID,													kIZMediaUIPrefix + 62 )
DECLARE_PMID(kWidgetIDSpace, kZMUIAdsTVNodeIconWidgetID,													kIZMediaUIPrefix + 63 )
DECLARE_PMID(kWidgetIDSpace, kZMUIAdsTVNodeNameWidgetID,													kIZMediaUIPrefix + 64 )
//Gap till eTVColAdsCount
#define kZMUIAdsTVNodeFirstColumnWidgetID kZMUIAdsTVNodeIconWidgetID

DECLARE_PMID(kWidgetIDSpace, kZMUITVHdrControlStripWidgetID,												kIZMediaUIPrefix + 200 )
DECLARE_PMID(kWidgetIDSpace, kZMUITVColumnHeaderStartWidgetID,												kIZMediaUIPrefix + 201)
//GAP of 15, reserved for column headers
DECLARE_PMID(kWidgetIDSpace, kZMUITVColumnHeaderEndWidgetID,												kIZMediaUIPrefix + 215 )


#endif //_h_iZMediaUI_WidgetID_
