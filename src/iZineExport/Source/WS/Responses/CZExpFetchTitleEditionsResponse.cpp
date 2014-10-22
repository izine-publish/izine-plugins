//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/WS/Responses/CZExpFetchTitleEditionsResponse.cpp $
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
#include "K2Vector.tpp"

//OS / ThirdParty includes

#include "IZExpID.h"

//IZP Interfaces

//IZP General includes
#include "CZExpFetchTitleEditionsResponse.h"

#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZExpFetchTitleEditionsResponse::CZExpFetchTitleEditionsResponse()
: AZExpWSResponse( eResp_FetchTitleEditions )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZExpFetchTitleEditionsResponse::~CZExpFetchTitleEditionsResponse()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetEditionList
//----------------------------------------------------------------------------------------
const ZExpIDPairList &
CZExpFetchTitleEditionsResponse::GetEditionList()const
{
	return mEditionList;
}

//----------------------------------------------------------------------------------------
// GetEditionList
//----------------------------------------------------------------------------------------
ZExpIDPairList &
CZExpFetchTitleEditionsResponse::GetEditionList()
{
	return mEditionList;
}
