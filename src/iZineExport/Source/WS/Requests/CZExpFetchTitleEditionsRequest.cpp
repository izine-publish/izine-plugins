//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/WS/Requests/CZExpFetchTitleEditionsRequest.cpp $
//	$Revision: 3230 $
//	$Date: 2011-06-14 10:00:48 +0200 (Tue, 14 Jun 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 14-6-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes

//OS / ThirdParty includes

#include "IZExpID.h"

//IZP Interfaces

//IZP General includes

#include "CZExpFetchTitleEditionsRequest.h"
#include "CZExpFetchTitleEditionsResponse.h"
#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZExpFetchTitleEditionsRequest::CZExpFetchTitleEditionsRequest()
: AZExpWSRequest( eReq_FetchTitleEditions )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZExpFetchTitleEditionsRequest::~CZExpFetchTitleEditionsRequest()
{

}

#pragma mark -
#ifdef Debug_WS
//----------------------------------------------------------------------------------------
// GetName
//----------------------------------------------------------------------------------------
const char *
CZExpFetchTitleEditionsRequest::GetName() const
{
	return "CZExpFetchTitleEditionsRequest";
}
#endif

//----------------------------------------------------------------------------------------
// CreateResponseObj
//----------------------------------------------------------------------------------------
AZExpWSResponse *
CZExpFetchTitleEditionsRequest::CreateResponseObj()
{
	return new CZExpFetchTitleEditionsResponse;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetTitleId
//----------------------------------------------------------------------------------------
const PMString &
CZExpFetchTitleEditionsRequest::GetTitleId()const
{
	return mTitleId;
}

//----------------------------------------------------------------------------------------
// SetTitleId
//----------------------------------------------------------------------------------------
void
CZExpFetchTitleEditionsRequest::SetTitleId(
	const PMString &			inTitleId)
{
	mTitleId = inTitleId;
}



