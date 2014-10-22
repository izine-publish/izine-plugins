//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Controls/AdsTreeView/ZMAdsTVAdFileNodeID.h $
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
#ifndef _h_ZMAdsTVAdFileNodeID_
#define _h_ZMAdsTVAdFileNodeID_
#pragma once

#include "ZMAdsTVNodeID.h"
class ZMAdsTVAdFileNodeID : public ZMAdsTVNodeID
{
	enum { kNodeType = eTVNodeAdFile };
public:
						ZMAdsTVAdFileNodeID();
	virtual				~ZMAdsTVAdFileNodeID();

	NodeType			GetNodeType() const
							{ return kNodeType; }
protected:
private:
};

#endif //_h_ZMAdsTVAdFileNodeID_
