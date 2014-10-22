//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/ZPData/IZPAssetVersionInfoData.h $
//	$Revision: 1376 $
//	$Date: 2010-06-09 13:47:43 +0200 (Wed, 09 Jun 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 9-6-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPAssetVersionInfoData_
#define _h_IZPAssetVersionInfoData_
#pragma once


class IZPAssetVersionInfoData : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPASSETVERSIONINFODATA };

	virtual const PMString &	GetVersionID() const = 0;
	virtual void				SetVersionID(
									const PMString &			inVersionID ) = 0;
	virtual const int			GetVersionNumber() const = 0;
	virtual void				SetVersionNumber(
									const int					inVersionNumber ) = 0;
	virtual const PMString &	GetVersionUserID() const = 0;
	virtual void				SetVersionUserID(
									const PMString &			inVersionUserID ) = 0;
	virtual const PMString &	GetVersionUserName() const = 0;
	virtual void				SetVersionUserName(
									const PMString &			inVersionUserName ) = 0;
};


#endif //_h_IZPAssetVersionInfoData_
