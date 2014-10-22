//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/Services/CZPAMDocumentCheckInService.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 28-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IAMService.h"
#include "IAMServiceData.h"
#include "IAMSPManager.h"
#include "IAMUIService.h"
#include "IAssignmentMgr.h"
#include "IBoolData.h"
#include "IDocument.h"
#include "IInCopyDocUtils.h"
#include "IManageableAsset.h"
#include "ISaveAssetContent.h"
#include "IStringData.h"
#include "IWorkgroupFacade.h"
#include "IWorkgroupStoryFacade.h"
#include "IWorkgroupUIFacade.h"
#include "IWorkspace.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes
#include "CAlert.h"
#include "ErrorUtils.h"
#include "FileUtils.h"
#include "PlatformFileSystemIterator.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPAMHelper.h"
#include "IZPAMWSUtils.h"
#include "IZPAssetCheckInData.h"
#include "IZPAssetDocUtils.h"
#include "IZPAssetUtils.h"
#include "IZPDocUtils.h"
#include "IZPFileUtils.h"
#include "IZPInDnUtils.h"
#include "IZPPrefs.h"

//IZP General includes
#include "ZPDataHelper.h"
#include "AZPAMService.h"

//Other plugin
#include "IZExpID.h"		//This should be first
#include "IZExpFileUtils.h"
#include "IZExpXMLWriteFacade.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AMService
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

/** Checks an asset into the asset management system and unlocks it. For most implementations this
	service will copy local changes to a secure location, such as the asset management database.
	After this service completes successfully, the asset is available for check out (see the enCheckOut
	service).
	
	Input IAMServiceData parameters:
	
		IAMService Data Identifier		 | Data Type		  | Notes
		---------------------------------+--------------------+--------------------------------------
		IAMService::enTargetAsset		 | IDFile			  | When specified, the service should use
										 |					  | the IDFile rather than the IManageableAsset *
										 |					  | argument on the IAMService::Do() method.
		IAMService::enCheckInComment	 | PMString			  | Empty PMString signifies a blank comment.
		IAMService::enForceSave			 | IWorkgroupFacade:: | Controls overwriting latest version with
										 |	ForceSave		  | local changes. 
		IAMService::enContentSaver		 | void *			  | Pointer to ISaveAssetContent callback.
										 |					  | When nil, save is not needed.
		IAMService::enOverrideAuthority  | boolean flag		  | For assignment packaging feature.
		IAMService::enCheckOutToUserName | PMString			  | For assignment packaging feature.
		IAMService::enUIFlags			 | UIFlags			  | UI permitted for use by the service.
	
	Output IAMServiceData parameters: None.
													  			
	@see ISaveAssetContent.
	@see IWorkgroupFacade.
*/

#define inheritClass AZPAMService
class CZPAMDocumentCheckInService : public inheritClass
{
public:
						CZPAMDocumentCheckInService(IPMUnknown * boss);
	virtual				~CZPAMDocumentCheckInService();

	virtual ErrorCode	Do( IManageableAsset * asset );
	virtual bool16		VerifyContext( IManageableAsset * asset );

protected:
	ErrorCode			ValidateCheckInData(
							IManageableAsset *			asset );
	ErrorCode			CheckInModelChanges(
							UIDRef assetRef, const IDFile & file );

	bool				GetAssetFile(
							const IManageableAsset *	inManageableAsset,
							IDFile &					oAssetFile) const;
private:
	void				CreateExportAndUpload(
							const IDocument *				inDoc,
							const PMString &				inAssetId,
							const PMString &				inVersionId,
							const IDFile &					inFile );
	bool				UploadImageFilesInDirectory(
							const IDFile &				inFileDirectory,
							const PMString &			inAssetID,
							const PMString &			inVersionID,
							const bool					inIsHighRes) const;

	ErrorCode			CheckInAllAssignment(
							IDocument *					inDoc,
							bool						inOnlyDirtyAssignment) const;
};

#define inheritClass_ICCheckIn CZPAMDocumentCheckInService
class CZPAMInCopyDocumentCheckInService : public inheritClass_ICCheckIn
{
public:
						CZPAMInCopyDocumentCheckInService(IPMUnknown * boss);
	virtual				~CZPAMInCopyDocumentCheckInService();

	virtual ErrorCode	Do( IManageableAsset * asset );
	bool16				VerifyContext( IManageableAsset * asset );

protected:
private:
};

CREATE_PMINTERFACE(CZPAMDocumentCheckInService, kZPAMDocumentCheckInServiceImpl)
CREATE_PMINTERFACE(CZPAMInCopyDocumentCheckInService, kZPAMICDocumentCheckInServiceImpl)

#pragma mark -
CZPAMDocumentCheckInService::CZPAMDocumentCheckInService(IPMUnknown * boss)
: inheritClass(boss)
{
	IZPLog_ObjC( thisFileLA );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMDocumentCheckInService::~CZPAMDocumentCheckInService()
{
	IZPLog_ObjD( thisFileLA );
}

#pragma mark -

//----------------------------------------------------------------------------------------
// Do
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMDocumentCheckInService::Do(
	IManageableAsset * asset )
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	this->LogManageableAssetInfo( asset );

	ErrorCode status = kFailure;
	
	InterfacePtr<IAMServiceData> serviceData( this, UseDefaultIID() );
	ASSERT(serviceData);
	
	IDFile assetFile;
	
	bool hasFilePath = this->GetAssetFile( asset, assetFile );	// get the local file
	
	//If inAsset is nil, then find it from open docs.
	if( !asset && hasFilePath )
	{
		const IDocument * assetDoc = this->GetDocumentForFile( assetFile );
		ASSERT( assetDoc );

		InterfacePtr<IManageableAsset> manageableAsset( assetDoc, UseDefaultIID() );
		ASSERT( manageableAsset );
		
		asset = manageableAsset;
	}
	
	if( asset )
		status = this->ValidateCheckInData( asset );

	if( status ==  kSuccess )
	{
		InterfacePtr<const IWorkspace> iWorkspace(gSession->QueryWorkspace());
		InterfacePtr<const IZPPrefs> zpPrefs( iWorkspace, UseDefaultIID() );
		ASSERT(zpPrefs);
		if( zpPrefs->GetIncludeAllAssignmentsOnDocumentCheckin() )
		{
			InterfacePtr<IDocument> assetDoc( asset, UseDefaultIID() );
			ASSERT( assetDoc );
			this->CheckInAllAssignment(assetDoc, false);
		}

		// check it in to our system
		UIDRef assetRef = ::GetUIDRef( asset );
		status = CheckInModelChanges( assetRef, assetFile );
	}
	
	if ( status == kCancel )
		ErrorUtils::PMSetGlobalErrorCode( kCancel );
	else if ( status != kSuccess )
		ErrorUtils::PMSetGlobalErrorCode( kFailure );
	
	 IZPLog_Str_( thisFileLA, enLT_DebugInfo, " status = %d", status);
	
	return status;
}

// return kTrue if story can be checked in
//----------------------------------------------------------------------------------------
// VerifyContext
//----------------------------------------------------------------------------------------
bool16
CZPAMDocumentCheckInService::VerifyContext(
	IManageableAsset * pAsset )
{
	LogFunctionEnterExit;
	this->LogManageableAssetInfo( pAsset );

	bool16 toReturn = kFalse;
	
	const IStringData *	dbAssetID = this->GetDBAsset_FromDoc( pAsset );

	//check if this lock is acquired by this user & from this app.
	if( dbAssetID && this->IsAssetLockedByCurrentUser( dbAssetID ) )
	{
		IDFile assetFile;
		Utils<IZPAMHelper>()->GetAssetFilePath( dbAssetID, assetFile, true);
		//check if this file locally modified or not.
		//Check-In only if file is locally modified & don't conflict.
		IManagedStatus::VersionState fileState = Utils<IZPAMHelper>()->GetAssetFileVersionState( dbAssetID, assetFile );
		if( fileState == IManagedStatus::enLocalIsNewer )
			toReturn = kTrue;
		else if( fileState == IManagedStatus::enUpToDate )
		{	//Enable if the document has unsaved changed.
			InterfacePtr<IDocument> assetDoc( pAsset, UseDefaultIID() );
			ASSERT( assetDoc );
			toReturn = assetDoc->IsModified( );
		}

	}
	
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " VerifyContext = %hd", toReturn);
	return toReturn;
}

//----------------------------------------------------------------------------------------
// ValidateCheckInData
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMDocumentCheckInService::ValidateCheckInData(
	IManageableAsset *			asset)
{
	LogFunctionEnterExit;
	ErrorCode toReturn = kSuccess;
	InterfacePtr<const IZPAssetCheckInData> assetCheckInData( asset, UseDefaultIID());
	ASSERT(assetCheckInData);
	
	const PMString & statusID = assetCheckInData->GetStatusID();
	if( statusID.IsEmpty() )
	{
		PMString checkInComments;
		UIDRef assetRef = ::GetUIDRef( asset );
		IAMUIService::FollowupAction followupAction = Utils<IWorkgroupUIFacade>()->DoGetVersionComments( assetRef, checkInComments );
		//IAMUIService::FollowupAction followupAction = this->AskVersionComments( assetRef, checkInComments );
		if(followupAction == IAMUIService::enCancel || followupAction == IAMUIService::enNone )
		{
			toReturn = kCancel;
		}
		else
		{
			InterfacePtr<IAMServiceData> serviceData( this, UseDefaultIID() );
			ASSERT( serviceData );
			serviceData->Set( IAMService::enCheckInComment, checkInComments);
		}
	}
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " VerifyContext = %hd", toReturn);
	return toReturn;
}

//----------------------------------------------------------------------------------------
// CheckInModelChanges
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMDocumentCheckInService::CheckInModelChanges(
	UIDRef assetRef, const IDFile& inFile )
{
	LogFunctionEnterExit;

	ErrorCode toReturn = kFailure;
	
	do {
	#if DEBUG
		this->LogFilePath( inFile );
		IZPLog_Str_( thisFileLA, enLT_Details, "UID = 0x%08X, ",assetRef.GetUID().Get() );
	#endif
		
		InterfacePtr<IAMServiceData> serviceData( this, UseDefaultIID() );
		ASSERT( serviceData );
		
		InterfacePtr<IManageableAsset> asset( assetRef, UseDefaultIID() );
		ASSERT(asset);
		
		const IStringData *	dbAssetID = this->GetDBAsset_FromDoc( asset );
		ASSERT( dbAssetID );

		if( !dbAssetID )
			break;

		// get some useful flags
		UIFlags uiFlags;
		serviceData->Get( IAMService::enUIFlags, uiFlags );
		
		int32 forceSave;
		serviceData->Get( IAMService::enForceSave, forceSave);	//Means override check-in without updating to head.

		PMString username;
		serviceData->Get( IAMService::enCheckOutToUserName, username);

		PMString comment;
		serviceData->Get( IAMService::enCheckInComment, comment);

		bool16 allowOverride = serviceData && serviceData->GetFlag( IAMService::enOverrideAuthority );

		/*
		IAMLockable::LockState lockState = IAMLockable::enUndefined;
		
		InterfacePtr<IAMLockable> assetLock( asset->GetLockable(), UseDefaultIID() );
		ASSERT(assetLock);
		
		if ( assetLock )
			lockState = assetLock->VerifyState();
		*/

		//TODO: RJK, move to UI pln.
		//SubTaskFoldupProgressBar bar(kIZPProgressBarUpdateDocumentKey, 1, 4);
	
		InterfacePtr<IDocument> document(asset, UseDefaultIID());
		ASSERT(document);
		if (!document)  {
			CAlert::InformationAlert("Document not set.");
			break;
		}
		
		//RJK: Not the place do it.
		//if(Utils<IZPInDnUtils>()->IsHostAppInCopy() == false)
		//	toReturn = Utils<IZPAMHelper>()->UpdateAssignmentsOnServer( assetRef );
		
		//TODO: RJK, move to UI pln
		//bar.SetTaskText(kIZPProgressBarUploadDocumentKey);

		//TODO: RJK - In case of ID document, it is already saved, then why are we saving it again?
		// Save the content to the local replica file, using the default ISaveAssetContent implementation
		// Note that the ISaveAssetContent just exports the placed story to our local, replica file
		//  We still need to then upload the replica back to the database
		ISaveAssetContent* saveAssetContent = static_cast<ISaveAssetContent*>( serviceData->Get( IAMService::enContentSaver ) );
		if ( saveAssetContent && saveAssetContent->Do( assetRef, inFile ) != kSuccess )
		{
			// failed to write story out to local file
			IZPLog_Str_( thisFileLA, enLT_Error, "** IAMService::enContentSaver FAILED**\n");
			ErrorUtils::PMSetGlobalErrorCode(kZPDocumentWritingError);
			break;
		}
		
		// get the unique key for the link (which is actually just the path to the original file)
		PMString newVersionID = Utils<IZPAssetUtils>()->NewGUID();
		
		//why taking asset id from file, when we have with assetRef
		const PMString & assetIDStr = dbAssetID->Get();// Utils<IZPAssetUtils>()->GetAssetIDFromLocalFile( inFile );

		PMString newHeadVersionId;
		toReturn = Utils<IZPAMWSUtils>()->UploadVersion_sync( assetIDStr, newVersionID, inFile, newHeadVersionId);
		if ( toReturn != kSuccess )
		{
			ErrorUtils::PMSetGlobalErrorCode(kZPWSDocumentUploadingError);
			break;
		}

		bool oldVersionUsed = (newVersionID.Compare(kFalse, newHeadVersionId) != 0);

		if( !oldVersionUsed )
			this->CreateExportAndUpload( document, assetIDStr, newVersionID, inFile);

		InterfacePtr<IZPAssetCheckInData> assetCheckInData( document, UseDefaultIID());
		ASSERT(assetCheckInData);
		
		const PMString statusID = assetCheckInData->GetStatusID();
		if( statusID.IsEmpty() )
		{
			CAlert::InformationAlert("Data not set.");
			break;
		}

		const PMString & checkInCommentsStr = assetCheckInData->GetDescription();

#pragma message("//TODO: pass old version number, so that server can compare it with latest/head.")
		//TODO:It should be last call, after uploading all data to server.
		toReturn = Utils<IZPAMWSUtils>()->CheckInAsset_sync( assetIDStr, newHeadVersionId, statusID, checkInCommentsStr, 0);
		if( toReturn != kSuccess )
		{
			ErrorUtils::PMSetGlobalErrorCode(kZPWSDocumentCheckinError);
			break;
		}

		//TODO: this must be done in AfterCheckIn service.
		this->DoAfterCheckIn_UpdateLocalInfo( dbAssetID );
		this->DoAfterCheckIn( assetCheckInData, dbAssetID );

		if(Utils<IZPInDnUtils>()->IsHostAppInCopy())	//No preview/pdf for InCopy document/templates
		{
			this->DoReleaseLock( dbAssetID );
			break;
		}

		//TODO: RJK, move to UI pln
		//bar.SetTaskText(kIZPProgressBarUpdatePagesKey);
		if( !oldVersionUsed )
			toReturn = Utils<IZPAMHelper>()->UpdatePagesOnServer( document, assetIDStr, newVersionID );

		
		//TODO: RJK, move to UI pln
		//bar.SetTaskText(kIZPProgressBarUpdatePdfKey);
		
		InterfacePtr<const IStringData> statusName( ZPDataHelper::GetTitleStatusByID( statusID ), IID_IZPDATANAME );
		ASSERT( statusName );
		PMString statusNameStr = statusName->Get();
		
		//Update PDF
		if( !oldVersionUsed )
		{
			toReturn = Utils<IZPAMHelper>()->UpdatePDFOnServer( document, assetIDStr, newVersionID, statusNameStr );
			if( toReturn != kSuccess )
			{
				ErrorUtils::PMSetGlobalErrorCode(kZPWSPDFUploadError);
				break;
			}
		}

		//this->DoReleaseLock( dbAsset );	//It will be release with document is closed.

		if(Utils<IZPInDnUtils>()->IsHostAppInCopy() == false)
			Utils<IZPAssetDocUtils>()->CheckAndUpdateUnAssignmentStoriesStatus( document );
	} while (kFalse);

	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetAssetFile
//----------------------------------------------------------------------------------------
bool
CZPAMDocumentCheckInService::GetAssetFile(
	const IManageableAsset *	inManageableAsset,
	IDFile &					oAssetFile) const
{
	return inheritClass::GetAssetFile( inManageableAsset, oAssetFile );
}

//----------------------------------------------------------------------------------------
// CreateExportAndUpload
//----------------------------------------------------------------------------------------
void
CZPAMDocumentCheckInService::CreateExportAndUpload(
	const IDocument *				inDoc,
	const PMString &				inAssetId,
	const PMString &				inVersionId,
	const IDFile &					inFile)
{
	LogFunctionEnterExit;
	do
	{
		if(Utils<IZPInDnUtils>()->IsHostAppInDesign() == false)		//Export xml and images only in indesign
			break;

		Utils<IZExpXMLWriteFacade> zpExpXMLWriteFacade;
		if (!zpExpXMLWriteFacade)
			break;

		PMString docName;
		inDoc->GetName(docName);

		const PMString xmlExt("xml");
		Utils<IZPFileUtils> fileUtils;
		IDFile filePath;
		fileUtils->GetParentDirectoryPath(inFile, filePath);
		fileUtils->AppendPath(filePath, docName, &xmlExt);

		zpExpXMLWriteFacade->ExportXMLToFile(filePath, inDoc);		//Export xml to file

		ErrorCode result = Utils<IZPAMWSUtils>()->UploadExport_sync( inAssetId, inVersionId, filePath, true, false, kNullString );
		if ( result == kSuccess )
		{
			IDFile lowResFile, highResFile;
			Utils<IZExpFileUtils> expFileUtils;
			expFileUtils->GetImageExportFileFolders( filePath, lowResFile, highResFile );

			if(this->UploadImageFilesInDirectory(lowResFile, inAssetId, inVersionId, false))
				this->UploadImageFilesInDirectory(highResFile, inAssetId, inVersionId, true);
		}
	}while(false);
}

//----------------------------------------------------------------------------------------
// UploadImageFilesInDirectory
//----------------------------------------------------------------------------------------
bool
CZPAMDocumentCheckInService::UploadImageFilesInDirectory(
	const IDFile &				inFileDirectory,
	const PMString &			inAssetID,
	const PMString &			inVersionID,
	const bool					inIsHighRes) const
{
	LogFunctionEnterExit;
	PlatformFileSystemIterator iter;
	if(!iter.IsDirectory(inFileDirectory))
		return false;

	iter.SetStartingPath(inFileDirectory);

	const PMString & folderName = inFileDirectory.GetFileName();

	IDFile tempIdFile;
	PMString filter("*.jpg");
	filter.InsertW(FileUtils::GetDirectorySeperator(), 0);
	IZPLog_Str( thisFileLA, enLT_DebugInfo, "File filter : %s", filter.GrabCString());

	const PMString thisDir(".");
	const PMString parentDir("..");
	bool16 hasNext = iter.FindFirstFile(tempIdFile, filter);
	while(hasNext) {
		PMString tempFileName = tempIdFile.GetFileName();
		if(tempFileName != thisDir && tempFileName != parentDir)
		{
#ifdef MACINTOSH
			PMString tempExtension;
			FileUtils::GetExtension(tempIdFile, tempExtension);
			if (tempExtension != "jpg")
			{
				hasNext= iter.FindNextFile(tempIdFile);
				continue;
			}
#endif

			ErrorCode result = Utils<IZPAMWSUtils>()->UploadExport_sync( inAssetID, inVersionID, tempIdFile, false, inIsHighRes, tempFileName );
			if (result != kSuccess)
				return false;
		}

		hasNext= iter.FindNextFile(tempIdFile);
	}

	return true;
}

#pragma mark -
CZPAMInCopyDocumentCheckInService::CZPAMInCopyDocumentCheckInService(IPMUnknown * boss)
: inheritClass_ICCheckIn(boss)
{
	IZPLog_ObjC( thisFileLA );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMInCopyDocumentCheckInService::~CZPAMInCopyDocumentCheckInService()
{
	IZPLog_ObjD( thisFileLA );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// Do
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMInCopyDocumentCheckInService::Do(
	IManageableAsset * asset )
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	if( !asset )
	{
		IDFile assetFile;
		bool hasFilePath = this->GetAssetFile( asset, assetFile );	// get the local file
		const IDocument * assetFileDoc = this->GetDocumentForFile( assetFile );
		InterfacePtr<IManageableAsset> manageableAsset( assetFileDoc , UseDefaultIID() );
		asset = manageableAsset;
	}
	ErrorCode status = inheritClass_ICCheckIn::Do(asset);
	if (status == kSuccess)
	{
		//Patch: IC document service handles only till the first check-in of a new IC document.
		//Init the AM again as if IC document was opened.
		const IDocument * assetDoc = nil;
		if(asset)
		{
			InterfacePtr<IDocument> theDoc( asset, UseDefaultIID() );
			assetDoc = theDoc;
		}
		
		UIDRef docRef = ::GetUIDRef(asset);
#if 0
		Utils<Facade::IWorkgroupFacade>()->TerminateAssetManagement(docRef);
		Utils<Facade::IWorkgroupFacade>()->Revert( docRef );
#elif 1
		Utils<Facade::IWorkgroupFacade>()->TerminateAssetManagement(docRef);
		InterfacePtr<IAMSPManager> iAMSPManager( gSession, UseDefaultIID() );
		ASSERT( iAMSPManager );
		
		iAMSPManager->UnbindAsset( asset );

#endif
	}

	return status;
}

//----------------------------------------------------------------------------------------
// VerifyContext
//----------------------------------------------------------------------------------------
bool16
CZPAMInCopyDocumentCheckInService::VerifyContext(
	IManageableAsset * pAsset )
{
	LogFunctionEnterExit;
	this->LogManageableAssetInfo( pAsset );

	bool16 toReturn = kFalse;
	
	do
	{
		if ( !pAsset )
			break;
		InterfacePtr<IDocument> assetDoc( pAsset, UseDefaultIID() );
		if ( assetDoc )	//Also comes for DataLink if not handled by Linked Story services.
		{
			bool16 isNewDocument = !assetDoc->IsSaved();
			InterfacePtr<const IBoolData> isTemplateCopy( assetDoc, IID_IZPISTEMPLATECOPY );
			ASSERT(isTemplateCopy);
			if( !isNewDocument )
				isNewDocument = isTemplateCopy->Get();

			toReturn = kTrue;
			if( isNewDocument )
				break;
		}

		const IStringData *	dbAssetID = this->GetDBAsset( pAsset );

		//check if this lock is acquired by this user & from this app.
		if( dbAssetID && this->IsAssetLockedByCurrentUser( dbAssetID ) )
		{
			IDFile assetFile;
			Utils<IZPAMHelper>()->GetAssetFilePath( dbAssetID, assetFile, true);
			//check if this file locally modified or not.
			//Check-In only if file is locally modified & don't conflict.
			IManagedStatus::VersionState fileState = Utils<IZPAMHelper>()->GetAssetFileVersionState( dbAssetID, assetFile );
			if( fileState == IManagedStatus::enLocalIsNewer )
				toReturn = kTrue;
			else if( fileState == IManagedStatus::enUpToDate )
			{	//Enable if the document has unsaved changed.
				InterfacePtr<IDocument> assetDoc( pAsset, UseDefaultIID() );
				if( assetDoc )
					toReturn = assetDoc->IsModified( );
				else
					toReturn = kTrue;
			}
		}

	}while(kFalse);

	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " VerifyContext = %hd", toReturn);
	return toReturn;
}

//----------------------------------------------------------------------------------------
// CheckInAllAssignment
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMDocumentCheckInService::CheckInAllAssignment(
	IDocument *					inDoc,
	bool						inOnlyDirtyAssignment) const
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	ErrorCode result = kFailure;
	if (Utils<IZPInDnUtils>()->IsHostAppInCopy())
		return result;

	if (!inDoc)
		return result;

	if (inOnlyDirtyAssignment == false)
		Utils<IZPDocUtils>()->AllAssignmentsSetDirty(inDoc, true);

	InterfacePtr<IAssignmentMgr> assignmentMgr(gSession, UseDefaultIID());
	ASSERT(assignmentMgr);
	result = assignmentMgr->SaveAssignments(inDoc);

	return result;
}
