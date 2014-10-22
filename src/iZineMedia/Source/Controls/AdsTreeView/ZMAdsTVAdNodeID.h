//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Controls/AdsTreeView/ZMAdsTVAdNodeID.h $
//	$Revision: 2725 $
//	$Date: 2011-04-07 09:32:16 +0200 (Thu, 07 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 7-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_ZMAdsTVAdNodeID_
#define _h_ZMAdsTVAdNodeID_
#pragma once

#include "ZMAdsTVNodeID.h"
class ZMAdsTVAdNodeID : public ZMAdsTVNodeID
{
	enum { kNodeType = eTVNodeAd };
public:
						ZMAdsTVAdNodeID();
	virtual				~ZMAdsTVAdNodeID();

	NodeType			GetNodeType() const
							{ return kNodeType; }
protected:
private:
};

#endif //_h_ZMAdsTVAdNodeID_
