//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/ZPUITypes.h $
//	$Revision: 2962 $
//	$Date: 2011-05-09 11:41:15 +0200 (Mon, 09 May 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 11-10-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_ZPUITypes_
#define _h_ZPUITypes_
#pragma once

enum enTVColTasks
{
	eTVColTasks_Subject = 0,
	eTVColTasks_Status,
	eTVColTasks_AssignedTo,
	eTVColTasks_Document,
	eTVColTasks_Category,
	eTVColTasks_Spread,
	eTVColTasks_Page,
	eTVColTasks_UpdatedBy,
	eTVColTasks_UpdatedOn,
	eTVColTasks_Creator,
	eTVColTasks_CreationDate,
	eTVColTasksCount
};

enum enTVColAssets
{
	eTVColAssets_Expander = 0,
	eTVColAssets_NodeIcon,
	eTVColAssets_LinkTypeIcon,
	eTVColAssets_LockIcon,
	eTVColAssets_Save,
	eTVColAssets_Delete,
	eTVColAssets_AssetName,
	eTVColAssets_Document,
	eTVColAssets_VersionNum,
	eTVColAssets_VersionAuthor,
	eTVColAssets_VersionDate,
	eTVColAssets_LocalState,
	eTVColAssets_Description,
	eTVColAssetsCount
};


typedef double ZPColumnPoint;	//Used to find column left right

#endif //_h_ZPUITypes_
