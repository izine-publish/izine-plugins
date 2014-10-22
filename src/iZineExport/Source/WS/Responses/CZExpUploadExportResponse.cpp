//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/WS/Responses/CZExpUploadExportResponse.cpp $
//	$Revision: 3232 $
//	$Date: 2011-06-14 10:04:23 +0200 (Tue, 14 Jun 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 14-6-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes

//OS / ThirdParty includes

#include "IZExpID.h"

//IZP Interfaces

//IZP General includes
#include "CZExpUploadExportResponse.h"

#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZExpUploadExportResponse::CZExpUploadExportResponse()
: AZExpWSResponse( eResp_UploadExport )
, mUploadSuccess(false)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZExpUploadExportResponse::~CZExpUploadExportResponse()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetUploadSuccess
//----------------------------------------------------------------------------------------
const bool
CZExpUploadExportResponse::GetUploadSuccess()const
{
	return mUploadSuccess;
}

//----------------------------------------------------------------------------------------
// SetUploadSuccess
//----------------------------------------------------------------------------------------
void
CZExpUploadExportResponse::SetUploadSuccess(
	const bool					inUploadSuccess)
{
	mUploadSuccess = inUploadSuccess;
}
