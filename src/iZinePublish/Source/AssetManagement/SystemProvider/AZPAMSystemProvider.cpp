//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/SystemProvider/AZPAMSystemProvider.cpp $
//	$Revision: 2148 $
//	$Date: 2010-09-23 08:36:04 +0200 (Thu, 23 Sep 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 28-6-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IAMServiceProvider.h"
#include "IAMSystemProvider.h"
#include "IAMUIServiceProvider.h"
#include "IAssetMonitor.h"

//SDK General includes

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPApp.h"
#include "IZPAppController.h"

//IZP General includes
#include "AZPAMSystemProvider.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AMSysP
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
AZPAMSystemProvider::AZPAMSystemProvider(IPMUnknown * boss)
: inheritClass_ZPAMSystemP(boss)
, mProviderIsAvailable( false )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
AZPAMSystemProvider::~AZPAMSystemProvider()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetAssetMonitor
//----------------------------------------------------------------------------------------
IAssetMonitor*
AZPAMSystemProvider::GetAssetMonitor()
{
	LogFunctionEnterExit;
	
	if ( mProviderIsAvailable )
	{
		//IZPLog_Str_( thisFileLA, enLT_Details, " Provider : %s", this->GetProviderName() );
		InterfacePtr<IAssetMonitor> assetMonitor( this, UseDefaultIID() );
		//Asset menitor is not compulsory, so no assert for it.

		return assetMonitor;
	}
	return nil;
}


//----------------------------------------------------------------------------------------
// GetServiceProvider
//----------------------------------------------------------------------------------------
IAMServiceProvider*
AZPAMSystemProvider::GetServiceProvider()
{
	LogFunctionEnterExit;
	
	if ( mProviderIsAvailable )
	{
		InterfacePtr<IAMServiceProvider> serviceProvider( this, UseDefaultIID() );
		ASSERT( serviceProvider );
		return serviceProvider;
	}
	return nil;
}


//----------------------------------------------------------------------------------------
// GetUIServiceProvider
//----------------------------------------------------------------------------------------
IAMUIServiceProvider*
AZPAMSystemProvider::GetUIServiceProvider()
{
	//LogFunctionEnterExit;	//Commented as it comming very frequent
	
	if ( mProviderIsAvailable )
	{
		InterfacePtr<IAMUIServiceProvider> uiServiceProvider( this, UseDefaultIID() );
		ASSERT( uiServiceProvider );
		return uiServiceProvider;
	}
	return nil;
}


//----------------------------------------------------------------------------------------
// Init
//----------------------------------------------------------------------------------------
ErrorCode
AZPAMSystemProvider::Init()
{
	LogFunctionEnterExit;
	
	IZPLog_Str_( thisFileLA, enLT_Details, " Provider : %s", this->GetProviderName() );
	//Should return success only if logged in.
	mProviderIsAvailable = true;//gZPApp->GetAppState() == eAppState_LoggedIn;
	
	//TODO: handle logout case.
	
	return mProviderIsAvailable ? kSuccess : kFailure;
}
