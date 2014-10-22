//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/Request/CZPCreateAssetRequest.cpp $
//	$Revision: 1982 $
//	$Date: 2010-08-17 15:06:40 +0200 (Tue, 17 Aug 2010) $
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

#include "CZPCreateAssetRequest.h"
#include "CZPCreateAssetResponse.h"
#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPCreateAssetRequest::CZPCreateAssetRequest(
	enRequestType				inReqType)
: AZPSoapRequest( inReqType )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPCreateAssetRequest::~CZPCreateAssetRequest()
{

}

#pragma mark -
#ifdef Debug_WS
//----------------------------------------------------------------------------------------
// GetName
//----------------------------------------------------------------------------------------
const char *
CZPCreateAssetRequest::GetName() const
{
	return "Create Asset";
}
#endif

//----------------------------------------------------------------------------------------
// CreateResponseObj
//----------------------------------------------------------------------------------------
AZPSoapResponse *
CZPCreateAssetRequest::CreateResponseObj()
{
	return new CZPCreateAssetResponse;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetName
//----------------------------------------------------------------------------------------
const PMString &
CZPCreateAssetRequest::GetAssetName()const
{
	return mAssetName;
}

//----------------------------------------------------------------------------------------
// SetName
//----------------------------------------------------------------------------------------
void
CZPCreateAssetRequest::SetAssetName(
	const PMString &			inName)
{
	mAssetName = inName;
}

//----------------------------------------------------------------------------------------
// GetEditionID
//----------------------------------------------------------------------------------------
const PMString &
CZPCreateAssetRequest::GetEditionID()const
{
	return mEditionID;
}

//----------------------------------------------------------------------------------------
// SetEditionID
//----------------------------------------------------------------------------------------
void
CZPCreateAssetRequest::SetEditionID(
	const PMString &			inEditionID)
{
	mEditionID = inEditionID;
}

//----------------------------------------------------------------------------------------
// GetDetailInfo
//----------------------------------------------------------------------------------------
ZPAssetDetailInfo &
CZPCreateAssetRequest::GetDetailInfo()
{
	return mDetailInfo;
}

//----------------------------------------------------------------------------------------
// GetDetailInfo
//----------------------------------------------------------------------------------------
const ZPAssetDetailInfo &
CZPCreateAssetRequest::GetDetailInfo()const
{
	return mDetailInfo;
}
