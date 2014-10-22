//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/Response/CZPFetchTitleStatusResponse.cpp $
//	$Revision: 1111 $
//	$Date: 2010-05-05 10:56:48 +0200 (Wed, 05 May 2010) $
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
#include "CZPFetchTitleStatusResponse.h"

#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPFetchTitleStatusResponse::CZPFetchTitleStatusResponse()
:AZPSoapResponse(eResp_FetchTitleStatus)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPFetchTitleStatusResponse::~CZPFetchTitleStatusResponse()
{

}

#pragma mark -

//----------------------------------------------------------------------------------------
// GetInfoList
//----------------------------------------------------------------------------------------
ZPTitleStatusInfoSList &
CZPFetchTitleStatusResponse::GetInfoList()
{
	return mInfoList;
}

//----------------------------------------------------------------------------------------
// GetInfoList
//----------------------------------------------------------------------------------------
const ZPTitleStatusInfoSList &
CZPFetchTitleStatusResponse::GetInfoList()const
{
	return mInfoList;
}


