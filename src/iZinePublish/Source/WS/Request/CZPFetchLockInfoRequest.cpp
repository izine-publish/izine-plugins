//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/Request/CZPFetchLockInfoRequest.cpp $
//	$Revision: 1784 $
//	$Date: 2010-07-20 14:35:42 +0200 (Tue, 20 Jul 2010) $
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

#include "CZPFetchLockInfoRequest.h"
#include "CZPFetchLockInfoResponse.h"
#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPFetchLockInfoRequest::CZPFetchLockInfoRequest()
: AZPSoapRequest( eReq_FetchLockInfo )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPFetchLockInfoRequest::~CZPFetchLockInfoRequest()
{

}

#pragma mark -
#ifdef Debug_WS
//----------------------------------------------------------------------------------------
// GetName
//----------------------------------------------------------------------------------------
const char *
CZPFetchLockInfoRequest::GetName() const
{
	return "Fetch Lock Info";
}
#endif

//----------------------------------------------------------------------------------------
// CreateResponseObj
//----------------------------------------------------------------------------------------
AZPSoapResponse *
CZPFetchLockInfoRequest::CreateResponseObj()
{
	return new CZPFetchLockInfoResponse;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetAssetIDs
//----------------------------------------------------------------------------------------
PMStringList &
CZPFetchLockInfoRequest::GetAssetIDs()
{
	return mAssetIDs;
}

//----------------------------------------------------------------------------------------
// GetAssetIDs
//----------------------------------------------------------------------------------------
const PMStringList &
CZPFetchLockInfoRequest::GetAssetIDs()const
{
	return mAssetIDs;
}
//----------------------------------------------------------------------------------------
// SetAssetIDs
//----------------------------------------------------------------------------------------
void
CZPFetchLockInfoRequest::SetAssetIDs(
	const ZPAssetIDsList &		inAssetIDs)
{
	mAssetIDs = inAssetIDs;
}
