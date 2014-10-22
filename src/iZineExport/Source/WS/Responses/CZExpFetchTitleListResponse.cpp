//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/WS/Responses/CZExpFetchTitleListResponse.cpp $
//	$Revision: 3228 $
//	$Date: 2011-06-14 09:41:01 +0200 (Tue, 14 Jun 2011) $
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
#include "K2Vector.tpp"

//OS / ThirdParty includes

#include "IZExpID.h"

//IZP Interfaces

//IZP General includes
#include "CZExpFetchTitleListResponse.h"

#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZExpFetchTitleListResponse::CZExpFetchTitleListResponse()
: AZExpWSResponse( eResp_FetchTitleList )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZExpFetchTitleListResponse::~CZExpFetchTitleListResponse()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetTitleList
//----------------------------------------------------------------------------------------
const ZExpIDPairList &
CZExpFetchTitleListResponse::GetTitleList()const
{
	return mTitleList;
}

//----------------------------------------------------------------------------------------
// GetTitleList
//----------------------------------------------------------------------------------------
ZExpIDPairList &
CZExpFetchTitleListResponse::GetTitleList()
{
	return mTitleList;
}
