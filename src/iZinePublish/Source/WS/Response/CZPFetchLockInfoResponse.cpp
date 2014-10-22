//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/Response/CZPFetchLockInfoResponse.cpp $
//	$Revision: 1780 $
//	$Date: 2010-07-20 09:29:49 +0200 (Tue, 20 Jul 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 20-7-2010
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
#include "CZPFetchLockInfoResponse.h"

#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPFetchLockInfoResponse::CZPFetchLockInfoResponse()
: AZPSoapResponse( eResp_FetchLockInfo )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPFetchLockInfoResponse::~CZPFetchLockInfoResponse()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetLockInfoList
//----------------------------------------------------------------------------------------
ZPAssetLockInfoSList &
CZPFetchLockInfoResponse::GetLockInfoList()
{
	return mLockInfoList;
}

//----------------------------------------------------------------------------------------
// GetLockInfoList
//----------------------------------------------------------------------------------------
const ZPAssetLockInfoSList &
CZPFetchLockInfoResponse::GetLockInfoList()const
{
	return mLockInfoList;
}
