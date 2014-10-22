//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/SDK/Public/ZMData/IZMFileInfo.h $
//	$Revision: 2706 $
//	$Date: 2011-04-06 14:33:48 +0200 (Wed, 06 Apr 2011) $
//	$Author: aman.alam $
//	
//	Creator: Aman Alam
//	Created: 6-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZMFileInfo_
#define _h_IZMFileInfo_
#pragma once


class IZMFileInfo : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZMFILEINFO };

	virtual uint32		GetLastUpdate() const = 0;
	virtual void		SetLastUpdate(
							uint32									inLastUpdateDate) = 0;
};


#endif //_h_IZMFileInfo_
