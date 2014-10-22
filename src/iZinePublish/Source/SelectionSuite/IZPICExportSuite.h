//========================================================================================
//	
//	$HeadURL: https://my.redfive.biz/svn/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/SelectionSuite/IZPICExportSuite.h $
//	$Revision: 2926 $
//	$Date: 2011-05-04 12:17:15 +0530 (Wed, 04 May 2011) $
//	$Author: mindfire $
//	
//	Creator: mindfire
//	Created: 7-5-2012
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#ifndef _h_IZPICExportSuite_
#define _h_IZPICExportSuite_
#pragma once

#include "IPMUnknown.h"
#include "UIDList.h"

class IZPICExportSuite : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPICEXPORTSUITE };
	
	/* get all selected page item uid list */
	virtual void		GetAllSelectedItems(UIDList& uidList) = 0;
	
};


#endif //_h_IZPICExportSuite_