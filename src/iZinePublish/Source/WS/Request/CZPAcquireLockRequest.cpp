//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/Request/CZPAcquireLockRequest.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 26-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IApplication.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces

//IZP General includes

#include "CZPAcquireLockRequest.h"
#include "CZPAcquireLockResponse.h"
#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPAcquireLockRequest::CZPAcquireLockRequest()
: AZPSoapRequest( eReq_AcquireLock )
{
	//Set the app name
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IApplication> application(gSession->QueryApplication());
	ASSERT(application);
			
	this->SetAppName(application->GetApplicationName());
	
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAcquireLockRequest::~CZPAcquireLockRequest()
{

}

#pragma mark -
#ifdef Debug_WS
//----------------------------------------------------------------------------------------
// GetName
//----------------------------------------------------------------------------------------
const char *
CZPAcquireLockRequest::GetName() const
{
	return "Acquire Lock";
}
#endif

//----------------------------------------------------------------------------------------
// CreateResponseObj
//----------------------------------------------------------------------------------------
AZPSoapResponse *
CZPAcquireLockRequest::CreateResponseObj()
{
	return new CZPAcquireLockResponse;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetAssetID
//----------------------------------------------------------------------------------------
const PMString &
CZPAcquireLockRequest::GetAssetID()const
{
	return mAssetID;
}

//----------------------------------------------------------------------------------------
// SetAssetID
//----------------------------------------------------------------------------------------
void
CZPAcquireLockRequest::SetAssetID(
	const PMString &			inAssetID)
{
	mAssetID = inAssetID;
}

//----------------------------------------------------------------------------------------
// GetLinkedWithDocID
//----------------------------------------------------------------------------------------
const PMString &
CZPAcquireLockRequest::GetLinkedWithDocID()const
{
	return mLinkedWithDocID;
}

//----------------------------------------------------------------------------------------
// SetLinkedWithDocID
//----------------------------------------------------------------------------------------
void
CZPAcquireLockRequest::SetLinkedWithDocID(
	const PMString &			inLinkedWithDocID)
{
	mLinkedWithDocID = inLinkedWithDocID;
}

//----------------------------------------------------------------------------------------
// GetLinkedWithDocName
//----------------------------------------------------------------------------------------
const PMString &
CZPAcquireLockRequest::GetLinkedWithDocName()const
{
	return mLinkedWithDocName;
}

//----------------------------------------------------------------------------------------
// SetLinkedWithDocName
//----------------------------------------------------------------------------------------
void
CZPAcquireLockRequest::SetLinkedWithDocName(
	const PMString &			inLinkedWithDocName)
{
	mLinkedWithDocName = inLinkedWithDocName;
}

//----------------------------------------------------------------------------------------
// GetAppName
//----------------------------------------------------------------------------------------
const PMString &
CZPAcquireLockRequest::GetAppName()const
{
	return mAppName;
}

//----------------------------------------------------------------------------------------
// SetAppName
//----------------------------------------------------------------------------------------
void
CZPAcquireLockRequest::SetAppName(
	const PMString &			inAppName)
{
	mAppName = inAppName;
}
