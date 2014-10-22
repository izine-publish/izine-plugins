//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/Response/CZPGetTaskHistoryResponse.cpp $
//	$Revision: 2983 $
//	$Date: 2011-05-11 06:22:34 +0200 (Wed, 11 May 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 18-10-2010
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
#include "CZPGetTaskHistoryResponse.h"

#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPGetTaskHistoryResponse::CZPGetTaskHistoryResponse()
: AZPSoapResponse( eResp_GetTaskHistory )
, mDataPtr( nil ), mOwnsPtr( false )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPGetTaskHistoryResponse::~CZPGetTaskHistoryResponse()
{
	if( mDataPtr && mOwnsPtr )
		delete [] mDataPtr;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetDataPtr
//----------------------------------------------------------------------------------------
const unsigned char *
CZPGetTaskHistoryResponse::GetDataPtr()const
{
	return mDataPtr;
}

//----------------------------------------------------------------------------------------
// SetDataPtr
//----------------------------------------------------------------------------------------
void
CZPGetTaskHistoryResponse::SetDataPtr(
	unsigned char *				inDataPtr,
	bool						inOwnsPtr )
{
	if( mDataPtr && mOwnsPtr )
		delete [] mDataPtr;
	mDataPtr = inDataPtr;
	mOwnsPtr = inOwnsPtr;
}

//----------------------------------------------------------------------------------------
// ForgetDataPtr
//----------------------------------------------------------------------------------------
unsigned char *
CZPGetTaskHistoryResponse::ForgetDataPtr() const
{
	mOwnsPtr = false;
	return mDataPtr;
}
