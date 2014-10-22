//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/CZPAMDocUIEnablementRules.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 9-7-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IAMService.h"
#include "IAMServiceProvider.h"
#include "IAMSPManager.h"
#include "IAMUIEnablementRules.h"
#include "IManageableAsset.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AM
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#ifdef DEBUG
static const char *	gUIEnablementCmdNames[] = {
						"enDisplayVersions",
						"enSaveVersion",
						"enRevertToProject",
						"enCheckOut",
						"enCheckIn",
						"enCancelCheckOut"
					};
#endif

/**
enDisplayVersions	= kWorkgroupPrefix + 1, // Versions... menu item 
enSaveVersion		= kWorkgroupPrefix + 2, // Save a Version... menu item 
enRevertToProject	= kWorkgroupPrefix + 3, // Revert menu item 
enCheckOut			= kWorkgroupPrefix + 4, // Story Check Out menu item 
enCheckIn			= kWorkgroupPrefix + 5, // Story Check In menu item 
enCancelCheckOut	= kWorkgroupPrefix + 6  // Story Cancel Check Out menu item 
*/
#define inheritClass CPMUnknown<IAMUIEnablementRules>
class CZPAMDocUIEnablementRules : public inheritClass
{
public:
						CZPAMDocUIEnablementRules(IPMUnknown * boss);
	virtual				~CZPAMDocUIEnablementRules();

#if defined(InDnCS5) || defined(InDnCS5_5)
	bool16				IsCommandEnabled(
							int32						commandID,
							UIDRef						assetRef,
							UIDRef						itemRef);
#else
	bool16				IsCommandEnabled(
							int32						commandID,
							IManageableAsset *			asset = nil );
#endif
protected:
private:
};


CREATE_PMINTERFACE(CZPAMDocUIEnablementRules, kZPAMDocUIEnablementRulesImpl)

CZPAMDocUIEnablementRules::CZPAMDocUIEnablementRules(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMDocUIEnablementRules::~CZPAMDocUIEnablementRules()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// IsCommandEnabled
//----------------------------------------------------------------------------------------
#if defined(InDnCS5) || defined(InDnCS5_5)
bool16
CZPAMDocUIEnablementRules::IsCommandEnabled(
	int32						commandID,
	UIDRef						assetRef,
	UIDRef						itemRef)
#else
bool16
CZPAMDocUIEnablementRules::IsCommandEnabled(
	int32						commandID,
	IManageableAsset *			pAsset)
#endif
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
	const IExecutionContext* ec = GetExecutionContext();
	ISession* gSession = ec->GetSession();

	InterfacePtr<IManageableAsset> pAsset( assetRef, UseDefaultIID() );
#endif
	
	bool16 isEnabled = kFalse;
	
	if ( pAsset != nil && pAsset->HasVersionedContent() )
	{

		InterfacePtr<IAMSPManager> iAMSPManager( gSession, UseDefaultIID() );
		ASSERT( iAMSPManager );
		ClassID theAssetSPClass = pAsset->GetServiceProviderClass();
		ASSERT( theAssetSPClass != kInvalidClass );

		IAMServiceProvider * amServiceProvider = iAMSPManager->GetServiceProvider( theAssetSPClass );
		ASSERT( amServiceProvider );

		InterfacePtr<IAMService> amService;
		
		//Note: following reset are OK as these are special case where Get is returning object with AddRef
		switch ( commandID )
		{
			case IAMUIEnablementRules::enDisplayVersions:
				isEnabled = kFalse;						//We don't support old version viewing.
				break;
			case IAMUIEnablementRules::enSaveVersion:		//File->"Check In..." menu item
			case IAMUIEnablementRules::enCheckIn:
				amService.reset( amServiceProvider->GetService( IAMServiceProvider::enCheckIn ) );
				ASSERT( amService );
				isEnabled = amService->VerifyContext( pAsset );
				break;
			case IAMUIEnablementRules::enRevertToProject:
				amService.reset( amServiceProvider->GetService( IAMServiceProvider::enRevert ) );
				ASSERT( amService );
				isEnabled = amService->VerifyContext( pAsset );
				break;
			case IAMUIEnablementRules::enCancelCheckOut:
				amService.reset( amServiceProvider->GetService( IAMServiceProvider::enCancelCheckOut ) );
				ASSERT( amService );
				isEnabled = amService->VerifyContext( pAsset );
				break;
			case IAMUIEnablementRules::enCheckOut :
				amService.reset( amServiceProvider->GetService( IAMServiceProvider::enCheckOut ) );
				ASSERT( amService );
				isEnabled = amService->VerifyContext( pAsset );
				break;
		}
	}
	
#ifdef DEBUG
	IZPLog_Str_( thisFileLA, enLT_DebugInfo," UI Enablement command : %s : %hd", gUIEnablementCmdNames[ commandID - enDisplayVersions ], isEnabled );
#endif
	return isEnabled;
}
