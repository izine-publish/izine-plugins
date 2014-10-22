//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/Response/CZPFetchEditionListResponse.cpp $
//	$Revision: 1115 $
//	$Date: 2010-05-05 11:42:40 +0200 (Wed, 05 May 2010) $
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
#include "CZPFetchEditionListResponse.h"

#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPFetchEditionListResponse::CZPFetchEditionListResponse()
:AZPSoapResponse(eResp_FetchEditionList)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPFetchEditionListResponse::~CZPFetchEditionListResponse()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetInfoList
//----------------------------------------------------------------------------------------
ZPTitleEditionInfoSList &
CZPFetchEditionListResponse::GetInfoList()
{
	return mInfoList;
}


//----------------------------------------------------------------------------------------
// GetInfoList
//----------------------------------------------------------------------------------------
const ZPTitleEditionInfoSList &
CZPFetchEditionListResponse::GetInfoList()const
{
	return mInfoList;
}



