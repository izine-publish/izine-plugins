//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/Response/CZPAcquireLockResponse.cpp $
//	$Revision: 1458 $
//	$Date: 2010-06-16 12:41:35 +0200 (Wed, 16 Jun 2010) $
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
#include "CZPAcquireLockResponse.h"

#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPAcquireLockResponse::CZPAcquireLockResponse()
: AZPSoapResponse( eResp_AcquireLock )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAcquireLockResponse::~CZPAcquireLockResponse()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetLockInfo
//----------------------------------------------------------------------------------------
const ZPAssetLockInfo &
CZPAcquireLockResponse::GetLockInfo()const
{
	return mLockInfo;
}

//----------------------------------------------------------------------------------------
// GetLockInfo
//----------------------------------------------------------------------------------------
ZPAssetLockInfo &
CZPAcquireLockResponse::GetLockInfo()
{
	return mLockInfo;
}

//----------------------------------------------------------------------------------------
// SetLockInfo
//----------------------------------------------------------------------------------------
void
CZPAcquireLockResponse::SetLockInfo(
	const ZPAssetLockInfo &		inLockInfo)
{
	mLockInfo = inLockInfo;
}
