//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/Utils/CZPAMDebugHelper.cpp $
//	$Revision: 3449 $
//	$Date: 2011-07-05 15:42:21 +0200 (Tue, 05 Jul 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 13-2-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"
// SDK Interfaces
#include "IAMServiceProvider.h"
#include "IAMUIServiceProvider.h"

// SDK includes
#include "CPMUnknown.h"
#include "K2Vector.tpp"
#include "Utils.h"

#include "IZPID.h"

// Project Interfaces.
#include "IZPAMDebugHelper.h"

// Project includes:
#include "IZPID.h"
#include "IZPLog.h"

#define thisFileLA	enLMA_Utils
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZPAMDebugHelper>
class CZPAMDebugHelper : public inheritClass
{
public:
	
						CZPAMDebugHelper(IPMUnknown* boss);
	virtual				~CZPAMDebugHelper();
	
#ifdef DEBUG
	const char *		GetAMServiceName(
							const IAMServiceProvider::AssetService			inService );
	const char *		GetAMUIServiceName(
							const IAMUIServiceProvider::UIService			inService );
#endif
};

CREATE_PMINTERFACE(CZPAMDebugHelper, kZPAMDebugHelperImpl)

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPAMDebugHelper::CZPAMDebugHelper(IPMUnknown* boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMDebugHelper::~CZPAMDebugHelper()
{

}

#pragma mark -

#ifdef DEBUG
const char * gAMServiceNames[IAMServiceProvider::enUnembed - IAMServiceProvider::enEnsureLatestVersion + 2] = {0};
const char * gAMUIServiceNames[IAMUIServiceProvider::enLiveEditConflictAlert - IAMUIServiceProvider::enSaveAsDialog + 2] = {0};

const char *
CZPAMDebugHelper::GetAMServiceName(
	const IAMServiceProvider::AssetService			inService )
{
	if(!gAMServiceNames[0])
	{
		gAMServiceNames[IAMServiceProvider::enInvalidService] = IZP_STRINGIZE2(enInvalidService);	//0,
		gAMServiceNames[IAMServiceProvider::enEnsureLatestVersion			-kWorkgroupPrefix] = IZP_STRINGIZE2(IAMServiceProvider::enEnsureLatestVersion			);	//kWorkgroupPrefix + 1,
		gAMServiceNames[IAMServiceProvider::enCheckIn						-kWorkgroupPrefix] = IZP_STRINGIZE2(IAMServiceProvider::enCheckIn						);	//kWorkgroupPrefix + 2,
		gAMServiceNames[IAMServiceProvider::enRevert						-kWorkgroupPrefix] = IZP_STRINGIZE2(IAMServiceProvider::enRevert						);	//kWorkgroupPrefix + 3,
		gAMServiceNames[IAMServiceProvider::enCheckOut						-kWorkgroupPrefix] = IZP_STRINGIZE2(IAMServiceProvider::enCheckOut						);	//kWorkgroupPrefix + 4,
		gAMServiceNames[IAMServiceProvider::enCancelCheckOut				-kWorkgroupPrefix] = IZP_STRINGIZE2(IAMServiceProvider::enCancelCheckOut				);	//kWorkgroupPrefix + 5,
		gAMServiceNames[IAMServiceProvider::enTerminate						-kWorkgroupPrefix] = IZP_STRINGIZE2(IAMServiceProvider::enTerminate						);	//kWorkgroupPrefix + 6,
		gAMServiceNames[IAMServiceProvider::enInitialize					-kWorkgroupPrefix] = IZP_STRINGIZE2(IAMServiceProvider::enInitialize					);	//kWorkgroupPrefix + 7,
		gAMServiceNames[IAMServiceProvider::enAfterSave						-kWorkgroupPrefix] = IZP_STRINGIZE2(IAMServiceProvider::enAfterSave						);	//kWorkgroupPrefix + 8,
		gAMServiceNames[IAMServiceProvider::enAfterSaveAs					-kWorkgroupPrefix] = IZP_STRINGIZE2(IAMServiceProvider::enAfterSaveAs					);	//kWorkgroupPrefix + 9,
		gAMServiceNames[IAMServiceProvider::enUpdateStoredAssetReference	-kWorkgroupPrefix] = IZP_STRINGIZE2(IAMServiceProvider::enUpdateStoredAssetReference	);	//kWorkgroupPrefix + 10,
		gAMServiceNames[IAMServiceProvider::enUpdateProxyVersionNumber		-kWorkgroupPrefix] = IZP_STRINGIZE2(IAMServiceProvider::enUpdateProxyVersionNumber		);	//kWorkgroupPrefix + 11,
		gAMServiceNames[IAMServiceProvider::enInvalidateProxyVersionNumber	-kWorkgroupPrefix] = IZP_STRINGIZE2(IAMServiceProvider::enInvalidateProxyVersionNumber	);	//kWorkgroupPrefix + 12,
		gAMServiceNames[IAMServiceProvider::enGetUniqueAssetLocation		-kWorkgroupPrefix] = IZP_STRINGIZE2(IAMServiceProvider::enGetUniqueAssetLocation		);	//kWorkgroupPrefix + 13,
		gAMServiceNames[IAMServiceProvider::enBatchCheckIn					-kWorkgroupPrefix] = IZP_STRINGIZE2(IAMServiceProvider::enBatchCheckIn					);	//kWorkgroupPrefix + 14,
		gAMServiceNames[IAMServiceProvider::enAfterCheckIn					-kWorkgroupPrefix] = IZP_STRINGIZE2(IAMServiceProvider::enAfterCheckIn					);	//kWorkgroupPrefix + 15,
		gAMServiceNames[IAMServiceProvider::enBeforeCheckOut				-kWorkgroupPrefix] = IZP_STRINGIZE2(IAMServiceProvider::enBeforeCheckOut				);	//kWorkgroupPrefix + 16,
		gAMServiceNames[IAMServiceProvider::enAfterCancelCheckOut			-kWorkgroupPrefix] = IZP_STRINGIZE2(IAMServiceProvider::enAfterCancelCheckOut			);	//kWorkgroupPrefix + 17,
		gAMServiceNames[IAMServiceProvider::enUpdateLinkLocation			-kWorkgroupPrefix] = IZP_STRINGIZE2(IAMServiceProvider::enUpdateLinkLocation			);	//kWorkgroupPrefix + 18,
		gAMServiceNames[IAMServiceProvider::enRelink						-kWorkgroupPrefix] = IZP_STRINGIZE2(IAMServiceProvider::enRelink						);	//kWorkgroupPrefix + 19,
		gAMServiceNames[IAMServiceProvider::enUnembed						-kWorkgroupPrefix] = IZP_STRINGIZE2(IAMServiceProvider::enUnembed						);	//kWorkgroupPrefix + 20 
	}

	if( inService >= IAMServiceProvider::enEnsureLatestVersion && inService <= IAMServiceProvider::enUnembed )
		return gAMServiceNames[inService - kWorkgroupPrefix];
	else if (inService  == IAMServiceProvider::enInvalidService )
		return gAMServiceNames[inService];
	return "";
}

const char *
CZPAMDebugHelper::GetAMUIServiceName(
	const IAMUIServiceProvider::UIService			inService )
{
	if(!gAMUIServiceNames[0])
	{
		gAMUIServiceNames[IAMUIServiceProvider::enInvalidService	]		= IZP_STRINGIZE2(IAMUIServiceProvider::enInvalidService);	//		= 0
		gAMUIServiceNames[IAMUIServiceProvider::enSaveAsDialog				-kWorkgroupPrefix] = IZP_STRINGIZE2(IAMUIServiceProvider::enSaveAsDialog				);	// kWorkgroupPrefix + 1
		gAMUIServiceNames[IAMUIServiceProvider::enViewVersions				-kWorkgroupPrefix] = IZP_STRINGIZE2(IAMUIServiceProvider::enViewVersions				);	// kWorkgroupPrefix + 2
		gAMUIServiceNames[IAMUIServiceProvider::enSaveAlert					-kWorkgroupPrefix] = IZP_STRINGIZE2(IAMUIServiceProvider::enSaveAlert					);	// kWorkgroupPrefix + 3
		gAMUIServiceNames[IAMUIServiceProvider::enRevertAlert				-kWorkgroupPrefix] = IZP_STRINGIZE2(IAMUIServiceProvider::enRevertAlert					);	// kWorkgroupPrefix + 4
		gAMUIServiceNames[IAMUIServiceProvider::enSaveACopyDialog			-kWorkgroupPrefix] = IZP_STRINGIZE2(IAMUIServiceProvider::enSaveACopyDialog				);	// kWorkgroupPrefix + 5
		gAMUIServiceNames[IAMUIServiceProvider::enSaveMultiLinkAlert		-kWorkgroupPrefix] = IZP_STRINGIZE2(IAMUIServiceProvider::enSaveMultiLinkAlert			);	// kWorkgroupPrefix + 6
		gAMUIServiceNames[IAMUIServiceProvider::enGetVersionComments		-kWorkgroupPrefix] = IZP_STRINGIZE2(IAMUIServiceProvider::enGetVersionComments			);	// kWorkgroupPrefix + 7
		gAMUIServiceNames[IAMUIServiceProvider::enGetBatchVersionComments	-kWorkgroupPrefix] = IZP_STRINGIZE2(IAMUIServiceProvider::enGetBatchVersionComments		);	// kWorkgroupPrefix + 8
		gAMUIServiceNames[IAMUIServiceProvider::enConflictAlert				-kWorkgroupPrefix] = IZP_STRINGIZE2(IAMUIServiceProvider::enConflictAlert				);	// kWorkgroupPrefix + 9
		gAMUIServiceNames[IAMUIServiceProvider::enLiveEditConflictAlert		-kWorkgroupPrefix] = IZP_STRINGIZE2(IAMUIServiceProvider::enLiveEditConflictAlert		);	// kWorkgroupPrefix + 10
	}
	if( inService >= IAMUIServiceProvider::enSaveAsDialog && inService <= IAMUIServiceProvider::enLiveEditConflictAlert )
		return gAMUIServiceNames[inService - kWorkgroupPrefix];
	else if (inService  == IAMUIServiceProvider::enInvalidService )
		return gAMUIServiceNames[inService];
	return "";
}
#endif //DEBUG
