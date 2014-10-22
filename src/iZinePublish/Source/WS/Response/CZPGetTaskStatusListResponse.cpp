//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/Response/CZPGetTaskStatusListResponse.cpp $
//	$Revision: 2199 $
//	$Date: 2010-09-29 16:51:26 +0200 (Wed, 29 Sep 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 23-9-2010
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
#include "CZPGetTaskStatusListResponse.h"

#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPGetTaskStatusListResponse::CZPGetTaskStatusListResponse()
: AZPSoapResponse( eResp_GetTaskStatusList )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPGetTaskStatusListResponse::~CZPGetTaskStatusListResponse()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetTaskStatusList
//----------------------------------------------------------------------------------------
ZPIDPairSList &
CZPGetTaskStatusListResponse::GetTaskStatusList()
{
	return mTaskStatusList;
}

//----------------------------------------------------------------------------------------
// GetTaskStatusList
//----------------------------------------------------------------------------------------
const ZPIDPairSList &
CZPGetTaskStatusListResponse::GetTaskStatusList()const
{
	return mTaskStatusList;
}
