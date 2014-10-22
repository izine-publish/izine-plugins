//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/Request/CZPFetchAssetListRequest.cpp $
//	$Revision: 2236 $
//	$Date: 2010-10-05 16:05:53 +0200 (Tue, 05 Oct 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 4-5-2010
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
#include "CZPFetchAssetListRequest.h"
#include "CZPFetchAssetListResponse.h"

#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPFetchAssetListRequest::CZPFetchAssetListRequest()
: AZPSoapRequest( eReq_FetchAssetList )
{
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPFetchAssetListRequest::~CZPFetchAssetListRequest()
{
}

#pragma mark -
#pragma mark -
#ifdef Debug_WS
//----------------------------------------------------------------------------------------
// GetName
//----------------------------------------------------------------------------------------
const char *
CZPFetchAssetListRequest::GetName()const
{
	return "Fetch Asset List";
}
#endif
//----------------------------------------------------------------------------------------
// CreateResponseObj
//----------------------------------------------------------------------------------------
AZPSoapResponse *
CZPFetchAssetListRequest::CreateResponseObj()
{
	return new CZPFetchAssetListResponse;
}

#pragma mark -

//----------------------------------------------------------------------------------------
// GetTitleID
//----------------------------------------------------------------------------------------
const PMString &
CZPFetchAssetListRequest::GetEditionID()const
{
	return mEditionID;
}

//----------------------------------------------------------------------------------------
// SetTitleID
//----------------------------------------------------------------------------------------
void
CZPFetchAssetListRequest::SetEditionID(
	const PMString &			inEditionID)
{
	mEditionID = inEditionID;
}


//----------------------------------------------------------------------------------------
// GetAssetTypes
//----------------------------------------------------------------------------------------
const K2Vector<PMString>&
CZPFetchAssetListRequest::GetAssetTypes()const
{
	return mAssetTypes;
}

//----------------------------------------------------------------------------------------
// SetAssetTypes
//----------------------------------------------------------------------------------------
void
CZPFetchAssetListRequest::SetAssetTypes(
	const K2Vector<PMString>&	inAssetTypes)
{
	mAssetTypes = inAssetTypes;
}
