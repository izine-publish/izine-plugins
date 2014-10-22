//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/ZPData/IZPAssetLockInfo.h $
//	$Revision: 2134 $
//	$Date: 2010-09-20 16:44:34 +0200 (Mon, 20 Sep 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 15-6-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPAssetLockInfo_
#define _h_IZPAssetLockInfo_
#pragma once

#include "IAMLockable.h"

class IZPAssetLockInfo : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPASSETLOCKINFO };

	virtual const PMString &	GetLockID() const = 0;
	virtual void				SetLockID(
									const PMString &			inLockID ) = 0;
	virtual const PMString &	GetDocumentID() const = 0;
	virtual void				SetDocumentID(
									const PMString &			inDocumentID ) = 0;
	virtual const PMString &	GetDocumentName() const = 0;
	virtual void				SetDocumentName(
									const PMString &			inDocumentName ) = 0;
	virtual const PMString &	GetUserID() const = 0;
	virtual void				SetUserID(
									const PMString &			inUserID ) = 0;
	virtual const PMString &	GetUserName() const = 0;
	virtual void				SetUserName(
									const PMString &			inUserName ) = 0;

	virtual const PMString &	GetAppName() const = 0;
	virtual void				SetAppName(
									const PMString &			inAppName ) = 0;

	virtual void				BroadcastMessage() = 0;

	//Cached Values
	virtual IAMLockable::LockState GetLockState() const = 0;
};


#endif //_h_IZPAssetLockInfo_
