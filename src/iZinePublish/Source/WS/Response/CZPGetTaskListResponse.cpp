//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/Response/CZPGetTaskListResponse.cpp $
//	$Revision: 2240 $
//	$Date: 2010-10-06 12:33:06 +0200 (Wed, 06 Oct 2010) $
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
#include "CZPGetTaskListResponse.h"

#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPGetTaskListResponse::CZPGetTaskListResponse()
: AZPSoapResponse( eResp_GetTaskList )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPGetTaskListResponse::~CZPGetTaskListResponse()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetTaskIDPairList
//----------------------------------------------------------------------------------------
ZPIDPairSList &
CZPGetTaskListResponse::GetTaskIDPairList()
{
	return mTaskList;
}

//----------------------------------------------------------------------------------------
// GetTaskIDPairList
//----------------------------------------------------------------------------------------
const ZPIDPairSList &
CZPGetTaskListResponse::GetTaskIDPairList()const
{
	return mTaskList;
}

//----------------------------------------------------------------------------------------
// GetTaskInfoList
//----------------------------------------------------------------------------------------
ZPTaskInfoSList &
CZPGetTaskListResponse::GetTaskInfoList()
{
	return mTaskInfoList;
}

//----------------------------------------------------------------------------------------
// GetTaskInfoList
//----------------------------------------------------------------------------------------
const ZPTaskInfoSList &
CZPGetTaskListResponse::GetTaskInfoList()const
{
	return mTaskInfoList;
}
