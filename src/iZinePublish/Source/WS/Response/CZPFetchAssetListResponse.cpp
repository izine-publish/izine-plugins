//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/Response/CZPFetchAssetListResponse.cpp $
//	$Revision: 1211 $
//	$Date: 2010-05-24 14:15:47 +0200 (Mon, 24 May 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 4-5-2010
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
#include "CZPFetchAssetListResponse.h"

#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPFetchAssetListResponse::CZPFetchAssetListResponse()
:AZPSoapResponse(eResp_FetchAssetList)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPFetchAssetListResponse::~CZPFetchAssetListResponse()
{

}

#pragma mark -

//----------------------------------------------------------------------------------------
// GetAssetList
//----------------------------------------------------------------------------------------
ZPIDPairSList &
CZPFetchAssetListResponse::GetAssetList()
{
	return mAssetList;
}

//----------------------------------------------------------------------------------------
// GetAssetList
//----------------------------------------------------------------------------------------
const ZPIDPairSList &
CZPFetchAssetListResponse::GetAssetList()const
{
	return mAssetList;
}

//----------------------------------------------------------------------------------------
// GetAssetDetailList
//----------------------------------------------------------------------------------------
ZPAssetDetailSList &
CZPFetchAssetListResponse::GetAssetDetailList()
{
	return mAssetDetailList;
}

//----------------------------------------------------------------------------------------
// GetAssetDetailList
//----------------------------------------------------------------------------------------
const ZPAssetDetailSList &
CZPFetchAssetListResponse::GetAssetDetailList()const
{
	return mAssetDetailList;
}
