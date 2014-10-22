//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/App/ZPConstants.h $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 6-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_ZPConstants_
#define _h_ZPConstants_
#pragma once

#define MaxSize_AssetID		40
#define MaxSize_EditionID	40
#define MaxSize_TitleID		40
#define MaxSize_VersionID	40
#define MaxSize_StatusID	40
#define MaxSize_AssetType	40
#define MaxSize_PageID		40
#define MaxSize_PageTypeID	40
#define MaxSize_TaskID		40

#define STR_EmptyGUID "00000000-0000-0000-0000-000000000000"
#ifdef ZPBuild
#define ZPEXPORTS PLUGIN_DECL
#else
#define ZPEXPORTS PUBLIC_DECL
#endif

extern ZPEXPORTS const PMString kEmptyGUID;


extern ZPEXPORTS const PMString kAssetType_InDesignTemplateID;
extern ZPEXPORTS const PMString kAssetType_InDesignDocumentID;
extern ZPEXPORTS const PMString kAssetType_InCopyTemplateID;
extern ZPEXPORTS const PMString kAssetType_InCopyDocumentID;
extern ZPEXPORTS const PMString kAssetType_InDesignStoryID;
extern ZPEXPORTS const PMString kAssetType_InCopyAssignmentID;

extern ZPEXPORTS const PMString kAssetFileExt_InDesignTemplate;
extern ZPEXPORTS const PMString kAssetFileExt_InDesignDocument;
extern ZPEXPORTS const PMString kAssetFileExt_InCopyTemplate;
extern ZPEXPORTS const PMString kAssetFileExt_InCopyDocument;
extern ZPEXPORTS const PMString kAssetFileExt_InCopyAssignment;

extern ZPEXPORTS const PMString kPageTypeLeft;
extern ZPEXPORTS const PMString kPageTypeRight;
extern ZPEXPORTS const PMString kPageTypeUnisex;

extern ZPEXPORTS const PMString kStatusState_Design;
extern ZPEXPORTS const PMString kStatusState_Editing;
extern ZPEXPORTS const PMString kStatusState_Editorial;
extern ZPEXPORTS const PMString kStatusState_Final;

extern ZPEXPORTS const PMString kTitleStatus_Assigned;
extern ZPEXPORTS const PMString kTitleStatus_Unassigned;
extern ZPEXPORTS const PMString kTitleStatus_Template;

#if defined(InDnCS5) || defined(InDnCS5_5)
extern const PMString kURIScheme;
#endif

const int kKeepCacheForDays		=	15;
const int kRunCacheCleanAfterDays	=	3;

#endif //_h_ZPConstants_
