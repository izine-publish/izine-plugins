//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/Request/CZPUpdateAssetRequest.cpp $
//	$Revision: 1984 $
//	$Date: 2010-08-17 16:09:39 +0200 (Tue, 17 Aug 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 17-8-2010
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

#include "CZPUpdateAssetRequest.h"
#include "CZPUpdateAssetResponse.h"
#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPUpdateAssetRequest::CZPUpdateAssetRequest()
: CZPCreateAssetRequest( eReq_UpdateAsset )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPUpdateAssetRequest::~CZPUpdateAssetRequest()
{

}

#pragma mark -
#ifdef Debug_WS
//----------------------------------------------------------------------------------------
// GetName
//----------------------------------------------------------------------------------------
const char *
CZPUpdateAssetRequest::GetName() const
{
	return "Update Asset";
}
#endif

//----------------------------------------------------------------------------------------
// CreateResponseObj
//----------------------------------------------------------------------------------------
AZPSoapResponse *
CZPUpdateAssetRequest::CreateResponseObj()
{
	return new CZPUpdateAssetResponse;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetAssetID
//----------------------------------------------------------------------------------------
const PMString &
CZPUpdateAssetRequest::GetAssetID()const
{
	return mAssetID;
}

//----------------------------------------------------------------------------------------
// SetAssetID
//----------------------------------------------------------------------------------------
void
CZPUpdateAssetRequest::SetAssetID(
	const PMString &			inAssetID)
{
	mAssetID = inAssetID;
}