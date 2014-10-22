//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/Response/CZPFetchTitleListResponse.cpp $
//	$Revision: 1061 $
//	$Date: 2010-04-26 11:24:57 +0200 (Mon, 26 Apr 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 23-4-2010
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
#include "CZPFetchTitleListResponse.h"

#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPFetchTitleListResponse::CZPFetchTitleListResponse()
: AZPSoapResponse ( eResp_FetchTitleList )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPFetchTitleListResponse::~CZPFetchTitleListResponse()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetTitleKeys
//----------------------------------------------------------------------------------------
const ZPPMStringSList &
CZPFetchTitleListResponse::GetTitleKeys()const
{
	return mTitleKeys;
}

//----------------------------------------------------------------------------------------
// GetTitleKeys
//----------------------------------------------------------------------------------------
ZPPMStringSList &
CZPFetchTitleListResponse::GetTitleKeys()
{
	return mTitleKeys;
}

//----------------------------------------------------------------------------------------
// SetTitleKeys
//----------------------------------------------------------------------------------------
void
CZPFetchTitleListResponse::SetTitleKeys(
	const ZPPMStringSList &		inTitleKeys)
{
	mTitleKeys.Copy( inTitleKeys, true );
}

//----------------------------------------------------------------------------------------
// GetTitleNames
//----------------------------------------------------------------------------------------
const ZPPMStringSList &
CZPFetchTitleListResponse::GetTitleNames()const
{
	return mTitleNames;
}

//----------------------------------------------------------------------------------------
// GetTitleNames
//----------------------------------------------------------------------------------------
ZPPMStringSList &
CZPFetchTitleListResponse::GetTitleNames()
{
	return mTitleNames;
}

//----------------------------------------------------------------------------------------
// SetTitleNames
//----------------------------------------------------------------------------------------
void
CZPFetchTitleListResponse::SetTitleNames(
	const ZPPMStringSList &		inTitleNames)
{
	mTitleNames.Copy( inTitleNames, true );
}

