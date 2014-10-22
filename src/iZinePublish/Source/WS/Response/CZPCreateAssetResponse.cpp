//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/Response/CZPCreateAssetResponse.cpp $
//	$Revision: 1768 $
//	$Date: 2010-07-19 09:29:32 +0200 (Mon, 19 Jul 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 26-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces

//IZP General includes
#include "CZPCreateAssetResponse.h"

#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPCreateAssetResponse::CZPCreateAssetResponse()
: AZPSoapResponse( eResp_CreateAsset )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPCreateAssetResponse::~CZPCreateAssetResponse()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetAssetID
//----------------------------------------------------------------------------------------
const PMString &
CZPCreateAssetResponse::GetAssetID()const
{
	return mAssetInfo.mAsset.mID;
}

//----------------------------------------------------------------------------------------
// SetAssetID
//----------------------------------------------------------------------------------------
void
CZPCreateAssetResponse::SetAssetID(
	const PMString &			inAssetID)
{
	mAssetInfo.mAsset.mID = inAssetID;
}

//----------------------------------------------------------------------------------------
// GetName
//----------------------------------------------------------------------------------------
const PMString &
CZPCreateAssetResponse::GetName()const
{
	return mAssetInfo.mAsset.mName;
}

//----------------------------------------------------------------------------------------
// SetName
//----------------------------------------------------------------------------------------
void
CZPCreateAssetResponse::SetName(
	const PMString &			inName)
{
	mAssetInfo.mAsset.mName = inName;
}

//----------------------------------------------------------------------------------------
// GetEditionID
//----------------------------------------------------------------------------------------
const PMString &
CZPCreateAssetResponse::GetEditionID()const
{
	return mAssetInfo.mEditionID;
}

//----------------------------------------------------------------------------------------
// SetEditionID
//----------------------------------------------------------------------------------------
void
CZPCreateAssetResponse::SetEditionID(
	const PMString &			inEditionID)
{
	mAssetInfo.mEditionID = inEditionID;
}

//----------------------------------------------------------------------------------------
// GetDetailInfo
//----------------------------------------------------------------------------------------
ZPAssetDetailInfo &
CZPCreateAssetResponse::GetDetailInfo()
{
	return mAssetInfo.mAssetDetailInfo;
}

//----------------------------------------------------------------------------------------
// GetDetailInfo
//----------------------------------------------------------------------------------------
const ZPAssetDetailInfo &
CZPCreateAssetResponse::GetDetailInfo()const
{
	return mAssetInfo.mAssetDetailInfo;
}
