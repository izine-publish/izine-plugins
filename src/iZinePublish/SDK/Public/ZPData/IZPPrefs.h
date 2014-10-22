//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/ZPData/IZPPrefs.h $
//	$Revision: 3534 $
//	$Date: 2011-07-25 09:28:27 +0200 (Mon, 25 Jul 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 13-9-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPPrefs_
#define _h_IZPPrefs_
#pragma once


class IZPPrefs : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPPREFS };
	
	virtual	int			GetAssetMonitorAutoUpdateDuration( ) const = 0;	//In seconds
	virtual void		SetAssetMonitorAutoUpdateDuration(
							int							inDuration ) = 0;

	virtual bool16		GetAutoUpdateLinkedStoriesContent( ) const = 0;
	virtual void		SetAutoUpdateLinkedStoriesContent(
							bool16						inAutoUpdate ) = 0;

	virtual bool16		GetAutoUnlockStories() const = 0;
	virtual void		SetAutoUnlockStories(
							bool16						inAutoUnlockStories ) = 0;

	virtual bool16		GetCreateAssignmentOnStoryImport() const = 0;
	virtual void		SetCreateAssignmentOnStoryImport(
							bool16						inCreateAssignmentOnStoryImport) = 0;

	virtual bool16		GetIncludeAllAssignmentsOnDocumentCheckin() const = 0;
	virtual void		SetIncludeAllAssignmentsOnDocumentCheckin(
							bool16						inIncludeAllAssignmentsOnDocumentCheckin) = 0;

	virtual bool16		GetRemoveStyles() const = 0;
	virtual void		SetRemoveStyles(
							bool16						inRemoveStyles ) = 0;
};

#endif //_h_IZPPrefs_
