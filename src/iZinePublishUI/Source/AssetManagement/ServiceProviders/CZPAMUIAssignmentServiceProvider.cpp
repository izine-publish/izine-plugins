//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/AssetManagement/ServiceProviders/CZPAMUIAssignmentServiceProvider.cpp $
//	$Revision: 3301 $
//	$Date: 2011-06-20 10:55:00 +0200 (Mon, 20 Jun 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 30-6-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IAMServiceData.h"
#include "IAMServiceProvider.h"
#include "IAMUIServiceProvider.h"

//SDK General includes
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPUIID.h"
#include "IZPID.h"

//IZP Interfaces
#include "IZPAMDebugHelper.h"

//IZP General includes
#include "AZPAMUIService.h"
#include "AZPAMUIServiceProvider.h"
#include "ZPTypes.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AMUISrvP
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass AZPAMUIServiceProvider
class CZPAMUIAssignmentServiceProvider : public inheritClass
{
public:
						CZPAMUIAssignmentServiceProvider(IPMUnknown * boss);
	virtual				~CZPAMUIAssignmentServiceProvider();

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


CREATE_PMINTERFACE(CZPAMUIAssignmentServiceProvider, kZPAMUIAssignmentServiceProviderImpl)

CZPAMUIAssignmentServiceProvider::CZPAMUIAssignmentServiceProvider(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMUIAssignmentServiceProvider::~CZPAMUIAssignmentServiceProvider()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetService
//----------------------------------------------------------------------------------------
IAMUIService*
CZPAMUIAssignmentServiceProvider::GetService(
	int32 uiServiceID)
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
			serviceBoss =	kZPAMUISaveAlertServiceBoss;
			break;
		case IAMUIServiceProvider::enLiveEditConflictAlert:
		case IAMUIServiceProvider::enConflictAlert :
			break;
		case IAMUIServiceProvider::enSaveAsDialog :
		case IAMUIServiceProvider::enSaveACopyDialog :
			serviceBoss =	kZPAMUISaveAsDialogServiceBoss;
			break;
		case IAMUIServiceProvider::enSaveMultiLinkAlert :
			break;
		case IAMUIServiceProvider::enGetVersionComments:
		case IAMUIServiceProvider::enGetBatchVersionComments :
			serviceBoss =	kZPAMUIGetVersionCommentsServiceBoss;	// some other possible UI services
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
		serviceData->Set( kZPAMUIServiceType, eAMUIServiceType_Assignment );
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
CZPAMUIAssignmentServiceProvider::IsServiceAvailable(
	UIDRef assetRef, int32 assetServiceID)
{
	LogFunctionEnterExit;
	
	bool16 isAvailable = kFalse;
	if( this->IsLoggedIn() )
	{
		switch ( assetServiceID )
		{
			case IAMUIServiceProvider::enInvalidService :
				break;
			case IAMUIServiceProvider::enConflictAlert :
			case IAMUIServiceProvider::enGetBatchVersionComments :
			case IAMUIServiceProvider::enLiveEditConflictAlert:
			case IAMUIServiceProvider::enSaveACopyDialog :
			case IAMUIServiceProvider::enSaveMultiLinkAlert :
			case IAMUIServiceProvider::enViewVersions :
				isAvailable = kFalse;
				break;
			
			case IAMUIServiceProvider::enGetVersionComments :
			case IAMUIServiceProvider::enSaveAlert :
			//case IAMUIServiceProvider::enSaveAsDialog :
			case IAMUIServiceProvider::enRevertAlert :
			case enAMUIUpdateVersionStatusService:
				isAvailable = kTrue;
				break;
		}
	}
	
#ifdef DEBUG
	IZPLog_Str_( thisFileLA, enLT_Details, "UID = 0x%08X, service %s : available = %hd",
		assetRef.GetUID().Get(), Utils<IZPAMDebugHelper>()->GetAMUIServiceName((const IAMUIServiceProvider::UIService)assetServiceID), isAvailable );
#endif

	return isAvailable;
}

//----------------------------------------------------------------------------------------
// GetProviderName
//----------------------------------------------------------------------------------------
const char *
CZPAMUIAssignmentServiceProvider::GetProviderName()const
{
	return "Assignment UI service provider";
}
