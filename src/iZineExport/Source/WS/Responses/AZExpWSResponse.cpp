//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/WS/Responses/AZExpWSResponse.cpp $
//	$Revision: 3223 $
//	$Date: 2011-06-14 08:58:14 +0200 (Tue, 14 Jun 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 19-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes

#include "IZExpID.h"

//IZP Interfaces

//IZP General includes
#include "AZExpWSResponse.h"

#include "IZPLog.h"

#pragma mark -
#define thisFileLA	enLMA_WS
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )
int gResponseObjectCount = 0;

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
AZExpWSResponse::AZExpWSResponse(
	enResponseType						inRespType)
: mRespType( inRespType )
, mErrorCode(0)
, mErrorDescription( NULL )
, mErrorDetails( NULL )
{
	++gResponseObjectCount;
	IZPLog_Str ( thisFileLA, enLT_ObjectCreations, "Object created at : %08X\nLive response objects: %d", this, gResponseObjectCount );

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
AZExpWSResponse::~AZExpWSResponse()
{
	--gResponseObjectCount;
	IZPLog_Str ( thisFileLA, enLT_ObjectCreations, "Object destoyed at : %08X\nLive response objects: %d", this, gResponseObjectCount );
}


//----------------------------------------------------------------------------------------
// GetErrorCode
//----------------------------------------------------------------------------------------
const int
AZExpWSResponse::GetErrorCode()const
{
	return mErrorCode;
}

//----------------------------------------------------------------------------------------
// SetErrorCode
//----------------------------------------------------------------------------------------
void
AZExpWSResponse::SetErrorCode(
	const int					inErrorCode)
{
	mErrorCode = inErrorCode;
}

//----------------------------------------------------------------------------------------
// GetErrorDescription
//----------------------------------------------------------------------------------------
const char *
AZExpWSResponse::GetErrorDescription()const
{
	return mErrorDescription();
}

//----------------------------------------------------------------------------------------
// SetErrorDescription
//----------------------------------------------------------------------------------------
void
AZExpWSResponse::SetErrorDescription(
	const char *				inErrorDescription)
{
	mErrorDescription.SetStr( inErrorDescription );
}

//----------------------------------------------------------------------------------------
// GetErrorDetails
//----------------------------------------------------------------------------------------
const char *
AZExpWSResponse::GetErrorDetails()const
{
	return mErrorDetails();
}

//----------------------------------------------------------------------------------------
// SetErrorDetails
//----------------------------------------------------------------------------------------
void
AZExpWSResponse::SetErrorDetails(
	const char *				inErrorDetails)
{
	mErrorDetails.SetStr( inErrorDetails );
}

//----------------------------------------------------------------------------------------
// GetRespType
//----------------------------------------------------------------------------------------
const enResponseType
AZExpWSResponse::GetRespType()const
{
	return mRespType;
}
