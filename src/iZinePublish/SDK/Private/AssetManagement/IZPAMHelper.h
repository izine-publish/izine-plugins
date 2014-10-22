//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Private/AssetManagement/IZPAMHelper.h $
//	$Revision: 3497 $
//	$Date: 2011-07-18 10:10:31 +0200 (Mon, 18 Jul 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 1-6-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPAMHelper_
#define _h_IZPAMHelper_
#pragma once

#include "IAMLockable.h"
#include "IAMServiceProvider.h"
#include "IAMUIService.h"
#include "IAMUIServiceProvider.h"
#include "IManagedStatus.h"
class IAssignment;
class IDocument;
class IStringData;
class IZPAssetData;
class IZPAssetCheckInData;

class IZPAssetLockInfo;

class IZPAMHelper : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPAMHELPER };

	//Save and check-in ID/IC doc to server.
	virtual ErrorCode	ProcessAction_DocSave(
							IDocument *					inDoc ) const = 0;
	
	virtual void		ProcessAction_DocSaveAs(
							IDocument *					inDoc ) const = 0;

	virtual ErrorCode	ProcessDocSaveAs_FromLocalDocument(
							IDocument *					inDoc ) const = 0;

	virtual IDFile		CreateNewStoryAsset(
							IDocument *					inTargetDoc,
							PMString *					ioAssetID = nil ) const = 0;
	virtual ErrorCode	CreateNewAsset(
							PMString &					oAssetID,
							const PMString &			inAssetName,
							const PMString &			inTypeID,
							const PMString &			inEditionID,
							const PMString &			inLinkedWithAssetID = kNullString ) const = 0;

	//Iterate on all assignments in this doc and update them on server.
	virtual ErrorCode	UpdateAssignmentsOnServer(
							UIDRef						inDocAssetRef ) const = 0;
	virtual void		UpdateAssetInfoOnServer(
							const IZPAssetData *		inAssetData,
							const IZPAssetCheckInData *	inAssetCheckInData,
							const IStringData *			inDBAssetID ) const = 0;

	//Create preview and upload them on server.
	virtual ErrorCode	UpdatePagesOnServer(
							IDocument *					inDoc,
							const PMString &			inAssetID,
							const PMString &			inVersionID ) const = 0;

	//Create pdf of all pages and upload it on server.
	//Warning: it may schedule pdf uploading, as pdf creation might take time.
	virtual ErrorCode	UpdatePDFOnServer(
							IDocument *					inDoc,
							const PMString &			inAssetID,
							const PMString &			inVersionID,
							const PMString &			inPDFStyleName ) const = 0;

	//Update assignment file on server.
	virtual ErrorCode	UpdateAssignment(
							IAssignment *				inAssignment ) const = 0;

	virtual bool		StoryNeedsStatusUpdateOnServer(	//If asset status is not equal to new status then return true.
							const IStringData *			inDBAssetID,
							const PMString &			inNewStatusID ) const = 0;
	virtual bool		MakeSureStoryStatus(	//If asset status is not equal to new status then update it on server.
							const IStringData *			inDBAssetID,
							const PMString &			inNewStatusID ) const = 0;

	//HI level function, called from UI.
	virtual void		ProcessAssetForOpening(			//Checks the asset type and opens it.
							const IStringData *			inAssetID,
							bool						inTemplateAsTemplate ) const = 0;

	virtual void		ProcessDocumentAssetForOpening(
							const IStringData *			inAssetID,
							bool						inTemplateAsTemplate ) const = 0;

	virtual void		ProcessAssignmentAssetForOpening(
							const IStringData *			inAssetID ) const = 0;

	//Called after opening a document asset, without window.
	virtual ErrorCode	PostProcessDocumentAssetOpen(
							const IStringData *			inAssetID,
							const UIDRef &				inDocRef,
							const IDFile &				inDocFileOpened ) const = 0;
	virtual ErrorCode	PostProcessTemplateAssetCopyOpen(
							const IStringData *			inAssetID,
							const UIDRef &				inDocRef ) const = 0;
	virtual void		PostProcessAssignmentAssetOpen(	//Only used in InCopy
							const IStringData *			inAssetID,
							const UIDRef &				inDocRef,
							const IDFile &				inAssignmentFile ) const = 0;

	virtual void		HandleDocSignal_BeforeOpen(
							const IDFile &				inFile ) const = 0;
	virtual void		HandleDocSignal_DuringOpen(
							const UIDRef &				inDocRef,
							const IDFile &				inFile ) const = 0;
	virtual void		HandleDocSignal_AfterOpen(
							const UIDRef &				inDocRef,
							const IDFile &				inFile ) const = 0;
	virtual void		HandleDocSignal_AfterRevert(
							const UIDRef &				inDocRef,
							const IDFile &				inFile ) const = 0;
	virtual void		HandleDocSignal_BeforeClose(
							const UIDRef &				inDocRef,
							const IDFile &				inFile ) const = 0;

	//Download assignments and updates it's info with document model.
	virtual ErrorCode	DownloadAssociatedAssignmentsAndUpdate(
							const IDocument *			inDocument ) const = 0;

	virtual ErrorCode	CheckForHeadAndDownloadIfNeeded(
							const IStringData *			inAssetID,
							const IDFile &				inFile,
							bool						inFetchAssetInfoFromWS ) const = 0;
	virtual ErrorCode	DoDownloadAsset(
							const IStringData *			inAssetID,
							const IDFile &				inFile ) const = 0;
	virtual void		DoAfterAssetDownloadComplete(
							const IStringData *			inAssetID ) const = 0;

	virtual bool		IsAssetLocked(
							const IStringData *			inAssetID ) const = 0;
	virtual bool		IsAssetLockedByCurrentUser(
							const IStringData *			inAssetID,
							PMString *					oLockedByUser = nil,
							bool						inFetchLockStatusFromWS = false ) const = 0;
	virtual bool		IsAssetLockedByCurrentUser(		//AssetID must in cache
							const PMString & 			inAssetID,
							PMString *					oLockedByUser = nil,
							bool						inFetchLockStatusFromWS = false) const = 0;
	virtual bool		CanChangeStatusForAsset(
							const IStringData *			inDBAssetID ) const = 0;

	virtual IAMLockable::LockState CalculateAssetLockState(
							const IZPAssetLockInfo *	inLockInfo ) const = 0;

	virtual bool		IsSubPathOfOurReplicaFolder(	//True if path is of current app replica folder
							const IDFile &				inFilePath ) const = 0;
	virtual bool		IsPathOfAnyReplicaFolder(		//True if path is from any replica folder
							const IDFile &				inFilePath ) const = 0;

	virtual bool		GetAssetFilePath(				//Creates file path from asset id and type extention
							const IStringData *			inDBAssetID,
							IDFile &					oFilePath,
							bool						inCreateDirectory = false) const = 0;
	virtual bool		GetPreviewFilePath(				//Returns preview path for document/template type only.
							const IStringData *			inDBAssetID,
							IDFile &					oFilePath ) const = 0;

	virtual IManagedStatus::VersionState	GetAssetVersionState(
							const IStringData *			inDBAssetID ) const = 0;
	virtual IManagedStatus::VersionState	GetAssetFileVersionState(		//Checks the local file version and server version.
							const IStringData *			inDBAssetID,
							const IDFile &				inFilePath ) const = 0;
	virtual IManagedStatus::EditingState	GetEditingState(
							const IAMLockable::LockState		inLockState,
							const IManagedStatus::VersionState	inVersionState ) const = 0;
#pragma mark -
//AM Service callers.
	virtual IAMUIService::FollowupAction	AskVersionComments(		//IWorkgroupUIFacade::DoGetVersionComments
							const UIDRef &				inAssetRef,	//Should be document asset
							PMString &					oCheckinComment ) const = 0;
	virtual IAMUIService::FollowupAction	AskVersionComments_Assignment(
							const IDFile &				inFilePath,	//Must be assignment asset
							const UIDRef &				inAssetRef,	//Must be assignment UIDRef
							PMString &					oCheckinComment ) const = 0;

	virtual IAMService * QueryAMService(
							IManageableAsset *			inAsset,
							ClassID						inServiceProvider,
							IAMServiceProvider::AssetService	inServiceID ) const = 0;

	virtual IAMUIService * QueryAMUIService(
							IManageableAsset *			inAsset,
							IAMUIServiceProvider::UIService	inServiceID ) const = 0;

	virtual ErrorCode	CallAMServiceDo(
							IManageableAsset *			inAsset,
							ClassID						inServiceProvider,
							IAMServiceProvider::AssetService	inServiceID) const = 0;
	virtual bool16		CallAMServiceVerifyContext(
							IManageableAsset *			inAsset,
							ClassID						inServiceProvider,
							IAMServiceProvider::AssetService	inServiceID) const = 0;

	virtual void		UpdateAssignmentFilePathToCurrentCacheFolder(
							const IDocument *			inDocument) const = 0;
};


#endif //_h_IZPAMHelper_
