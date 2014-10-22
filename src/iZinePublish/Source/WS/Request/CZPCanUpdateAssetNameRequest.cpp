//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/Request/CZPCanUpdateAssetNameRequest.cpp $
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

#include "CZPCanUpdateAssetNameRequest.h"
#include "CZPCanUpdateAssetNameResponse.h"
#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPCanUpdateAssetNameRequest::CZPCanUpdateAssetNameRequest()
: CZPCreateAssetRequest( eReq_CanUpdateAssetName )
, mAssetID( kNullString )
, mEditionID( kNullString )
, mTypeID( kNullString )
, mParentID( kNullString )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPCanUpdateAssetNameRequest::~CZPCanUpdateAssetNameRequest()
{

}

#pragma mark -
#ifdef Debug_WS
//----------------------------------------------------------------------------------------
// GetName
//----------------------------------------------------------------------------------------
const char *
CZPCanUpdateAssetNameRequest::GetName() const
{
	return "Update Asset Name";
}
#endif

//----------------------------------------------------------------------------------------
// CreateResponseObj
//----------------------------------------------------------------------------------------
AZPSoapResponse *
CZPCanUpdateAssetNameRequest::CreateResponseObj()
{
	return new CZPCanUpdateAssetNameResponse;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetAssetID
//----------------------------------------------------------------------------------------
const PMString &
CZPCanUpdateAssetNameRequest::GetAssetID()const
{
	return mAssetID;
}

//----------------------------------------------------------------------------------------
// SetAssetID
//----------------------------------------------------------------------------------------
void
CZPCanUpdateAssetNameRequest::SetAssetID(
	const PMString &			inAssetID)
{
	mAssetID = inAssetID;
}

//----------------------------------------------------------------------------------------
// GetEditionID
//----------------------------------------------------------------------------------------
const PMString &
CZPCanUpdateAssetNameRequest::GetEditionID()const
{
	return mEditionID;
}

//----------------------------------------------------------------------------------------
// SetEditionID
//----------------------------------------------------------------------------------------
void
CZPCanUpdateAssetNameRequest::SetEditionID(
	const PMString &			inEditionID)
{
	mEditionID = inEditionID;
}

//----------------------------------------------------------------------------------------
// GetTypeID
//----------------------------------------------------------------------------------------
const PMString &
CZPCanUpdateAssetNameRequest::GetTypeID()const
{
	return mTypeID;
}

//----------------------------------------------------------------------------------------
// SetTypeID
//----------------------------------------------------------------------------------------
void
CZPCanUpdateAssetNameRequest::SetTypeID(
	const PMString &			inTypeID)
{
	mTypeID = inTypeID;
}

//----------------------------------------------------------------------------------------
// GetParentID
//----------------------------------------------------------------------------------------
const PMString &
CZPCanUpdateAssetNameRequest::GetParentID()const
{
	return mParentID;
}

//----------------------------------------------------------------------------------------
// SetParentID
//----------------------------------------------------------------------------------------
void
CZPCanUpdateAssetNameRequest::SetParentID(
	const PMString &			inParentID)
{
	mParentID = inParentID;
}
