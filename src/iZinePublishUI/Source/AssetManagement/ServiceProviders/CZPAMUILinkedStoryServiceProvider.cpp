//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/AssetManagement/ServiceProviders/CZPAMUILinkedStoryServiceProvider.cpp $
//	$Revision: 2052 $
//	$Date: 2010-09-08 15:10:24 +0200 (Wed, 08 Sep 2010) $
//	$Author: rajkumar.sehrawat $
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

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AMUISrvP
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass AZPAMUIServiceProvider
class CZPAMUILinkedStoryServiceProvider : public inheritClass
{
public:
						CZPAMUILinkedStoryServiceProvider(IPMUnknown * boss);
	virtual				~CZPAMUILinkedStoryServiceProvider();

	IAMUIService*		GetService(
							int32 uiServiceID );
	bool16				IsServiceAvailable(
							UIDRef assetRef, int32 assetServiceID );
protected:
	const char *		GetProviderName() const;
private:
};


CREATE_PMINTERFACE(CZPAMUILinkedStoryServiceProvider, kZPAMUILinkedStoryServiceProviderImpl)

CZPAMUILinkedStoryServiceProvider::CZPAMUILinkedStoryServiceProvider(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMUILinkedStoryServiceProvider::~CZPAMUILinkedStoryServiceProvider()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetService
//----------------------------------------------------------------------------------------
IAMUIService*
CZPAMUILinkedStoryServiceProvider::GetService(
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
			serviceBoss =	kZPAMUISaveAlertServiceBoss;
			break;
		case IAMUIServiceProvider::enSaveAsDialog :
			//serviceBoss =	kZPAMUISaveAsDialogServiceBoss;
			break;
		case IAMUIServiceProvider::enSaveACopyDialog :
			//serviceBoss =	kZPAMUISaveAsDialogServiceBoss;
			break;
		case IAMUIServiceProvider::enRevertAlert:
			//serviceBoss =	kStdRevertAlertServiceBoss;
			serviceBoss = kZPAMUIRevertServiceBoss;
		case IAMUIServiceProvider::enLiveEditConflictAlert:
		case IAMUIServiceProvider::enConflictAlert :
			break;
		case IAMUIServiceProvider::enSaveMultiLinkAlert :
			break;
		case IAMUIServiceProvider::enGetVersionComments:
		case IAMUIServiceProvider::enGetBatchVersionComments :
			serviceBoss =	kZPAMUIGetVersionCommentsServiceBoss;	// some other possible UI services
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
		serviceData->Set( kZPAMUIServiceType, eAMUIServiceType_LinkedStory );
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
CZPAMUILinkedStoryServiceProvider::IsServiceAvailable(
	UIDRef assetRef, int32 assetServiceID)
{
	LogFunctionEnterExit;
	
	bool16 isAvailable = kFalse;
	do
	{
		if( assetRef == UIDRef::gNull )
			break;
	bool isDocument = assetRef.GetUID() == assetRef.GetDataBase()->GetRootUID();

	if( this->IsLoggedIn() && isDocument == false )
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
			case IAMUIServiceProvider::enSaveAsDialog :
			case IAMUIServiceProvider::enRevertAlert :
				isAvailable = kTrue;
				break;
		}
	}
	}while(kFalse);
	
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
CZPAMUILinkedStoryServiceProvider::GetProviderName()const
{
	return "Linked story UI service provider";
}
