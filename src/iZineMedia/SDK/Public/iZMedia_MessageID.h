//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/SDK/Public/iZMedia_MessageID.h $
//	$Revision: 3028 $
//	$Date: 2011-05-18 08:09:30 +0200 (Wed, 18 May 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 30-3-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_iZMedia_MessageID_
#define _h_iZMedia_MessageID_
#pragma once

// Message IDs:
DECLARE_PMID(kMessageIDSpace, kZMTitleListWillChange,														kIZMediaPrefix + 0 )
DECLARE_PMID(kMessageIDSpace, kZMTitleListChanged,															kIZMediaPrefix + 1 )
DECLARE_PMID(kMessageIDSpace, kZMEditionListWillChange,														kIZMediaPrefix + 2 )
	//Send by Edition Mgr with protocol IZMEditionMgr::kDefaultIID & by EditionList with protocol IZMEditionList::kDefaultIID
	//Changed by is the edition list pointer which was changed. Can be nil if msg is sent by mgr.
DECLARE_PMID(kMessageIDSpace, kZMEditionListChanged,														kIZMediaPrefix + 3 )
DECLARE_PMID(kMessageIDSpace, kZMAdFileListWillChangeMsg,													kIZMediaPrefix + 4 )
DECLARE_PMID(kMessageIDSpace, kZMAdFileListChangedMsg,														kIZMediaPrefix + 5 )
DECLARE_PMID(kMessageIDSpace, kZMAdListWillChangeMsg,														kIZMediaPrefix + 6 )
DECLARE_PMID(kMessageIDSpace, kZMAdListChangedMsg,															kIZMediaPrefix + 7 )
	//Send by ADInfo with protocol IZMAdInfo::kDefaultIID
DECLARE_PMID(kMessageIDSpace, kZMAdInfoChangedMsg,															kIZMediaPrefix + 10 )
	//Send by -------.
	// Changed by is -------.
//DECLARE_PMID(kMessageIDSpace, kZPWSResponseAvailable,														kIZMediaPrefix + 19 )

DECLARE_PMID(kMessageIDSpace, kZMSavePrefsCmdMsg,															kIZMediaPrefix + 30 )


#endif //_h_iZMedia_MessageID_
