//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/Request/CZPUpdateAssetStatusRequest.cpp $
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

#include "CZPUpdateAssetStatusRequest.h"
#include "CZPUpdateAssetStatusResponse.h"
#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPUpdateAssetStatusRequest::CZPUpdateAssetStatusRequest()
: AZPSoapRequest( eReq_UpdateAssetStatus )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPUpdateAssetStatusRequest::~CZPUpdateAssetStatusRequest()
{

}

#pragma mark -
#ifdef Debug_WS
//----------------------------------------------------------------------------------------
// GetName
//----------------------------------------------------------------------------------------
const char *
CZPUpdateAssetStatusRequest::GetName() const
{
	return "";
}
#endif

//----------------------------------------------------------------------------------------
// CreateResponseObj
//----------------------------------------------------------------------------------------
AZPSoapResponse *
CZPUpdateAssetStatusRequest::CreateResponseObj()
{
	return new CZPUpdateAssetStatusResponse;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetAssetID
//----------------------------------------------------------------------------------------
const PMString &
CZPUpdateAssetStatusRequest::GetAssetID()const
{
	return mAssetID;
}

//----------------------------------------------------------------------------------------
// SetAssetID
//----------------------------------------------------------------------------------------
void
CZPUpdateAssetStatusRequest::SetAssetID(
	const PMString &			inAssetID)
{
	mAssetID = inAssetID;
}

//----------------------------------------------------------------------------------------
// GetOldStatusID
//----------------------------------------------------------------------------------------
const PMString &
CZPUpdateAssetStatusRequest::GetOldStatusID()const
{
	return mOldStatusID;
}

//----------------------------------------------------------------------------------------
// SetOldStatusID
//----------------------------------------------------------------------------------------
void
CZPUpdateAssetStatusRequest::SetOldStatusID(
	const PMString &			inOldStatusID)
{
	mOldStatusID = inOldStatusID;
}

//----------------------------------------------------------------------------------------
// GetNewStatusID
//----------------------------------------------------------------------------------------
const PMString &
CZPUpdateAssetStatusRequest::GetNewStatusID()const
{
	return mNewStatusID;
}

//----------------------------------------------------------------------------------------
// SetNewStatusID
//----------------------------------------------------------------------------------------
void
CZPUpdateAssetStatusRequest::SetNewStatusID(
	const PMString &			inNewStatusID)
{
	mNewStatusID = inNewStatusID;
}

