//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/Response/CZPFetchAssetResponse.cpp $
//	$Revision: 1241 $
//	$Date: 2010-05-26 12:07:45 +0200 (Wed, 26 May 2010) $
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
#include "CZPFetchAssetResponse.h"

#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPFetchAssetResponse::CZPFetchAssetResponse()
: AZPSoapResponse( eResp_FetchAsset )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPFetchAssetResponse::~CZPFetchAssetResponse()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetName
//----------------------------------------------------------------------------------------
const PMString &
CZPFetchAssetResponse::GetName()const
{
	return mName;
}

//----------------------------------------------------------------------------------------
// SetName
//----------------------------------------------------------------------------------------
void
CZPFetchAssetResponse::SetName(
	const PMString &			inName)
{
	mName = inName;
}

//----------------------------------------------------------------------------------------
// GetEditionID
//----------------------------------------------------------------------------------------
const PMString &
CZPFetchAssetResponse::GetEditionID()const
{
	return mEditionID;
}

//----------------------------------------------------------------------------------------
// SetEditionID
//----------------------------------------------------------------------------------------
void
CZPFetchAssetResponse::SetEditionID(
	const PMString &			inEditionID)
{
	mEditionID = inEditionID;
}

//----------------------------------------------------------------------------------------
// GetDetailInfo
//----------------------------------------------------------------------------------------
ZPAssetDetailInfo &
CZPFetchAssetResponse::GetDetailInfo()
{
	return mDetailInfo;
}

//----------------------------------------------------------------------------------------
// GetDetailInfo
//----------------------------------------------------------------------------------------
const ZPAssetDetailInfo &
CZPFetchAssetResponse::GetDetailInfo()const
{
	return mDetailInfo;
}
