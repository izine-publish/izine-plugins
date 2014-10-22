//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/AssetManagement/ServiceProviders/AZPAMUIServiceProvider.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 30-6-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IAMServiceProvider.h"
#include "IAMUIServiceProvider.h"
#include "IBoolData.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPDefaultSession.h"
#include "IZPUserCredentials.h"

//IZP General includes
#include "AZPAMUIServiceProvider.h"

#pragma mark -
#include "IZPLog.h"

#define thisFileLA	enLMA_AMSrvP
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#pragma mark -
//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
AZPAMUIServiceProvider::AZPAMUIServiceProvider(IPMUnknown * boss)
: inheritClass_ZPAMUIServiceP( boss )
, mIsEnabled( kTrue )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
AZPAMUIServiceProvider::~AZPAMUIServiceProvider()
{

}

#pragma mark -

//----------------------------------------------------------------------------------------
// Enable
//----------------------------------------------------------------------------------------
void
AZPAMUIServiceProvider::Enable(
	bool16 inFlag)
{
	mIsEnabled = inFlag;
}

//----------------------------------------------------------------------------------------
// IsEnabled
//----------------------------------------------------------------------------------------
bool16
AZPAMUIServiceProvider::IsEnabled( void )
{
	LogFunctionEnterExit;
	
	return mIsEnabled;
}

//----------------------------------------------------------------------------------------
// IsLoggedIn
//----------------------------------------------------------------------------------------
bool
AZPAMUIServiceProvider::IsLoggedIn()const
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IZPDefaultSession> defaultSession(gSession, UseDefaultIID());
	ASSERT(defaultSession);

	// allow default behaviour if not logged in
	const IZPUserCredentials * defaultCredentials = defaultSession->GetUserCredentials();
	if ( !defaultCredentials )
		return false;

	//check for valid flag on user credentials.
	InterfacePtr<const IBoolData> isValidData( defaultCredentials, IID_IZPISVALID );
	if( !isValidData || !isValidData->Get())
		return false;
	
	return true;
}

//----------------------------------------------------------------------------------------
// ShowAllWarningDialogs
//----------------------------------------------------------------------------------------
void
AZPAMUIServiceProvider::ShowAllWarningDialogs()
{
	LogFunctionEnterExit;
	IZPLog_Str_( thisFileLA, enLT_Details, "%s", this->GetProviderName() );
}
