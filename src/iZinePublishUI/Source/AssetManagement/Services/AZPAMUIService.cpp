//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/AssetManagement/Services/AZPAMUIService.cpp $
//	$Revision: 3019 $
//	$Date: 2011-05-13 15:47:12 +0200 (Fri, 13 May 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 10-7-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IAMService.h"
#include "IAMServiceData.h"
#include "IAMUIService.h"
#include "IAMSPManager.h"

//SDK General includes
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPUIID.h"
#include "IZPID.h"

//IZP Interfaces
#include "IZPAssetData.h"
#include "IZPAMHelper.h"

//IZP General includes
#include "AZPAMUIService.h"
#include "ZPDataHelper.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AMUIService
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#pragma mark -
//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
AZPAMUIService::AZPAMUIService(IPMUnknown * boss)
: inheritClass_ZPAMUIService(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
AZPAMUIService::~AZPAMUIService()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetDBAsset_FromDoc
//----------------------------------------------------------------------------------------
const IStringData *
AZPAMUIService::GetDBAsset_FromDoc(
	const IManageableAsset *	inAsset) const
{
	const IStringData * toReturn = nil;
	if( !inAsset )
		return toReturn;
	ASSERT( ::GetUID( inAsset ).Get() == ::GetDataBase(inAsset)->GetRootUID() );

	InterfacePtr<const IZPAssetData> docAssetData( inAsset, UseDefaultIID() );
	ASSERT( docAssetData );

	const PMString & assetIDStr = docAssetData->GetAssetID();
	const IStringData * docAssetID = nil;
	if( assetIDStr.IsEmpty() == kFalse )
		docAssetID = ZPDataHelper::GetAssetByID( assetIDStr );
	
	toReturn = docAssetID;

	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetDBAsset_FromIDStory
//----------------------------------------------------------------------------------------
const IStringData *
AZPAMUIService::GetDBAsset_FromIDStory(
	const IManageableAsset *	inAsset) const
{
	const IStringData * toReturn = nil;

	ASSERT( inAsset );

	InterfacePtr<const IZPAssetData> idStoryAssetData( inAsset, UseDefaultIID() );
	ASSERT( idStoryAssetData );

	const PMString & assetIDStr = idStoryAssetData->GetAssetID();
	const IStringData * assetID = nil;
	if( assetIDStr.IsEmpty() == kFalse )
		assetID = ZPDataHelper::GetAssetByID( assetIDStr );
	
	toReturn = assetID;

	return toReturn;
}
//----------------------------------------------------------------------------------------
// GetServiceType
//----------------------------------------------------------------------------------------
enAMUIServiceType
AZPAMUIService::GetServiceType()const
{
	InterfacePtr<const IAMServiceData> selfAMServiceData(this, UseDefaultIID());
	ASSERT( selfAMServiceData );
	enAMUIServiceType toReturn = eAMUIServiceType_Document;
	int32 val = 0;
	selfAMServiceData->Get( kZPAMUIServiceType, val );
	toReturn = (enAMUIServiceType)val;
	return toReturn;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// CallVerifyService
//----------------------------------------------------------------------------------------
bool16
AZPAMUIService::CallVerifyService(
IManageableAsset *			pAsset,
IAMServiceProvider::AssetService inServiceID)
{
	bool16 toReturn = kFalse;

	if ( pAsset != nil && pAsset->HasVersionedContent() )
	{
		ClassID theAssetSPClass = pAsset->GetServiceProviderClass();
		ASSERT( theAssetSPClass != kInvalidClass );

		toReturn = kTrue == Utils<IZPAMHelper>()->CallAMServiceVerifyContext( pAsset, theAssetSPClass, inServiceID );
	}

	return toReturn;
}
