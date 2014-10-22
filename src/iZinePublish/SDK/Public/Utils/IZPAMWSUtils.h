//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/Utils/IZPAMWSUtils.h $
//	$Revision: 3543 $
//	$Date: 2011-07-26 14:38:11 +0200 (Tue, 26 Jul 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 7-6-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPAMWSUtils_
#define _h_IZPAMWSUtils_
#pragma once

#include "ZPTypes.h"

class AZPSoapRequest;

class IZPAMWSUtils : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPAMWSUTILS };

	//Sync call to create a new asset of given type.
	virtual ErrorCode	CanUpdateAssetName_sync(
							const PMString &			inAssetID,
							const PMString &			inEditionID,
							const PMString &			inTypeID,
							const PMString &			inParentID,
							const PMString &			inAssetName ) const = 0;

	virtual ErrorCode	CreateNewAsset(
							PMString &					oAssetID,
							const PMString &			inAssetName,
							const PMString &			inTypeID,
							const PMString &			inEditionID,
							const PMString &			inLinkedWithAssetID = kNullString ) const = 0;

	virtual ErrorCode	UpdateAsset_sync(
							const PMString &			inAssetID,
							const PMString &			inAssetName,
							const PMString &			inEditionID,
							const PMString &			inVersionID,
							const PMString &			inVersionStatusID ) const = 0;

	virtual ErrorCode	UpdateAssetName_sync(
							const PMString &			inAssetID,
							const PMString &			inAssetName ) const = 0;

	virtual ErrorCode		UpdateAssetStatus_sync(
							const PMString &			inAssetID,
							const PMString &			inOldStatusID,
							const PMString &			inNewStatusID ) const = 0;

	//Sync call
	virtual ErrorCode	AcquireLock_sync(
							const PMString &			inAssetID,
							const PMString &			inLinkedWithDocID,
							const PMString &			inLinkedWithDocName ) const = 0;

	//Sync call
	virtual ErrorCode	ReleaseLock_sync(
							const PMString &			inAssetID ) const = 0;

	//Sync call
	virtual ErrorCode	UploadVersion_sync(
							const PMString &			inAssetID,
							const PMString &			inNewVersionID,
							const IDFile &				inAssetFile,
							PMString &					oVersionID) const = 0;

	virtual ErrorCode	UploadExport_sync(
							const PMString &			inAssetID,
							const PMString &			inNewVersionID,
							const IDFile &				inAssetFile,
							const bool					inIsXML,
							const bool					inIsHighRes,
							const PMString &			inImageFileName ) const = 0;

	virtual ErrorCode	UploadPDF_sync(
							const PMString &			inAssetID,
							const PMString &			inNewVersionID,
							const IDFile &				inPDFFile ) const = 0;

	virtual ErrorCode	UploadPagePreview_sync(
							const PMString &			inAssetID,
							const PMString &			inNewVersionID,
							const PMString &			inPageID,
							int32						inPageNumber,
							const PMString &			inPageType,
							uchar *						inDataBuffer,
							int							inDataLen ) const = 0;

	virtual ErrorCode	CheckInAsset_sync(
							const PMString &			inAssetID,
							const PMString &			inNewVersionID,
							const PMString &			inVersionStatusID,
							const PMString &			inCheckInComments,
							int							inHeadVersion ) const = 0;
#pragma mark -
// async updater functions.
	virtual void		UploadPDF_async(
							const PMString &			inAssetID,
							const PMString &			inNewVersionID,
							const IDFile &				inPDFFile ) const = 0;
#pragma mark -
// sync Getter functions, no changes made on WS state.

	virtual ErrorCode	FetchAssetInfo_sync(
							const PMString &			inAssetID ) const = 0;
	virtual ErrorCode	FetchAssetInfo_sync(
							const ZPAssetIDsList &		inAssetIDs ) const = 0;
	virtual void		FetchAssetsLockInfo_sync(
							const ZPAssetIDsList &		inAssetIDs ) const = 0;

	virtual ErrorCode	DownloadVersion_sync(
							const PMString &			inAssetID,	//Used to update local info when download is done.
							const PMString &			inVersionID,
							const IDFile &				inFileToSaveIn ) const = 0;
#pragma mark -
// async Getter functions, no changes made on WS state.
	virtual void		FetchAssetsLockInfo_async(
							const ZPAssetIDsList &		inAssetIDs ) const = 0;
	virtual void		FetchAssetsInfo_async(
							const ZPAssetIDsList &		inAssetIDs ) const = 0;
	virtual void		DownloadThumbnail_async(
							const PMString &			inAssetID,
							const IDFile &				inFileToSaveIn ) const = 0;
#pragma mark -
//Create requests - WS state mofiying

	virtual AZPSoapRequest *	CreateCreateAssetRequest(
							const PMString &			inAssetName,
							const PMString &			inTypeID,
							const PMString &			inEditionID,
							const PMString &			inLinkedWithAssetID = kNullString ) const = 0;

	virtual AZPSoapRequest *	CreateUpdateAssetRequest(
							const PMString &			inAssetID,
							const PMString &			inAssetName,
							const PMString &			inEditionID,
							const PMString &			inVersionID,
							const PMString &			inVersionStatusID) const = 0;

	virtual AZPSoapRequest *	CreateUpdateAssetNameRequest(
							const PMString &			inAssetID,
							const PMString &			inAssetName) const = 0;

	virtual AZPSoapRequest *	CreateUpdateAssetStatusRequest(
							const PMString &			inAssetID,
							const PMString &			inOldStatusID,
							const PMString &			inNewStatusID) const = 0;

	virtual AZPSoapRequest *	CreateAcquireLockRequest(
							const PMString &			inAssetID,
							const PMString &			inLinkedWithDocID,
							const PMString &			inLinkedWithDocName ) const = 0;

	virtual AZPSoapRequest *	CreateReleaseLockRequest(
							const PMString &			inAssetID ) const = 0;

	virtual AZPSoapRequest *	CreateUploadVersionRequest(
							const PMString &			inAssetID,
							const PMString &			inNewVersionID,
							const IDFile &				inAssetFile ) const = 0;

	virtual AZPSoapRequest *	CreateUploadExportRequest(
							const PMString &			inAssetID,
							const PMString &			inNewVersionID,
							const IDFile &				inAssetFile,
							const bool					inIsXML,
							const bool					inIsHighRes,
							const PMString &			inImageFileName ) const = 0;

	virtual AZPSoapRequest *	CreateUploadPDFRequest(
							const PMString &			inAssetID,
							const PMString &			inNewVersionID,
							const IDFile &				inPDFFile ) const = 0;

	virtual AZPSoapRequest *	CreateUploadPagePreviewRequest(
							const PMString &			inAssetID,
							const PMString &			inNewVersionID,
							const PMString &			inPageID,
							int32						inPageNumber,
							const PMString &			inPageType,
							uchar *						inDataBuffer,		//Request will own the buffer
							int							inDataLen ) const = 0;

	virtual AZPSoapRequest *	CreateCheckInAssetRequest(
							const PMString &			inAssetID,
							const PMString &			inNewVersionID,
							const PMString &			inVersionStatusID,
							const PMString &			inCheckInComments,
							int							inHeadVersion) const = 0;

#pragma mark -
//Create requests - WS state not mofiying
	virtual AZPSoapRequest *	CreateFetchAssetInfoRequest(
							const PMString &			inAssetID ) const = 0;
	virtual AZPSoapRequest *	CreateFetchAssetsInfoRequest(
							const ZPAssetIDsList &			inAssetID ) const = 0;
	virtual AZPSoapRequest *	CreateFetchLockInfoRequest(
							const ZPAssetIDsList &		inAssetIDs ) const = 0;
	
	virtual AZPSoapRequest *	CreateDownloadVersionRequest(
							const PMString &			inVersionID,
							const IDFile &				inFileToSaveIn ) const = 0;
	virtual AZPSoapRequest *	CreateDownloadThumbnailRequest(
							const PMString &			inAssetID,
							const IDFile &				inFileToSaveIn ) const = 0;

};


#endif //_h_IZPAMWSUtils_
