//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/WS/Requests/AZExpWSRequest.cpp $
//	$Revision: 3592 $
//	$Date: 2011-08-17 08:48:52 +0200 (Wed, 17 Aug 2011) $
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
#include "IWorkspace.h"
#ifdef InDnCS5
#include "IExecutionContext.h"
#endif

//SDK General includes

#include "IZExpID.h"

//IZP Interfaces
#include "IZExpPrefs.h"

//IZP General includes
#include "AZExpWSRequest.h"
#include "IZPLog.h"

#pragma mark -

#define thisFileLA	enLMA_WS
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )
int gRequestObjectCount = 0;

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
AZExpWSRequest::AZExpWSRequest(
	enWSRequestType				inReqType)
//	bool						isAsync)
: mReqType( inReqType )
{
	++gRequestObjectCount;
	IZPLog_Str ( thisFileLA, enLT_ObjectCreations, "Object created at : %08X\nLive request objects: %d", this, gRequestObjectCount );

	//set the default WS Config and UserCredentials
//	InterfacePtr<const IWorkspace> theWorkspace(gSession->QueryWorkspace());
//	ASSERT(theWorkspace);

//	InterfacePtr<const IZPWSConfig> iZPWSConfig( theWorkspace, UseDefaultIID() );
//	ASSERT(iZPWSConfig);
//	this->SetWSConfig( iZPWSConfig );

//	InterfacePtr<const IZPDefaultSession> defaultSession( gSession, UseDefaultIID() );
//	ASSERT( defaultSession );
//	this->SetUserCredentials( defaultSession->GetUserCredentials() );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
AZExpWSRequest::~AZExpWSRequest()
{
//	if(mWSConfig)
//		mWSConfig->Release();
//	if(mUserCredentials)
//		mUserCredentials->Release();
	--gRequestObjectCount;
	IZPLog_Str ( thisFileLA, enLT_ObjectCreations, "Object destoyed at : %08X\nLive request objects: %d", this, gRequestObjectCount );
}

//----------------------------------------------------------------------------------------
// GetRequestType
//----------------------------------------------------------------------------------------
enWSRequestType
AZExpWSRequest::GetRequestType()const
{
	return mReqType;
}

//----------------------------------------------------------------------------------------
// GetRequestURL
//----------------------------------------------------------------------------------------
const char *
AZExpWSRequest::GetRequestURL()const
{
	return "http://87.253.130.51/export9348/home";	//TODO: will this come from some configuration?
}

//----------------------------------------------------------------------------------------
// GetWSConfig
//----------------------------------------------------------------------------------------
//const IZPWSConfig *
//AZExpWSRequest::GetWSConfig()const
//{
//	return mWSConfig;
//}

//----------------------------------------------------------------------------------------
// SetWSConfig
//----------------------------------------------------------------------------------------
//void
//AZExpWSRequest::SetWSConfig(
//	const IZPWSConfig *			inWSConfig)
//{
//	if(mWSConfig)
//		mWSConfig->Release();
//	mWSConfig = inWSConfig;
//	if( mWSConfig )
//		mWSConfig->AddRef();
//}

//----------------------------------------------------------------------------------------
// GetPluginID
//----------------------------------------------------------------------------------------
const PMString &
AZExpWSRequest::GetPluginID()const
{
#ifdef InDnCS5
	const IExecutionContext* ec = GetExecutionContext();
	ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IWorkspace> theWorkspace(gSession->QueryWorkspace());
	ASSERT(theWorkspace);

	InterfacePtr<const IZExpPrefs> iZExpPrefs( theWorkspace, UseDefaultIID() );
	ASSERT(iZExpPrefs);
	return iZExpPrefs->GetPluginId();
}

#ifdef Debug_WS
//----------------------------------------------------------------------------------------
// PrintToLogs
//----------------------------------------------------------------------------------------
void
AZExpWSRequest::PrintToLogs()const
{
	IZPLog_Str ( thisFileLA, enLT_DebugInfo, "Request object: %s, at %08X", this->GetName(), this );
}
#endif
