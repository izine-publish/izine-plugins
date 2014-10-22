//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/Request/CZPRelinkAssetRequest.cpp $
//	$Revision: 1932 $
//	$Date: 2010-08-10 13:29:07 +0200 (Tue, 10 Aug 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 10-8-2010
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

#include "CZPRelinkAssetRequest.h"
#include "CZPRelinkAssetResponse.h"
#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPRelinkAssetRequest::CZPRelinkAssetRequest()
: AZPSoapRequest( eReq_RelinkAsset )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPRelinkAssetRequest::~CZPRelinkAssetRequest()
{

}

#pragma mark -
#ifdef Debug_WS
//----------------------------------------------------------------------------------------
// GetName
//----------------------------------------------------------------------------------------
const char *
CZPRelinkAssetRequest::GetName() const
{
	return "Relink-Asset";
}
#endif

//----------------------------------------------------------------------------------------
// CreateResponseObj
//----------------------------------------------------------------------------------------
AZPSoapResponse *
CZPRelinkAssetRequest::CreateResponseObj()
{
	return new CZPRelinkAssetResponse;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetAssetID
//----------------------------------------------------------------------------------------
const PMString &
CZPRelinkAssetRequest::GetAssetID()const
{
	return mAssetID;
}

//----------------------------------------------------------------------------------------
// SetAssetID
//----------------------------------------------------------------------------------------
void
CZPRelinkAssetRequest::SetAssetID(
	const PMString &			inAssetID)
{
	mAssetID = inAssetID;
}

//----------------------------------------------------------------------------------------
// GetLinkedWithAssetID
//----------------------------------------------------------------------------------------
const PMString &
CZPRelinkAssetRequest::GetLinkedWithAssetID()const
{
	return mLinkedWithAssetID;
}

//----------------------------------------------------------------------------------------
// SetLinkedWithAssetID
//----------------------------------------------------------------------------------------
void
CZPRelinkAssetRequest::SetLinkedWithAssetID(
	const PMString &			inLinkedWithAssetID)
{
	mLinkedWithAssetID = inLinkedWithAssetID;
}
