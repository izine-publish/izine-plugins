//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/Response/CZPCanUpdateAssetNameResponse.cpp $
//	$Revision: 3541 $
//	$Date: 2011-07-26 14:33:45 +0200 (Tue, 26 Jul 2011) $
//	$Author: aman.alam $
//	
//	Creator: Aman Alam
//	Created: 26-7-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
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
#include "CZPCanUpdateAssetNameResponse.h"

#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPCanUpdateAssetNameResponse::CZPCanUpdateAssetNameResponse()
: AZPSoapResponse( eResp_CanUpdateAssetName )
, mIsSuccessful( false )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPCanUpdateAssetNameResponse::~CZPCanUpdateAssetNameResponse()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetIsSuccessful
//----------------------------------------------------------------------------------------
const bool
CZPCanUpdateAssetNameResponse::GetIsSuccessful()const
{
	return mIsSuccessful;
}

//----------------------------------------------------------------------------------------
// SetIsSuccessful
//----------------------------------------------------------------------------------------
void
CZPCanUpdateAssetNameResponse::SetIsSuccessful(
	const bool					inIsSuccessful)
{
	mIsSuccessful = inIsSuccessful;
}
