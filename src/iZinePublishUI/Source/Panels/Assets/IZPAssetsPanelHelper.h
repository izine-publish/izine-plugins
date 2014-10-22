//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Panels/Assets/IZPAssetsPanelHelper.h $
//	$Revision: 2462 $
//	$Date: 2010-11-08 11:47:08 +0100 (Mon, 08 Nov 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 8-11-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPAssetsPanelHelper_
#define _h_IZPAssetsPanelHelper_
#pragma once

class IStringData;

class IZPAssetsPanelHelper : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPASSETSPANELHELPER };

	virtual const IStringData *	GetSelectedAsset() const = 0;
};


#endif //_h_IZPAssetsPanelHelper_
