//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/Response/CZPGetTaskCategoryListResponse.cpp $
//	$Revision: 2302 $
//	$Date: 2010-10-14 14:18:41 +0200 (Thu, 14 Oct 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 14-10-2010
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
#include "CZPGetTaskCategoryListResponse.h"

#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPGetTaskCategoryListResponse::CZPGetTaskCategoryListResponse()
: AZPSoapResponse( eResp_GetTaskCategoryList )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPGetTaskCategoryListResponse::~CZPGetTaskCategoryListResponse()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetTaskCategoryList
//----------------------------------------------------------------------------------------
ZPIDPairSList &
CZPGetTaskCategoryListResponse::GetTaskCategoryList()
{
	return mTaskCategoryList;
}

//----------------------------------------------------------------------------------------
// GetTaskCategoryList
//----------------------------------------------------------------------------------------
const ZPIDPairSList &
CZPGetTaskCategoryListResponse::GetTaskCategoryList()const
{
	return mTaskCategoryList;
}
