//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/Response/CZPGetUsersForTitleResponse.cpp $
//	$Revision: 2213 $
//	$Date: 2010-10-01 12:44:26 +0200 (Fri, 01 Oct 2010) $
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
#include "CZPGetUsersForTitleResponse.h"

#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPGetUsersForTitleResponse::CZPGetUsersForTitleResponse()
: AZPSoapResponse( eResp_GetUsersForTitle )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPGetUsersForTitleResponse::~CZPGetUsersForTitleResponse()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetTitleUsersList
//----------------------------------------------------------------------------------------
ZPIDPairSList &
CZPGetUsersForTitleResponse::GetTitleUsersList()
{
	return mTitleUsersList;
}

//----------------------------------------------------------------------------------------
// GetTitleUsersList
//----------------------------------------------------------------------------------------
const ZPIDPairSList &
CZPGetUsersForTitleResponse::GetTitleUsersList()const
{
	return mTitleUsersList;
}
