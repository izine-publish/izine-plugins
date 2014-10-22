//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/ZPData/IZPAssetInfo.h $
//	$Revision: 1454 $
//	$Date: 2010-06-16 11:14:14 +0200 (Wed, 16 Jun 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 24-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPAssetInfo_
#define _h_IZPAssetInfo_
#pragma once

#include "ZPWSTypeDefs.h"
#include "ZPTypes.h"

class IZPAssetInfo : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPASSETINFO };

	virtual void		SetAssetDetail(
							const ZPAssetDetailInfo &			inDetail ) = 0;

	virtual ZPAssetDetailInfo &
						GetAssetDetail() = 0;
	virtual const ZPAssetDetailInfo &
						GetAssetDetail() const = 0;

	virtual const PMString &	GetStatusID() const = 0;
	virtual const PMString &	GetLinkedWithDocumentID() const = 0;
	virtual const PMString &	GetLinkedWithDocumentName() const = 0;
	virtual const PMString &	GetDescription() const = 0;
	virtual const PMString &	GetVersionID() const = 0;
	virtual const PMString &	GetVersionAuthor() const = 0;
	virtual int					GetVersionNumber() const = 0;
	virtual const time_t &		GetVersionDate() const = 0;
	virtual const PMString &	GetTypeID() const = 0;
	virtual enAssetType			GetType() const = 0;

	virtual bool		IsTypeIDTemplate() const = 0;
	virtual bool		IsTypeIDDocument() const = 0;
	virtual bool		IsTypeIDStory() const = 0;
	virtual bool		IsTypeICDocument() const = 0;
	virtual bool		IsTypeICTemplate() const = 0;
	virtual bool		IsTypeAssignment() const = 0;
};


#endif //_h_IZPAssetInfo_
