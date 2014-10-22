//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/Utils/CZPAMHelper.cpp $
//	$Revision: 4097 $
//	$Date: 2013-08-14 11:17:30 +0200 (Wed, 14 Aug 2013) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 1-6-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description: Main business logic is in this file.
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IActiveContext.h"
#include "IAMService.h"
#include "IAMServiceData.h"
#include "IAMSPManager.h"
#include "IAMUIService.h"
#include "IAMUIServiceProvider.h"
#include "IApplication.h"
#include "IAssignedDocument.h"
#include "IAssignmentMgr.h"
#include "IClassIDData.h"
#include "IDocument.h"
#include "IManageableAsset.h"
#include "IInCopyDocUtils.h"
#include "ISectionList.h"
#include "IShape.h"
#include "IStandAloneDoc.h"
#include "IStringData.h"
#include "ISubject.h"
#include "IWorkgroupFacade.h"
#include "IWorkgroupStoryFacade.h"
#include "IWorkgroupUIFacade.h"
#include "IWorkspace.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#include "FileTypeRegistry.h"
#endif

//SDK General includes
#include "AcquireModalCursor.h"
#include "ErrorUtils.h"
#include "StreamUtil.h"
#include "StringUtils.h"
#include "UIDList.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPAlertUtils.h"
#include "IZPAMHelper.h"
#include "IZPAMWSUtils.h"
#include "IZPApp.h"
#include "IZPAssetCheckInData.h"
#include "IZPAssetData.h"
#include "IZPAssetDocUtils.h"
#include "IZPAssetInfo.h"
#include "IZPAssetLocalInfo.h"
#include "IZPAssetLockInfo.h"
#include "IZPAssetMgr.h"
#include "IZPAssetsLocalInfoMgr.h"
#include "IZPAssetUtils.h"
#include "IZPCommandFacade.h"
#include "IZPDocUtils.h"
#include "IZPFileUtils.h"
#include "IZPInDnUtils.h"
#include "IZPPrefs.h"
#include "IZPStoryUtils.h"

//IZP General includes
#include "MemoryXferBytes.h"
#include "StInDesignUtils.h"
#include "ZPConstants.h"
#include "ZPDataHelper.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AM
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZPAMHelper>
class CZPAMHelper : public inheritClass
{
public:
						CZPAMHelper(IPMUnknown * boss);
	virtual				~CZPAMHelper();

	ErrorCode			ProcessAction_DocSave(
							IDocument *					inDoc ) const;
	void				ProcessAction_DocSaveAs(
							IDocument *					inDoc ) const;

	ErrorCode			ProcessDocSave_NewDocument(		//No check-in made for this document yet.
							IDocument *					inDoc ) const;
	ErrorCode			ProcessDocSaveAs_FromLocalDocument(		//No check-in made for this document yet.
							IDocument *					inDoc ) const;
	ErrorCode			ProcessDocSave_NewVersion(		//Old document in repository.
							IDocument *					inDoc ) const;

	IDFile				CreateNewStoryAsset(
							IDocument *					inTargetDoc,
							PMString *					ioAssetID ) const;
	ErrorCode			CreateNewAsset(
							PMString &					oAssetID,
							const PMString &			inAssetName,
							const PMString &			inTypeID,
							const PMString &			inEditionID,
							const PMString &			inLinkedWithAssetID = kNullString ) const;

	ErrorCode			UpdateAssignmentsOnServer(
							UIDRef						inDocAssetRef ) const;
	void				UpdateAssetInfoOnServer(
							const IZPAssetData *		inAssetData,
							const IZPAssetCheckInData *	inAssetCheckInData,
							const IStringData *			inDBAssetID ) const;

	ErrorCode			UpdatePagesOnServer(
							IDocument *					inDoc,
							const PMString &			inAssetID,
							const PMString &			inVersionID ) const;

	ErrorCode			UpdatePDFOnServer(
							IDocument *					inDoc,
							const PMString &			inAssetID,
							const PMString &			inVersionID,
							const PMString &			inPDFStyleName ) const;

	ErrorCode			UpdateAssignment(
							IAssignment *				inAssignment ) const;
	
	bool				StoryNeedsStatusUpdateOnServer(	//If asset status is not equal to new status then return true.
							const IStringData *			inDBAssetID,
							const PMString &			inNewStatusID ) const;
	bool				MakeSureStoryStatus(	//If asset status is not equal to new status then update it on server.
							const IStringData *			inDBAssetID,
							const PMString &			inNewStatusID ) const;
#pragma mark -
	void				ProcessAssetForOpening(
							const IStringData *			inAssetID,
							bool						inTemplateAsTemplate ) const;

	void				ProcessDocumentAssetForOpening(
							const IStringData *			inAssetID,
							bool						inTemplateAsTemplate ) const;

	void				ProcessAssignmentAssetForOpening(
							const IStringData *			inAssetID ) const;

	ErrorCode			PostProcessDocumentAssetOpen(
							const IStringData *			inAssetID,
							const UIDRef &				inDocRef,
							const IDFile &				inDocFileOpened ) const;

	ErrorCode			PostProcessTemplateAssetCopyOpen(
							const IStringData *			inAssetID,
							const UIDRef &				inDocRef ) const;

	void				PostProcessAssignmentAssetOpen(	//Only used in InCopy
							const IStringData *			inAssetID,
							const UIDRef &				inDocRef,
							const IDFile &				inAssignmentFile ) const;

	void				HandleDocSignal_BeforeOpen(
							const IDFile &				inFile ) const;
	void				HandleDocSignal_DuringOpen(
							const UIDRef &				inDocRef,
							const IDFile &				inFile ) const;
	void				HandleDocSignal_AfterOpen(
							const UIDRef &				inDocRef,
							const IDFile &				inFile ) const;
	void				HandleDocSignal_AfterRevert(
							const UIDRef &				inDocRef,
							const IDFile &				inFile ) const;
	void				HandleDocSignal_BeforeClose(
							const UIDRef &				inDocRef,
							const IDFile &				inFile ) const;

	ErrorCode			DownloadAssociatedAssignmentsAndUpdate(
							const IDocument *			inDocument ) const;

	ErrorCode			CheckForHeadAndDownloadIfNeeded(
							const IStringData *			inAssetID,
							const IDFile &				inFile,
							bool						inFetchAssetInfoFromWS ) const;
	ErrorCode			DoDownloadAsset(
							const IStringData *			inAssetID,
							const IDFile &				inFile ) const;
	void				DoAfterAssetDownloadComplete(
							const IStringData *			inAssetID ) const;

	bool				IsSubPathOfOurReplicaFolder(
							const IDFile &				inFilePath ) const;
	bool				IsPathOfAnyReplicaFolder(
							const IDFile &				inFilePath ) const;

	bool				GetAssetFilePath(				//Creates file path from asset id and type extention
							const IStringData *			inDBAssetID,
							IDFile &					oFilePath,
							bool						inCreateDirectory = false) const;
	bool				GetPreviewFilePath(
							const IStringData *			inDBAssetID,
							IDFile &					oFilePath ) const;

	IManagedStatus::VersionState	GetAssetVersionState(
							const IStringData *			inDBAssetID ) const;
	IManagedStatus::VersionState	GetAssetFileVersionState(
							const IStringData *			inDBAssetID,
							const IDFile &				inFilePath ) const;
	IManagedStatus::EditingState	GetEditingState(
							const IAMLockable::LockState		inLockState,
							const IManagedStatus::VersionState	inVersionState ) const;

	bool				IsAssetLocked(
							const IStringData *			inAssetID ) const;
	bool				IsAssetLockedByCurrentUser(
							const IStringData *			inAssetID,
							PMString *					oLockedByUser,
							bool						inFetchLockStatusFromWS ) const;
	bool				IsAssetLockedByCurrentUser(		//AssetID must in cache
							const PMString & 			inAssetID,
							PMString *					oLockedByUser,
							bool						inFetchLockStatusFromWS ) const;
	IAMLockable::LockState CalculateAssetLockState(
							const IZPAssetLockInfo *	inLockInfo ) const;

	bool				CanChangeStatusForAsset(
							const IStringData *			inDBAssetID ) const;
#pragma mark -
	IAMUIService::FollowupAction	AskVersionComments(
							const UIDRef &				inAssetRef,	//Should be document asset
							PMString &					oCheckinComment ) const;
	IAMUIService::FollowupAction	AskVersionComments_Assignment(
							const IDFile &				inFilePath,	//Must be assignment asset
							const UIDRef &				inAssetRef,	//Must be assignment UIDRef
							PMString &					oCheckinComment ) const;

	IAMService *		QueryAMService(
							IManageableAsset *			inAsset,
							ClassID						inServiceProvider,
							IAMServiceProvider::AssetService	inServiceID ) const;

	IAMUIService *		QueryAMUIService(
							IManageableAsset *			inAsset,
							IAMUIServiceProvider::UIService	inServiceID ) const;

	ErrorCode			CallAMServiceDo(
							IManageableAsset *			inAsset,
							ClassID						inServiceProvider,
							IAMServiceProvider::AssetService	inServiceID) const;
	bool16				CallAMServiceVerifyContext(
							IManageableAsset *			inAsset,
							ClassID						inServiceProvider,
							IAMServiceProvider::AssetService	inServiceID) const;
	
	void				InitAssetManagementForStory(	//Finds data link for story and starts AM
							const UIDRef &				inStoryRef,
							const IDFile &				inFilePath ) const;

	void				UpdateAssignmentFilePathToCurrentCacheFolder(
							const IDocument *			inDocument) const;
protected:
private:
};


CREATE_PMINTERFACE(CZPAMHelper, kZPAMHelperImpl)

CZPAMHelper::CZPAMHelper(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMHelper::~CZPAMHelper()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// ProcessAction_DocSave
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMHelper::ProcessAction_DocSave(
	IDocument *					inDoc)const
{
	LogFunctionEnterExit;
	ErrorCode errorCode = kSuccess;
	do
	{
		//InterfacePtr<IManageableAsset> docManageableAsset(inDoc, UseDefaultIID());
		//ASSERT(docManageableAsset);
		
		// disable preview and thumbnail creation
		errorCode = Utils<IZPDocUtils>()->DisablePreviewGeneration( inDoc );
		
		bool isNewDocument = inDoc->IsSaved() == kFalse;
		{	//Don't remove brackets.
			InterfacePtr<const IBoolData> isTemplateCopy( inDoc, IID_IZPISTEMPLATECOPY );
			ASSERT(isTemplateCopy);
			if( !isNewDocument )
				isNewDocument = isTemplateCopy->Get();
		}

		// check if new document
		if ( !isNewDocument )
		{
			errorCode = this->ProcessDocSave_NewVersion( inDoc );
		} else
		{
			errorCode = this->ProcessDocSave_NewDocument( inDoc );

			//TODO: UI update
			//this->UpdateAssetsTreeView();
		}
	} while (kFalse);
	return errorCode;
}

//----------------------------------------------------------------------------------------
// ProcessAction_DocSaveAs
//----------------------------------------------------------------------------------------
void
CZPAMHelper::ProcessAction_DocSaveAs(
	IDocument *					inDoc) const
{
	LogFunctionEnterExit;
	do
	{
#if 0 //TODO: RJK - Old Trunk code
		if (Utils<IIZPUtils>()->IsInCopy()) {
			Utils<IDocumentUtils>()->DoSaveAs(document);
		} else if ( Utils<IIZPUtils>()->IsInDesign())
		{
			// disable preview and thumbnail creation
			Utils<IZPDocUtils>()->DisablePreviewGeneration( inDoc );
		}
		
		InterfacePtr<IAssetData> data(document, UseDefaultIID());
		ASSERT(data);
		
		// show saveas dialog if new
		InterfacePtr<IManageableAsset> asset(document, UseDefaultIID());
		ASSERT(asset);
		
		UIDRef assetRef = ::GetUIDRef( asset );
		
		bool16 stationary;
		
		// or should we call the dialog directly?
		IDFile file;
		if (Utils<IWorkgroupUIFacade>()->DoSaveAsDialog(assetRef, file, kFalse, file, stationary)==IAMUIService::enCancel)
			break;

		
		AcquireWaitCursor busyCursor;
		
		TaskProgressBar bar(kIZPProgressBarSavingDocumentKey, 1, kTrue, kFalse, nil, kFalse);

		// save document as
		InterfacePtr<ICommand> cmd(CmdUtils::CreateCommand(kSaveAsDocCmdBoss));
		ASSERT(cmd);
		
		IDataBase* db = ::GetDataBase(document);
		
		UIDList list(db, ::GetUID(document));
		cmd->SetItemList(list);
		
		InterfacePtr<IUIFlagData> uiFlagData(cmd, UseDefaultIID());
		ASSERT(uiFlagData);
		uiFlagData->Set();
		
		InterfacePtr<ISysFileData> sysFileData(cmd, UseDefaultIID());
		ASSERT(sysFileData);
		sysFileData->Set(file);
		
		// stationary
		InterfacePtr<IBoolData> boolData(cmd, UseDefaultIID());
		ASSERT(boolData);
		boolData->Set(stationary);
		
		CmdUtils::ProcessCommand(cmd);
		
		// set doc name cmd
		Utils<IIZPUtils>()->SetDocumentName(::GetUIDRef(document), data->GetName());
		
		ErrorCode result = kFailure;
		
		// remove story links
		InterfacePtr<IStoryList> storyList (document, UseDefaultIID());

		UIDList links(db);
		
		int storyCount = storyList->GetUserAccessibleStoryCount();
		for(int i = 0; i < storyCount; i++)
		{
			UIDRef storyRef = storyList->GetNthUserAccessibleStoryUID(i);
			
			do {
				InterfacePtr<IDataLinkReference> iDataReference(storyRef, UseDefaultIID());
				if(!iDataReference)
					break;
				
				UID dataLinkUID = iDataReference->GetUID();
				if( dataLinkUID == 0 )
					break;
				InterfacePtr<IDataLink> iDataLink(db, dataLinkUID, UseDefaultIID());
				if(!iDataLink)
					break;
				
				links.push_back(iDataReference->GetUID());
			} while (kFalse);
		}
		
		InterfacePtr<ICommand> removeDataLink (CmdUtils::CreateCommand(kRemoveLinksCmdBoss));
		
		if(removeDataLink)
		{
			removeDataLink->SetItemList(links);
			removeDataLink->SetUndoability(ICommand::kAutoUndo);
			result = CmdUtils::ProcessCommand(removeDataLink);
		}
		
		// remove assignments
		InterfacePtr<IAssignedDocument> iad(document, UseDefaultIID());
		ASSERT(iad);
		
		InterfacePtr<IAssignmentMgr> assMgr(gSession, UseDefaultIID());
		ASSERT(assMgr);
		
		UIDList assignmentList = iad->GetAssignments();
		for (int32 i = 0 ; i < assignmentList.Length(); i++) {
			UIDRef assignmentref(db, assignmentList[i]);
			
			InterfacePtr<IAssignment> assignment(assignmentref, UseDefaultIID());
			ASSERT(assignment);
			
			
			// remove assignment from document
			InterfacePtr<ICommand> unassignDocCmd(CmdUtils::CreateCommand(kUnassignDocCmdBoss));
			UIDList uidl(::GetUIDRef(iad));
			unassignDocCmd->SetItemList(uidl);
			InterfacePtr<IStringData> sd(unassignDocCmd, UseDefaultIID());
			sd->Set(assignment->GetFile());
			result = CmdUtils::ProcessCommand(unassignDocCmd);
			
		}
		
		// checkin
		Utils<Facade::IWorkgroupFacade>()->InitDocumentAssetManagement(assetRef, file);
		
		Utils<Facade::IWorkgroupFacade>()->CheckIn(assetRef, kFalse);

		this->UpdateAssetsTreeView();
#endif
	}while( kFalse );
}

//----------------------------------------------------------------------------------------
// ProcessDocSave_NewDocument
// No check-in made for this document yet.
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMHelper::ProcessDocSave_NewDocument(
	IDocument *					inDoc) const
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	ErrorCode errorCode = kFailure;
	PMString assetToOpen;
	do
	{
		UIDRef docRef = ::GetUIDRef( inDoc );
		// show saveas dialog if new
		bool16 saveAsStationary = kFalse;
		
		// or should we call the dialog directly?
		IDFile saveToFile;
		IAMUIService::FollowupAction followupAction = Utils<IWorkgroupUIFacade>()->DoSaveAsDialog(docRef, saveToFile, kFalse, saveToFile, saveAsStationary);
		if(followupAction == IAMUIService::enCancel)
			break;

		//TODO: move this code to UI, to show progress.
		//////TaskProgressBar bar(kIZPProgressBarSavingDocumentKey, 1, kTrue, kFalse, nil, kFalse);

		//TODO: handle it for InDn server
		AcquireWaitCursor busyCursor;

		// set doc name
		InterfacePtr<const IZPAssetCheckInData> assetCheckInData( inDoc, UseDefaultIID());
		ASSERT( assetCheckInData );
		
		errorCode = Utils<IZPDocUtils>()->SetDocumentName( inDoc, assetCheckInData->GetAssetName());
		//TODO: check error status

		// save document as
		InterfacePtr<IBoolData> isTemplateCopy( inDoc, IID_IZPISTEMPLATECOPY );
		ASSERT(isTemplateCopy);
		if( isTemplateCopy->Get() )
			isTemplateCopy->Set(kFalse);

		InterfacePtr<IStandAloneDoc> standAlone( inDoc, IID_ISTANDALONEDOC);
		if( standAlone && !standAlone->IsStandAloneDoc() )
			standAlone->SetIsStandAloneDoc( kTrue );
		errorCode = Utils<IZPDocUtils>()->SaveDocToFile( inDoc, saveToFile, saveAsStationary, true);
		
		//TODO: check error status
#if 0
		// checkin also for template? Or just uploadversion?
		Utils<Facade::IWorkgroupFacade>()->InitDocumentAssetManagement( docRef, saveToFile );
		Utils<Facade::IWorkgroupFacade>()->CheckIn( docRef, kFalse );
		if( standAlone && !standAlone->IsStandAloneDoc() )
			standAlone->SetIsStandAloneDoc( kTrue );
#else
		Utils<Facade::IWorkgroupFacade>()->CheckIn( saveToFile, nil, Facade::IWorkgroupFacade::enForceSaveYes );
#endif

		//Patch: Try revert to allow style editing.
		//Utils<Facade::IWorkgroupFacade>()->Revert( docRef );	//Commented as story is coming out of date

		// Patch: IC template crash in CS5.5. If IC template, do not init linked story asset
#if defined(InDnCS5) || defined(InDnCS5_5)
		if( !(Utils<IZPInDnUtils>()->IsHostAppInCopy() && 
			FileTypeRegistry::GetFileTypeInfoIDForFile(saveToFile) == kInCopyTemplateFileTypeInfoID) )
		{
			//Init asset management for data link, Linked Story AM will now handle it as it's version is now > 0
			UIDRef storyUIDRef = Utils<IInCopyDocUtils>()->GetFirstUserStory ( inDoc );
			this->InitAssetManagementForStory( storyUIDRef, saveToFile );
		}
#else
		//Init asset management for data link, Linked Story AM will now handle it as it's version is now > 0
		UIDRef storyUIDRef = Utils<IInCopyDocUtils>()->GetFirstUserStory ( inDoc );
		this->InitAssetManagementForStory( storyUIDRef, saveToFile );
#endif
				

		//Patch: Close the doc and open again to allow style editing.
		InterfacePtr<const IZPAssetData> assetData( inDoc, UseDefaultIID());
		ASSERT( assetData );
		assetToOpen = assetData->GetAssetID();
	
		InterfacePtr<IActiveContext> ac(gSession->GetActiveContext(), UseDefaultIID());
		ASSERT(ac);
		InterfacePtr<ISubject> acSubject(ac, UseDefaultIID());
		IActiveContext::ContextInfo changedBy(IID_IDOCUMENT, inDoc );
		acSubject->Change( kDocumentClosedMsg, IActiveContext::kDefaultIID, &changedBy);

		//Close the doc
		//Utils<IInCopyDocUtils>()->DoClose( docRef, kSuppressUI, kFalse, IDocFileHandler::kProcess );
	}while( kFalse );

	//Patch: Open it again
	if( !assetToOpen.IsEmpty() )
	{
		const IStringData * dbAssetID = ZPDataHelper::GetAssetByID( assetToOpen );
		//this->ProcessAssetForOpening( dbAssetID, true );
	}
	
	return errorCode;
}

//----------------------------------------------------------------------------------------
// ProcessDocSaveAs_FromLocalDocument
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMHelper::ProcessDocSaveAs_FromLocalDocument(	//No check-in made for this document yet.
	IDocument *					inDoc) const
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	ErrorCode errorCode = kFailure;
	PMString assetToOpen;
	do
	{
		UIDRef docRef = ::GetUIDRef( inDoc );
		InterfacePtr<IManageableAsset> pAsset(inDoc , UseDefaultIID() );
		InterfacePtr<IAMSPManager> iAMSPManager( gSession, UseDefaultIID() );
		ASSERT( iAMSPManager );
		IAMServiceProvider * amServiceProvider = iAMSPManager->GetServiceProvider( kZPAMSPDocumentBoss );
		ASSERT( amServiceProvider );

		InterfacePtr<IAMUIServiceProvider> amUIServiceProvider( amServiceProvider, UseDefaultIID() );
		InterfacePtr<IAMUIService> amUIService( amUIServiceProvider->GetService(IAMUIServiceProvider::enSaveAsDialog));

		errorCode = amUIService->Do(pAsset);
		if (errorCode != kSuccess)
			break;

		InterfacePtr<IAMServiceData> amServiceData(amUIService, UseDefaultIID());
		ASSERT(amServiceData);
		IAMUIService::FollowupAction userAction = IAMUIService::enCancel;
		amServiceData->Get(IAMUIService::enFollowupAction, userAction);
		if( userAction == IAMUIService::enSave )
		{
			bool16 saveAsStationary = amServiceData->GetFlag(IAMUIService::enSaveAsStationery);
			IDFile selectedFile;
			amServiceData->Get( IAMUIService::enSelectedFile, selectedFile );
			errorCode = Utils<IZPDocUtils>()->SaveDocToFile( inDoc, selectedFile, saveAsStationary, true);
			if (errorCode != kSuccess)
				break;

			Utils<Facade::IWorkgroupFacade>()->InitDocumentAssetManagement( docRef, selectedFile );
			Utils<Facade::IWorkgroupFacade>()->CheckIn( selectedFile, nil, Facade::IWorkgroupFacade::enForceSaveYes );
		}
	}while( kFalse );

	return errorCode;
}

//----------------------------------------------------------------------------------------
// ProcessDocSave_NewVersion
// Old document in repository.
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMHelper::ProcessDocSave_NewVersion(
	IDocument *					inDoc) const
{
	LogFunctionEnterExit;
	ErrorCode toReturn = kFailure;
	do
	{	// save a new version of existing document

		UIDRef assetRef = ::GetUIDRef( inDoc );

		bool16 canCheckInTheDoc = Utils<Facade::IWorkgroupFacade>()->CanCheckIn( assetRef );

		PMString checkInComments;
		IAMUIService::FollowupAction followupAction = Utils<IWorkgroupUIFacade>()->DoGetVersionComments( assetRef, checkInComments );
		//IAMUIService::FollowupAction followupAction = this->AskVersionComments( assetRef, checkInComments );
		if(followupAction == IAMUIService::enCancel || followupAction == IAMUIService::enNone )
			break;

		//TODO: move it to UI plugin
		//AcquireWaitCursor busyCursor;

		//Upload a new version.
		//TODO:

		//TODO: move it to UI plugin
		//TaskProgressBar bar(kIZPProgressBarSavingDocumentKey, 1, kTrue, kFalse, nil, kFalse);
		
		//Update Asset info i.e Name, Description & Status. Title/Edition/Type cannot be changed.
		//TODO:
		//////if (!IZPServer::UpdateAsset(izpasset)) {
		//////	CAlert::InformationAlert("Error occured while updating asset data");
		//////	break;
		//////}

		InterfacePtr<const IZPAssetCheckInData> assetCheckInData( inDoc, UseDefaultIID());
		ASSERT( assetCheckInData );
		
		toReturn = Utils<IZPDocUtils>()->SetDocumentName( inDoc, assetCheckInData->GetAssetName());
		
		// checkin
		Utils<Facade::IWorkgroupFacade>()->CheckIn(assetRef, kTrue);

	} while (false);
	return toReturn;
}


//----------------------------------------------------------------------------------------
// CreateNewStoryAsset
//----------------------------------------------------------------------------------------
IDFile
CZPAMHelper::CreateNewStoryAsset(
	IDocument *					inTargetDoc,
	PMString *					ioAssetID) const
{
	LogFunctionEnterExit;
	IDFile toReturn;
	do
	{
		InterfacePtr<const IZPAssetData> targetDocAssetData( inTargetDoc, UseDefaultIID() );
		ASSERT( targetDocAssetData );

		//TODO: find some solution for name.
		const PMString newAssetName = kNullString;	//Change the name afterwards, as we don't know the name at this point.
		const PMString & docEditionID = targetDocAssetData->GetEditionID();
		const PMString & assetType = Utils<IZPAssetUtils>()->GetAssetTypeID( eAssetType_IDStory );
		PMString newAssetID;

		if( docEditionID.IsEmpty() )
			break;

		IZPLog_Str_( thisFileLA, enLT_Details, "Creating new asset of type - ID Story" );
		if (kSuccess != this->CreateNewAsset( newAssetID, newAssetName, assetType, docEditionID))
		{
			//CAlert::ErrorAlert("Error occured creating asset in repository.");
			break;	//Supress further processing of the command
		}

		if( ioAssetID )
			*ioAssetID = newAssetID;

		const PMString & idStoryFileExt = Utils<IZPAssetUtils>()->GetAssetFileExtenstion( eAssetType_IDStory );
		toReturn = Utils<IZPAssetUtils>()->CreateLocalFilePathFromAssetID( newAssetID, idStoryFileExt, newAssetName );

	}while(kFalse);

	return toReturn;
}

//----------------------------------------------------------------------------------------
// CreateNewAsset
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMHelper::CreateNewAsset(
	PMString &					oAssetID,
	const PMString &			inAssetName,
	const PMString &			inTypeID,
	const PMString &			inEditionID,
	const PMString &			inLinkedWithAssetID) const
{
	LogFunctionEnterExit;
	ErrorCode toReturn = Utils<IZPAMWSUtils>()->CreateNewAsset( oAssetID, inAssetName, inTypeID, inEditionID, inLinkedWithAssetID);
#ifdef CacheWithFolder	//Create the folder before file gets saved.
	if( toReturn == kSuccess )
		Utils<IZPAssetUtils>()->CreateAssetCacheFolderPath(oAssetID);
#endif
	return toReturn;
}

//----------------------------------------------------------------------------------------
// StoryNeedsStatusUpdateOnServer
//----------------------------------------------------------------------------------------
//If asset status is not equal to new status then return true.
bool
CZPAMHelper::StoryNeedsStatusUpdateOnServer(
	const IStringData *			inDBAssetID,
	const PMString &			inNewStatusID) const
{
	LogFunctionEnterExit;

	InterfacePtr<const IZPAssetInfo> dbAssetInfo( inDBAssetID, UseDefaultIID() );
	ASSERT( dbAssetInfo );
	if( !dbAssetInfo )
		return false;

	const PMString & oldAssetStatusID = dbAssetInfo->GetStatusID();
	if( oldAssetStatusID.Compare( kFalse, inNewStatusID ) != 0 )
		return true;
	return false;
}

//----------------------------------------------------------------------------------------
// MakeSureStoryStatus
//----------------------------------------------------------------------------------------
//If asset status is not equal to new status then update it on server.
//Returns true if change was requested to server and was successful else return false.
bool
CZPAMHelper::MakeSureStoryStatus(
	const IStringData *			inDBAssetID,
	const PMString &			inNewStatusID) const
{
	LogFunctionEnterExit;

	InterfacePtr<const IZPAssetInfo> dbAssetInfo( inDBAssetID, UseDefaultIID() );
	ASSERT( dbAssetInfo );
	if( !dbAssetInfo )
		return false;

	const PMString & oldAssetStatusID = dbAssetInfo->GetStatusID();
	if( oldAssetStatusID.Compare( kFalse, inNewStatusID ) != 0 )
	{	//call the WS method to update the status of asset.
		ErrorCode wsError = Utils<IZPAMWSUtils>()->UpdateAssetStatus_sync(inDBAssetID->Get(), oldAssetStatusID, inNewStatusID);
		return ( wsError == kSuccess );
	}

	return false;
}

//----------------------------------------------------------------------------------------
// UpdateAssignmentsOnServer
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMHelper::UpdateAssignmentsOnServer(
	UIDRef						inDocAssetRef ) const
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	ErrorCode toReturn = kFailure;
	do
	{
		InterfacePtr<IDocument> theDocument( inDocAssetRef, UseDefaultIID());
		ASSERT( theDocument );
		if ( !theDocument )
		{
			//TODO: move this to UI pln
			Utils<IZPAlertUtils>()->ShowAlert("Document not set.", kNullString, kNullString, CAlert::eInformationIcon );
			break;
		}
		
		// Get a pointer to the document database
		IDataBase* theDataBase = ::GetDataBase( theDocument);
		
		InterfacePtr<IAssignedDocument> assignedDocument( theDocument, UseDefaultIID());
		ASSERT( assignedDocument );
		
		InterfacePtr<IAssignmentMgr> assMgr(gSession, UseDefaultIID());
		ASSERT(assMgr);
		
		UIDList assignmentList = assignedDocument->GetAssignments();
		toReturn = kSuccess;
		for (int32 i = 0 ; i < assignmentList.Length(); i++) {
			UIDRef assignmentRef(theDataBase, assignmentList[i]);
			
			InterfacePtr<IAssignment> currAssignment(assignmentRef, UseDefaultIID());
			ASSERT(currAssignment);
			
			/*
			 // not always correctly detected, temporarily disabled
			 if (currAssignment->GetDirty()!=kTrue)
			 continue;
			 */
			
			if ( currAssignment->GetFile().IsEmpty() )	//TODO: check this on multiple systems
				continue;
			
			IDFile file;
			Utils<IZPFileUtils>()->ConvertStrToIDFile( file, currAssignment->GetFile() );
			
			//TODO: RJK copy from old pln
			//Why to get asset id from file, when we have assignment object.
			//PMString assetid = Utils<IIZPFacade>()->GetAssetIdentifierFromLocalFile(file);
			//if (assetid.IsEmpty())
			//	continue;
			
			//TODO: RJK check what if lock is not acquired for this assignment.
			toReturn = Utils<IZPAMHelper>()->UpdateAssignment( currAssignment );
			if( toReturn != kSuccess )
				break;
		}
	}while( kFalse );
	return toReturn;
}

//----------------------------------------------------------------------------------------
// UpdateAssetInfoOnServer
//----------------------------------------------------------------------------------------
void
CZPAMHelper::UpdateAssetInfoOnServer(
	const IZPAssetData *		inAssetData,
	const IZPAssetCheckInData *	inAssetCheckInData,
	const IStringData *			inDBAssetID) const
{
	LogFunctionEnterExit;
	do
	{
		const PMString & statusID = inAssetCheckInData->GetStatusID();
		if( statusID.IsEmpty() )
		{
			//CAlert::InformationAlert("Data not set.");	//It should be ASSERT
			return;
		}

		const PMString & assetName = inAssetCheckInData->GetAssetName();

		InterfacePtr<const IZPAssetInfo> dbAssetInfo( inDBAssetID, UseDefaultIID() );
		ASSERT( dbAssetInfo && dbAssetInfo->GetVersionNumber() > 0 );
		const PMString & theVersionID = dbAssetInfo->GetVersionID();

		//check if info is changed, then only make a server call.
		const PMString & assetStatusID = dbAssetInfo->GetStatusID();
		if (statusID == assetStatusID)
			return;

		#pragma message("//TODO: pass old version number, so that server can compare it with latest/head.")
		//TODO: pass checkin comments
		ErrorCode toReturn = Utils<IZPAMWSUtils>()->UpdateAsset_sync( inAssetData->GetAssetID(), assetName, inAssetData->GetEditionID(), theVersionID, statusID );
		if( toReturn != kSuccess )
		{
			CAlert::ErrorAlert("Error occured during updating assignment.");		//Setting error code will crash app.
			break;
		}
	}while(kFalse);
}

//----------------------------------------------------------------------------------------
// UpdatePagesOnServer
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMHelper::UpdatePagesOnServer(
	IDocument *					inDoc,
	const PMString &			inAssetID,
	const PMString &			inVersionID ) const
{
	LogFunctionEnterExit;
	ErrorCode toReturn = kFailure;
	do
	{
		InterfacePtr<ISectionList> sectionList( inDoc, UseDefaultIID());
		if (sectionList == nil)
		{
			CAlert::ErrorAlert("ISectionList.");

			ASSERT_FAIL("Could not get ISectionList. Is this really a document?");
			break;
		}
		
		InterfacePtr<IPageList> pageList( inDoc, UseDefaultIID());
		if (pageList == nil)
		{
			CAlert::ErrorAlert("IPageList.");
			
			ASSERT_FAIL("Could not get IPageList. Is this really a document?");
			break;
		}
		
		for (int i=0; i<pageList->GetPageCount(); i++)
		{
			UID pageUid = pageList->GetNthPageUID(i);
			
			do {
				uchar* buffer = NULL;
				PageType pageType = pageList->GetPageType(pageUid);
				//////int32 pageIndex = pageList->GetPageIndex(pageUid);
				int32 pageNumber = sectionList->GetPageNumber(pageUid);
				//////int32 sectionIndex = pageList->GetSectionIndex(pageUid);
				
				
				PMString pageTypeId;
				switch (pageType)
				{
					case kLeftPage:
						pageTypeId = kPageTypeLeft;
						break;
					case kUnisexPage:
						pageTypeId = kPageTypeUnisex;
						break;
					case kRightPage:
						pageTypeId = kPageTypeRight;
						break;
						
				}
		
				MemoryXferBytes currentOutBuffer;	//TODO: optimize the loop to use the same stream buffer again.
				InterfacePtr<IPMStream> currentOutStream(StreamUtil::CreateMemoryStreamWrite(&currentOutBuffer, kFalse));
				
				toReturn = Utils<IZPDocUtils>()->CreatePageSnapshot( inDoc, pageUid, currentOutStream );
				if( toReturn != kSuccess )
				{
					ErrorUtils::PMSetGlobalErrorCode(kZPExportImageToJpegError);
					break;
				}
		
				InterfacePtr<IPMStream> currentInStream( StreamUtil::CreateMemoryStreamRead(&currentOutBuffer, kFalse));
				StStreamCloser			autoCloseStream( currentInStream );
				
				int fileSize = 0;
				if( false == Utils<IZPFileUtils>()->ReadStream( currentInStream, buffer, fileSize ) )
					break;
				
				PMString pageID = Utils<IZPAssetUtils>()->NewGUID();

				toReturn = Utils<IZPAMWSUtils>()->UploadPagePreview_sync( inAssetID, inVersionID, pageID, pageNumber, pageTypeId, buffer, fileSize );
				if( toReturn != kSuccess )
					break;

				//if (buffer!=NULL)	//buffer is deleted by upload request.
				//	delete buffer;

			} while (false);
		}
		toReturn = kSuccess;
	}while( false );

	return toReturn;
}

//----------------------------------------------------------------------------------------
// UpdatePDFOnServer
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMHelper::UpdatePDFOnServer(
	IDocument *					inDoc,
	const PMString &			inAssetID,
	const PMString &			inVersionID,
	const PMString &			inPDFStyleName ) const
{
	LogFunctionEnterExit;
	const PMString pdf("pdf");
	IDFile pdfFile = Utils<IZPAssetUtils>()->CreateAssetCacheFolderPath(inAssetID);
	Utils<IZPFileUtils>()->AppendPath( pdfFile, inVersionID, &pdf );
		
	ErrorCode toReturn = Utils<IZPDocUtils>()->CreatePDF( inDoc, pdfFile, inPDFStyleName );
				
	if( toReturn == kSuccess )
	{
		toReturn = Utils<IZPAMWSUtils>()->UploadPDF_sync( inAssetID, inVersionID, pdfFile );
		//The file creation might not be completed before this uploading starts. So making it async
		//Utils<IZPAMWSUtils>()->UploadPDF_async( inAssetID, inVersionID, pdfFile );
	}
	return toReturn;
}

//----------------------------------------------------------------------------------------
// UpdateAssignment
//----------------------------------------------------------------------------------------
//Not being used.
ErrorCode
CZPAMHelper::UpdateAssignment(
	IAssignment *				inAssignment ) const
{
	LogFunctionEnterExit;
	ErrorCode toReturn = kFailure;
#if 0 //TODO: save assignment to server.
	do
	{
		InterfacePtr<IIZPAssignmentData> data(assignment, UseDefaultIID());
		ASSERT(data);
		
		InterfacePtr<IAssignmentMgr> assMgr(gSession, UseDefaultIID());
		ASSERT(assMgr);
		
		MemoryXferBytes currentOutBuffer;
		InterfacePtr<IPMStream> currentOutStream(StreamUtil::CreateMemoryStreamWrite(&currentOutBuffer, kFalse));
		
		assMgr->ExportAssignment(assignment, currentOutStream, kSuppressUI);
		
		IDFile file;
		FileUtils::PMStringToIDFile(assignment->GetFile(), file);
		PMString assetid = Utils<IIZPFacade>()->GetAssetIdentifierFromLocalFile(file);
		
		InterfacePtr<IIZPSessionData> sessionData(gSession, UseDefaultIID());
		ASSERT(sessionData);
		
		PMString versionid = Utils<IIZPUtils>()->NewId();
		
		InterfacePtr<IPMStream> currentInStream(StreamUtil::CreateMemoryStreamRead(&currentOutBuffer, kFalse));
		ASSERT(currentInStream);
		
		int32 curr = currentInStream->Seek(0, kSeekFromCurrent);
		int32 fileSize = currentInStream->Seek(0, kSeekFromEnd);
		currentInStream->Seek(curr, kSeekFromStart);
		
		uchar* buf = new uchar[fileSize];
		
		currentInStream->XferByte(&buf[0], fileSize);
		currentInStream->Close();
		
		currentOutStream->Close();
		
		if (!IZPServer::UploadVersion(assetid, versionid, &buf[0], fileSize))
			break;

		delete buf;
		
		
		PMString name = assignment->GetName();
		
		IZPAsset asset;
		asset.assetid = assetid;
		asset.name = name;
		
		PMString statusid = data->GetStatusId();
		asset.statusid = statusid;
		asset.titleid = data->GetTitleId();
		asset.shelveid = data->GetShelveId();
		if (!IZPServer::UpdateAsset(asset)) {
			CAlert::ErrorAlert("Error occured during update assignment properties.");
			break;
		}
		
		/*
		// update shelve, title
		PMString description("", -1, PMString::kNoTranslate);
		
		// update status
		if (!IZPServer::CheckInAsset(assetid, statusid, description))
		{
			CAlert::ErrorAlert("Error occured during checkin assignment.");
			break;
		}
		*/
		
		InterfacePtr<ICommand> cmd(CmdUtils::CreateCommand(kAssignSetPropsCmdBoss));
		ASSERT(cmd);
		UIDList list(::GetUIDRef(assignment));
		cmd->SetItemList(list);
		InterfacePtr<IAssignSetPropsCmdData> cmddata(cmd, UseDefaultIID());
		
		bool16 dirty = kFalse;
		cmddata->Set(nil /* name */, nil /* file */, nil /* user */, nil /* color */, nil /* options */, &dirty /* dirty */, nil /* includeLinkedWhenPackage */);
		toReturn = CmdUtils::ProcessCommand(cmd);

	}while(kFalse);
#endif
	return toReturn;
}

//----------------------------------------------------------------------------------------
// ProcessAssetForOpening
//----------------------------------------------------------------------------------------
void
CZPAMHelper::ProcessAssetForOpening(
	const IStringData *			inAssetID,
	bool						inTemplateAsTemplate ) const
{
	LogFunctionEnterExit;

	ASSERT( inAssetID );
	if( !inAssetID )
		return;

	InterfacePtr<const IZPAssetInfo> assetInfo( inAssetID, UseDefaultIID() );
	ASSERT( assetInfo );

	const enAssetType assetType = assetInfo->GetType();

	//Only template documents can be opened as template.
	if( inTemplateAsTemplate && assetType != eAssetType_IDTemplate  && assetType != eAssetType_ICTemplate )
		inTemplateAsTemplate = false;

	if( false == Utils<IZPAssetUtils>()->CanOpenAssetType( assetType ) )
	{	//Open the document with which this asset is linked to.
		
		const PMString & parentAssetIDStr = assetInfo->GetLinkedWithDocumentID();
		const IStringData * parentAssetID = ZPDataHelper::GetAssetByID( parentAssetIDStr );
		ASSERT( parentAssetID );
		if(!parentAssetID)
			return;

		this->ProcessAssetForOpening( parentAssetID, inTemplateAsTemplate );
		return;
	}
	
	//Update the asset info before opening it, so that ensure latest works correctly.
	Utils<IZPAMWSUtils>()->FetchAssetInfo_sync( inAssetID->Get() );

	//If InCopy and opening Assignment.
	if( assetType == eAssetType_ICAssignment ) //Host app must be InCopy //&& Utils<IZPInDnUtils>()->IsHostAppInCopy() )
		this->ProcessAssignmentAssetForOpening( inAssetID );
	else //It is document asset
		this->ProcessDocumentAssetForOpening( inAssetID, inTemplateAsTemplate );

}

//----------------------------------------------------------------------------------------
// ProcessDocumentAssetForOpening
//----------------------------------------------------------------------------------------
void
CZPAMHelper::ProcessDocumentAssetForOpening(
	const IStringData *			inAssetID,
	bool						inTemplateAsTemplate ) const
{
	LogFunctionEnterExit;

	ASSERT( inAssetID );
	if( !inAssetID )
		return;
	do {
		const PMString theAssetIDStr = inAssetID->Get();
		IDocument* currentDoc = Utils<IZPAssetDocUtils>()->GetDocumentForDocAssetID( theAssetIDStr );
		if( currentDoc )
		{
			ErrorUtils::PMSetGlobalErrorCode(kZPDocumentAlreadyOpenError);		//TODO: move it to UI plugin.
			return;
		}
		
		AcquireWaitCursor busyCursor;	//TODO: move to UI plugin
		
		InterfacePtr<const IZPAssetInfo> assetInfo( inAssetID, UseDefaultIID() );
		ASSERT( assetInfo );

		bool16 isTemplateDocumentCopy = kFalse;	//True when opening copy of template.
		isTemplateDocumentCopy = inTemplateAsTemplate == false && Utils<IZPAssetUtils>()->IsTemplateType( assetInfo->GetType() );

		ErrorCode errorCode = kSuccess;

		//TODO: lock should be checked after opening it. Opening can be in read only mode. And lock can be acquired in idle loop.
		PMString assetLockedByUser;
		if( !isTemplateDocumentCopy && false == this->IsAssetLockedByCurrentUser( inAssetID, & assetLockedByUser, false ))
		{
			errorCode = Utils<IZPAMWSUtils>()->AcquireLock_sync( theAssetIDStr, kNullString, kNullString );
			if( false == this->IsAssetLockedByCurrentUser( inAssetID, & assetLockedByUser, false ))
				errorCode = kFailure;
		}
		if( errorCode != kSuccess )
		{
			ErrorUtils::PMSetGlobalErrorCode(kZPLockAcquireFailError);
			break;
		}

		// find out if the document is currently locked and retrieve owner.
		{
			
			if( !isTemplateDocumentCopy && false == this->IsAssetLockedByCurrentUser( inAssetID, & assetLockedByUser, false ))
			{
#if defined(InDnCS5) || defined(InDnCS5_5)
				PMString message("Document is geopend door ^1.", PMString::kDontTranslateDuringCall);
#else
				PMString message("Document is geopend door ^1.", -1, PMString::kNoTranslate);
#endif
				StringUtils::ReplaceStringParameters(&message, assetLockedByUser);
				CAlert::InformationAlert(message);
				break;
			}
		}

		//TODO: move it to UI plugin
		//TaskProgressBar bar(kIZPProgressBarOpenDocumentKey, 3, kTrue, kFalse, nil, nil);
		
		IDFile documentFile;
		this->GetAssetFilePath( inAssetID, documentFile );
		
#if defined(InDnCS5) || defined(InDnCS5_5)
		errorCode = this->CheckForHeadAndDownloadIfNeeded( inAssetID, documentFile, false );
#endif

		//TODO: show error if not success
		if( errorCode != kSuccess )
			break;

#if defined(InDnCS5) || defined(InDnCS5_5)
		
		FileTypeInfoID fileType = FileTypeRegistry::GetFileTypeInfoIDForFile(documentFile);
		
		if( Utils<IZPInDnUtils>()->IsHostAppInCopy() && fileType == kInCopyXMLMarkupFileTypeInfoID )
		{
			uint32 nType = Utils<IZPFileUtils>()->GetAssetFileType(documentFile);

			if( nType == kInCopyXMLInterchangeFileTypeInfoID )
			{
				PMString ext( "incx" );
				
				/* Delete if an incx file of same asset exists otherwise rename will fail */
				IDFile parentFolder;
				PMString folderPath;
				Utils<IZPFileUtils>()->GetParentDirectoryPath( documentFile, parentFolder );
				Utils<IZPFileUtils>()->ConvertIDFileToStr( parentFolder, folderPath );			
				Utils<IZPFileUtils>()->RemoveAllFiles( folderPath, ext );				
				
				Utils<IZPFileUtils>()->RenameFile( documentFile, ext );
				Utils<IZPAMWSUtils>()->ReleaseLock_sync( theAssetIDStr );
			}
		}
#endif					

		IOpenFileCmdData::OpenFlags 	openFlags;
		
		if( isTemplateDocumentCopy )
			openFlags = IOpenFileCmdData::kOpenCopy;
		else
			openFlags = IOpenFileCmdData::kOpenOriginal;
		
#if 0	//TODO: if lock is checked above then why are we now making call for asset lock
		InterfacePtr<IApplication> application(gSession->QueryApplication());
		ASSERT(application);
		
		PMString applicationName(application->GetApplicationName());;
		lock.applicationname = application->GetApplicationName();
		lock.documentname = name;
		
		if (!IZPServer::Lock(assetid, lock)) {
			CAlert::ErrorAlert("Error occured during lock of document.");

			break;
		}
#endif
		
		// to prevent errors we could retrieve the assignments in front of opening the document

		UIDRef theOpenedDocRef;
		
		errorCode = Utils<IZPDocUtils>()->OpenDocumentFile( documentFile, openFlags, theOpenedDocRef );
		
		ASSERT(errorCode == kSuccess);
		if(errorCode != kSuccess)
		{
			ErrorUtils::PMSetGlobalErrorCode(kZPFileOpeningError);
			break;
		}

		//Patch: Check if document is converted from other format then save it again with current version of the plugin.
		//This is done because after opening old docs "[Converted]", comes in document title,
		//Can we cannot check-in the same document.
		//InterfacePtr<IDocument> openedDoc( theOpenedDocRef, UseDefaultIID() );
		//ASSERT( openedDoc );
		//if( openedDoc->IsConverted() )
		//	Utils<IZPDocUtils>()->SaveDocToFile( openedDoc, documentFile, inTemplateAsTemplate, true );
#if defined(InDnCS5) || defined(InDnCS5_5)		
		if( Utils<IZPInDnUtils>()->IsHostAppInDesign() )
		{
			InterfacePtr<IDocument> openedDoc( theOpenedDocRef, UseDefaultIID() );
			ASSERT( openedDoc );

			if( !isTemplateDocumentCopy && openedDoc->IsConverted() )
				Utils<IZPAMWSUtils>()->ReleaseLock_sync( theAssetIDStr );
		}
#endif
		
		if( isTemplateDocumentCopy )
			errorCode = this->PostProcessTemplateAssetCopyOpen( inAssetID, theOpenedDocRef );
		else
			errorCode = this->PostProcessDocumentAssetOpen( inAssetID, theOpenedDocRef, documentFile );

		if( errorCode == kSuccess )
			errorCode = Utils<IZPDocUtils>()->OpenDocumentWindow( theOpenedDocRef );

	} while (kFalse);
}

//----------------------------------------------------------------------------------------
// PostProcessDocumentAssetOpen
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMHelper::PostProcessDocumentAssetOpen(
	const IStringData *			inAssetID,
	const UIDRef &				inDocRef,
	const IDFile &				inDocFileOpened ) const
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	ErrorCode toReturn = kFailure;
	do
	{
		//RJK: commented as it is also called by AM.
		//Utils<Facade::IWorkgroupFacade>()->InitDocumentAssetManagement( inDocRef, inDocFileOpened );

		InterfacePtr<IDocument> theDocument( inDocRef, UseDefaultIID() );
		ASSERT( theDocument );
		if ( !theDocument )
			break;
		
		InterfacePtr<const IZPAssetData> assetData( theDocument, UseDefaultIID() );
		ASSERT( assetData );
		if ( !assetData )
			break;
		
		toReturn = kSuccess;
		
		// set asset data (could be updated from the database, don't save with document itself) right?
		//TODO: RJK: why are we setting up the doc name again, when it was set during save?
		InterfacePtr<const IStringData> assetName( inAssetID, IID_IZPDATANAME );
		ASSERT( assetName );

		Utils<IZPDocUtils> docUtils;
		docUtils->SetDocumentName( theDocument, assetName->Get() );

		//TODO: move to UI plugin
		//bar.DoTask(kIZPProgressBarUpdateLinksKey);

		docUtils->EnsureLatestAllAssignments(theDocument);
		docUtils->EnsureLatestAllLinkedStories(theDocument);

		Utils<IZPAssetDocUtils> assetDocUtils;
		assetDocUtils->ValidateAssignmentsName(theDocument);
		assetDocUtils->ValidateStoriesName(theDocument);

		//Update stories only if set in user preference to auto-update.
		InterfacePtr<const IWorkspace> iWorkspace(gSession->QueryWorkspace());
		
		InterfacePtr<const IZPPrefs> zpPrefs( iWorkspace, UseDefaultIID() );
		ASSERT(zpPrefs);
		if( zpPrefs->GetAutoUpdateLinkedStoriesContent() )
		{
			toReturn = Utils<IZPStoryUtils>()->ReLinkOutOfDateStories( theDocument );
#if defined(InDnCS5) || defined(InDnCS5_5)
			Utils<IZPStoryUtils>()->UpdateICStoryLinks( theDocument );
#endif
		}

		// should we create a relink list, and execute the relink at once?

		//TODO: move to UI plugin
		//bar.DoTask(kIZPProgressBarUpdateDocumentKey);

		// download associated assignments
		//TODO: RJK: when opening an assignment in InCopy, why to we need this?
		//toReturn = this->DownloadAssociatedAssignmentsAndUpdate( theDocument );

		toReturn = kSuccess;	//Ignore the return value of relink and assignment downloads.

#if 0	//TODO:
		IZPVecHistoryData outVecHistoryData;
		Utils<IIZPUtils>()->GetHistoryDataForAsset(assetid, outVecHistoryData);
		CGlobals::GetInstance()->AddDocHistoryData(::GetUIDRef(document), outVecHistoryData);
#endif
	}while(false);
	return toReturn;
}

//----------------------------------------------------------------------------------------
// PostProcessTemplateAssetCopyOpen
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMHelper::PostProcessTemplateAssetCopyOpen(
	const IStringData *			inAssetID,
	const UIDRef &				inDocRef) const
{
	LogFunctionEnterExit;
	ErrorCode toReturn = kFailure;
	do
	{
		//TODO: AM was being initialized after template opening. Why?
		//why were we initiating AM with the file that is opened as copy.

		InterfacePtr<IDocument> theDocument( inDocRef, UseDefaultIID() );
		ASSERT( theDocument );
		if ( !theDocument )
			break;
		
		InterfacePtr<IZPAssetData> assetData( theDocument, UseDefaultIID() );
		ASSERT( assetData );
		if ( !assetData )
			break;
		
		toReturn = kSuccess;
		// if new document is created from template then don't fill its asset data.
		ZPCommandHelper::SaveAssetData( assetData, kNullString, kNullString, kNullString );	//assetData->Clear();
		
		IDataBase * docDb = ::GetDataBase( theDocument );
		if( docDb->IsModified() )
			docDb->SetModified( kFalse );	//Allow this document to be closed without alert if user don't change it.

		if( Utils<IZPInDnUtils>()->IsHostAppInCopy() )
		{
			InterfacePtr<IStandAloneDoc> standAlone(theDocument, IID_ISTANDALONEDOC);
			ASSERT( standAlone );
			//standAlone->SetIsStandAloneDoc( kFalse );
		}

		if( Utils<IZPInDnUtils>()->IsHostAppInCopy() )
		{
			//----- Install our doc file handler
			InterfacePtr<IClassIDData> docFileHandlerData(inDocRef, IID_ICLASSIDDATA);
			ClassID prevHandler = docFileHandlerData->GetClassID();
			//docFileHandlerData->Set(kZPDocFileHandlerBoss);

			InterfacePtr<IBoolData> isTemplateCopy( theDocument, IID_IZPISTEMPLATECOPY );
			ASSERT(isTemplateCopy);
			isTemplateCopy->Set( kTrue );

			bool16 initialized = kFalse;
			InterfacePtr<IManageableAsset> asset( theDocument, UseDefaultIID() );
			ASSERT(asset);
			if( asset )
			{	//Patch: AM init is not getting call for documents created from InCopy template
				ClassID amProviderClass = kZPAMSPICDocumentBoss;//asset->GetServiceProviderClass();
				initialized = Utils<IZPAMHelper>()->CallAMServiceDo( asset, amProviderClass, IAMServiceProvider::enInitialize );
			}
		}

		//RJK: commented, we don't persist type with document
		//////if (typeid_.Compare( kFalse, Constants::InDesignTemplateID) == 0)
		//////	// erase data for documents based on template
		//////	data->SetTypeId(Constants::InDesignDocumentID);
		//////else if (typeid_.Compare( kFalse, Constants::InCopyTemplateID) == 0)
		//////	data->SetTypeId(Constants::InCopyDocumentID);
		
	}while( false );

	return toReturn;
}

//----------------------------------------------------------------------------------------
// ProcessAssignmentAssetForOpening
// Must be called in InCopy only
//----------------------------------------------------------------------------------------
void
CZPAMHelper::ProcessAssignmentAssetForOpening(
	const IStringData *			inAssetID ) const
{
	LogFunctionEnterExit;
	do
	{
		//Find if the assignment is already open or not.
		InterfacePtr<const IZPAssetInfo> dbAssetInfo( inAssetID, UseDefaultIID() );
		ASSERT( dbAssetInfo );
		
		const PMString & theDocAssetIDStr = dbAssetInfo->GetLinkedWithDocumentID();
		
		if( theDocAssetIDStr.IsEmpty() )
			break;

		IDocument* currentDoc = Utils<IZPAssetDocUtils>()->GetDocumentForDocAssetID( theDocAssetIDStr );
		if( currentDoc )
			break;	//Document is already open

		//RJK: No need to check if assignment is opened/locked by another user or not.
		//As from InCopy we cannot check-in Assignment. And ID story is automatically check-out when editing.

		IDFile assignmentFile;
		bool hasFilePath = this->GetAssetFilePath( inAssetID, assignmentFile );
		ASSERT( hasFilePath );
		if( !hasFilePath )
			break;

#if defined(InDnCS5) || defined(InDnCS5_5)

		this->CheckForHeadAndDownloadIfNeeded( inAssetID, assignmentFile, false );
		FileTypeInfoID fileType = FileTypeRegistry::GetFileTypeInfoIDForFile(assignmentFile);

		if( Utils<IZPInDnUtils>()->IsHostAppInCopy() && fileType == kAssignmentMarkupFileTypeInfoID )
		{
			uint32 nType = Utils<IZPFileUtils>()->GetAssetFileType(assignmentFile);

			if( nType == kAssignmentFileTypeInfoID )
				break;
			
		}
#endif		
		UIDRef theOpenedDocRef;
		
		IOpenFileCmdData::OpenFlags openFlags = IOpenFileCmdData::kOpenOriginal;
		ErrorCode errorCode = Utils<IZPDocUtils>()->OpenDocumentFile( assignmentFile, openFlags, theOpenedDocRef );
		
		ASSERT(errorCode == kSuccess);
		if(errorCode != kSuccess)
		{
			ErrorUtils::PMSetGlobalErrorCode(kZPFileOpeningError);
			break;
		}

		//Commented as it will get called by doc responder.
		//this->PostProcessAssignmentAssetOpen( inAssetID, theOpenedDocRef, assignmentFile );

		if( errorCode == kSuccess )
			errorCode = Utils<IZPDocUtils>()->OpenDocumentWindow( theOpenedDocRef );

	}while(kFalse);
}

//----------------------------------------------------------------------------------------
// PostProcessAssignmentAssetOpen
//----------------------------------------------------------------------------------------
void
CZPAMHelper::PostProcessAssignmentAssetOpen(	//Only used in InCopy
	const IStringData *			inAssetID,
	const UIDRef &				inDocRef,
	const IDFile &				inAssignmentFile ) const
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	do
	{
#if 0
		InterfacePtr<IDocument> openedDoc(inDocRef, UseDefaultIID());
		if(!openedDoc)
			break;

		InterfacePtr<IAssignment> iAssignment( Utils<IZPDocUtils>()->QueryAssignmentForFilePath( openedDoc, inAssignmentFile) );
		if(!iAssignment)
			break;

		InterfacePtr<IManageableAsset> manageableAsset( Utils<IZPDocUtils>()->QueryManageableAssetForAssignment( openedDoc, iAssignment ) );
		ASSERT( manageableAsset );
		
		UIDRef assignmentUIDRef = ::GetUIDRef(manageableAsset);
		Utils<Facade::IWorkgroupFacade>()->InitDocumentAssetManagement(assignmentUIDRef, inAssignmentFile);
#else
		Utils<Facade::IWorkgroupFacade>()->InitDocumentAssetManagement(inDocRef, inAssignmentFile);
#endif

		InterfacePtr<IDocument> theDocument( inDocRef, UseDefaultIID() );
		ASSERT( theDocument );
		if( theDocument )
		{
			Utils<IZPAssetDocUtils> assetDocUtils;
			assetDocUtils->ValidateAssignmentsName(theDocument);
			assetDocUtils->ValidateStoriesName(theDocument);
			//InterfacePtr<IAssignment> iAssignment( Utils<IZPDocUtils>()->QueryAssignmentForFilePath( theDocument, inAssignmentFile) );
			//assetDocUtils->ValidateAssignedStoriesName(theDocument);
		}

		InterfacePtr<const IWorkspace> iWorkspace(gSession->QueryWorkspace());
		
		InterfacePtr<const IZPPrefs> zpPrefs( iWorkspace, UseDefaultIID() );
		ASSERT(zpPrefs);

		if (zpPrefs->GetAutoUnlockStories())
		{
			InterfacePtr<IDocument> openedDoc(inDocRef, UseDefaultIID());
			if(!openedDoc)
				break;

			InterfacePtr<IAssignment> iAssignment( Utils<IZPDocUtils>()->QueryAssignmentForFilePath( openedDoc, inAssignmentFile) );
			if(!iAssignment)
				break;

#if defined(InDnCS5) || defined(InDnCS5_5)
			uint32 nType = Utils<IZPFileUtils>()->GetAssetFileType(inAssignmentFile);
			if( nType != kAssignmentFileTypeInfoID )
#endif
				Utils<IZPStoryUtils>()->CheckoutAssignmentAllStories(iAssignment);
		}
	}while(kFalse);
}

//----------------------------------------------------------------------------------------
// HandleDocSignal_BeforeOpen
//----------------------------------------------------------------------------------------
void
CZPAMHelper::HandleDocSignal_BeforeOpen(
	const IDFile &				inFile) const
{
	LogFunctionEnterExit;
	do
	{
		//Check if path is from our cache folder.
		bool isOurFilePath = Utils<IZPAMHelper>()->IsSubPathOfOurReplicaFolder( inFile );
		if( !isOurFilePath )
			break;

		if( !ZPDataHelper::IsLoggedIn() )
		{
			ErrorUtils::PMSetGlobalErrorCode(kZPOpenFileWithoutLoginError);
			break;
		}
	}while(kFalse);
}

//----------------------------------------------------------------------------------------
// HandleDocSignal_DuringOpen
//----------------------------------------------------------------------------------------
void
CZPAMHelper::HandleDocSignal_DuringOpen(
	const UIDRef &				inDocRef,
	const IDFile &				inFile) const
{
	LogFunctionEnterExit;
	do
	{
		//Check if path is from our cache folder.
		bool isOurFilePath = Utils<IZPAMHelper>()->IsSubPathOfOurReplicaFolder( inFile );
		if( !isOurFilePath )
			break;

		InterfacePtr<IDocument> theDocument( inDocRef, UseDefaultIID() );
		ASSERT( theDocument );
		if ( !theDocument )
			break;

		ZPAssetIDsList assetIdList;
		Utils<IZPAssetDocUtils>()->GetAssetIdsUsedInDoc(theDocument, assetIdList);

		if( assetIdList.size() <= 0 )
			break;

		Utils<IZPAMWSUtils>()->FetchAssetInfo_sync( assetIdList );
		Utils<IZPAMWSUtils>()->FetchAssetsLockInfo_sync( assetIdList );

		//Assignment file path may be of other machine or other user
		//Update the file location if it is not from our cache folder.
		this->UpdateAssignmentFilePathToCurrentCacheFolder(theDocument);

		IDataBase * docDb = ::GetDataBase( theDocument );
		if( docDb->IsModified() )
			docDb->SetModified( kFalse );
	}while(kFalse);
}

//----------------------------------------------------------------------------------------
// HandleDocSignal_AfterOpen
//----------------------------------------------------------------------------------------
void
CZPAMHelper::HandleDocSignal_AfterOpen(
	const UIDRef &				inDocRef,
	const IDFile &				inFile) const
{
	LogFunctionEnterExit;
	do
	{
		//Check if path is from our cache folder.
		bool isOurFilePath = Utils<IZPAMHelper>()->IsSubPathOfOurReplicaFolder( inFile );
		if( !isOurFilePath )
			break;

		if( !ZPDataHelper::IsLoggedIn() )
		{
			ErrorUtils::PMSetGlobalErrorCode(kZPOpenFileWithoutLoginError);
			break;
		}

		//Get the asset related with file.
		PMString assetIDStr = Utils<IZPAssetUtils>()->GetAssetIDFromLocalFile( inFile );
		const IStringData * dbAssetID = nil;
	
		if( assetIDStr.IsEmpty() == kFalse )
			dbAssetID = ZPDataHelper::GetAssetByID( assetIDStr );

		if( !dbAssetID )
			break;

		//Check the asset type and call respective function.
		InterfacePtr<const IZPAssetInfo> assetInfo( dbAssetID, UseDefaultIID() );
		ASSERT( assetInfo );
		if( assetInfo->GetType() == eAssetType_ICAssignment )
		{
			this->PostProcessAssignmentAssetOpen( dbAssetID, inDocRef, inFile );
		}

	}while(kFalse);
}

//----------------------------------------------------------------------------------------
// HandleDocSignal_AfterRevert
//----------------------------------------------------------------------------------------
void
CZPAMHelper::HandleDocSignal_AfterRevert(
	const UIDRef &				inDocRef,
	const IDFile &				inFile) const
{
	LogFunctionEnterExit;
	do
	{
		//Check if path is from our cache folder.
		bool isOurFilePath = Utils<IZPAMHelper>()->IsSubPathOfOurReplicaFolder( inFile );
		if( !isOurFilePath )
			break;

		if( !ZPDataHelper::IsLoggedIn() )
		{
			ErrorUtils::PMSetGlobalErrorCode(kZPOpenFileWithoutLoginError);
			break;
		}

		//Get the asset related with file.
		PMString assetIDStr = Utils<IZPAssetUtils>()->GetAssetIDFromLocalFile( inFile );
		const IStringData * dbAssetID = nil;

		if( assetIDStr.IsEmpty() == kFalse )
			dbAssetID = ZPDataHelper::GetAssetByID( assetIDStr );

		if( !dbAssetID )
			break;

		if( Utils<IZPInDnUtils>()->IsHostAppInDesign() )
		{
			//TODO: lock should be checked after opening it. Opening can be in read only mode. And lock can be acquired in idle loop.
			PMString assetLockedByUser;
			ErrorCode errorCode = kSuccess;
			if( false == this->IsAssetLockedByCurrentUser( dbAssetID, & assetLockedByUser, false ))
			{
				errorCode = Utils<IZPAMWSUtils>()->AcquireLock_sync( assetIDStr, kNullString, kNullString );
				if( errorCode != kSuccess )
				{
					ErrorUtils::PMSetGlobalErrorCode(kZPLockAcquireFailError);
					break;
				}
			}
			// find out if the document is currently locked and retrieve owner.
			if( false == this->IsAssetLockedByCurrentUser( dbAssetID, & assetLockedByUser, false ))
			{
#if defined(InDnCS5) || defined(InDnCS5_5)
				PMString message("Document is geopend door ^1.", PMString::kDontTranslateDuringCall);
#else
				PMString message("Document is geopend door ^1.", -1, PMString::kNoTranslate);
#endif
				StringUtils::ReplaceStringParameters(&message, assetLockedByUser);
				CAlert::InformationAlert(message);
				break;
			}
		}
	}while(false);
}

//----------------------------------------------------------------------------------------
// HandleDocSignal_BeforeClose
//----------------------------------------------------------------------------------------
void
CZPAMHelper::HandleDocSignal_BeforeClose(
	const UIDRef &				inDocRef,
	const IDFile &				inFile) const
{
	LogFunctionEnterExit;
	do
	{
		//Check if path is from our cache folder.
		bool isOurFilePath = Utils<IZPAMHelper>()->IsSubPathOfOurReplicaFolder( inFile );
		if( !isOurFilePath )
			break;

		if( !ZPDataHelper::IsLoggedIn() )
			break;

		InterfacePtr<IDocument> theDocument( inDocRef, UseDefaultIID() );
		ASSERT( theDocument );
		if ( !theDocument )
			break;

		InterfacePtr<IManageableAsset> docManageableAsset(theDocument, UseDefaultIID());
		ASSERT(docManageableAsset);

		bool isInDesign = Utils<IZPInDnUtils>()->IsHostAppInDesign();
		if (isInDesign && this->CallAMServiceVerifyContext(docManageableAsset, kInvalidClass, IAMServiceProvider::enCancelCheckOut))
		{
			this->CallAMServiceDo(docManageableAsset, kInvalidClass, IAMServiceProvider::enCancelCheckOut);
		}
	}while(false);
}

//----------------------------------------------------------------------------------------
// DownloadAssociatedAssignmentsAndUpdate
//----------------------------------------------------------------------------------------
//Not being used.
ErrorCode
CZPAMHelper::DownloadAssociatedAssignmentsAndUpdate(
	const IDocument *			inDocument) const
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	ErrorCode toReturn = kFailure;
	do
	{
		UIDList assignmentUIDs;
		ZPAssetIDsList assignmentAssetIDs;

		toReturn = Utils<IZPAssetDocUtils>()->GetDocumentAssignments( inDocument, &assignmentAssetIDs, &assignmentUIDs );

		if( toReturn != kSuccess )
			break;

		int listLen = assignmentAssetIDs.size();
		if( listLen == 0 )
		{
			toReturn = kSuccess;
			break;
		}
		//Update Assets cached info from server.
		
		toReturn = Utils<IZPAMWSUtils>()->FetchAssetInfo_sync( assignmentAssetIDs );
		if( toReturn != kSuccess )
		{
			//TODO: move it to UI plugin
			CAlert::InformationAlert("Error occured while retrieving assignment properties.");
		}

		InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
		ASSERT( zpApp );

		InterfacePtr<const IZPAssetMgr> assetMgr( zpApp->GetAssetMgr(), UseDefaultIID() );
		ASSERT(assetMgr);

		//Download all assignment files, if required.
		
		
		for (int32 i = 0 ; i < listLen; ++i)
		{
			const PMString & currAssetID = assignmentAssetIDs[i];
			IDFile assignmentFile;
			const IStringData * theAssetID = assetMgr->GetAssetByID( currAssetID );
			if( !theAssetID )
			{
				//TODO: move it to UI plugin
				//TODO: show this message once.
				CAlert::InformationAlert("Some of the assignments are deleted on server.");
				toReturn = kFailure;
				continue;
			}
			this->GetAssetFilePath( theAssetID, assignmentFile );
			//toReturn = this->CheckForHeadAndDownloadIfNeeded( theAssetID, assignmentFile, false );	//download via service.

			if( toReturn != kSuccess )
			{
				//TODO: move it to UI plugin
				CAlert::InformationAlert("Error occured while downloading assignment version.");
				continue;
			}

			// set file path for assignment
			UIDRef assignmentRef( assignmentUIDs.GetRef(i) );
			toReturn = Utils<IZPDocUtils>()->SetFilePathWithAssignment( assignmentRef, assignmentFile );

#if 0
			//RJK: why to we need to store so many things with assignment. IZPAssetData is sufficient.
			//And we don't need to update asset data here, as assetID, titleID & editionID does not changes.

			// causes indesign document to change, showing the save dialog
			{
				InterfacePtr<ICommand> cmd(CmdUtils::CreateCommand(kIZPUpdateAssignmentDataCmdBoss));
				ASSERT(cmd);
				UIDList assignments(assignmentref);
				cmd->SetItemList(assignments);
				cmd->SetUndoability(ICommand::kAutoUndo);

				InterfacePtr<IIZPAssignmentData> newdata(cmd, UseDefaultIID());
				newdata->SetId(assetid);
				newdata->SetName(izpasset.name);
				newdata->SetDescription(izpasset.description);
				newdata->SetShelve(izpasset.shelve);
				newdata->SetShelveId(izpasset.shelveid);
				newdata->SetTitle(izpasset.title);
				newdata->SetTitleId(izpasset.titleid);
				newdata->SetStatus(izpasset.status);
				newdata->SetStatusId(izpasset.statusid);
				CmdUtils::ProcessCommand(cmd);
			}
#endif
		}
	}while( false );

	return toReturn;
}
#pragma mark -

//----------------------------------------------------------------------------------------
// CheckForHeadAndDownloadIfNeeded
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMHelper::CheckForHeadAndDownloadIfNeeded(
	const IStringData *			inAssetID,
	const IDFile &				inFile,
	bool						inFetchAssetInfoFromWS) const
{
	//LogFunctionEnterExit;
	ErrorCode toReturn = kFailure;

	ASSERT(inAssetID);
	if( !inAssetID )
		return toReturn;

	const PMString & theAssetIDStr = inAssetID->Get();
	if( inFetchAssetInfoFromWS )
	{
		toReturn = Utils<IZPAMWSUtils>()->FetchAssetInfo_sync( theAssetIDStr );
		if( toReturn != kSuccess )
		{
			//TODO: Show error in UI
			return toReturn;
		}
	}
	
	Utils<IZPAssetUtils>()->CreateAssetCacheFolderPath(theAssetIDStr);
	IManagedStatus::VersionState needsDownload = this->GetAssetFileVersionState( inAssetID, inFile );
	if( needsDownload != IManagedStatus::enLocalIsOutOfDate )
	{
		toReturn = kSuccess;
	}
	else
	{
		toReturn = this->DoDownloadAsset( inAssetID, inFile );
	}
	return toReturn;
}

//----------------------------------------------------------------------------------------
// DoDownloadAsset
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMHelper::DoDownloadAsset(
	const IStringData *			inAssetID,
	const IDFile &				inFile) const
{
	//LogFunctionEnterExit;
	ErrorCode toReturn = kFailure;

	InterfacePtr<const IZPAssetInfo> assetInfo( inAssetID, UseDefaultIID() );
	ASSERT( assetInfo );
	const PMString & theAssetIDStr = inAssetID->Get();
	toReturn = Utils<IZPAMWSUtils>()->DownloadVersion_sync( theAssetIDStr, assetInfo->GetVersionID(), inFile );
	if( toReturn == kSuccess )
	{
		this->DoAfterAssetDownloadComplete( inAssetID );
	}
	else
	{
		//TODO: move it to UI plugin
		CAlert::ErrorAlert("Error occured downloading asset version in repository.");
	}
	return toReturn;
}

//----------------------------------------------------------------------------------------
// DoAfterAssetDownloadComplete
//----------------------------------------------------------------------------------------
void
CZPAMHelper::DoAfterAssetDownloadComplete(
	const IStringData *			inAssetID) const
{
	//LogFunctionEnterExit;
	//We always download the latest version, so update the local info

#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IZPAssetsLocalInfoMgr> assetsLocalInfoMgr( gSession, UseDefaultIID() );
	ASSERT( assetsLocalInfoMgr );

	const PMString & theAssetIDStr = inAssetID->Get();
	InterfacePtr<const IZPAssetLocalInfo> assetLocalInfo( assetsLocalInfoMgr->QueryInfoForAsset( theAssetIDStr, true ) );
	ASSERT( assetLocalInfo );

	InterfacePtr<const IZPAssetInfo> dbAssetInfo( inAssetID, UseDefaultIID() );
	ASSERT( dbAssetInfo );

	bool isModified = false;
	int32 versionNumber = 0;
	versionNumber = dbAssetInfo->GetVersionNumber();
	
	Utils<IZPCommandFacade>()->UpdateAssetLocalInfo( assetLocalInfo, nil, &isModified , &versionNumber );
}

//----------------------------------------------------------------------------------------
// IsSubPathOfOurReplicaFolder
//----------------------------------------------------------------------------------------
bool
CZPAMHelper::IsSubPathOfOurReplicaFolder(
	const IDFile &				inFilePath) const
{
	LogFunctionEnterExit;
	IDFile		replicaFolderPath;
	bool toReturn = false;

	if ( Utils<IZPFileUtils>()->GetLocalStoreFolder( replicaFolderPath ) )
	{
		toReturn = Utils<IZPFileUtils>()->IsSubPath( replicaFolderPath, inFilePath );
	}

#ifdef DEBUG
	if( !toReturn )
	{
		IZPLog_Str_( thisFileLA, enLT_Warning, "path not from our replica folder" );
	}
#endif

	return toReturn;
}

//----------------------------------------------------------------------------------------
// IsPathOfAnyReplicaFolder
//----------------------------------------------------------------------------------------
bool
CZPAMHelper::IsPathOfAnyReplicaFolder(
	//True if path is from any replica folder
	const IDFile &				inFilePath) const
{
	//LogFunctionEnterExit;
	bool toReturn = false;

	PMString fullPath;
	Utils<IZPFileUtils>()->ConvertIDFileToStr( inFilePath, fullPath );

	const char * subPathToFind1 = nil;
	const char * subPathToFind2 = nil;
	const char * subPathToFind3 = nil;
	const char * subPathToFind4 = nil;
	const char * subPathToFind5 = nil;
	const char * subPathToFind6 = nil;
	//TODO: proper finding of our path.
	subPathToFind1 = "Local Settings\\Application Data\\Adobe\\InCopy\\";//Version 5.0\\Caches\\iZINE\\";
	subPathToFind2 = "Local Settings\\Application Data\\Adobe\\InDesign\\";//Version 5.0\\Caches\\iZINE\\";
	subPathToFind3 = "Library:Caches:Adobe InCopy:";//Version 5.0:iZINE:";
	subPathToFind4 = "Library:Caches:Adobe InDesign:";//Version 5.0:iZINE:";
	subPathToFind5 = "AppData\\Local\\Adobe\\InDesign";//Version 5.0\\Caches\\iZINE\\";
	subPathToFind6 = "AppData\\Local\\Adobe\\InCopy";//Version 5.0\\Caches\\iZINE\\";


#if defined(InDnCS5) || defined(InDnCS5_5)
#ifdef WINDOWS
	PMString partialPath1( subPathToFind1, PMString::kDontTranslateDuringCall );
	PMString partialPath2( subPathToFind2, PMString::kDontTranslateDuringCall );
	PMString partialPath3( subPathToFind3, PMString::kDontTranslateDuringCall );
	PMString partialPath4( subPathToFind4, PMString::kDontTranslateDuringCall );
	PMString partialPath5( subPathToFind5, PMString::kDontTranslateDuringCall );
	PMString partialPath6( subPathToFind6, PMString::kDontTranslateDuringCall );

#else
	PMString partialPath1( subPathToFind3, PMString::kDontTranslateDuringCall );
	PMString partialPath2( subPathToFind4, PMString::kDontTranslateDuringCall );
	PMString partialPath3( subPathToFind1, PMString::kDontTranslateDuringCall );
	PMString partialPath4( subPathToFind2, PMString::kDontTranslateDuringCall );
	PMString partialPath5( subPathToFind5, PMString::kDontTranslateDuringCall );
	PMString partialPath6( subPathToFind6, PMString::kDontTranslateDuringCall );
#endif

	PMString winSuffix( "\\Caches\\iZINE\\", PMString::kDontTranslateDuringCall );
	PMString macSuffix( ":iZINE:", PMString::kDontTranslateDuringCall );
#else
#ifdef WINDOWS
	PMString partialPath1( subPathToFind1, -1, PMString::kNoTranslate );
	PMString partialPath2( subPathToFind2, -1, PMString::kNoTranslate );
	PMString partialPath3( subPathToFind3, -1, PMString::kNoTranslate );
	PMString partialPath4( subPathToFind4, -1, PMString::kNoTranslate );	
	PMString partialPath5( subPathToFind5, -1, PMString::kNoTranslate );
	PMString partialPath6( subPathToFind6, -1, PMString::kNoTranslate );	
#else
	PMString partialPath1( subPathToFind3, -1, PMString::kNoTranslate );
	PMString partialPath2( subPathToFind4, -1, PMString::kNoTranslate );
	PMString partialPath3( subPathToFind1, -1, PMString::kNoTranslate );
	PMString partialPath4( subPathToFind2, -1, PMString::kNoTranslate );
#endif

	PMString winSuffix( "5.0\\Caches\\iZINE\\", -1, PMString::kNoTranslate );
	PMString macSuffix( "5.0:iZINE:", -1, PMString::kNoTranslate );
#endif
	CharCounter foundIndex = -1;
	if( (foundIndex = fullPath.IndexOfString( partialPath1 )) != -1 
	|| (foundIndex = fullPath.IndexOfString( partialPath2 )) != -1 || (foundIndex = fullPath.IndexOfString( partialPath5 )) != -1
	|| (foundIndex = fullPath.IndexOfString( partialPath6 )) != -1)
	{
#ifdef WINDOWS
		foundIndex = fullPath.IndexOfString( winSuffix, foundIndex );
#else
		foundIndex = fullPath.IndexOfString( macSuffix, foundIndex );
#endif
		if( foundIndex != -1 )
			toReturn = true;
	}
	else if( (foundIndex = fullPath.IndexOfString( partialPath3 )) != -1
	|| (foundIndex = fullPath.IndexOfString( partialPath4 )) != -1 )
	{
#ifdef WINDOWS
		foundIndex = fullPath.IndexOfString( macSuffix, foundIndex );
#else
		foundIndex = fullPath.IndexOfString( winSuffix, foundIndex );
#endif
		if( foundIndex != -1 )
			toReturn = true;
	}

	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetAssetFilePath
//----------------------------------------------------------------------------------------
bool
CZPAMHelper::GetAssetFilePath(
	//Creates file path from asset id and type extention
	const IStringData *			inDBAssetID,
	IDFile &					oFilePath,
	bool						inCreateDirectory) const
{
	//LogFunctionEnterExit;
	InterfacePtr<const IZPAssetInfo> assetInfo( inDBAssetID, UseDefaultIID() );
	ASSERT( assetInfo );
	if( !assetInfo )
		return false;
	
	//Someday: store file path with db asset, during opening the document, and remove after closing the document.
	
	const PMString & theAssetIDStr = inDBAssetID->Get();
	const PMString & strExt = Utils<IZPAssetUtils>()->GetAssetFileExtenstion( assetInfo->GetType() );
	if (inCreateDirectory)
		Utils<IZPAssetUtils>()->CreateAssetCacheFolderPath(theAssetIDStr);

	InterfacePtr<const IStringData> assetName( inDBAssetID, IID_IZPDATANAME );
	ASSERT( assetName );
	const PMString & theAssetName = assetName->Get();
	oFilePath = Utils<IZPAssetUtils>()->CreateLocalFilePathFromAssetID( theAssetIDStr, strExt, theAssetName);

	return true;
}

//----------------------------------------------------------------------------------------
// GetPreviewFilePath
//----------------------------------------------------------------------------------------
bool
CZPAMHelper::GetPreviewFilePath(
	//Returns preview path for document/template type only.
	const IStringData *			inDBAssetID,
	IDFile &					oFilePath) const
{
	//LogFunctionEnterExit;
	bool toReturn = false;
	do
	{
		if( !inDBAssetID )
			break;

		InterfacePtr<const IZPAssetInfo> assetInfo( inDBAssetID, UseDefaultIID() );
		ASSERT( assetInfo );
		
		enAssetType assetType = assetInfo->GetType();
		if( assetType == eAssetType_IDDocument
			|| assetType == eAssetType_IDTemplate
			|| assetType == eAssetType_ICTemplate
			|| assetType == eAssetType_ICDocument )
		{
			const PMString & theAssetIDStr = inDBAssetID->Get();
			const PMString strExt = "jpg";
		
			InterfacePtr<const IStringData> assetName( inDBAssetID, IID_IZPDATANAME );
			ASSERT( assetName );
			const PMString & theAssetName = assetName->Get();
			oFilePath = Utils<IZPAssetUtils>()->CreateLocalFilePathFromAssetID( theAssetIDStr, strExt, theAssetName);
			toReturn = true;
		}

	}while(kFalse);

	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetAssetVersionState
//----------------------------------------------------------------------------------------
IManagedStatus::VersionState
CZPAMHelper::GetAssetVersionState(
	const IStringData *			inDBAssetID) const
{
	//LogFunctionEnterExit;
	IManagedStatus::VersionState toReturn = IManagedStatus::enUnknownVersionState;
	do
	{
		if( !inDBAssetID )
			break;

		IDFile assetFile;
		Utils<IZPAMHelper>()->GetAssetFilePath( inDBAssetID, assetFile );
		toReturn = this->GetAssetFileVersionState( inDBAssetID, assetFile );

	}while(kFalse);

	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetAssetFileVersionState
//	Checks the local file version and server version.
//----------------------------------------------------------------------------------------
IManagedStatus::VersionState
CZPAMHelper::GetAssetFileVersionState(
	const IStringData *			inDBAssetID,
	const IDFile &				inFilePath) const
{
	//LogFunctionEnterExit;
	IManagedStatus::VersionState toReturn = IManagedStatus::enLocalIsOutOfDate;
	bool fileExists = Utils<IZPFileUtils>()->DoesFileExist( inFilePath );
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	do
	{
		if( !fileExists )
			break;
	
		//check if asset head version is already downloaded or we have earlier version, modified locally
		InterfacePtr<const IZPAssetsLocalInfoMgr> assetsLocalInfoMgr( gSession, UseDefaultIID() );
		ASSERT( assetsLocalInfoMgr );

		const PMString & theAssetIDStr = inDBAssetID->Get();

		InterfacePtr<const IZPAssetLocalInfo> assetLocalInfo( assetsLocalInfoMgr->QueryInfoForAsset( theAssetIDStr ) );
		if( !assetLocalInfo )
			break;

		//Download if 
		//	local info does not exist
		//	locally not modified and server version is greater than local
		bool16 locallyModified = assetLocalInfo->GetFileIsLocallyModified();
		if( locallyModified == kTrue )
		{
			IZPLog_Str_( thisFileLA, enLT_Details, "File is locally modified." );
		}

		InterfacePtr<const IZPAssetInfo> dbAssetInfo( inDBAssetID, UseDefaultIID() );
		ASSERT( dbAssetInfo );
		bool localVersionIsSame = dbAssetInfo->GetVersionNumber() == assetLocalInfo->GetFileVersionNumber();

		if( localVersionIsSame )
		{
			IZPLog_Str_( thisFileLA, enLT_Details, "File is of same version." );
			if( locallyModified )
				toReturn = IManagedStatus::enLocalIsNewer;
			else
				toReturn = IManagedStatus::enUpToDate;
		}
		else
		{
			IZPLog_Str_( thisFileLA, enLT_Details, "File is of different version." );
			ASSERT( dbAssetInfo->GetVersionNumber() > assetLocalInfo->GetFileVersionNumber() );
			if( locallyModified )
				toReturn = IManagedStatus::enVersionConflict;
			else
				toReturn = IManagedStatus::enLocalIsOutOfDate;
		}
	}while( false );

	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetEditingState
//----------------------------------------------------------------------------------------
IManagedStatus::EditingState
CZPAMHelper::GetEditingState(
	const IAMLockable::LockState		inLockState,
	const IManagedStatus::VersionState	inVersionState) const
{
	//LogFunctionEnterExit;
	IManagedStatus::EditingState toReturn = IManagedStatus::enUnknownEditingState;
	do
	{
		if( (inLockState == IAMLockable::enAvailable || inLockState == IAMLockable::enUnavailable )
			&& ( inVersionState != IManagedStatus::enLocalIsNewer && inVersionState != IManagedStatus::enVersionConflict ) )
		{	//	asset is not currently being edited 
			toReturn = IManagedStatus::enNotEditing;
		}
		else if ( inLockState == IAMLockable::enCheckedOut 
			&& ( inVersionState == IManagedStatus::enLocalIsNewer || inVersionState == IManagedStatus::enUpToDate ) )
		{	//	this instance is being edited (edited here) 
			toReturn = IManagedStatus::enEditingLocally;
		}
		else if ( inLockState == IAMLockable::enLocked 
			&& ( inVersionState != IManagedStatus::enLocalIsNewer && inVersionState != IManagedStatus::enVersionConflict ) )
		{	//	being edited elsewhere 
			toReturn = IManagedStatus::enEditingRemotely;
		}
		else if ( inVersionState == IManagedStatus::enVersionConflict )
		{	// editing here and elsewhere at the same time 
			toReturn = IManagedStatus::enEditingConflict;
		}

	}while(kFalse);
	return toReturn;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// IsAssetLocked
//----------------------------------------------------------------------------------------
bool
CZPAMHelper::IsAssetLocked(
	const IStringData *			inAssetID) const
{
	//LogFunctionEnterExit;
	InterfacePtr<const IZPAssetLockInfo> assetLockInfo( inAssetID, UseDefaultIID() );
	ASSERT( assetLockInfo );

	//Check if lock is acquired by current user.
	if( assetLockInfo->GetLockID().IsEmpty() || assetLockInfo->GetLockID() == kEmptyGUID )
		return false;
	return true;
}

//----------------------------------------------------------------------------------------
// IsAssetLockedByCurrentUser
//----------------------------------------------------------------------------------------
bool
CZPAMHelper::IsAssetLockedByCurrentUser(
	//AssetID must in cache
	const PMString &			inAssetID,
	PMString *					oLockedByUser,
	bool						inFetchLockStatusFromWS) const
{
	LogFunctionEnterExit;
	const IStringData * assetID = ZPDataHelper::GetAssetByID( inAssetID );
	ASSERT( assetID );
	if(!assetID)
		return false;
	
	return this->IsAssetLockedByCurrentUser( assetID, oLockedByUser, inFetchLockStatusFromWS );
}

//----------------------------------------------------------------------------------------
// CanChangeStatusForAsset
//----------------------------------------------------------------------------------------
bool
CZPAMHelper::CanChangeStatusForAsset(
	const IStringData *			inDBAssetID) const
{
	//LogFunctionEnterExit;
	if (!inDBAssetID)
		return false;

	do
	{
		InterfacePtr<const IZPAssetInfo> dbAssetInfo( inDBAssetID, UseDefaultIID() );
		ASSERT( dbAssetInfo );
		if( !dbAssetInfo )
			break;

		enAssetType type = dbAssetInfo->GetType();
		if ( type == eAssetType_ICDocument
			|| type == eAssetType_ICTemplate
			|| type == eAssetType_IDStory )
			break;

		return true;
	}while(false);

	return false;
}

//----------------------------------------------------------------------------------------
// IsAssetLockedByCurrentUser
//----------------------------------------------------------------------------------------
bool
CZPAMHelper::IsAssetLockedByCurrentUser(
	const IStringData *			inAssetID,
	PMString *					oLockedByUser,
	bool						inFetchLockStatusFromWS) const
{
	LogFunctionEnterExit;
	if( oLockedByUser )
		*oLockedByUser = kNullString;
	bool toReturn = false;
	do
	{
		if( !inAssetID )
			break;

		if( oLockedByUser )
			*oLockedByUser = kNullString;
		
		if( inFetchLockStatusFromWS )
		{
			ErrorCode errorCode = kSuccess;
			//TODO: FetchAssetsLockInfo_sync
			//errorCode = Utils<IZPAMWSUtils>()->AcquireLock_sync( theAssetIDStr );
			
			if( errorCode != kSuccess )
				break;
		}

		InterfacePtr<const IZPAssetLockInfo> assetLockInfo( inAssetID, UseDefaultIID() );
		ASSERT( assetLockInfo );
		//Check if lock is acquired by current user.
		if( oLockedByUser )
			*oLockedByUser = assetLockInfo->GetUserName();
		
		if( assetLockInfo->GetLockState() == IAMLockable::enCheckedOut )
			toReturn = true;
		else if( assetLockInfo->GetLockState() == IAMLockable::enUnavailable )
			toReturn = true;

	}while(false);

	return toReturn;
}

//----------------------------------------------------------------------------------------
// CalculateAssetLockState
//----------------------------------------------------------------------------------------
IAMLockable::LockState
CZPAMHelper::CalculateAssetLockState(
	const IZPAssetLockInfo *	inLockInfo) const
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	IAMLockable::LockState toReturn = IAMLockable::enUndefined;
	do
	{
		InterfacePtr<const IZPAssetInfo> dbAssetInfo( inLockInfo, UseDefaultIID());
		ASSERT( dbAssetInfo );
		if( dbAssetInfo && dbAssetInfo->GetVersionNumber() == 0 )
		{
			toReturn = IAMLockable::enUnavailable;
			break;
		}
		if( inLockInfo->GetLockID().IsEmpty() || inLockInfo->GetLockID() == kEmptyGUID )
		{
			toReturn = IAMLockable::enAvailable;
			break;
		}

		toReturn = IAMLockable::enLocked;

		const PMString & currUserID = ZPDataHelper::GetCurrUserID();
		if( inLockInfo->GetUserID().Compare( kFalse, currUserID ) == 0 )
		{
			InterfacePtr<const IApplication> application(gSession->QueryApplication());
			ASSERT(application);
			const PMString appName = application->GetApplicationName();
			if( inLockInfo->GetAppName().Compare( kTrue, appName ) == 0 )
				toReturn = IAMLockable::enCheckedOut;
		}

	}while(kFalse);

	return toReturn;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// AskVersionComments
//----------------------------------------------------------------------------------------
IAMUIService::FollowupAction
CZPAMHelper::AskVersionComments(
	const UIDRef &				inAssetRef,	//Should be document asset
	PMString &					oCheckinComment) const
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	IAMUIService::FollowupAction toReturn = IAMUIService::enNone;
	ErrorCode status(kFailure);
	
	InterfacePtr<IManageableAsset> asset( inAssetRef, UseDefaultIID() );
	InterfacePtr<IAMSPManager> iAMSPManager( gSession, UseDefaultIID() );
	if ( asset && asset->IsBound() && iAMSPManager )
	{
		InterfacePtr<IAMUIServiceProvider> amsp( iAMSPManager->GetUIServiceProvider( 
			asset, IAMUIServiceProvider::enGetVersionComments ), UseDefaultIID() );
		if ( amsp )
		{
			status = kSuccess; // always assume kSuccess if no service is available
			InterfacePtr<IAMUIService> versionCommentsService( amsp->GetService( IAMUIServiceProvider::enGetVersionComments ) );
			if ( versionCommentsService )
			{
				InterfacePtr<IAMServiceData> serviceData( versionCommentsService, UseDefaultIID() );

				status = versionCommentsService->Do( asset );
				if( status == kSuccess )
				{
					serviceData->Get( IAMUIService::enVersionComment, oCheckinComment );
					serviceData->Get( IAMUIService::enFollowupAction , toReturn );
				}
			}
		}
	}
	return toReturn;
}
//----------------------------------------------------------------------------------------
// AskVersionComments_Assignment
//----------------------------------------------------------------------------------------
IAMUIService::FollowupAction
CZPAMHelper::AskVersionComments_Assignment(
	const IDFile &				inFilePath,	//Must be assignment asset
	const UIDRef &				inAssetRef,	//Must be assignment UIDRef
	PMString &					oCheckinComment) const
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	IAMUIService::FollowupAction toReturn = IAMUIService::enNone;
	ErrorCode status(kFailure);
	
	InterfacePtr<IAMSPManager> iAMSPManager( gSession, UseDefaultIID() );
	if( iAMSPManager )
	{
		InterfacePtr<IAMUIServiceProvider> amsp( iAMSPManager->GetUIServiceProvider( 
			inFilePath, IAMUIServiceProvider::enGetVersionComments ), UseDefaultIID() );
		if ( amsp )
		{
			status = kSuccess; // always assume kSuccess if no service is available
			InterfacePtr<IAMUIService> versionCommentsService( amsp->GetService( IAMUIServiceProvider::enGetVersionComments ) );
			if ( versionCommentsService )
			{
				InterfacePtr<IAMServiceData> serviceData( versionCommentsService, UseDefaultIID() );
				serviceData->Set( IAMService::enTargetAsset, inAssetRef );

				status = versionCommentsService->Do( nil );

				if( status == kSuccess )
				{
					serviceData->Get( IAMUIService::enVersionComment, oCheckinComment );
					serviceData->Get( IAMUIService::enFollowupAction , toReturn );
				}
			}
		}
	}
	return toReturn;
}

//----------------------------------------------------------------------------------------
// QueryAMService
//----------------------------------------------------------------------------------------
IAMService *
CZPAMHelper::QueryAMService(
	IManageableAsset *			inAsset,
	ClassID						inServiceProvider,
	IAMServiceProvider::AssetService	inServiceID) const
{
	//LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	do
	{
		InterfacePtr<IAMSPManager> iAMSPManager( gSession, UseDefaultIID() );
		ASSERT( iAMSPManager );
		
		if( inServiceProvider == kInvalidClass && !inAsset )
			break;
		if( inServiceProvider == kInvalidClass )
			inServiceProvider = inAsset->GetServiceProviderClass();
		ASSERT( inServiceProvider != kInvalidClass );
		
		IAMServiceProvider * amServiceProvider = iAMSPManager->GetServiceProvider( inServiceProvider );
		ASSERT( amServiceProvider );

		if( amServiceProvider )
		{
			InterfacePtr<IAMService> amService( amServiceProvider->GetService( inServiceID ) );
			ASSERT( amService );
			return amService.forget();
		}
	}while(false);
	return nil;
}

//----------------------------------------------------------------------------------------
// QueryAMUIService
//----------------------------------------------------------------------------------------
IAMUIService *
CZPAMHelper::QueryAMUIService(
	IManageableAsset *			inAsset,
	IAMUIServiceProvider::UIService	inServiceID) const
{
	//LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	do
	{
		InterfacePtr<IAMSPManager> iAMSPManager( gSession, UseDefaultIID() );
		ASSERT( iAMSPManager );
		
		IAMUIServiceProvider * amUIServiceProvider = iAMSPManager->GetUIServiceProvider( inAsset, inServiceID );
		ASSERT( amUIServiceProvider );

		if( amUIServiceProvider )
		{
			InterfacePtr<IAMUIService> amUIService( amUIServiceProvider->GetService( inServiceID ) );
			//ASSERT( amUIService );
			return amUIService.forget();
		}
	}while(false);
	return nil;
}

//----------------------------------------------------------------------------------------
// CallAMServiceDo
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMHelper::CallAMServiceDo(
	IManageableAsset *			inAsset,
	ClassID						inServiceProvider,
	IAMServiceProvider::AssetService	inServiceID) const
{
	//LogFunctionEnterExit;
	ErrorCode toReturn = kFailure;

	if ( inAsset != nil )
	{
		InterfacePtr<IAMService> amService( this->QueryAMService(inAsset, inServiceProvider, inServiceID ) );
		ASSERT( amService );
		toReturn = amService->Do( inAsset );
	}

	return toReturn;
}

//----------------------------------------------------------------------------------------
// CallAMServiceVerifyContext
//----------------------------------------------------------------------------------------
bool16
CZPAMHelper::CallAMServiceVerifyContext(
	IManageableAsset *			inAsset,
	ClassID						inServiceProvider,
	IAMServiceProvider::AssetService	inServiceID) const
{
	//LogFunctionEnterExit;
	bool16 toReturn = kFalse;
	do
	{
		InterfacePtr<IAMService> amService( this->QueryAMService(inAsset, inServiceProvider, inServiceID ) );
		ASSERT( amService );
		toReturn = amService->VerifyContext( inAsset );
	}while(kFalse);

	return toReturn;
}

//----------------------------------------------------------------------------------------
// InitAssetManagementForStory
//----------------------------------------------------------------------------------------
void
CZPAMHelper::InitAssetManagementForStory(
	//Finds data link for story and starts AM
	const UIDRef &				inStoryRef,
	const IDFile &				inFilePath) const
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IAMSPManager> iAMSPManager( gSession, UseDefaultIID() );
	ASSERT( iAMSPManager );
	
	UIDRef storyAssetUIDRef = Utils<Facade::IWorkgroupStoryFacade>()->GetAssetUIDRef( inStoryRef );

	if( storyAssetUIDRef.GetUID() == kInvalidUID )
		return;

	InterfacePtr<IManageableAsset> storyAsset( storyAssetUIDRef , UseDefaultIID() );
	iAMSPManager->BindAsset( storyAsset, IManageableAsset::enLinked, inFilePath );

	Utils<Facade::IWorkgroupFacade>()->InitLinkAssetManagement( storyAssetUIDRef );

#if defined(InDnCS5) || defined(InDnCS5_5)
	Utils<Facade::IWorkgroupFacade>()->UpdateStoredAssetReference( storyAssetUIDRef );
	Utils<Facade::IWorkgroupFacade>()->UpdateProxyVersionNumber( storyAssetUIDRef );
#else
	InterfacePtr<IDataLink> assetDataLink( storyAssetUIDRef , UseDefaultIID() );
	Utils<Facade::IWorkgroupFacade>()->UpdateStoredAssetReference( assetDataLink );
	Utils<Facade::IWorkgroupFacade>()->UpdateProxyVersionNumber( assetDataLink );
#endif

	//Start monitoring, this will update the lock status.
	Utils<Facade::IWorkgroupFacade>()->StartMonitoring( storyAssetUIDRef );
}

//----------------------------------------------------------------------------------------
// UpdateAssignmentFilePathToCurrentCacheFolder
//----------------------------------------------------------------------------------------
void
CZPAMHelper::UpdateAssignmentFilePathToCurrentCacheFolder(
	const IDocument *			inDocument) const
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	if(!inDocument)
		return;

	IDataBase* db = ::GetDataBase(inDocument);
	if(!db)
		return;
	
	InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<const IZPAssetMgr> assetMgr( zpApp->GetAssetMgr(), UseDefaultIID() );
	ASSERT(assetMgr);

	//Assignments path
	InterfacePtr<IAssignedDocument> assignmentDoc(inDocument, UseDefaultIID());
	ASSERT(assignmentDoc);

	Utils<IZPDocUtils> docUtils;

	//Assignments file path may be of other machine or other user
	//Update the file location if it is not from our cache folder.
		
	UIDList assignmentList = assignmentDoc->GetAssignments();
	IDFile idFile;
	for (int32 i = 0 ; i < assignmentList.Length(); ++i) 
	{
		UIDRef assignmentref = assignmentList.GetRef(i);
		
		InterfacePtr<IAssignment> assignment(assignmentref, UseDefaultIID());
		ASSERT(assignment);

		const PMString & filePath = assignment->GetFile();
		if( filePath.IsEmpty() )
			continue;
		Utils<IZPFileUtils>()->ConvertStrToIDFile(idFile, filePath);

		if(! this->IsSubPathOfOurReplicaFolder(idFile))
		{
			InterfacePtr<IDataLink> dataLink(assignment, UseDefaultIID());
			if(!dataLink)
				continue;

			InterfacePtr<IZPAssetData> asset(dataLink, UseDefaultIID());
			if(!asset)
				continue;

			if (! asset->GetAssetID().IsEmpty())
			{
				const IStringData * assetId = assetMgr->GetAssetByID(asset->GetAssetID());
				this->GetAssetFilePath(assetId, idFile);

				docUtils->SetFilePathWithAssignment(assignmentref, idFile);
			}
		}
	}
}
