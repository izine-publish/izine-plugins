//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/Common/ZPTypes.h $
//	$Revision: 4096 $
//	$Date: 2013-04-01 09:20:00 +0200 (Mon, 01 Apr 2013) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 31-3-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#ifndef _h_ZPTypes_
#define _h_ZPTypes_
#pragma once

#include <map>
class IStringData;

#if defined(InDnCS5) || defined(InDnCS5_5)
#define PMStringCharPtrParam	PMString::kDontTranslateDuringCall
#else
#define PMStringCharPtrParam	-1,PMString::kNoTranslate
#endif


enum enAppState
{
	eAppState_NotLoggedIn,
	eAppState_Authenticating,
	eAppState_Authenticated,
	eAppState_LoggedIn,
	eAppState_AutoLogout,
	eAppState_LogoutInProgress
};

typedef enum
{
	eUserRole_None,
	eUserRole_User,
	eUserRole_Admin,
	eUserRole_KeyUser,
	eUserRole_Default = eUserRole_User
}enZPUserRole;


enum enAssetType
{
	eAssetTypeNone = -1,
	eAssetType_IDDocument = 0,
	eAssetType_IDTemplate,
	eAssetType_ICTemplate,
	eAssetType_ICDocument,		//Stand alone document, created from InCopy
	eAssetType_ICAssignment,
	eAssetType_IDStory,			//Part of Assignment, created by InDesign
	eAssetTypeCount
};

enum enAssetTypeFlag
{
	eAssetTypeFlagAll = -1,
	eAssetTypeFlag_IDDocument	= 1 << eAssetType_IDDocument,
	eAssetTypeFlag_IDTemplate	= 1 << eAssetType_IDTemplate,
	eAssetTypeFlag_ICTemplate	= 1 << eAssetType_ICTemplate,
	eAssetTypeFlag_ICDocument	= 1 << eAssetType_ICDocument,
	eAssetTypeFlag_ICAssignment	= 1 << eAssetType_ICAssignment,
	eAssetTypeFlag_IDStory		= 1 << eAssetType_IDStory,
};

enum enTitleStatusState
{
	eTitleStatusState_UnKnown,
	eTitleStatusState_Design,
	eTitleStatusState_Editing,
	eTitleStatusState_Editorial,
	eTitleStatusState_Final
};

enum enUpdateAssetLocalInfoFlags
{
	eUpdateAssetLocalInfo_AssetID		= 1 << 1,
	eUpdateAssetLocalInfo_IsModified	= 1 << 2,
	eUpdateAssetLocalInfo_VersionNumber	= 1 << 3
};

#ifdef kIZPPrefix
enum
{
	//Input:
	//Asset passed is document asset.
	//Output:
		//enSelectedFile: Destination file
		//enFollowupAction: followup action
		//Following string values are also set
		//enDocumentName: Asset Name
		//enVersionComment: Description string
		//kZPAMServiceDataAssetID:	Asset ID created by dialog service
		//kZPAMServiceDataTitleID:	
		//kZPAMServiceDataEditionID:
		//kZPAMServiceDataStatusID:	New status selected by user.
	enAMUINewAssignmentService			= kIZPPrefix + 1,
	enAMUIUpdateVersionStatusService
};
#endif

typedef K2Vector<PMString>				PMStringList;
typedef PMStringList::iterator			PMStringListIter;
typedef PMStringList::const_iterator	PMStringListCIter;

class IStringData;

typedef std::map<PMString, const IStringData *>	ZPStatusIDMap;
typedef K2Vector<PMString>					ZPStatusIDsList;
typedef ZPStatusIDMap::iterator				ZPStatusIDMapIter;
typedef ZPStatusIDMap::const_iterator		ZPStatusIDMapConstIter;


typedef std::map<PMString, const IStringData *>	ZPAssetIDMap;
typedef K2Vector<PMString>					ZPAssetIDsList;
typedef ZPAssetIDMap::iterator				ZPAssetIDMapIter;
typedef ZPAssetIDMap::const_iterator		ZPAssetIDMapConstIter;

typedef std::map<PMString, const IStringData *>	ZPTaskIDMap;
typedef K2Vector<PMString>					ZPTaskIDsList;
typedef ZPTaskIDMap::iterator				ZPTaskIDMapIter;
typedef ZPTaskIDMap::const_iterator			ZPTaskIDMapConstIter;

#if defined(InDnCS5) || defined(InDnCS5_5)

typedef std::map<PMString, UIDList> ZPAssgStoriesMap;
typedef std::map<PMString, UIDList>::iterator ZPAssgStoriesMapIter;
#endif
#endif //_h_ZPTypes_
