//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/IZP_MessageID.h $
//	$Revision: 3572 $
//	$Date: 2011-08-11 08:47:35 +0200 (Thu, 11 Aug 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 13-5-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZP_MessageID_
#define _h_IZP_MessageID_
#pragma once

// Message IDs:
DECLARE_PMID(kMessageIDSpace, kZPLogin_BeforeLoginMsg,														kIZPPrefix + 0 )
DECLARE_PMID(kMessageIDSpace, kZPLogin_AuthenticationInProgressMsg,											kIZPPrefix + 1 )
DECLARE_PMID(kMessageIDSpace, kZPLogin_AuthenticationDoneMsg,												kIZPPrefix + 2 )
DECLARE_PMID(kMessageIDSpace, kZPLogin_ConfigDataFetchingMsg,												kIZPPrefix + 3 )
DECLARE_PMID(kMessageIDSpace, kZPLogin_ConfigDataFetchedMsg,												kIZPPrefix + 4 )
DECLARE_PMID(kMessageIDSpace, kZPLogin_BeforeLoggedInMsg,													kIZPPrefix + 5 )
DECLARE_PMID(kMessageIDSpace, kZPLogin_LoggedInMsg,															kIZPPrefix + 6 )
DECLARE_PMID(kMessageIDSpace, kZPLogin_BeforeLogoutMsg,														kIZPPrefix + 7 )
DECLARE_PMID(kMessageIDSpace, kZPLogin_NotLoggedInMsg,														kIZPPrefix + 8 )
DECLARE_PMID(kMessageIDSpace, kZPLogin_CanLogoutMsg,														kIZPPrefix + 9 )

	//Send by WSResponseHandler if the response is not handled explicitly.
	// Changed by is the AZPSoapResponse *.
DECLARE_PMID(kMessageIDSpace, kZPWSResponseAvailable,														kIZPPrefix + 30 )

DECLARE_PMID(kMessageIDSpace, kZPAppStateChanged,															kIZPPrefix + 40 )

	//Send by Title Mgr
DECLARE_PMID(kMessageIDSpace, kZPTitleListWillChange,														kIZPPrefix + 50 )
DECLARE_PMID(kMessageIDSpace, kZPTitleListChanged,															kIZPPrefix + 51 )
	//Send by TitleStatus Mgr with protocol IZPTitleStatusMgr::kDefaultIID & by StatusList with protocol IZPStatusList::kDefaultIID
DECLARE_PMID(kMessageIDSpace, kZPTitleStatusListWillChange,													kIZPPrefix + 52 )
DECLARE_PMID(kMessageIDSpace, kZPTitleStatusListChanged,													kIZPPrefix + 53 )
	//Send by Edition Mgr with protocol IZPEditionMgr::kDefaultIID & by EditionList with protocol IZPEditionList::kDefaultIID
	//Changed by is the edition list pointer which was changed. Can be nil if msg is sent by mgr.
DECLARE_PMID(kMessageIDSpace, kZPEditionListWillChange,														kIZPPrefix + 54 )
DECLARE_PMID(kMessageIDSpace, kZPEditionListChanged,														kIZPPrefix + 55 )
	//Send by AssetList with protocol IZPAssetList::kDefaultIID
DECLARE_PMID(kMessageIDSpace, kZPAssetListWillChangeMsg,													kIZPPrefix + 56 )
DECLARE_PMID(kMessageIDSpace, kZPAssetListChangedMsg,														kIZPPrefix + 57 )

	//Send by dbAsset with protocol IZPDataName, changed by is PMString new name
DECLARE_PMID(kMessageIDSpace, kZPDataNameWillChangeMsg,														kIZPPrefix + 70 )
	//Send by dbAsset with protocol IZPDataName
DECLARE_PMID(kMessageIDSpace, kZPDataNameChangedMsg,														kIZPPrefix + 71 )
	//Send by AssetInfo with protocol IZPAssetInfo::kDefaultIID
DECLARE_PMID(kMessageIDSpace, kZPAssetInfoChangedMsg,														kIZPPrefix + 72 )
	//Send by AssetLockInfo with protocol IZPAssetLockInfo::kDefaultIID
DECLARE_PMID(kMessageIDSpace, kZPAssetLockInfoChangedMsg,													kIZPPrefix + 73 )
	//Send by CZPModifyAssetLocalInfoCmd with protocol IZPAssetLocalInfo::kDefaultIID
	//Changed by is the command that changed.
DECLARE_PMID(kMessageIDSpace, kZPAssetLocalInfoChangedMsg,													kIZPPrefix + 74 )

	//Send by TaskStatus Mgr with protocol IZPTaskStatusMgr::kDefaultIID
DECLARE_PMID(kMessageIDSpace, kZPTaskStatusListWillChange,													kIZPPrefix + 80 )
DECLARE_PMID(kMessageIDSpace, kZPTaskStatusListChanged,														kIZPPrefix + 81 )
	//Send by User Mgr with protocol IZPUserMgr::kDefaultIID & by UserList with protocol IZPUserList::kDefaultIID
DECLARE_PMID(kMessageIDSpace, kZPUserListWillChangeMsg,														kIZPPrefix + 82 )
DECLARE_PMID(kMessageIDSpace, kZPUserListChangedMsg,														kIZPPrefix + 83 )
	//Send by Task Mgr with protocol IZPTaskMgr::kDefaultIID & by TaskList with protocol IZPTaskList::kDefaultIID
DECLARE_PMID(kMessageIDSpace, kZPTaskListWillChangeMsg,														kIZPPrefix + 84 )
DECLARE_PMID(kMessageIDSpace, kZPTaskListChangedMsg,														kIZPPrefix + 85 )
	//Send by TaskInfo with protocol IZPTaskInfo::kDefaultIID
	//Send by TaskMgr with protocol IZPTaskMgr::kDefaultIID, changed by is IStringData * of dbTaskID, used where multiple object listening is not required.
DECLARE_PMID(kMessageIDSpace, kZPTaskInfoChangedMsg,														kIZPPrefix + 86 )
	//Send by TaskCategory Mgr with protocol IZPTaskCategoryMgr::kDefaultIID
DECLARE_PMID(kMessageIDSpace, kZPTaskCategoryListWillChange,												kIZPPrefix + 87 )
DECLARE_PMID(kMessageIDSpace, kZPTaskCategoryListChanged,													kIZPPrefix + 88 )


	//Send by Preview Mgr with protocol IZPPreviewMgr::kDefaultIID
DECLARE_PMID(kMessageIDSpace, kZPAssetThumbnailAvailableMsg,												kIZPPrefix + 100 )








#endif //_h_IZP_MessageID_
