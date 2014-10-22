//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/ZPDataMgr/IZPAssetStatusMgr.h $
//	$Revision: 1180 $
//	$Date: 2010-05-19 13:26:10 +0200 (Wed, 19 May 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 29-3-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#ifndef _h_IZPAssetStatusMgr_
#define _h_IZPAssetStatusMgr_
#pragma once

class IZPAssetStatusMgr : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPASSETSTATUSMGR };

	//for internal use only
	virtual void		Shutdown() = 0;
};

#endif //_h_IZPAssetStatusMgr_
