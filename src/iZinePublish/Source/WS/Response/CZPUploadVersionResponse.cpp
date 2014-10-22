//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/Response/CZPUploadVersionResponse.cpp $
//	$Revision: 3519 $
//	$Date: 2011-07-20 09:06:15 +0200 (Wed, 20 Jul 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 26-5-2010
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
#include "CZPUploadVersionResponse.h"

#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPUploadVersionResponse::CZPUploadVersionResponse()
: AZPSoapResponse( eResp_UploadVersion )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPUploadVersionResponse::~CZPUploadVersionResponse()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetVersionId
//----------------------------------------------------------------------------------------
const PMString &
CZPUploadVersionResponse::GetVersionId()const
{
	return mVersionId;
}

//----------------------------------------------------------------------------------------
// SetVersionId
//----------------------------------------------------------------------------------------
void
CZPUploadVersionResponse::SetVersionId(
	const PMString &			inVersionId)
{
	mVersionId = inVersionId;
}
