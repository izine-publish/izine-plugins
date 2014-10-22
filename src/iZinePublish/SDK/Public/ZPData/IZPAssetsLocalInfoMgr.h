//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/ZPData/IZPAssetsLocalInfoMgr.h $
//	$Revision: 3156 $
//	$Date: 2011-06-03 15:36:26 +0200 (Fri, 03 Jun 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 2-7-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPAssetsLocalInfoMgr_
#define _h_IZPAssetsLocalInfoMgr_
#pragma once

class IZPAssetLocalInfo;


class IZPAssetsLocalInfoMgr : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPASSETSLOCALINFOMGR };

	virtual const IZPAssetLocalInfo *	QueryInfoForAsset(
							const PMString &			inAssetID,
							bool						inCreateIfNotFound = false) const = 0;
	
	virtual UIDRef		AddInfoForAsset(
							const PMString &			inAssetID ) = 0;
	virtual void		RemoveAssetInfo(
							const PMString &			inAssetID ) = 0;
	virtual void		RemoveAssetInfo(
							K2Vector<UID> &				inAssetUIDList ) = 0;

	virtual void		RemoveMissingLocalAssetsInfo() = 0;		//Local info not exist
	virtual void		ClearCache_OldAssets() = 0;
	virtual void		RemoveMissingCacheLocalInfo() = 0;		//File not exist
};

#endif //_h_IZPAssetsLocalInfoMgr_
