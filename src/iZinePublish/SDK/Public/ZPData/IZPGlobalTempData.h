//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/ZPData/IZPGlobalTempData.h $
//	$Revision: 3666 $
//	$Date: 2011-11-23 09:17:50 +0100 (Wed, 23 Nov 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 10-6-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPGlobalTempData_
#define _h_IZPGlobalTempData_
#pragma once


class IZPGlobalTempData : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPGLOBALTEMPDATA };
	
	virtual const PMString &	GetLastDraggedAssetID() const = 0;
	virtual void				SetLastDraggedAssetID(
									const PMString &			inLastDraggedAssetID ) = 0;

	virtual const PMString &		GetAssignmentNewStatusID() const = 0;
	virtual void					SetAssignmentNewStatusID(
										const PMString &			inAssignmentNewStatusID ) = 0;

	virtual const bool			GetRemoveStoryNameOnUnlink() const = 0;
	virtual void				SetRemoveStoryNameOnUnlink(
									const bool					inRemoveStoryNameOnUnlink ) = 0;

	virtual const bool			GetSkipAddLink() const = 0;
	virtual void				SetSkipAddLink(
									const bool					inSkipAddLink) = 0;
	virtual const UIDRef		GetStoryUIDRefToSkipAddLink() const = 0;
	virtual void				SetStoryUIDRefToSkipAddLink(
									const UIDRef				inStoryUIDRefToSkipAddLink) = 0;

	virtual const int			GetSkipAddLinkCount() const = 0;
	virtual void				SetSkipAddLinkCount(
									const int					inSkipAddLinkCount) = 0;
};


#endif //_h_IZPGlobalTempData_
