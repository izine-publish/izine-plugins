//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/Response/CZPUpdateAssetStatusResponse.cpp $
//	$Revision: 2929 $
//	$Date: 2011-05-04 11:14:15 +0200 (Wed, 04 May 2011) $
//	$Author: aman.alam $
//	
//	Creator: Aman Alam
//	Created: 4-5-2011
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
#include "CZPUpdateAssetStatusResponse.h"

#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPUpdateAssetStatusResponse::CZPUpdateAssetStatusResponse()
: AZPSoapResponse( eResp_UpdateAssetStatus )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPUpdateAssetStatusResponse::~CZPUpdateAssetStatusResponse()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetIsSuccessful
//----------------------------------------------------------------------------------------
const bool
CZPUpdateAssetStatusResponse::GetIsSuccessful()const
{
	return mIsSuccessful;
}

//----------------------------------------------------------------------------------------
// SetIsSuccessful
//----------------------------------------------------------------------------------------
void
CZPUpdateAssetStatusResponse::SetIsSuccessful(
	const bool					inIsSuccessful)
{
	mIsSuccessful = inIsSuccessful;
}
