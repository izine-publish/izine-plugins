//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/Request/CZPFetchAssetsRequest.cpp $
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

#include "CZPFetchAssetsRequest.h"
#include "CZPFetchAssetsResponse.h"
#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPFetchAssetsRequest::CZPFetchAssetsRequest()
: AZPSoapRequest( eReq_FetchAssets )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPFetchAssetsRequest::~CZPFetchAssetsRequest()
{

}

#pragma mark -
#ifdef Debug_WS
//----------------------------------------------------------------------------------------
// GetName
//----------------------------------------------------------------------------------------
const char *
CZPFetchAssetsRequest::GetName() const
{
	return "Fetch Asset";
}
#endif

//----------------------------------------------------------------------------------------
// CreateResponseObj
//----------------------------------------------------------------------------------------
AZPSoapResponse *
CZPFetchAssetsRequest::CreateResponseObj()
{
	return new CZPFetchAssetsResponse;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetAssetIDs
//----------------------------------------------------------------------------------------
PMStringList &
CZPFetchAssetsRequest::GetAssetIDs()
{
	return mAssetIDs;
}

//----------------------------------------------------------------------------------------
// GetAssetIDs
//----------------------------------------------------------------------------------------
const PMStringList &
CZPFetchAssetsRequest::GetAssetIDs()const
{
	return mAssetIDs;
}
//----------------------------------------------------------------------------------------
// SetAssetIDs
//----------------------------------------------------------------------------------------
void
CZPFetchAssetsRequest::SetAssetIDs(
	const ZPAssetIDsList &		inAssetIDs)
{
	mAssetIDs = inAssetIDs;
}