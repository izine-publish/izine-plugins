//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/WS/CZPTestWSCalling.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 26-8-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPAMWSUtils.h"
#include "IZPAssetUtils.h"
#include "IZPFileUtils.h"
#include "IZPSoapRequest.h"
#include "IZPSoapResponse.h"
#include "IZPUserCredentials.h"
#include "IZPWSController.h"
#include "IZPWSResponseHandler.h"

//IZP General includes
#include "ZPWSTypeDefs.h"
#include "CZPTestWSCalling.h"
#include "CZPUploadVersionRequest.h"
#include "CZPUploadVersionResponse.h"
#include "CZPUpdateAssetStatusRequest.h"
#include "CZPUpdateAssetStatusResponse.h"
#include "CZPDownloadVersionResponse.h"
#include "CZPWSCallProcessors.h"

#pragma mark -

#include "IZPLog.h"
#define thisFileLA	enLMA_None
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPTestWSCalling::CZPTestWSCalling()
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPTestWSCalling::~CZPTestWSCalling()
{

}

#pragma mark -

//----------------------------------------------------------------------------------------
// QueryTestingUserCredentials
//----------------------------------------------------------------------------------------
IZPUserCredentials *
CZPTestWSCalling::QueryTestingUserCredentials()
{
	InterfacePtr<IZPUserCredentials> requestCredentials( ::CreateObject2<IZPUserCredentials>( kZPSaveUserCredentialCmdBoss, IZPUserCredentials::kDefaultIID ) );

	requestCredentials->SetUserName( "amana" );
	requestCredentials->SetPwd( "a" );

	return requestCredentials.forget();
}

//----------------------------------------------------------------------------------------
// TestUploadVersion
//----------------------------------------------------------------------------------------
void
CZPTestWSCalling::TestUploadVersion()
{
	PMString assetID( "11128780-dd51-4135-85b1-a780f7082b81" );
	PMString versionID = Utils<IZPAssetUtils>()->NewGUID();
	PMString assetFilePath( "C:\\Documents and Settings\\Rajkumar S\\Local Settings\\Application Data\\Adobe\\InDesign\\Version 5.0\\Caches\\iZINE\\11128780-dd51-4135-85b1-a780f7082b81.indd" );
	
	IDFile assetFile;
	Utils<IZPFileUtils>()->ConvertStrToIDFile( assetFile, assetFilePath );

	CZPTestWSCalling::TestUploadVersion( assetID, versionID, assetFile );
}


//----------------------------------------------------------------------------------------
// TestUploadVersion
//----------------------------------------------------------------------------------------
void
CZPTestWSCalling::TestUploadVersion(
	const PMString &			inAssetID,
	const PMString &			inNewVersionID,
	const IDFile &				inAssetFile)
{
#ifndef DEBUG
	return;
#endif
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IZPWSController> wsController( gSession , UseDefaultIID() );
	ASSERT(wsController);

	AZPSoapRequest * theRequest = Utils<IZPAMWSUtils>()->CreateUploadVersionRequest( inAssetID, inNewVersionID, inAssetFile );

	NewWSSyncRequestWithRequestData(theRequest)

	InterfacePtr<IZPUserCredentials> requestCredentials( QueryTestingUserCredentials() );

	soapRequest->GetRequestData()->SetUserCredentials( requestCredentials );

	wsController->AddRequest( soapRequest, false);	//Sync call.

	ErrorCode toReturn = kFailure;
	ZPWSError soapError = soapResponse->GetWSError();
	if( soapError == 0 )
	{
		const CZPUploadVersionResponse * theResponse = static_cast<const CZPUploadVersionResponse*>( soapResponse->GetResponseData() );
		if( theResponse && theResponse->GetErrorCode() == 0)
		{
			toReturn = kSuccess;
		}
	}
}

//----------------------------------------------------------------------------------------
// TestUpdateStatus
//----------------------------------------------------------------------------------------
void
CZPTestWSCalling::TestUpdateStatus()
{
	//Failed in staging
	PMString assetID( "8f03ffcd-f802-493f-9a06-bf2409182cce" );
	PMString oldStatusId( "a350e418-e5b0-4fe5-92ee-66d9814e29c2" );
	PMString newStatusId("69c76023-3cf2-4a58-90d7-ad410435f47f");
	
	////PC 16 data
	//PMString assetID( "05d18e85-69e5-4dd9-a8ea-837f74b898ff" );
	//PMString oldStatusId( "b8289b6b-a055-47da-8334-dbaf4ce47df0" );
	//PMString newStatusId("603bd6ba-2964-4604-b9c7-b454bd48af0b");

	CZPTestWSCalling::TestUpdateStatus( assetID, oldStatusId, newStatusId );

	/*
	
<statusId>b8289b6b-a055-47da-8334-dbaf4ce47df0</statusId><statusName>Redactie</statusName><layout>true</layout><text>true</text><stateId>a40327cf-5190-4694-94ed-babaeda98c3f</stateId></StatusDTO><StatusDTO>
<statusId>69c76023-3cf2-4a58-90d7-ad410435f47f</statusId><statusName>Redactie</statusName><layout>true</layout><text>true</text><stateId>a40327cf-5190-4694-94ed-babaeda98c3f</stateId></StatusDTO><StatusDTO>
<statusId>49074cec-eb07-4ee3-b46d-8149d29202e3</statusId><statusName>Vormgeving</statusName><layout>true</layout><text>true</text><stateId>a40327cf-5190-4694-94ed-babaeda98c3f</stateId></StatusDTO><StatusDTO>
<statusId>ddcc8f00-14ed-4ad2-b797-a89b0f9415f0</statusId><statusName>Definitief</statusName><layout>true</layout><text>true</text><stateId>ab136ae9-728a-421d-873b-eae32d5e33c7</stateId></StatusDTO><StatusDTO>
<statusId>603bd6ba-2964-4604-b9c7-b454bd48af0b</statusId><statusName>Eindredactie</statusName><layout>true</layout><text>true</text><stateId>ab136ae9-728a-421d-873b-eae32d5e33c7</stateId></StatusDTO><StatusDTO>
<statusId>a350e418-e5b0-4fe5-92ee-66d9814e29c2</statusId><statusName>Vormgeving</statusName><layout>true</layout><text>true</text><stateId>a40327cf-5190-4694-94ed-babaeda98c3f</stateId></StatusDTO><StatusDTO>
<statusId>0b75fc2d-eda3-4a56-a6ea-27dc9970e80b</statusId><statusName>Eindredactie</statusName><layout>true</layout><text>true</text><stateId>a40327cf-5190-4694-94ed-babaeda98c3f</stateId></StatusDTO><StatusDTO>
<statusId>22f8acbd-8e7c-4437-a767-e5773db76083</statusId><statusName>Definitief</statusName><layout>true</layout><text>true</text><stateId>da43364a-0f49-4735-b88b-674b6d20639b</stateId></StatusDTO></statuses></TitleStatusDTO></GetStatusByTitleResult></GetStatusByTitleResponse>
	*/
}

//----------------------------------------------------------------------------------------
// TestUpdateStatus
//----------------------------------------------------------------------------------------
void
CZPTestWSCalling::TestUpdateStatus(
	const PMString &			inAssetID,
	const PMString &			inOldStatusID,
	const PMString &			inNewStatusID)
{
#ifndef DEBUG
	return;
#endif
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IZPWSController> wsController( gSession , UseDefaultIID() );
	ASSERT(wsController);

	AZPSoapRequest * theRequest = Utils<IZPAMWSUtils>()->CreateUpdateAssetStatusRequest( inAssetID, inOldStatusID, inNewStatusID );

	NewWSSyncRequestWithRequestData(theRequest)

	InterfacePtr<IZPUserCredentials> requestCredentials( QueryTestingUserCredentials() );

	soapRequest->GetRequestData()->SetUserCredentials( requestCredentials );

	wsController->AddRequest( soapRequest, false);	//Sync call.

	ErrorCode toReturn = kFailure;
	ZPWSError soapError = soapResponse->GetWSError();
	if( soapError == 0 )
	{
		const CZPUpdateAssetStatusResponse * theResponse = static_cast<const CZPUpdateAssetStatusResponse*>( soapResponse->GetResponseData() );
		if( theResponse && theResponse->GetErrorCode() == 0)
		{
			toReturn = kSuccess;
		}
	}
}

//----------------------------------------------------------------------------------------
// TestDownloadVersion
//----------------------------------------------------------------------------------------
void
CZPTestWSCalling::TestDownloadVersion()
{
	PMString versionID( "1e0a8c59-7348-43e7-94a3-e3324dbb69dd" );
	
	PMString downloadedFilePath( "C:\\Documents and Settings\\Rajkumar S\\Local Settings\\Application Data\\Adobe\\InDesign\\Version 5.0\\Caches\\iZINE\\DownloadedFile" );
	
	IDFile downloadedFile;
	Utils<IZPFileUtils>()->ConvertStrToIDFile( downloadedFile, downloadedFilePath );

	CZPTestWSCalling::TestDownloadVersion( versionID, downloadedFile );
}

//----------------------------------------------------------------------------------------
// TestDownloadVersion
//----------------------------------------------------------------------------------------
void
CZPTestWSCalling::TestDownloadVersion(
	const PMString &			inVersionID,
	const IDFile &				inFileToSaveIn)
{
	#ifndef DEBUG
	return;
#endif
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IZPWSController> wsController( gSession , UseDefaultIID() );
	ASSERT(wsController);

	AZPSoapRequest * theRequest = Utils<IZPAMWSUtils>()->CreateDownloadVersionRequest( inVersionID, inFileToSaveIn );

	NewWSSyncRequestWithRequestData(theRequest)

	InterfacePtr<IZPUserCredentials> requestCredentials( QueryTestingUserCredentials() );

	soapRequest->GetRequestData()->SetUserCredentials( requestCredentials );

	wsController->AddRequest( soapRequest, false);	//Sync call.

	ErrorCode toReturn = kFailure;
	ZPWSError soapError = soapResponse->GetWSError();
	if( soapError == 0 )
	{
		const CZPDownloadVersionResponse * theResponse = static_cast<const CZPDownloadVersionResponse*>( soapResponse->GetResponseData() );
		if( theResponse && theResponse->GetErrorCode() == 0)
		{
			toReturn = kSuccess;
		}
	}
}
