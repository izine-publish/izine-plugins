//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/ZPPresistDefaults.h $
//	$Revision: 3534 $
//	$Date: 2011-07-25 09:28:27 +0200 (Mon, 25 Jul 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 31-5-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_ZPPresistDefaults_
#define _h_ZPPresistDefaults_
#pragma once

#define kZPWSConfigDefault_UseProxy				kFalse
#define kZPWSConfigDefault_ProxyPort			0

#define kZPPrefDefault_AssetFetchInterval		10
#define kZPPrefDefault_AutoUpdateLinkedStories	kTrue
#define kZPPrefDefault_AutoUnlockLinkedStories	kTrue
#define kZPPrefDefault_CreateAssignmentOnStoryImport kTrue
#define kZPPrefDefault_IncludeAllAssignmentsOnDocumentCheckin kTrue
#define kZPPrefDefault_RemoveStyles				kTrue

//Not persist yet
#define kZPPrefDefault_NewAsgnExpOption			2	//IAssignment::kAssignmentExportAll
#define kZPPrefDefault_NewAsgnIncludeLinked		kTrue

#endif //_h_ZPPresistDefaults_
