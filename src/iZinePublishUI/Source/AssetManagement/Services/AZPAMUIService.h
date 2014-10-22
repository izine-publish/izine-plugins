//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/AssetManagement/Services/AZPAMUIService.h $
//	$Revision: 1960 $
//	$Date: 2010-08-13 12:50:04 +0200 (Fri, 13 Aug 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 10-7-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_AZPAMUIService_
#define _h_AZPAMUIService_
#pragma once

#include "IAMUIService.h"
#include "IAMServiceProvider.h"

class IStringData;

enum enAMUIServiceType
{
	eAMUIServiceType_Document = 0,
	eAMUIServiceType_Assignment,
	eAMUIServiceType_LinkedStory
};

//ID used to store with service data.
#define kZPAMUIServiceType				kIZPUIPrefix + 1

#define inheritClass_ZPAMUIService CPMUnknown<IAMUIService>
class AZPAMUIService : public inheritClass_ZPAMUIService
{
public:
	virtual				~AZPAMUIService();
protected:
						AZPAMUIService(IPMUnknown * boss);

	virtual const IStringData *	GetDBAsset_FromDoc(
							const IManageableAsset *	inAsset ) const;
	virtual const IStringData * GetDBAsset_FromIDStory(
							const IManageableAsset *	inAsset ) const;

	virtual enAMUIServiceType	GetServiceType() const;
#pragma mark -
	bool16				CallVerifyService(
							IManageableAsset *			pAsset,
							IAMServiceProvider::AssetService inServiceID );
private:
};

#endif //_h_AZPAMUIService_
