//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/Request/CZPGetTaskCategoryListRequest.cpp $
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

#include "CZPGetTaskCategoryListRequest.h"
#include "CZPGetTaskCategoryListResponse.h"
#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPGetTaskCategoryListRequest::CZPGetTaskCategoryListRequest()
: AZPSoapRequest( eReq_GetTaskCategoryList )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPGetTaskCategoryListRequest::~CZPGetTaskCategoryListRequest()
{

}

#pragma mark -
#ifdef Debug_WS
//----------------------------------------------------------------------------------------
// GetName
//----------------------------------------------------------------------------------------
const char *
CZPGetTaskCategoryListRequest::GetName() const
{
	return "";
}
#endif

//----------------------------------------------------------------------------------------
// CreateResponseObj
//----------------------------------------------------------------------------------------
AZPSoapResponse *
CZPGetTaskCategoryListRequest::CreateResponseObj()
{
	return new CZPGetTaskCategoryListResponse;
}

#pragma mark -
