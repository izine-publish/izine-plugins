//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/Request/CZPFetchEditionListRequest.cpp $
//	$Revision: 1203 $
//	$Date: 2010-05-24 08:16:41 +0200 (Mon, 24 May 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 29-4-2010
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
#include "CZPFetchEditionListRequest.h"
#include "CZPFetchEditionListResponse.h"

#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPFetchEditionListRequest::CZPFetchEditionListRequest()
: AZPSoapRequest( eReq_FetchEditionList )
{
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPFetchEditionListRequest::~CZPFetchEditionListRequest()
{
}

#pragma mark -
#pragma mark -
#ifdef Debug_WS
//----------------------------------------------------------------------------------------
// GetName
//----------------------------------------------------------------------------------------
const char *
CZPFetchEditionListRequest::GetName()const
{
	return "Fetch title editions";
}
#endif
//----------------------------------------------------------------------------------------
// CreateResponseObj
//----------------------------------------------------------------------------------------
AZPSoapResponse *
CZPFetchEditionListRequest::CreateResponseObj()
{
	return new CZPFetchEditionListResponse;
}

#pragma mark -

//----------------------------------------------------------------------------------------
// GetTitleId
//----------------------------------------------------------------------------------------
const PMString &
CZPFetchEditionListRequest::GetTitleID()const
{
	return mTitleID;
}

//----------------------------------------------------------------------------------------
// SetTitleId
//----------------------------------------------------------------------------------------
void
CZPFetchEditionListRequest::SetTitleID(
	const PMString &			inTitleID)
{
	mTitleID = inTitleID;
}

