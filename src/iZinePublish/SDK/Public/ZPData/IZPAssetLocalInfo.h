//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/ZPData/IZPAssetLocalInfo.h $
//	$Revision: 1627 $
//	$Date: 2010-07-04 11:28:27 +0200 (Sun, 04 Jul 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 2-7-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPAssetLocalInfo_
#define _h_IZPAssetLocalInfo_
#pragma once


class IZPAssetLocalInfo : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPASSETLOCALINFO };

	virtual int32		GetFileVersionNumber() const = 0;
	
	virtual void		SetFileVersionNumber(
							int32						inVersionNumber ) = 0;

	virtual bool16		GetFileIsLocallyModified() const = 0;
	
	virtual void		SetFileIsLocallyModified(
							bool16						inIsModified ) = 0;
};


#endif //_h_IZPAssetLocalInfo_
