//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/Request/CZPFetchTitleStatusRequest.cpp $
//	$Revision: 1122 $
//	$Date: 2010-05-06 13:02:46 +0200 (Thu, 06 May 2010) $
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
#include "CZPFetchTitleStatusRequest.h"
#include "CZPFetchTitleStatusResponse.h"

#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPFetchTitleStatusRequest::CZPFetchTitleStatusRequest()
: AZPSoapRequest( eReq_FetchTitleStatus)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPFetchTitleStatusRequest::~CZPFetchTitleStatusRequest()
{

}

#pragma mark -
#ifdef Debug_WS
//----------------------------------------------------------------------------------------
// GetName
//----------------------------------------------------------------------------------------
const char *
CZPFetchTitleStatusRequest::GetName()const
{
	return "Fetch title status";
}
#endif
//----------------------------------------------------------------------------------------
// CreateResponseObj
//----------------------------------------------------------------------------------------
AZPSoapResponse *
CZPFetchTitleStatusRequest::CreateResponseObj()
{
	return new CZPFetchTitleStatusResponse;
}

//----------------------------------------------------------------------------------------
// GetTitleId
//----------------------------------------------------------------------------------------
const PMString &
CZPFetchTitleStatusRequest::GetTitleId()const
{
	return mTitleId;
}

//----------------------------------------------------------------------------------------
// SetTitleId
//----------------------------------------------------------------------------------------
void
CZPFetchTitleStatusRequest::SetTitleId(
	const PMString &			inTitleId)
{
	mTitleId = inTitleId;
}