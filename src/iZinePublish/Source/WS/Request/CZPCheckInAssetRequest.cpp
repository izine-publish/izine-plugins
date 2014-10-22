//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/Request/CZPCheckInAssetRequest.cpp $
//	$Revision: 1777 $
//	$Date: 2010-07-20 08:29:01 +0200 (Tue, 20 Jul 2010) $
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

#include "CZPCheckInAssetRequest.h"
#include "CZPCheckInAssetResponse.h"
#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPCheckInAssetRequest::CZPCheckInAssetRequest()
: AZPSoapRequest( eReq_CheckInAsset )
, mHeadVersion(0)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPCheckInAssetRequest::~CZPCheckInAssetRequest()
{

}

#pragma mark -
#ifdef Debug_WS
//----------------------------------------------------------------------------------------
// GetName
//----------------------------------------------------------------------------------------
const char *
CZPCheckInAssetRequest::GetName() const
{
	return "Check-In Asset";
}
#endif

//----------------------------------------------------------------------------------------
// CreateResponseObj
//----------------------------------------------------------------------------------------
AZPSoapResponse *
CZPCheckInAssetRequest::CreateResponseObj()
{
	return new CZPCheckInAssetResponse;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetAssetID
//----------------------------------------------------------------------------------------
const PMString &
CZPCheckInAssetRequest::GetAssetID()const
{
	return mAssetID;
}

//----------------------------------------------------------------------------------------
// SetAssetID
//----------------------------------------------------------------------------------------
void
CZPCheckInAssetRequest::SetAssetID(
	const PMString &			inAssetID)
{
	mAssetID = inAssetID;
}

//----------------------------------------------------------------------------------------
// GetStatusID
//----------------------------------------------------------------------------------------
const PMString &
CZPCheckInAssetRequest::GetStatusID()const
{
	return mStatusID;
}

//----------------------------------------------------------------------------------------
// GetNewVersionID
//----------------------------------------------------------------------------------------
const PMString &
CZPCheckInAssetRequest::GetNewVersionID()const
{
	return mNewVersionID;
}

//----------------------------------------------------------------------------------------
// SetNewVersionID
//----------------------------------------------------------------------------------------
void
CZPCheckInAssetRequest::SetNewVersionID(
	const PMString &			inNewVersionID)
{
	mNewVersionID = inNewVersionID;
}

//----------------------------------------------------------------------------------------
// SetStatusID
//----------------------------------------------------------------------------------------
void
CZPCheckInAssetRequest::SetStatusID(
	const PMString &			inStatusID)
{
	mStatusID = inStatusID;
}

//----------------------------------------------------------------------------------------
// GetComment
//----------------------------------------------------------------------------------------
const PMString &
CZPCheckInAssetRequest::GetComment()const
{
	return mComment;
}

//----------------------------------------------------------------------------------------
// SetComment
//----------------------------------------------------------------------------------------
void
CZPCheckInAssetRequest::SetComment(
	const PMString &			inComment)
{
	mComment = inComment;
}

//----------------------------------------------------------------------------------------
// GetHeadVersion
//----------------------------------------------------------------------------------------
int
CZPCheckInAssetRequest::GetHeadVersion()const
{
	return mHeadVersion;
}

//----------------------------------------------------------------------------------------
// SetHeadVersion
//----------------------------------------------------------------------------------------
void
CZPCheckInAssetRequest::SetHeadVersion(
	int							inHeadVersion)
{
	mHeadVersion = inHeadVersion;
}


