//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/Requests/AZPSoapRequest.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 16-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IWorkspace.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPWSConfig.h"
#include "IZPUserCredentials.h"
#include "IZPDefaultSession.h"

//IZP General includes
#include "AZPSoapRequest.h"
#include "IZPLog.h"

#pragma mark -

#define thisFileLA	enLMA_WS
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )
int gRequestObjectCount = 0;

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
AZPSoapRequest::AZPSoapRequest(
	enRequestType				inReqType,
	bool						isAsync)
: mReqType( inReqType )
, mIsAsync( isAsync )
, mWSConfig( NULL )
, mUserCredentials( NULL )
{
	++gRequestObjectCount;
	IZPLog_Str ( thisFileLA, enLT_ObjectCreations, "Object created at : %08X\nLive request objects: %d", this, gRequestObjectCount );

	//set the default WS Config and UserCredentials
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IWorkspace> theWorkspace(gSession->QueryWorkspace());
	ASSERT(theWorkspace);

	InterfacePtr<const IZPWSConfig> iZPWSConfig( theWorkspace, UseDefaultIID() );
	ASSERT(iZPWSConfig);
	this->SetWSConfig( iZPWSConfig );

	InterfacePtr<const IZPDefaultSession> defaultSession( gSession, UseDefaultIID() );
	ASSERT( defaultSession );
	this->SetUserCredentials( defaultSession->GetUserCredentials() );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
AZPSoapRequest::~AZPSoapRequest()
{
	if(mWSConfig)
		mWSConfig->Release();
	if(mUserCredentials)
		mUserCredentials->Release();
	--gRequestObjectCount;
	IZPLog_Str ( thisFileLA, enLT_ObjectCreations, "Object destoyed at : %08X\nLive request objects: %d", this, gRequestObjectCount );
}

//----------------------------------------------------------------------------------------
// GetRequestType
//----------------------------------------------------------------------------------------
enRequestType
AZPSoapRequest::GetRequestType()const
{
	return mReqType;
}


//----------------------------------------------------------------------------------------
// IsAsync
//----------------------------------------------------------------------------------------
bool
AZPSoapRequest::IsAsync()const
{
	return mIsAsync;
}

//----------------------------------------------------------------------------------------
// SetAsync
//----------------------------------------------------------------------------------------
void
AZPSoapRequest::SetAsync(
	bool						inIsAsync)
{
	mIsAsync = inIsAsync;
}


//----------------------------------------------------------------------------------------
// GetWSConfig
//----------------------------------------------------------------------------------------
const IZPWSConfig *
AZPSoapRequest::GetWSConfig()const
{
	return mWSConfig;
}

//----------------------------------------------------------------------------------------
// SetWSConfig
//----------------------------------------------------------------------------------------
void
AZPSoapRequest::SetWSConfig(
	const IZPWSConfig *			inWSConfig)
{
	if(mWSConfig)
		mWSConfig->Release();
	mWSConfig = inWSConfig;
	if( mWSConfig )
		mWSConfig->AddRef();
}

//----------------------------------------------------------------------------------------
// GetUserCredentials
//----------------------------------------------------------------------------------------
const IZPUserCredentials *
AZPSoapRequest::GetUserCredentials()const
{
	return mUserCredentials;
}

//----------------------------------------------------------------------------------------
// SetUserCredentials
//----------------------------------------------------------------------------------------
void
AZPSoapRequest::SetUserCredentials(
	const IZPUserCredentials *	inUserCredentials)
{
	if(mUserCredentials)
		mUserCredentials->Release();
	mUserCredentials = inUserCredentials;
	if( mUserCredentials )
		mUserCredentials->AddRef();
}

#ifdef Debug_WS
//----------------------------------------------------------------------------------------
// PrintToLogs
//----------------------------------------------------------------------------------------
void
AZPSoapRequest::PrintToLogs()const
{
	IZPLog_Str ( thisFileLA, enLT_DebugInfo, "Request object: %s, at %08X", this->GetName(), this );
}
#endif
