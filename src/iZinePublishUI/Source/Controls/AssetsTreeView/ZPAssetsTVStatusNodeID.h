//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/AssetsTreeView/ZPAssetsTVStatusNodeID.h $
//	$Revision: 1077 $
//	$Date: 2010-04-29 10:51:01 +0200 (Thu, 29 Apr 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 29-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_ZPAssetsTVStatusNodeID_
#define _h_ZPAssetsTVStatusNodeID_
#pragma once

#include "ZPAssetsTVNodeID.h"
class ZPAssetsTVStatusNodeID : public ZPAssetsTVNodeID
{
	enum { kNodeType = eTVNodeStatus };
public:
						ZPAssetsTVStatusNodeID();
	virtual				~ZPAssetsTVStatusNodeID();

	NodeType			GetNodeType() const
							{ return kNodeType; }
protected:
private:
};

#endif //_h_ZPAssetsTVStatusNodeID_
