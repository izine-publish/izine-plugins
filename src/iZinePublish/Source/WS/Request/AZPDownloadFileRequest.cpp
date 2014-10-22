//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/Request/AZPDownloadFileRequest.cpp $
//	$Revision: 1835 $
//	$Date: 2010-07-26 10:35:29 +0200 (Mon, 26 Jul 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 26-7-2010
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
#include "AZPDownloadFileRequest.h"

#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
AZPDownloadFileRequest::AZPDownloadFileRequest(
	enRequestType				inReqType)
: AZPSoapRequest( inReqType )
, mNeedDecompression( false )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
AZPDownloadFileRequest::~AZPDownloadFileRequest()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetOutputFile
//----------------------------------------------------------------------------------------
const IDFile &
AZPDownloadFileRequest::GetOutputFile()const
{
	return mOutputFile;
}

//----------------------------------------------------------------------------------------
// SetOutputFile
//----------------------------------------------------------------------------------------
void
AZPDownloadFileRequest::SetOutputFile(
	const IDFile &				inOutputFile)
{
	mOutputFile = inOutputFile;
}

//----------------------------------------------------------------------------------------
// GetNeedDecompression
//----------------------------------------------------------------------------------------
const bool
AZPDownloadFileRequest::GetNeedDecompression()const
{
	return mNeedDecompression;
}

//----------------------------------------------------------------------------------------
// SetNeedDecompression
//----------------------------------------------------------------------------------------
void
AZPDownloadFileRequest::SetNeedDecompression(
	const bool					inNeedDecompression)
{
	mNeedDecompression = inNeedDecompression;
}


