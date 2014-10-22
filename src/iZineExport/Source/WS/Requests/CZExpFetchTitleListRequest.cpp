//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/WS/Requests/CZExpFetchTitleListRequest.cpp $
//	$Revision: 3223 $
//	$Date: 2011-06-14 08:58:14 +0200 (Tue, 14 Jun 2011) $
//	$Author: rajkumar.sehrawat $
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

#include "CZExpFetchTitleListRequest.h"
#include "CZExpFetchTitleListResponse.h"
#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZExpFetchTitleListRequest::CZExpFetchTitleListRequest()
: AZExpWSRequest( eReq_FetchTitleList )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZExpFetchTitleListRequest::~CZExpFetchTitleListRequest()
{

}

#pragma mark -
#ifdef Debug_WS
//----------------------------------------------------------------------------------------
// GetName
//----------------------------------------------------------------------------------------
const char *
CZExpFetchTitleListRequest::GetName() const
{
	return "CZExpFetchTitleListRequest";
}
#endif

//----------------------------------------------------------------------------------------
// CreateResponseObj
//----------------------------------------------------------------------------------------
AZExpWSResponse *
CZExpFetchTitleListRequest::CreateResponseObj()
{
	return new CZExpFetchTitleListResponse;
}

#pragma mark -
