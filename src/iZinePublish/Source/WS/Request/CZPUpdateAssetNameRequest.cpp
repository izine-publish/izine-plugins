//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/Request/CZPUpdateAssetNameRequest.cpp $
//	$Revision: 3508 $
//	$Date: 2011-07-19 11:59:58 +0200 (Tue, 19 Jul 2011) $
//	$Author: aman.alam $
//	
//	Creator: Aman Alam
//	Created: 19-7-2011
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

#include "CZPUpdateAssetNameRequest.h"
#include "CZPUpdateAssetNameResponse.h"
#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPUpdateAssetNameRequest::CZPUpdateAssetNameRequest()
: CZPCreateAssetRequest( eReq_UpdateAssetName )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPUpdateAssetNameRequest::~CZPUpdateAssetNameRequest()
{

}

#pragma mark -
#ifdef Debug_WS
//----------------------------------------------------------------------------------------
// GetName
//----------------------------------------------------------------------------------------
const char *
CZPUpdateAssetNameRequest::GetName() const
{
	return "Update Asset Name";
}
#endif

//----------------------------------------------------------------------------------------
// CreateResponseObj
//----------------------------------------------------------------------------------------
AZPSoapResponse *
CZPUpdateAssetNameRequest::CreateResponseObj()
{
	return new CZPUpdateAssetNameResponse;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetAssetID
//----------------------------------------------------------------------------------------
const PMString &
CZPUpdateAssetNameRequest::GetAssetID()const
{
	return mAssetID;
}

//----------------------------------------------------------------------------------------
// SetAssetID
//----------------------------------------------------------------------------------------
void
CZPUpdateAssetNameRequest::SetAssetID(
	const PMString &			inAssetID)
{
	mAssetID = inAssetID;
}