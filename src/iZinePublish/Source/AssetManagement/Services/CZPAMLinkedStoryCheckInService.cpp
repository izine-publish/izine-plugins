//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/Services/CZPAMLinkedStoryCheckInService.cpp $
//	$Revision: 4062 $
//	$Date: 2012-09-18 16:26:22 +0200 (Tue, 18 Sep 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 2-7-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IAMServiceData.h"
#include "IAssignedStory.h"
#include "IAssignment.h"
#include "IBoolData.h"
#include "ISaveAssetContent.h"
#include "IStandAloneDoc.h"
#include "IStoryLabel.h"
#include "IStringData.h"
#include "IWorkgroupStoryFacade.h"
#include "IWorkgroupUIFacade.h"

//SDK General includes
#include "ErrorUtils.h"
#include "Utils.h"
#include "CAlert.h"	//TODO: move to UI pln

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPAMHelper.h"
#include "IZPAMWSUtils.h"
#include "IZPAssetCheckInData.h"
#include "IZPAssetInfo.h"
#include "IZPAssetUtils.h"
#include "IZPInDnUtils.h"
#include "IZPStoryUtils.h"

//IZP General includes
#include "AZPAMService.h"
#include "StInDesignUtils.h"
#include "ZPConstants.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AMService
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass AZPAMService
class CZPAMLinkedStoryCheckInService : public inheritClass
{
public:
						CZPAMLinkedStoryCheckInService(IPMUnknown * boss);
	virtual				~CZPAMLinkedStoryCheckInService();

	ErrorCode			Do(
							IManageableAsset *			pAsset);
	bool16				VerifyContext(
							IManageableAsset *			pAsset);
protected:
	bool				GetAssetFile(
							const IManageableAsset *	inManageableAsset,
							IDFile &					oAssetFile) const;
	ErrorCode			ValidateCheckInData(
							IManageableAsset *			pAsset );
	ErrorCode			CheckInModelChanges(
							IManageableAsset *			pAsset,
							const IDFile &				inAssetFile );
private:
};


CREATE_PMINTERFACE(CZPAMLinkedStoryCheckInService, kZPAMLinkedStoryCheckInServiceImpl)

CZPAMLinkedStoryCheckInService::CZPAMLinkedStoryCheckInService(IPMUnknown * boss)
: inheritClass(boss)
{
	IZPLog_ObjC( thisFileLA );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMLinkedStoryCheckInService::~CZPAMLinkedStoryCheckInService()
{
	IZPLog_ObjD( thisFileLA );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// Do
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMLinkedStoryCheckInService::Do(
	IManageableAsset * pAsset)
{
	LogFunctionEnterExit;
	this->LogManageableAssetInfo( pAsset );

	ErrorCode toReturn = kFailure;
	
	IDFile assetFile;
	
	do
	{
		if( this->GetAssetFile( pAsset, assetFile ) )
			toReturn = this->ValidateCheckInData( pAsset );

		if( toReturn ==  kSuccess )
		{
			// check it in to our system
			//UIDRef assetRef = ::GetUIDRef( pAsset );
			toReturn = this->CheckInModelChanges( pAsset, assetFile );
		}

		if ( toReturn == kCancel )
			ErrorUtils::PMSetGlobalErrorCode( kCancel );
		else if ( toReturn != kSuccess )
			ErrorUtils::PMSetGlobalErrorCode( kFailure );
	}while( false );

	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " service DO status = %d", toReturn);
	
	return toReturn;
}

//----------------------------------------------------------------------------------------
// VerifyContext
//----------------------------------------------------------------------------------------
bool16
CZPAMLinkedStoryCheckInService::VerifyContext(
	IManageableAsset * pAsset)
{
	LogFunctionEnterExit;
	this->LogManageableAssetInfo( pAsset );

	bool16 toReturn = kFalse;
	do
	{
		if( !this->IsLinkedStoryAsset( pAsset ) )
			break;
		
		const IStringData *	dbAssetID = this->GetDBAsset_FromIDStory( pAsset );

		//check if this lock is acquired by this user & from this app.
		if( dbAssetID && this->IsAssetLockedByCurrentUser( dbAssetID ) )
		{
			IDFile assetFile;
			if( !this->GetAssetFile( pAsset, assetFile ) )
				Utils<IZPAMHelper>()->GetAssetFilePath( dbAssetID, assetFile );
			//check if this file locally modified or not.
			//Check-In only if file is locally modified & don't conflict.
			IManagedStatus::VersionState fileState = Utils<IZPAMHelper>()->GetAssetFileVersionState( dbAssetID, assetFile );
			if( fileState == IManagedStatus::enLocalIsNewer )
				toReturn = kTrue;
			else if( fileState == IManagedStatus::enUpToDate )
			{	//Enable if the document has unsaved changed.
				InterfacePtr<IAssignedStory> assignedStory( this->GetAssignedStory( pAsset ), UseDefaultIID() );
				//ASSERT( assignedStory );
				//toReturn = assignedStory->IsDirty( );
				toReturn = kTrue;
			}
		}
		if( dbAssetID && toReturn == kFalse)	//TODO: remove this when assignedStory is found
		{
			InterfacePtr<const IZPAssetInfo> dbAssetInfo( dbAssetID, UseDefaultIID() );
			ASSERT( dbAssetInfo );
			if( dbAssetInfo->GetVersionNumber() == 0 )
				toReturn = kTrue;
		}
	}while(kFalse);

	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " VerifyContext = %hd", toReturn);
	return toReturn;
}

//----------------------------------------------------------------------------------------
// ValidateCheckInData
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMLinkedStoryCheckInService::ValidateCheckInData(
	IManageableAsset *			pAsset)
{
	LogFunctionEnterExit;
	ErrorCode toReturn = kSuccess;
	InterfacePtr<const IZPAssetCheckInData> assetCheckInData( pAsset, UseDefaultIID());
	ASSERT(assetCheckInData);
	
	const PMString & statusID = assetCheckInData->GetStatusID();
	if( statusID.IsEmpty() )
	{
		PMString checkInComments;
		UIDRef assetRef = ::GetUIDRef( pAsset );

#if 0
		IAMUIService::FollowupAction followupAction = Utils<IWorkgroupUIFacade>()->DoGetVersionComments( assetRef, checkInComments );
#else
		IAMUIService::FollowupAction followupAction = IAMUIService::enSave;
#if 0
		//IC document after first check-in is treated as Linked story.
		//Also for check-in comments.
		if( this->IsInCopyDocument( pAsset ) || this->IsInCopyTemplate( pAsset ) )
			followupAction = Utils<IWorkgroupUIFacade>()->DoGetVersionComments( assetRef, checkInComments );
		else
			this->CopyParentCheckInData( pAsset );	//Copy Assignment or Document status ID.
#else
		//Now Robin wants "not to show check-in dialog for story assets (IC/ID story or IC tempate)".
		//Redmine #1838, 2011May26
		//So copy the check-in data from self.
		if( false == this->FillCheckInDataFromSelf( pAsset ))
		{	//This case is for new ID story asset., set the status to assigned.
			InterfacePtr<IZPAssetCheckInData> assetCheckInData( pAsset, UseDefaultIID());
			ASSERT(assetCheckInData);
			assetCheckInData->SetStatusID( kTitleStatus_Assigned );
		}
#endif
#endif
		
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
CZPAMLinkedStoryCheckInService::CheckInModelChanges(
	IManageableAsset*			pAsset,
	const IDFile&				inFile )
{
	LogFunctionEnterExit;

	ErrorCode toReturn = kFailure;
	
	do {
	#if DEBUG
		this->LogFilePath( inFile );
		IZPLog_Str_( thisFileLA, enLT_Details, "UID = 0x%08X, ", ::GetUID( pAsset ).Get() );
	#endif
		
		InterfacePtr<IAMServiceData> selfServiceData( this, UseDefaultIID() );
		ASSERT( selfServiceData );
		
		const IStringData *	dbAssetID = this->GetDBAsset_FromIDStory( pAsset );
		ASSERT( dbAssetID );
		if( !dbAssetID )
			break;

		// get some useful flags
		UIFlags uiFlags;
		selfServiceData->Get( IAMService::enUIFlags, uiFlags );
		
		int32 forceSave;
		selfServiceData->Get( IAMService::enForceSave, forceSave);	//Means override check-in without updating to head.

		PMString username;
		selfServiceData->Get( IAMService::enCheckOutToUserName, username);
		if( !username.IsEmpty() )
			IZPLog_Str_( thisFileLA, enLT_Error, "Check-in comments: %s", username.GrabCString() );

		PMString comment;
		selfServiceData->Get( IAMService::enCheckInComment, comment);
		if( !comment.IsEmpty() )
			IZPLog_Str_( thisFileLA, enLT_Error, "Check-in comments: %s", comment.GrabCString() );

		bool16 allowOverride = selfServiceData && selfServiceData->GetFlag( IAMService::enOverrideAuthority );

		//TODO: RJK, move to UI pln.
		//SubTaskFoldupProgressBar bar(kIZPProgressBarUpdateDocumentKey, 1, 4);
	
		//TODO: RJK, move to UI pln
		//bar.SetTaskText(kIZPProgressBarUploadDocumentKey);

		UIDRef assetRef = ::GetUIDRef( pAsset );
		ISaveAssetContent* saveAssetContent = static_cast<ISaveAssetContent*>( selfServiceData->Get( IAMService::enContentSaver ) );
		
		bool isInCopy = Utils<IZPInDnUtils>()->IsHostAppInCopy();
		InterfacePtr<const IAssignment> storyParentAssignment( Utils<IZPStoryUtils>()->QueryAssignmentForStoryAsset( pAsset ) );
		
		IDataBase * db = ::GetDataBase( pAsset );
		UIDRef docRef( db, db->GetRootUID() );
		InterfacePtr<IStandAloneDoc> standAlone( docRef, IID_ISTANDALONEDOC);
		bool bWasStandAloneDoc = false;
		if( standAlone )
			bWasStandAloneDoc = standAlone->IsStandAloneDoc();
		PMString asgnFilePath;
		if( storyParentAssignment )
			asgnFilePath = ((IAssignment*)storyParentAssignment.get())->GetFile();
		if( isInCopy && saveAssetContent && !bWasStandAloneDoc && (!storyParentAssignment || asgnFilePath.IsEmpty() ))
			standAlone->SetIsStandAloneDoc( kTrue );		//Patch: keep unused styles with InCopy Document.

		if ( saveAssetContent && saveAssetContent->Do( assetRef, inFile ) != kSuccess )
		{
			// failed to write story out to local file
			IZPLog_Str_( thisFileLA, enLT_Error, "** IAMService::enContentSaver FAILED**\n");
			ErrorUtils::PMSetGlobalErrorCode(kZPLinkedStoryWritingError);
			break;
		}
		
		{
			StRetainModifiedFlag dontModifyDB( db );
			if( isInCopy && saveAssetContent && !bWasStandAloneDoc && (!storyParentAssignment || asgnFilePath.IsEmpty() ))
				standAlone->SetIsStandAloneDoc( kFalse );	//Patch: restore flag chagned above
		}

		// get the unique key for the link (which is actually just the path to the original file)
		PMString newVersionID = Utils<IZPAssetUtils>()->NewGUID();
		
		//why taking asset id from file, when we have with assetRef.
		const PMString & assetIDStr = dbAssetID->Get();//Utils<IZPAssetUtils>()->GetAssetIDFromLocalFile( inFile );

		PMString newHeadVersionId;
		toReturn = Utils<IZPAMWSUtils>()->UploadVersion_sync( assetIDStr, newVersionID, inFile, newHeadVersionId );
		if ( toReturn != kSuccess )
		{
			CAlert::InformationAlert(kZPWSAssignmentUploadingErrorKey);		//Setting error code will crash app.
			break;
		}

		InterfacePtr<IZPAssetCheckInData> assetCheckInData( pAsset, UseDefaultIID());
		ASSERT(assetCheckInData);
		
		const PMString statusID = assetCheckInData->GetStatusID();
		if( statusID.IsEmpty() )
		{
			CAlert::InformationAlert("Data not set.");
			break;
		}

		const PMString & assetName = assetCheckInData->GetAssetName();
		if( assetName.IsEmpty() )
		{	//Set the story name if this is empty.
			UIDRef storyRef = Utils<Facade::IWorkgroupStoryFacade>()->GetStoryUIDRef( assetRef );
			InterfacePtr<IStoryLabel> storyLabel( storyRef, IID_ISTORYLABEL);
			if( storyLabel )
			{
				PMString storyNewLabel = storyLabel->GetLabel();
				if( dbAssetID->Get().Compare( false, storyNewLabel ) != 0 )	//Don't set Asset name as Asset ID.
					assetCheckInData->SetAssetName(storyNewLabel);
			}
		}

		const PMString & checkInCommentsStr = assetCheckInData->GetDescription();

#pragma message("//TODO: pass old version number, so that server can compare it with latest/head.")
		toReturn = Utils<IZPAMWSUtils>()->CheckInAsset_sync( assetIDStr, newHeadVersionId, statusID, checkInCommentsStr, 0);
		if( toReturn != kSuccess )
		{
			CAlert::ErrorAlert(kZPWSDocumentCheckinErrorKey);		//Setting error code will crash app.
			break;
		}

		//TODO: this must be done in AfterCheckIn service.
		this->DoAfterCheckIn_UpdateLocalInfo( dbAssetID );
		this->DoAfterCheckIn( assetCheckInData, dbAssetID );

		this->DoReleaseLock( dbAssetID );	//It will be release with document is closed.

		//Update assignment, some ID story check-in made.
		InterfacePtr<IBoolData> storyCheckInMade( storyParentAssignment, IID_IZPSTORYCHECKINMADE);
		if( storyCheckInMade )
			storyCheckInMade->Set( kTrue );
	} while (kFalse);

	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetAssetFile
//----------------------------------------------------------------------------------------
bool
CZPAMLinkedStoryCheckInService::GetAssetFile(
	const IManageableAsset *	inManageableAsset,
	IDFile &					oAssetFile) const
{
	//LogFunctionEnterExit;
	return inheritClass::GetIDStoryFile( inManageableAsset, oAssetFile );
}
