//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/AssetManagement/ServiceProviders/CZPAMUIDocumentServiceProvider.cpp $
//	$Revision: 3281 $
//	$Date: 2011-06-17 11:50:06 +0200 (Fri, 17 Jun 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 27-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IAMServiceData.h"
#include "IAMUIService.h"
#include "IAMServiceProvider.h"
#include "IAMUIServiceProvider.h"
#include "IBoolData.h"
#include "IManageableAsset.h"
#include "InCopyBridgeID.h"


//SDK General includes
#include "CAlert.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPUIID.h"
#include "IZPID.h"

//IZP Interfaces
#include "IZPDefaultSession.h"
#include "IZPAMDebugHelper.h"

//IZP General includes
#include "AZPAMUIService.h"
#include "AZPAMUIServiceProvider.h"
#include "ZPTypes.h"

#pragma mark -

#include "IZPLog.h"

//workgroupid.h is given in SDK but workgroupUIID.h is not given
// Copying the ID from model dump
#define kStdSaveDialogServiceBoss		0x15b06
#define kStdSaveAlertServiceBoss		0x15b07
#define kStdRevertAlertServiceBoss		0x15b08


#define thisFileLA	enLMA_AMUISrvP
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass AZPAMUIServiceProvider
class CZPAMUIDocumentServiceProvider : public inheritClass
{
public:
						CZPAMUIDocumentServiceProvider(IPMUnknown * boss);
	virtual				~CZPAMUIDocumentServiceProvider();
#pragma mark -
	IAMUIService*		GetService(
							int32 uiServiceID );
	bool16				IsServiceAvailable(
							UIDRef assetRef, int32 assetServiceID );
#pragma mark -

protected:
	const char *		GetProviderName() const;
private:
};


CREATE_PMINTERFACE(CZPAMUIDocumentServiceProvider, kZPAMUIDocumentServiceProviderImpl)

CZPAMUIDocumentServiceProvider::CZPAMUIDocumentServiceProvider(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMUIDocumentServiceProvider::~CZPAMUIDocumentServiceProvider()
{

}

#pragma mark -
// this method returns a boss to handle each requested service that we support
//----------------------------------------------------------------------------------------
// GetService
//----------------------------------------------------------------------------------------
IAMUIService*
CZPAMUIDocumentServiceProvider::GetService(
	int32 uiServiceID )
{
	LogFunctionEnterExit;
	
	IAMUIService * service = nil;
	
	ClassID serviceBoss = kInvalidClass;

	switch ( uiServiceID )
	{
		case IAMUIServiceProvider::enInvalidService :
			break;
		case IAMUIServiceProvider::enViewVersions :
			break;
		case IAMUIServiceProvider::enSaveAlert:
			//serviceBoss =	kStdSaveAlertServiceBoss;
			serviceBoss =	kZPAMUISaveAlertServiceBoss;
			break;
		case IAMUIServiceProvider::enSaveAsDialog :
			serviceBoss =	kZPAMUISaveAsDialogServiceBoss;
			break;
		case IAMUIServiceProvider::enSaveACopyDialog :
			//serviceBoss =	kZPAMUISaveAsDialogServiceBoss;
			break;
		case IAMUIServiceProvider::enRevertAlert:
			//serviceBoss =	kStdRevertAlertServiceBoss;
			serviceBoss = kZPAMUIRevertServiceBoss;
			break;
		case IAMUIServiceProvider::enLiveEditConflictAlert:
		case IAMUIServiceProvider::enConflictAlert :
			break;
		case IAMUIServiceProvider::enSaveMultiLinkAlert :
			break;
		case IAMUIServiceProvider::enGetVersionComments:
		case IAMUIServiceProvider::enGetBatchVersionComments :
			serviceBoss =	kZPAMUIGetVersionCommentsServiceBoss;
			break;
		case enAMUINewAssignmentService:
			serviceBoss =	kZPAMUINewAssignmentServiceBoss;
			break;
		case enAMUIUpdateVersionStatusService:
			serviceBoss =	kZPAMUIUpdateVersionStatusServiceBoss;
			break;
	}
	
	if( !service && serviceBoss.Get() != kInvalidClass.Get() )
	{
		service = ::CreateObject2<IAMUIService>( serviceBoss );
	}
	InterfacePtr<IAMServiceData> serviceData( service, UseDefaultIID() );
	if ( serviceData )
	{
		serviceData->Set( IAMUIService::enServiceID, uiServiceID );
		serviceData->Set( kZPAMUIServiceType, eAMUIServiceType_Document );
	}
	
#ifdef DEBUG
	IZPLog_Str_( thisFileLA, enLT_Details, "%s : %08X", Utils<IZPAMDebugHelper>()->GetAMUIServiceName((const IAMUIServiceProvider::UIService)uiServiceID), service );
#endif
	return service;
}


//----------------------------------------------------------------------------------------
// IsServiceAvailable
//----------------------------------------------------------------------------------------
bool16
CZPAMUIDocumentServiceProvider::IsServiceAvailable(
	UIDRef assetRef, int32 uiServiceID )
{
	LogFunctionEnterExit;
	
	bool16 isAvailable = kFalse;
	
	if( this->IsLoggedIn() )	//allow default behaviour if not logged in
	switch ( uiServiceID )
	{
		case IAMUIServiceProvider::enInvalidService :
			break;
		case IAMUIServiceProvider::enConflictAlert :
		case IAMUIServiceProvider::enGetBatchVersionComments :
		case IAMUIServiceProvider::enLiveEditConflictAlert:
		//case IAMUIServiceProvider::enRevertAlert :
		case IAMUIServiceProvider::enSaveACopyDialog :
		case IAMUIServiceProvider::enSaveMultiLinkAlert :
		case IAMUIServiceProvider::enViewVersions :
			isAvailable = kFalse;
			break;
		
		case IAMUIServiceProvider::enGetVersionComments :
		case IAMUIServiceProvider::enSaveAlert :
		case IAMUIServiceProvider::enSaveAsDialog :
		case IAMUIServiceProvider::enRevertAlert:
		case enAMUINewAssignmentService:
		case enAMUIUpdateVersionStatusService:
			isAvailable = kTrue;
			break;
	}
	
#ifdef DEBUG
	IZPLog_Str_( thisFileLA, enLT_Details, "UID = 0x%08X, service %s : %hd",
		assetRef.GetUID().Get(), Utils<IZPAMDebugHelper>()->GetAMUIServiceName((const IAMUIServiceProvider::UIService)uiServiceID), isAvailable );
#endif

	return isAvailable;
}

//----------------------------------------------------------------------------------------
// GetProviderName
//----------------------------------------------------------------------------------------
const char *
CZPAMUIDocumentServiceProvider::GetProviderName()const
{
	return "Document UI service provider";
}


