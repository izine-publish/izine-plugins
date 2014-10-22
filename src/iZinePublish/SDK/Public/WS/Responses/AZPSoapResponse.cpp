//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Responses/AZPSoapResponse.cpp $
//	$Revision: 3998 $
//	$Date: 2012-04-02 13:17:17 +0200 (Mon, 02 Apr 2012) $
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

#include "IZPID.h"

//IZP Interfaces

//IZP General includes
#include "AZPSoapResponse.h"

#include "IZPLog.h"

#pragma mark -
#define thisFileLA	enLMA_WS
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )
int gResponseObjectCount = 0;

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
AZPSoapResponse::AZPSoapResponse(
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
AZPSoapResponse::~AZPSoapResponse()
{
	--gResponseObjectCount;
	IZPLog_Str ( thisFileLA, enLT_ObjectCreations, "Object destoyed at : %08X\nLive response objects: %d", this, gResponseObjectCount );
}


//----------------------------------------------------------------------------------------
// GetErrorCode
//----------------------------------------------------------------------------------------
const int
AZPSoapResponse::GetErrorCode()const
{
	return mErrorCode;
}

//----------------------------------------------------------------------------------------
// SetErrorCode
//----------------------------------------------------------------------------------------
void
AZPSoapResponse::SetErrorCode(
	const int					inErrorCode)
{
	mErrorCode = inErrorCode;
}

//----------------------------------------------------------------------------------------
// GetErrorDescription
//----------------------------------------------------------------------------------------
const char *
AZPSoapResponse::GetErrorDescription()const
{
	return mErrorDescription();
}

//----------------------------------------------------------------------------------------
// SetErrorDescription
//----------------------------------------------------------------------------------------
void
AZPSoapResponse::SetErrorDescription(
	const char *				inErrorDescription)
{
	mErrorDescription.SetStr( inErrorDescription );
}

//----------------------------------------------------------------------------------------
// GetErrorDetails
//----------------------------------------------------------------------------------------
const char *
AZPSoapResponse::GetErrorDetails()const
{
	return mErrorDetails();
}

//----------------------------------------------------------------------------------------
// SetErrorDetails
//----------------------------------------------------------------------------------------
void
AZPSoapResponse::SetErrorDetails(
	const char *				inErrorDetails)
{
	mErrorDetails.SetStr( inErrorDetails );
}

//----------------------------------------------------------------------------------------
// SetErrorDetails
//----------------------------------------------------------------------------------------
void
AZPSoapResponse::SetErrorDetails(
	const PMString &			inErrorDetails)
{
	mErrorDetails.SetStr( inErrorDetails.GrabCString(), inErrorDetails.CharCount() );
}

//----------------------------------------------------------------------------------------
// GetRespType
//----------------------------------------------------------------------------------------
const enResponseType
AZPSoapResponse::GetRespType()const
{
	return mRespType;
}
