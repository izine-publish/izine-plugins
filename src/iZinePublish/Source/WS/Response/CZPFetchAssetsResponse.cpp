//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/Response/CZPFetchAssetsResponse.cpp $
//	$Revision: 1827 $
//	$Date: 2010-07-23 14:15:26 +0200 (Fri, 23 Jul 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 23-7-2010
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
#include "CZPFetchAssetsResponse.h"

#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPFetchAssetsResponse::CZPFetchAssetsResponse()
: AZPSoapResponse( eResp_FetchAssets )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPFetchAssetsResponse::~CZPFetchAssetsResponse()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetAssets
//----------------------------------------------------------------------------------------
const ZPAssetInfoSList &
CZPFetchAssetsResponse::GetAssets()const
{
	return mAssets;
}

//----------------------------------------------------------------------------------------
// GetAssets
//----------------------------------------------------------------------------------------
ZPAssetInfoSList &
CZPFetchAssetsResponse::GetAssets()
{
	return mAssets;
}
