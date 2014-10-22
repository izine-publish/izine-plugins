//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/Utils/CZPAMWSUtils.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 7-6-2010
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

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPApp.h"
#include "IZPAssetMgr.h"
#include "IZPAMWSUtils.h"
#include "IZPSoapRequest.h"
#include "IZPSoapResponse.h"
#include "IZPWSController.h"
#include "IZPWSResponseHandler.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//IZP General includes
#include "CZPAcquireLockRequest.h"
#include "CZPAcquireLockResponse.h"
#include "CZPCanUpdateAssetNameRequest.h"
#include "CZPCanUpdateAssetNameResponse.h"
#include "CZPCheckInAssetRequest.h"
#include "CZPCheckInAssetResponse.h"
#include "CZPCreateAssetRequest.h"
#include "CZPCreateAssetResponse.h"
#include "CZPDownloadThumbnailRequest.h"
#include "CZPDownloadVersionRequest.h"
#include "CZPDownloadVersionResponse.h"
#include "CZPFetchAssetRequest.h"
#include "CZPFetchAssetResponse.h"
#include "CZPFetchAssetsRequest.h"
#include "CZPFetchAssetsResponse.h"
#include "CZPFetchLockInfoRequest.h"
#include "CZPReleaseLockRequest.h"
#include "CZPReleaseLockResponse.h"
#include "CZPUpdateAssetNameRequest.h"
#include "CZPUpdateAssetNameResponse.h"
#include "CZPUpdateAssetRequest.h"
#include "CZPUpdateAssetResponse.h"
#include "CZPUpdateAssetStatusRequest.h"
#include "CZPUpdateAssetStatusResponse.h"
#include "CZPUploadExportRequest.h"
#include "CZPUploadExportResponse.h"
#include "CZPUploadPagePreviewRequest.h"
#include "CZPUploadPagePreviewResponse.h"
#include "CZPUploadPDFRequest.h"
#include "CZPUploadPDFResponse.h"
#include "CZPUploadVersionRequest.h"
#include "CZPUploadVersionResponse.h"
#include "StUtils.h"
#include "ZPDataHelper.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_WS
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define CreateAutoDelRequestObj(reqClass)				\
	reqClass * theRequest = new reqClass;				\
	StPtrDeleter<AZPSoapRequest>	autoDel( theRequest, true)
#define ReturnTheRequest								\
	autoDel.Forget();									\
	return theRequest

#define inheritClass CPMUnknown<IZPAMWSUtils>
class CZPAMWSUtils : public inheritClass
{
public:
						CZPAMWSUtils(IPMUnknown * boss);
	virtual				~CZPAMWSUtils();

	ErrorCode			CanUpdateAssetName_sync(
							const PMString &			inAssetID,
							const PMString &			inEditionID,
							const PMString &			inTypeID,
							const PMString &			inParentID,
							const PMString &			inAssetName ) const;

	ErrorCode			CreateNewAsset(
							PMString &					oAssetID,
							const PMString &			inAssetName,
							const PMString &			inTypeID,
							const PMString &			inEditionID,
							const PMString &			inLinkedWithAssetID ) const;

	ErrorCode			UpdateAsset_sync(
							const PMString &			inAssetID,
							const PMString &			inAssetName,
							const PMString &			inEditionID,
							const PMString &			inVersionID,
							const PMString &			inVersionStatusID ) const;

	ErrorCode			UpdateAssetName_sync(
							const PMString &			inAssetID,
							const PMString &			inAssetName ) const;

	ErrorCode			UpdateAssetStatus_sync(
							const PMString &			inAssetID,
							const PMString &			inOldStatusID,
							const PMString &			inNewStatusID ) const;

	ErrorCode			AcquireLock_sync(
							const PMString &			inAssetID,
							const PMString &			inLinkedWithDocID,
							const PMString &			inLinkedWithDocName ) const;

	ErrorCode			ReleaseLock_sync(
							const PMString &			inAssetID ) const;

	ErrorCode			UploadVersion_sync(
							const PMString &			inAssetID,
							const PMString &			inNewVersionID,
							const IDFile &				inAssetFile,
							PMString &					oVersionID) const;

	ErrorCode			UploadExport_sync(
							const PMString &			inAssetID,
							const PMString &			inNewVersionID,
							const IDFile &				inAssetFile,
							const bool					inIsXML,
							const bool					inIsHighRes,
							const PMString &			inImageFileName ) const;

	ErrorCode			UploadPDF_sync(
							const PMString &			inAssetID,
							const PMString &			inNewVersionID,
							const IDFile &				inPDFFile ) const;

	ErrorCode			UploadPagePreview_sync(
							const PMString &			inAssetID,
							const PMString &			inNewVersionID,
							const PMString &			inPageID,
							int32						inPageNumber,
							const PMString &			inPageType,
							uchar *						inDataBuffer,
							int							inDataLen ) const;

	ErrorCode			CheckInAsset_sync(
							const PMString &			inAssetID,
							const PMString &			inNewVersionID,
							const PMString &			inVersionStatusID,
							const PMString &			inCheckInComments,
							int							inHeadVersion) const;
#pragma mark -
// async updater functions.
	void				UploadPDF_async(
							const PMString &			inAssetID,
							const PMString &			inNewVersionID,
							const IDFile &				inPDFFile ) const;
#pragma mark -
// sync Getter functions, no changes made on WS state.
	ErrorCode			FetchAssetInfo_sync(
							const PMString &			inAssetID ) const;
	ErrorCode			FetchAssetInfo_sync(
							const ZPAssetIDsList &		inAssetIDs ) const;
	ErrorCode			FetchAssetInfo_sync_OneByOne(
							const ZPAssetIDsList &		inAssetIDs ) const;
	void				FetchAssetsLockInfo_sync(
							const ZPAssetIDsList &		inAssetIDs ) const;
	
	ErrorCode			DownloadVersion_sync(
							const PMString &			inAssetID,
							const PMString &			inVersionID,
							const IDFile &				inFileToSaveIn ) const;
#pragma mark -
// async Getter functions, no changes made on WS state.
	void				FetchAssetsLockInfo_async(
							const ZPAssetIDsList &		inAssetIDs ) const;

	void				FetchAssetsInfo_async(
							const ZPAssetIDsList &		inAssetIDs ) const;

	void				DownloadThumbnail_async(
							const PMString &			inAssetID,
							const IDFile &				inFileToSaveIn ) const;
#pragma mark -
//Create requests - WS state mofiying
	AZPSoapRequest *	CreateCanUpdateAssetNameRequest(
							const PMString &			inAssetID,
							const PMString &			inEditionID,
							const PMString &			inTypeID,
							const PMString &			inParentID,
							const PMString &			inAssetName) const;

	AZPSoapRequest *	CreateCreateAssetRequest(
							const PMString &			inAssetName,
							const PMString &			inTypeID,
							const PMString &			inEditionID,
							const PMString &			inLinkedWithAssetID ) const;

	AZPSoapRequest *	CreateUpdateAssetRequest(
							const PMString &			inAssetID,
							const PMString &			inAssetName,
							const PMString &			inEditionID,
							const PMString &			inVersionID,
							const PMString &			inVersionStatusID) const;

	AZPSoapRequest *	CreateUpdateAssetNameRequest(
							const PMString &			inAssetID,
							const PMString &			inAssetName) const;

	AZPSoapRequest *	CreateUpdateAssetStatusRequest(
							const PMString &			inAssetID,
							const PMString &			inOldStatusID,
							const PMString &			inNewStatusID) const;

	AZPSoapRequest *	CreateAcquireLockRequest(
							const PMString &			inAssetID,
							const PMString &			inLinkedWithDocID,
							const PMString &			inLinkedWithDocName ) const;

	AZPSoapRequest *	CreateReleaseLockRequest(
							const PMString &			inAssetID ) const;

	AZPSoapRequest *	CreateUploadVersionRequest(
							const PMString &			inAssetID,
							const PMString &			inNewVersionID,
							const IDFile &				inAssetFile ) const;

	AZPSoapRequest *	CreateUploadExportRequest(
							const PMString &			inAssetID,
							const PMString &			inNewVersionID,
							const IDFile &				inAssetFile,
							const bool					inIsXML,
							const bool					inIsHighRes,
							const PMString &			inImageFileName ) const;

	AZPSoapRequest *	CreateUploadPDFRequest(
							const PMString &			inAssetID,
							const PMString &			inNewVersionID,
							const IDFile &				inPDFFile ) const;

	AZPSoapRequest *	CreateUploadPagePreviewRequest(
							const PMString &			inAssetID,
							const PMString &			inNewVersionID,
							const PMString &			inPageID,
							int32						inPageNumber,
							const PMString &			inPageType,
							uchar *						inDataBuffer,
							int							inDataLen ) const;

	AZPSoapRequest *	CreateCheckInAssetRequest(
							const PMString &			inAssetID,
							const PMString &			inNewVersionID,
							const PMString &			inVersionStatusID,
							const PMString &			inCheckInComments,
							int							inHeadVersion) const;
#pragma mark -
//Create requests - WS state not mofiying
	AZPSoapRequest *	CreateFetchAssetInfoRequest(
							const PMString &			inAssetID ) const;
	AZPSoapRequest *	CreateFetchAssetsInfoRequest(
							const ZPAssetIDsList &		inAssetID ) const;
	AZPSoapRequest *	CreateFetchLockInfoRequest(
							const ZPAssetIDsList &		inAssetIDs ) const;
	
	AZPSoapRequest *	CreateDownloadVersionRequest(
							const PMString &			inVersionID,
							const IDFile &				inFileToSaveIn ) const;

	AZPSoapRequest *	CreateDownloadThumbnailRequest(
							const PMString &			inAssetID,
							const IDFile &				inFileToSaveIn ) const;


protected:
private:
};


CREATE_PMINTERFACE(CZPAMWSUtils, kZPAMWSUtilsImpl)

CZPAMWSUtils::CZPAMWSUtils(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMWSUtils::~CZPAMWSUtils()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// CanUpdateAssetName_sync
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMWSUtils::CanUpdateAssetName_sync(
	const PMString &			inAssetID,
	const PMString &			inEditionID,
	const PMString &			inTypeID,
	const PMString &			inParentID,
	const PMString &			inAssetName) const
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IZPWSController> wsController( gSession , UseDefaultIID() );
	ASSERT(wsController);

	AZPSoapRequest * theRequest = this->CreateCanUpdateAssetNameRequest( inAssetID, inEditionID, inTypeID, inParentID, inAssetName );

	NewWSSyncRequestWithRequestData(theRequest)

#ifdef DEBUG
	InterfacePtr<const IControllingUnknown> controlling( soapRequest, UseDefaultIID() );
	int theRefCount = controlling->GetRefCount();
	IZPLog_Str( thisFileLA, enLT_ObjectCreations, "Sync Request boss at 0x%08X has refcount = %d", controlling.get(), theRefCount );
#endif
	wsController->AddRequest( soapRequest, false);	//Sync call.

	ErrorCode toReturn = kFailure;
	ZPWSError soapError = soapResponse->GetWSError();
	if( soapError == 0 )
	{
		const CZPCanUpdateAssetNameResponse * theResponse = static_cast<const CZPCanUpdateAssetNameResponse*>( soapResponse->GetResponseData() );
		if( theResponse )
		{
			if( theResponse->GetIsSuccessful())
				toReturn = kSuccess;
		}
	}
#ifdef DEBUG
	theRefCount = controlling->GetRefCount();
	IZPLog_Str( thisFileLA, enLT_ObjectCreations, "Sync Request boss at 0x%08X has refcount = %d", controlling.get(), theRefCount );
#endif
	return toReturn;
}

//----------------------------------------------------------------------------------------
// CreateNewAsset
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMWSUtils::CreateNewAsset(
	PMString &					oAssetID,
	const PMString &			inAssetName,
	const PMString &			inTypeID,
	const PMString &			inEditionID,
	const PMString &			inLinkedWithAssetID ) const
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	oAssetID = kNullString;

	InterfacePtr<IZPWSController> wsController( gSession , UseDefaultIID() );
	ASSERT(wsController);

	AZPSoapRequest * theRequest = this->CreateCreateAssetRequest( inAssetName, inTypeID, inEditionID, inLinkedWithAssetID );

	NewWSSyncRequestWithRequestData(theRequest)

#ifdef DEBUG
	InterfacePtr<const IControllingUnknown> controlling( soapRequest, UseDefaultIID() );
	int theRefCount = controlling->GetRefCount();
	IZPLog_Str( thisFileLA, enLT_ObjectCreations, "Sync Request boss at 0x%08X has refcount = %d", controlling.get(), theRefCount );
#endif
	wsController->AddRequest( soapRequest, false);	//Sync call.

	ErrorCode toReturn = kFailure;
	ZPWSError soapError = soapResponse->GetWSError();
	if( soapError == 0 )
	{
		const CZPCreateAssetResponse * theResponse = static_cast<const CZPCreateAssetResponse*>( soapResponse->GetResponseData() );
		if( theResponse )
		{
			toReturn = kSuccess;
			oAssetID = theResponse->GetAssetID();

			InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
			ASSERT( zpApp );

			InterfacePtr<IZPAssetMgr> theAssetMgr( zpApp->GetAssetMgr(), UseDefaultIID() );
			ASSERT(theAssetMgr);
			
			const IStringData * assetID = nil;
			const IStringData * editionID = ZPDataHelper::GetEditionByID( inEditionID );
			const ZPAssetDetailInfo & assetDetailInfo = theResponse->GetDetailInfo();
			theAssetMgr->AddAsset( editionID, oAssetID, &inAssetName, &assetDetailInfo );
		}
	}
#ifdef DEBUG
	theRefCount = controlling->GetRefCount();
	IZPLog_Str( thisFileLA, enLT_ObjectCreations, "Sync Request boss at 0x%08X has refcount = %d", controlling.get(), theRefCount );
#endif
	return toReturn;
}

//----------------------------------------------------------------------------------------
// UpdateAsset_sync
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMWSUtils::UpdateAsset_sync(
	const PMString &			inAssetID,
	const PMString &			inAssetName,
	const PMString &			inEditionID,
	const PMString &			inVersionID,
	const PMString &			inVersionStatusID) const
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IZPWSController> wsController( gSession , UseDefaultIID() );
	ASSERT(wsController);

	AZPSoapRequest * theRequest = this->CreateUpdateAssetRequest( inAssetID, inAssetName, inEditionID, inVersionID, inVersionStatusID );

	NewWSSyncRequestWithRequestData(theRequest)

#ifdef DEBUG
	InterfacePtr<const IControllingUnknown> controlling( soapRequest, UseDefaultIID() );
	int theRefCount = controlling->GetRefCount();
	IZPLog_Str( thisFileLA, enLT_ObjectCreations, "Sync Request boss at 0x%08X has refcount = %d", controlling.get(), theRefCount );
#endif
	wsController->AddRequest( soapRequest, false);	//Sync call.

	ErrorCode toReturn = kFailure;
	ZPWSError soapError = soapResponse->GetWSError();
	if( soapError == 0 )
	{
		const CZPUpdateAssetResponse * theResponse = static_cast<const CZPUpdateAssetResponse*>( soapResponse->GetResponseData() );
		if( theResponse )
		{
			toReturn = kSuccess;
			
			InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
			ASSERT( zpApp );

			InterfacePtr<IZPAssetMgr> theAssetMgr( zpApp->GetAssetMgr(), UseDefaultIID() );
			ASSERT(theAssetMgr);
			
			//update the Asset object
			//Someday: find a way that server call is not made for this
			ZPAssetIDsList assetList;
			assetList.push_back( inAssetID );
			this->FetchAssetInfo_sync( inAssetID );
		}
	}
#ifdef DEBUG
	theRefCount = controlling->GetRefCount();
	IZPLog_Str( thisFileLA, enLT_ObjectCreations, "Sync Request boss at 0x%08X has refcount = %d", controlling.get(), theRefCount );
#endif
	return toReturn;
}

//----------------------------------------------------------------------------------------
// UpdateAssetName_sync
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMWSUtils::UpdateAssetName_sync(
	const PMString &			inAssetID,
	const PMString &			inAssetName) const
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IZPWSController> wsController( gSession , UseDefaultIID() );
	ASSERT(wsController);

	AZPSoapRequest * theRequest = this->CreateUpdateAssetNameRequest( inAssetID, inAssetName );

	NewWSSyncRequestWithRequestData(theRequest)

#ifdef DEBUG
	InterfacePtr<const IControllingUnknown> controlling( soapRequest, UseDefaultIID() );
	int theRefCount = controlling->GetRefCount();
	IZPLog_Str( thisFileLA, enLT_ObjectCreations, "Sync Request boss at 0x%08X has refcount = %d", controlling.get(), theRefCount );
#endif
	wsController->AddRequest( soapRequest, false);	//Sync call.

	ErrorCode toReturn = kFailure;
	ZPWSError soapError = soapResponse->GetWSError();
	if( soapError == 0 )
	{
		const CZPUpdateAssetNameResponse * theResponse = static_cast<const CZPUpdateAssetNameResponse*>( soapResponse->GetResponseData() );
		if( theResponse )
		{
			toReturn = kSuccess;
			
			InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
			ASSERT( zpApp );

			InterfacePtr<IZPAssetMgr> theAssetMgr( zpApp->GetAssetMgr(), UseDefaultIID() );
			ASSERT(theAssetMgr);

			theAssetMgr->UpdateAssetName(inAssetID, inAssetName);
		}
	}
#ifdef DEBUG
	theRefCount = controlling->GetRefCount();
	IZPLog_Str( thisFileLA, enLT_ObjectCreations, "Sync Request boss at 0x%08X has refcount = %d", controlling.get(), theRefCount );
#endif
	return toReturn;
}

//----------------------------------------------------------------------------------------
// UpdateAssetStatus_sync
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMWSUtils::UpdateAssetStatus_sync(
	const PMString &			inAssetID,
	const PMString &			inOldStatusID,
	const PMString &			inNewStatusID) const
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IZPWSController> wsController( gSession , UseDefaultIID() );
	ASSERT(wsController);

	AZPSoapRequest * theRequest = this->CreateUpdateAssetStatusRequest( inAssetID, inOldStatusID, inNewStatusID );

	NewWSSyncRequestWithRequestData(theRequest)

#ifdef DEBUG
	InterfacePtr<const IControllingUnknown> controlling( soapRequest, UseDefaultIID() );
	int theRefCount = controlling->GetRefCount();
	IZPLog_Str( thisFileLA, enLT_ObjectCreations, "Sync Request boss at 0x%08X has refcount = %d", controlling.get(), theRefCount );
#endif
	wsController->AddRequest( soapRequest, false);	//Sync call.

	ErrorCode toReturn = kFailure;
	ZPWSError soapError = soapResponse->GetWSError();
	if( soapError == 0 )
	{
		const CZPUpdateAssetStatusResponse * theResponse = static_cast<const CZPUpdateAssetStatusResponse*>( soapResponse->GetResponseData() );
		if( theResponse && theResponse->GetIsSuccessful() )
		{
			toReturn = kSuccess;
			
			InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
			ASSERT( zpApp );

			InterfacePtr<IZPAssetMgr> theAssetMgr( zpApp->GetAssetMgr(), UseDefaultIID() );
			ASSERT(theAssetMgr);
			
			theAssetMgr->UpdateAssetStatus(inAssetID, inNewStatusID);
		}
	}
#ifdef DEBUG
	theRefCount = controlling->GetRefCount();
	IZPLog_Str( thisFileLA, enLT_ObjectCreations, "Sync Request boss at 0x%08X has refcount = %d", controlling.get(), theRefCount );
#endif
	return toReturn;
}

//----------------------------------------------------------------------------------------
// AcquireLock_sync
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMWSUtils::AcquireLock_sync(
	const PMString &			inAssetID,
	const PMString &			inLinkedWithDocID,
	const PMString &			inLinkedWithDocName) const
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IZPWSController> wsController( gSession , UseDefaultIID() );
	ASSERT(wsController);

	AZPSoapRequest * theRequest = this->CreateAcquireLockRequest( inAssetID, inLinkedWithDocID, inLinkedWithDocName );

	NewWSSyncRequestWithRequestData(theRequest)

	wsController->AddRequest( soapRequest, false);	//Sync call.

	ErrorCode toReturn = kFailure;
	ZPWSError soapError = soapResponse->GetWSError();
	if( soapError == 0 )
	{
		const CZPAcquireLockResponse * theResponse = static_cast<const CZPAcquireLockResponse*>( soapResponse->GetResponseData() );
		if( theResponse && theResponse->GetErrorCode() == 0)
		{
			//update the lock status with the Asset.
			InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
			ASSERT( zpApp );

			InterfacePtr<IZPAssetMgr> theAssetMgr( zpApp->GetAssetMgr(), UseDefaultIID() );
			ASSERT(theAssetMgr);

			theAssetMgr->UpdateAssetLockInfo( theResponse->GetLockInfo() );
			toReturn = kSuccess;
		}
	}
	return toReturn;
}

//----------------------------------------------------------------------------------------
// ReleaseLock_sync
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMWSUtils::ReleaseLock_sync(
	const PMString &			inAssetID) const
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IZPWSController> wsController( gSession , UseDefaultIID() );
	ASSERT(wsController);

	AZPSoapRequest * theRequest = this->CreateReleaseLockRequest( inAssetID );

	NewWSSyncRequestWithRequestData(theRequest)

	wsController->AddRequest( soapRequest, false);	//Sync call.

	ErrorCode toReturn = kFailure;
	ZPWSError soapError = soapResponse->GetWSError();
	if( soapError == 0 )
	{
		const CZPReleaseLockResponse * theResponse = static_cast<const CZPReleaseLockResponse*>( soapResponse->GetResponseData() );
		if( theResponse && theResponse->GetErrorCode() == 0)
		{
			//update the lock status with the Asset.
			InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
			ASSERT( zpApp );

			InterfacePtr<IZPAssetMgr> theAssetMgr( zpApp->GetAssetMgr(), UseDefaultIID() );
			ASSERT(theAssetMgr);

			ZPAssetLockInfo emptyLockInfo;
			emptyLockInfo.mAssetID = inAssetID;
			theAssetMgr->UpdateAssetLockInfo( emptyLockInfo );
			toReturn = kSuccess;
		}
	}
	return toReturn;
}

//----------------------------------------------------------------------------------------
// UploadVersion_sync
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMWSUtils::UploadVersion_sync(
	const PMString &			inAssetID,
	const PMString &			inNewVersionID,
	const IDFile &				inAssetFile,
	PMString &					oVersionID) const
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	oVersionID = kNullString;

	InterfacePtr<IZPWSController> wsController( gSession , UseDefaultIID() );
	ASSERT(wsController);

	AZPSoapRequest * theRequest = this->CreateUploadVersionRequest( inAssetID, inNewVersionID, inAssetFile );

	NewWSSyncRequestWithRequestData(theRequest)

	wsController->AddRequest( soapRequest, false);	//Sync call.

	ErrorCode toReturn = kFailure;
	ZPWSError soapError = soapResponse->GetWSError();
	if( soapError == 0 )
	{
		const CZPUploadVersionResponse * theResponse = static_cast<const CZPUploadVersionResponse*>( soapResponse->GetResponseData() );
		if( theResponse && theResponse->GetErrorCode() == 0)
		{
			toReturn = kSuccess;
			oVersionID = theResponse->GetVersionId();
		}
	}
	return toReturn;
}

//----------------------------------------------------------------------------------------
// UploadExport_sync
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMWSUtils::UploadExport_sync(
	const PMString &			inAssetID,
	const PMString &			inNewVersionID,
	const IDFile &				inAssetFile,
	const bool					inIsXML,
	const bool					inIsHighRes,
	const PMString &			inImageFileName) const
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IZPWSController> wsController( gSession , UseDefaultIID() );
	ASSERT(wsController);

	AZPSoapRequest * theRequest = this->CreateUploadExportRequest( inAssetID, inNewVersionID, inAssetFile, inIsXML, inIsHighRes, inImageFileName );

	NewWSSyncRequestWithRequestData(theRequest)

	wsController->AddRequest( soapRequest, false);	//Sync call.

	ErrorCode toReturn = kFailure;
	ZPWSError soapError = soapResponse->GetWSError();
	if( soapError == 0 )
	{
		const CZPUploadExportResponse * theResponse = static_cast<const CZPUploadExportResponse*>( soapResponse->GetResponseData() );
		if( theResponse && theResponse->GetErrorCode() == 0)
		{
			//TODO:
			toReturn = kSuccess;
		}
	}
	return toReturn;
}

//----------------------------------------------------------------------------------------
// UploadPDF_sync
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMWSUtils::UploadPDF_sync(
	const PMString &			inAssetID,
	const PMString &			inNewVersionID,
	const IDFile &				inPDFFile) const
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IZPWSController> wsController( gSession , UseDefaultIID() );
	ASSERT(wsController);

	AZPSoapRequest * theRequest = this->CreateUploadPDFRequest( inAssetID, inNewVersionID, inPDFFile );

	NewWSSyncRequestWithRequestData(theRequest)

	wsController->AddRequest( soapRequest, false);	//Sync call.

	ErrorCode toReturn = kFailure;
	ZPWSError soapError = soapResponse->GetWSError();
	if( soapError == 0 )
	{
		const CZPUploadPDFResponse * theResponse = static_cast<const CZPUploadPDFResponse*>( soapResponse->GetResponseData() );
		if( theResponse && theResponse->GetErrorCode() == 0)
		{
			//TODO:
			toReturn = kSuccess;
		}
	}
	return toReturn;
}

//----------------------------------------------------------------------------------------
// UploadPagePreview_sync
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMWSUtils::UploadPagePreview_sync(
	const PMString &			inAssetID,
	const PMString &			inNewVersionID,
	const PMString &			inPageID,
	int32						inPageNumber,
	const PMString &			inPageType,
	uchar *						inDataBuffer,
	int							inDataLen) const
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IZPWSController> wsController( gSession , UseDefaultIID() );
	ASSERT(wsController);

	AZPSoapRequest * theRequest = this->CreateUploadPagePreviewRequest( inAssetID, inNewVersionID, inPageID, inPageNumber, inPageType, inDataBuffer, inDataLen );

	NewWSSyncRequestWithRequestData(theRequest)

	wsController->AddRequest( soapRequest, false);	//Sync call.

	ErrorCode toReturn = kFailure;
	ZPWSError soapError = soapResponse->GetWSError();
	if( soapError == 0 )
	{
		const CZPUploadPagePreviewResponse * theResponse = static_cast<const CZPUploadPagePreviewResponse*>( soapResponse->GetResponseData() );
		if( theResponse && theResponse->GetErrorCode() == 0)
		{
			//TODO:
			toReturn = kSuccess;
		}
	}
	return toReturn;
}

//----------------------------------------------------------------------------------------
// CheckInAsset_sync
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMWSUtils::CheckInAsset_sync(
	const PMString &			inAssetID,
	const PMString &			inNewVersionID,
	const PMString &			inVersionStatusID,
	const PMString &			inCheckInComments,
	int							inHeadVersion ) const
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IZPWSController> wsController( gSession , UseDefaultIID() );
	ASSERT(wsController);

	AZPSoapRequest * theRequest = this->CreateCheckInAssetRequest( inAssetID, inNewVersionID, inVersionStatusID, inCheckInComments, inHeadVersion );

	NewWSSyncRequestWithRequestData(theRequest)

	wsController->AddRequest( soapRequest, false);	//Sync call.

	ErrorCode toReturn = kFailure;
	ZPWSError soapError = soapResponse->GetWSError();
	if( soapError == 0 )
	{
		const CZPCheckInAssetResponse * theResponse = static_cast<const CZPCheckInAssetResponse*>( soapResponse->GetResponseData() );
		if( theResponse && theResponse->GetErrorCode() == 0)
		{
			//Someday: Remove following call, as now we get this in asset dto back in check in response.
			this->FetchAssetInfo_sync( inAssetID );

			toReturn = kSuccess;
		}
	}
	return toReturn;
}
#pragma mark -
//----------------------------------------------------------------------------------------
// UploadPDF_async
//----------------------------------------------------------------------------------------
void
CZPAMWSUtils::UploadPDF_async(
	const PMString &			inAssetID,
	const PMString &			inNewVersionID,
	const IDFile &				inPDFFile) const
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IZPWSController> wsController( gSession , UseDefaultIID() );
	ASSERT(wsController);

	AZPSoapRequest * theRequest = this->CreateUploadPDFRequest( inAssetID, inNewVersionID, inPDFFile );
	NewWSRequestWithData( theRequest )

	wsController->AddRequest( soapRequest );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// FetchAssetInfo_sync
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMWSUtils::FetchAssetInfo_sync(
	const PMString &			inAssetID) const
{
	ZPAssetIDsList assetIDsList;
	assetIDsList.push_back( inAssetID );
	return this->FetchAssetInfo_sync( assetIDsList );
}

//----------------------------------------------------------------------------------------
// FetchAssetInfo_sync
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMWSUtils::FetchAssetInfo_sync(
	const ZPAssetIDsList &		inAssetIDs) const
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IZPWSController> wsController( gSession , UseDefaultIID() );
	ASSERT(wsController);

	ZPAssetIDsList::const_iterator currIter = inAssetIDs.begin();
	ZPAssetIDsList::const_iterator endIter = inAssetIDs.end();

	ErrorCode toReturn = kFailure;
	
	AZPSoapRequest * theRequest = this->CreateFetchAssetsInfoRequest( inAssetIDs );

	NewWSSyncRequestWithRequestData(theRequest)

	wsController->AddRequest( soapRequest, false);	//Sync call.

	ZPWSError soapError = soapResponse->GetWSError();
	if( soapError == 0 )
	{
		const CZPFetchAssetsResponse * theResponse = static_cast<const CZPFetchAssetsResponse*>( soapResponse->GetResponseData() );
		if( theResponse && theResponse->GetErrorCode() == 0)
		{
			InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
			ASSERT( zpApp );

			InterfacePtr<IZPAssetMgr> theAssetMgr( zpApp->GetAssetMgr(), UseDefaultIID() );
			ASSERT(theAssetMgr);

			theAssetMgr->HandleWSResponse( soapRequest, theResponse, 0 );

			toReturn = kSuccess;
		}
	}
	return toReturn;
}

//----------------------------------------------------------------------------------------
// FetchAssetsLockInfo_sync
//----------------------------------------------------------------------------------------
void
CZPAMWSUtils::FetchAssetsLockInfo_sync(
	const ZPAssetIDsList &		inAssetIDs) const
{
	//LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IZPWSController> wsController( gSession , UseDefaultIID() );
	ASSERT(wsController);

	AZPSoapRequest * theRequest = this->CreateFetchLockInfoRequest( inAssetIDs );
	NewWSRequestWithData( theRequest )

	wsController->AddRequest( soapRequest, false );	//Sync call.
}

//----------------------------------------------------------------------------------------
// FetchAssetInfo_sync_OneByOne
// Not being used.
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMWSUtils::FetchAssetInfo_sync_OneByOne(
	const ZPAssetIDsList &		inAssetIDs) const
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IZPWSController> wsController( gSession , UseDefaultIID() );
	ASSERT(wsController);

	ZPAssetIDsList::const_iterator currIter = inAssetIDs.begin();
	ZPAssetIDsList::const_iterator endIter = inAssetIDs.end();

	ErrorCode toReturn = kFailure;
	const IStringData * editionID = nil;
	PMString prevEditionID;	//For optimization, as editionID will be same in all response.

	while( currIter != endIter )
	{
		AZPSoapRequest * theRequest = this->CreateFetchAssetInfoRequest( *currIter );	//handling assetid arrary in another function

		NewWSSyncRequestWithRequestData(theRequest)

		wsController->AddRequest( soapRequest, false);	//Sync call.

		ZPWSError soapError = soapResponse->GetWSError();
		if( soapError == 0 )
		{
			const CZPFetchAssetResponse * theResponse = static_cast<const CZPFetchAssetResponse*>( soapResponse->GetResponseData() );
			if( theResponse && theResponse->GetErrorCode() == 0)	//Someday: find a better place to process response, so that same can be used for Asyn calls.
			{
				//update the asset info with the Asset.
				InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
				ASSERT( zpApp );

				InterfacePtr<IZPAssetMgr> theAssetMgr( zpApp->GetAssetMgr(), UseDefaultIID() );
				ASSERT(theAssetMgr);

				const IStringData * assetID = theAssetMgr->GetAssetByID( *currIter );
				if( assetID )
					theAssetMgr->UpdateAsset( assetID, &(theResponse->GetName()), &(theResponse->GetDetailInfo()));
				else
				{
					if( editionID == nil || prevEditionID.Compare( kFalse, theResponse->GetEditionID() ) != 0 )
					{	//Update the cached pointer to edition
						prevEditionID = theResponse->GetEditionID();
						editionID = ZPDataHelper::GetEditionByID( prevEditionID );
					}

					theAssetMgr->AddAsset( editionID, *currIter, &(theResponse->GetName()), &(theResponse->GetDetailInfo()));
				}
				toReturn = kSuccess;
			}
			else
			{
				toReturn = kFailure;
				break;//No further processing. //TODO: check if asset was deleted or not.In such case remove from cache and continue.
			}
		}
		++currIter;
	}
	return toReturn;
}

//----------------------------------------------------------------------------------------
// DownloadVersion_sync
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMWSUtils::DownloadVersion_sync(
	const PMString &			inAssetID,
	const PMString &			inVersionID,
	const IDFile &				inFileToSaveIn) const
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IZPWSController> wsController( gSession , UseDefaultIID() );
	ASSERT(wsController);

	AZPSoapRequest * theRequest = this->CreateDownloadVersionRequest( inVersionID, inFileToSaveIn );

	NewWSSyncRequestWithRequestData(theRequest)

	wsController->AddRequest( soapRequest, false);	//Sync call.

	ErrorCode toReturn = kFailure;
	ZPWSError soapError = soapResponse->GetWSError();
	if( soapError == 0 )
	{
		const CZPDownloadVersionResponse * theResponse = static_cast<const CZPDownloadVersionResponse*>( soapResponse->GetResponseData() );
		if( theResponse && theResponse->GetErrorCode() == 0)
		{
			//We always download the latest version, so update the local info

			toReturn = kSuccess;
		}
	}
	return toReturn;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// FetchAssetsLockInfo_async
//----------------------------------------------------------------------------------------
void
CZPAMWSUtils::FetchAssetsLockInfo_async(
	const ZPAssetIDsList &		inAssetIDs) const
{
	//LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IZPWSController> wsController( gSession , UseDefaultIID() );
	ASSERT(wsController);

	AZPSoapRequest * theRequest = this->CreateFetchLockInfoRequest( inAssetIDs );
	NewWSRequestWithData( theRequest )

	wsController->AddRequest( soapRequest );
}

//----------------------------------------------------------------------------------------
// FetchAssetsInfo_async
//----------------------------------------------------------------------------------------
void
CZPAMWSUtils::FetchAssetsInfo_async(
	const ZPAssetIDsList &		inAssetIDs) const
{
	//LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IZPWSController> wsController( gSession , UseDefaultIID() );
	ASSERT(wsController);

	AZPSoapRequest * theRequest = this->CreateFetchAssetsInfoRequest( inAssetIDs );
	NewWSRequestWithData( theRequest )

	wsController->AddRequest( soapRequest );
}

//----------------------------------------------------------------------------------------
// DownloadThumbnail_async
//----------------------------------------------------------------------------------------
void
CZPAMWSUtils::DownloadThumbnail_async(
	const PMString &			inAssetID,
	const IDFile &				inFileToSaveIn) const
{
	//LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IZPWSController> wsController( gSession , UseDefaultIID() );
	ASSERT(wsController);

	AZPSoapRequest * theRequest = this->CreateDownloadThumbnailRequest( inAssetID, inFileToSaveIn );
	NewWSRequestWithData( theRequest )

	wsController->AddRequest( soapRequest );
}

#pragma mark - 
//----------------------------------------------------------------------------------------
// CreateCanUpdateAssetNameRequest
//----------------------------------------------------------------------------------------
AZPSoapRequest *
CZPAMWSUtils::CreateCanUpdateAssetNameRequest(
	const PMString &			inAssetID,
	const PMString &			inEditionID,
	const PMString &			inTypeID,
	const PMString &			inParentID,
	const PMString &			inAssetName) const
{
	LogFunctionEnterExit;
	CreateAutoDelRequestObj(CZPCanUpdateAssetNameRequest);

	theRequest->SetAssetID( inAssetID );
	theRequest->SetEditionID( inEditionID );
	theRequest->SetTypeID( inTypeID );
	theRequest->SetParentID( inParentID );
	theRequest->SetAssetName( inAssetName );

	ReturnTheRequest;
}

//----------------------------------------------------------------------------------------
// CreateCreateAssetRequest
//----------------------------------------------------------------------------------------
AZPSoapRequest *
CZPAMWSUtils::CreateCreateAssetRequest(
	const PMString &			inAssetName,
	const PMString &			inTypeID,
	const PMString &			inEditionID,
	const PMString &			inLinkedWithAssetID ) const
{
	LogFunctionEnterExit;
	CreateAutoDelRequestObj(CZPCreateAssetRequest);

	theRequest->SetAssetName( inAssetName );
	theRequest->SetEditionID( inEditionID );
	theRequest->GetDetailInfo().mTypeID = inTypeID;
	theRequest->GetDetailInfo().mDocInfo.mID = inLinkedWithAssetID;

	ReturnTheRequest;
}

//----------------------------------------------------------------------------------------
// CreateUpdateAssetRequest
//----------------------------------------------------------------------------------------
AZPSoapRequest *
CZPAMWSUtils::CreateUpdateAssetRequest(
	const PMString &			inAssetID,
	const PMString &			inAssetName,
	const PMString &			inEditionID,
	const PMString &			inVersionID,
	const PMString &			inVersionStatusID) const
{
	LogFunctionEnterExit;
	CreateAutoDelRequestObj(CZPUpdateAssetRequest);

	theRequest->SetAssetID( inAssetID );
	theRequest->SetAssetName( inAssetName );
	theRequest->SetEditionID( inEditionID );
	theRequest->GetDetailInfo().mVersionID = inVersionID;
	theRequest->GetDetailInfo().mStatusID = inVersionStatusID;

	ReturnTheRequest;
}

//----------------------------------------------------------------------------------------
// CreateUpdateAssetNameRequest
//----------------------------------------------------------------------------------------
AZPSoapRequest *
CZPAMWSUtils::CreateUpdateAssetNameRequest(
	const PMString &			inAssetID,
	const PMString &			inAssetName) const
{
	LogFunctionEnterExit;
	CreateAutoDelRequestObj(CZPUpdateAssetNameRequest);

	theRequest->SetAssetID( inAssetID );
	theRequest->SetAssetName( inAssetName );

	ReturnTheRequest;
}

//----------------------------------------------------------------------------------------
// CreateUpdateAssetStatusRequest
//----------------------------------------------------------------------------------------
AZPSoapRequest *
CZPAMWSUtils::CreateUpdateAssetStatusRequest(
	const PMString &			inAssetID,
	const PMString &			inOldStatusID,
	const PMString &			inNewStatusID) const
{
	LogFunctionEnterExit;
	CreateAutoDelRequestObj(CZPUpdateAssetStatusRequest);

	theRequest->SetAssetID( inAssetID );
	theRequest->SetOldStatusID( inOldStatusID );
	theRequest->SetNewStatusID( inNewStatusID );

	ReturnTheRequest;
}

//----------------------------------------------------------------------------------------
// CreateAcquireLockRequest
//----------------------------------------------------------------------------------------
AZPSoapRequest *
CZPAMWSUtils::CreateAcquireLockRequest(
	const PMString &			inAssetID,
	const PMString &			inLinkedWithDocID,
	const PMString &			inLinkedWithDocName ) const
{
	LogFunctionEnterExit;
	CreateAutoDelRequestObj(CZPAcquireLockRequest);

	theRequest->SetLinkedWithDocID( inLinkedWithDocID );
	theRequest->SetLinkedWithDocName( inLinkedWithDocName );
	theRequest->SetAssetID( inAssetID );

	ReturnTheRequest;
}

//----------------------------------------------------------------------------------------
// CreateReleaseLockRequest
//----------------------------------------------------------------------------------------
AZPSoapRequest *
CZPAMWSUtils::CreateReleaseLockRequest(
	const PMString &			inAssetID) const
{
	LogFunctionEnterExit;
	CreateAutoDelRequestObj(CZPReleaseLockRequest);

	theRequest->SetAssetID( inAssetID );

	ReturnTheRequest;
}

//----------------------------------------------------------------------------------------
// CreateUploadVersionRequest
//----------------------------------------------------------------------------------------
AZPSoapRequest *
CZPAMWSUtils::CreateUploadVersionRequest(
	const PMString &			inAssetID,
	const PMString &			inNewVersionID,
	const IDFile &				inAssetFile) const
{
	LogFunctionEnterExit;
	CreateAutoDelRequestObj(CZPUploadVersionRequest);

	theRequest->SetAssetID( inAssetID );
	theRequest->SetVersionID( inNewVersionID );
	theRequest->CopyFileContents( inAssetFile );

	ReturnTheRequest;
}

//----------------------------------------------------------------------------------------
// CreateUploadExportRequest
//----------------------------------------------------------------------------------------
AZPSoapRequest *
CZPAMWSUtils::CreateUploadExportRequest(
	const PMString &			inAssetID,
	const PMString &			inNewVersionID,
	const IDFile &				inAssetFile,
	const bool					inIsXML,
	const bool					inIsHighRes,
	const PMString &			inImageFileName) const
{
	LogFunctionEnterExit;
	CreateAutoDelRequestObj(CZPUploadExportRequest);

	theRequest->SetAssetID( inAssetID );
	theRequest->SetVersionID( inNewVersionID );
	theRequest->SetIsXML( inIsXML );
	theRequest->SetIsHighRes( inIsHighRes );
	theRequest->SetImageFileName( inImageFileName );
	theRequest->CopyFileContents( inAssetFile );

	ReturnTheRequest;
}

//----------------------------------------------------------------------------------------
// CreateUploadPDFRequest
//----------------------------------------------------------------------------------------
AZPSoapRequest *
CZPAMWSUtils::CreateUploadPDFRequest(
	const PMString &			inAssetID,
	const PMString &			inNewVersionID,
	const IDFile &				inPDFFile) const
{
	LogFunctionEnterExit;
	CreateAutoDelRequestObj(CZPUploadPDFRequest);

	theRequest->SetAssetID( inAssetID );
	theRequest->SetVersionID( inNewVersionID );
	theRequest->CopyFileContents( inPDFFile );

	ReturnTheRequest;
}

//----------------------------------------------------------------------------------------
// CreateUploadPagePreviewRequest
//----------------------------------------------------------------------------------------
AZPSoapRequest *
CZPAMWSUtils::CreateUploadPagePreviewRequest(
	const PMString &			inAssetID,
	const PMString &			inNewVersionID,
	const PMString &			inPageID,
	int32						inPageNumber,
	const PMString &			inPageType,
	uchar *						inDataBuffer,
	int							inDataLen) const
{
	LogFunctionEnterExit;
	CreateAutoDelRequestObj(CZPUploadPagePreviewRequest);

	theRequest->SetAssetID( inAssetID );
	theRequest->SetVersionID( inNewVersionID );
	theRequest->SetCompressData( false );				//Must be set before setting data.
	theRequest->SetDataBuff( inDataBuffer, inDataLen );	//Request will own the buffer.
	theRequest->SetPageID( inPageID );
	theRequest->SetPageTypeID( inPageType );
	theRequest->SetPageNumber( inPageNumber );

	ReturnTheRequest;
}

//----------------------------------------------------------------------------------------
// CreateCheckInAssetRequest
//----------------------------------------------------------------------------------------
AZPSoapRequest *
CZPAMWSUtils::CreateCheckInAssetRequest(
	const PMString &			inAssetID,
	const PMString &			inNewVersionID,
	const PMString &			inVersionStatusID,
	const PMString &			inCheckInComments,
	int							inHeadVersion) const
{
	LogFunctionEnterExit;
	CreateAutoDelRequestObj(CZPCheckInAssetRequest);

	theRequest->SetAssetID( inAssetID );
	theRequest->SetNewVersionID( inNewVersionID );
	theRequest->SetStatusID( inVersionStatusID );
	theRequest->SetComment( inCheckInComments );
	theRequest->SetHeadVersion( inHeadVersion );

	ReturnTheRequest;
}
#pragma mark -
//Create requests - WS state not mofiying

//----------------------------------------------------------------------------------------
// CreateFetchAssetInfoRequest
//----------------------------------------------------------------------------------------
AZPSoapRequest *
CZPAMWSUtils::CreateFetchAssetInfoRequest(
	const PMString &			inAssetID) const
{
	LogFunctionEnterExit;
	CreateAutoDelRequestObj(CZPFetchAssetRequest);

	theRequest->SetAssetID( inAssetID );

	ReturnTheRequest;
}

//----------------------------------------------------------------------------------------
// CreateFetchAssetsInfoRequest
//----------------------------------------------------------------------------------------
AZPSoapRequest *
CZPAMWSUtils::CreateFetchAssetsInfoRequest(
	const ZPAssetIDsList &			inAssetID) const
{
	LogFunctionEnterExit;
	CreateAutoDelRequestObj(CZPFetchAssetsRequest);

	theRequest->SetAssetIDs( inAssetID );

	ReturnTheRequest;
}

//----------------------------------------------------------------------------------------
// CreateFetchLockInfoRequest
//----------------------------------------------------------------------------------------
AZPSoapRequest *
CZPAMWSUtils::CreateFetchLockInfoRequest(
	const ZPAssetIDsList &		inAssetIDs) const
{
	LogFunctionEnterExit;
	CreateAutoDelRequestObj(CZPFetchLockInfoRequest);

	theRequest->SetAssetIDs( inAssetIDs );

	ReturnTheRequest;
}

//----------------------------------------------------------------------------------------
// CreateDownloadVersionRequest
//----------------------------------------------------------------------------------------
AZPSoapRequest *
CZPAMWSUtils::CreateDownloadVersionRequest(
	const PMString &			inVersionID,
	const IDFile &				inFileToSaveIn) const
{
	LogFunctionEnterExit;
	CreateAutoDelRequestObj(CZPDownloadVersionRequest);

	theRequest->SetVersionID( inVersionID );
	theRequest->SetOutputFile( inFileToSaveIn );

	ReturnTheRequest;
}

//----------------------------------------------------------------------------------------
// CreateDownloadThumbnailRequest
//----------------------------------------------------------------------------------------
AZPSoapRequest *
CZPAMWSUtils::CreateDownloadThumbnailRequest(
	const PMString &			inAssetID,
	const IDFile &				inFileToSaveIn) const
{
	LogFunctionEnterExit;
	CreateAutoDelRequestObj(CZPDownloadThumbnailRequest);

	theRequest->SetAssetID( inAssetID );
	theRequest->SetOutputFile( inFileToSaveIn );

	ReturnTheRequest;
}



