//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/ZPData/IZPAssetData.h $
//	$Revision: 1531 $
//	$Date: 2010-06-24 12:41:48 +0200 (Thu, 24 Jun 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 1-6-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description: Data stored with Asset document/datalink
//========================================================================================
#ifndef _h_IZPAssetData_
#define _h_IZPAssetData_
#pragma once


class IZPAssetData : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPASSETDATA };

	virtual void				Copy(
									const IZPAssetData *		inValuesFrom ) = 0;
	
	virtual const PMString &	GetAssetID() const = 0;
	virtual void				SetAssetID(
									const PMString &			inAssetID ) = 0;

	virtual const PMString &	GetTitleID() const = 0;
	virtual void				SetTitleID(
									const PMString &			inTitleID ) = 0;

	virtual const PMString &	GetEditionID() const = 0;
	virtual void				SetEditionID(
									const PMString &			inEditionID ) = 0;

	virtual void				Clear() = 0;
};


namespace ZPCommandHelper
{

	ErrorCode			SaveAssetData(					//Used for persist impl
							const IPMUnknown *			inTarget,
							const PMString &			inAssetID,
							const PMString &			inTitleID,
							const PMString &			inEditionID );
}

#endif //_h_IZPAssetData_
