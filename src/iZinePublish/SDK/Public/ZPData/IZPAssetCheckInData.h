//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/ZPData/IZPAssetCheckInData.h $
//	$Revision: 1306 $
//	$Date: 2010-06-03 07:54:48 +0200 (Thu, 03 Jun 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 3-6-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPAssetCheckInData_
#define _h_IZPAssetCheckInData_
#pragma once


class IZPAssetCheckInData : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPASSETCHECKINDATA };

	virtual const PMString &	GetAssetName() const = 0;
	virtual void				SetAssetName(
									const PMString &		inAssetName ) = 0;
	virtual const PMString &	GetDescription() const = 0;
	virtual void				SetDescription(
									const PMString &		inDescription ) = 0;
	virtual const PMString &	GetStatusID() const = 0;
	virtual void				SetStatusID(
									const PMString &		inStatusID ) = 0;
	virtual const PMString &	GetTypeID() const = 0;
	virtual void				SetTypeID(
									const PMString &		inTypeID ) = 0;
};


#endif //_h_IZPAssetCheckInData_
