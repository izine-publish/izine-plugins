//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/Services/CZPAMAssignmentCheckInService.cpp $
//	$Revision: 4060 $
//	$Date: 2012-09-18 16:21:23 +0200 (Tue, 18 Sep 2012) $
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
#include "IAMService.h"
#include "IAMServiceData.h"
#include "IAssignment.h"
#include "IManageableAsset.h"
#include "ISaveAssetContent.h"
#include "IStringData.h"
#include "IWorkgroupUIFacade.h"

//SDK General includes
#include "ErrorUtils.h"
#include "Utils.h"
#include "CAlert.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPAMHelper.h"
#include "IZPAMWSUtils.h"
#include "IZPAssetCheckInData.h"
#include "IZPAssetMgr.h"
#include "IZPAssetUtils.h"
#include "IZPInDnUtils.h"
#include "IZPAssetDocUtils.h"

//IZP General includes
#include "AZPAMService.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AMService
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass AZPAMService
class CZPAMAssignmentCheckInService : public inheritClass
{
public:
						CZPAMAssignmentCheckInService(IPMUnknown * boss);
	virtual				~CZPAMAssignmentCheckInService();

	ErrorCode			Do(
							IManageableAsset * pAsset);
	bool16				VerifyContext(
							IManageableAsset * pAsset);
protected:
	bool				GetAssetFile(
							const IManageableAsset *	inManageableAsset,
							IDFile &					oAssetFile) const;
	ErrorCode			ValidateCheckInData(
							IAssignment *			asset,
							const IDFile &			inAssetFile);
	ErrorCode			CheckInModelChanges(
							IAssignment *			asset,
							const IDFile &			file );
private:
};


CREATE_PMINTERFACE(CZPAMAssignmentCheckInService, kZPAMAssignmentCheckInServiceImpl)

CZPAMAssignmentCheckInService::CZPAMAssignmentCheckInService(IPMUnknown * boss)
: inheritClass(boss)
{
	IZPLog_ObjC( thisFileLA );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMAssignmentCheckInService::~CZPAMAssignmentCheckInService()
{
	IZPLog_ObjD( thisFileLA );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// Do
//	pAsset comes nil for the first check-in.
//	
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMAssignmentCheckInService::Do(
	IManageableAsset * pAsset)
{
	LogFunctionEnterExit;
	this->LogManageableAssetInfo( pAsset );

	ErrorCode toReturn = kFailure;
	
	InterfacePtr<IAMServiceData> serviceData( this, UseDefaultIID() );
	ASSERT(serviceData);
	
	//Someday: iterate on all the data set with serviceData.

	IDFile assetFile;
	
	bool hasFilePath = this->GetAssetFile( pAsset, assetFile );	// get the local file
	
	do
	{
		//If inAsset is nil, then find it from front open doc.
		//if( !pAsset )							//RJK: commented as data link is not created till it is check-in.
		//	pAsset = this->GetAssignmentAssetForFile( assetFile );

		InterfacePtr<IAssignment> assignment( this->GetAssignmentForFile(assetFile), UseDefaultIID() );
		ASSERT( assignment );
		if (!assignment)
			break;

		if( hasFilePath )
			toReturn = this->ValidateCheckInData( assignment, assetFile );

		if( toReturn ==  kSuccess )
		{
			// check it in to our system
			//UIDRef assetRef = ::GetUIDRef( pAsset );
			toReturn = this->CheckInModelChanges( assignment, assetFile );
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
CZPAMAssignmentCheckInService::VerifyContext(
	IManageableAsset * pAsset)
{
	LogFunctionEnterExit;
	this->LogManageableAssetInfo( pAsset );

	bool16 toReturn = kFalse;
	
	do {
		if ( ! pAsset )
			break;
			
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "VerifyContext asset UID = 0x%08X",::GetUID(pAsset).Get());
		
		const IStringData *	dbAsset = this->GetDBAsset_FromAsgmt( pAsset );

		//check if this lock is acquired by this user & from this app.
		if( dbAsset && this->IsAssetLockedByCurrentUser( dbAsset ) )
		{
			IDFile assetFile;
			Utils<IZPAMHelper>()->GetAssetFilePath( dbAsset, assetFile );
			//check if this file locally modified or not.
			//Check-In only if file is locally modified & don't conflict.
			IManagedStatus::VersionState fileState = Utils<IZPAMHelper>()->GetAssetFileVersionState( dbAsset, assetFile );
			if( fileState == IManagedStatus::enLocalIsNewer )
				toReturn = kTrue;
			else if( fileState == IManagedStatus::enUpToDate )
			{	//Enable if the document has unsaved changed.
				InterfacePtr<IAssignment> assignment( this->GetAssignment( pAsset ), UseDefaultIID() );
				ASSERT( assignment );
				toReturn = assignment->GetDirty( );
			}
		}
	} while (kFalse);

	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " VerifyContext = %hd", toReturn);
	return toReturn;
}

//----------------------------------------------------------------------------------------
// ValidateCheckInData
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMAssignmentCheckInService::ValidateCheckInData(
	IAssignment *			asset,
	const IDFile &			inAssetFile)
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

#if 0
		IAMUIService::FollowupAction followupAction = Utils<IWorkgroupUIFacade>()->DoGetVersionComments( assetRef, checkInComments );
#else
		bool valueCopied = false;
		IAMUIService::FollowupAction followupAction = IAMUIService::enSave;
		//Copy check-in data from DB asset, so that there is no change in status and asset name.
		//This is done for InDesign when the status state is not design.
		if( !Utils<IZPInDnUtils>()->IsHostAppInCopy() )
		{	
			//Commented as Remco now wants to ask for check-in comments always, irrespective of state.
			//const IStringData *	dbAssetID = this->GetDBAsset_FromAsgmt( asset );
			//if( IZPAssetMgr::GetAssetState( dbAssetID ) != eTitleStatusState_Design )
				//2011May31: enabled for all cases, as Robin does not want it to show during update assignment.
				valueCopied = this->FillCheckInDataFromSelf( asset );
		}
		
		if( !valueCopied )
		{
			//this->CopyParentCheckInData( asset );
			//or
			//followupAction = Utils<IWorkgroupUIFacade>()->DoGetVersionComments( assetRef, checkInComments );
			followupAction = Utils<IZPAMHelper>()->AskVersionComments_Assignment( inAssetFile, assetRef, checkInComments );
		}
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
CZPAMAssignmentCheckInService::CheckInModelChanges(
	IAssignment* inAssignment, const IDFile& inFile )
{
	LogFunctionEnterExit;

	ErrorCode toReturn = kFailure;
	
	do {
	#if DEBUG
		this->LogFilePath( inFile );
		IZPLog_Str_( thisFileLA, enLT_Details, "UID = 0x%08X, ", ::GetUID( inAssignment ).Get() );
	#endif
		
		InterfacePtr<IAMServiceData> selfServiceData( this, UseDefaultIID() );
		ASSERT( selfServiceData );
		
		const IStringData *	dbAssetID = this->GetDBAsset_FromAsgmt( inAssignment );
		ASSERT( dbAssetID );

		if( dbAssetID && !this->IsAssetLocked( dbAssetID ) )	//Patch: Check-Out is not made on assignment, so acquire lock now.
			toReturn = this->DoAcquireLock( dbAssetID );

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
	
		//TODO: RJK, move to UI pln.
		//bar.SetTaskText(kIZPProgressBarUploadDocumentKey);

		ISaveAssetContent* saveAssetContent = static_cast<ISaveAssetContent*>( selfServiceData->Get( IAMService::enContentSaver ) );
		//////if ( saveAssetContent && saveAssetContent->Do( assetRef, inFile ) != kSuccess )
		//////{
		//////	// failed to write story out to local file
		//////	IZPLog_Str_( thisFileLA, enLT_Error, "** IAMService::enContentSaver FAILED**\n");
		//////	CAlert::InformationAlert("Error during writing assignment.");
		//////	break;
		//////}
		
		// get the unique key for the link (which is actually just the path to the original file)
		PMString newVersionID = Utils<IZPAssetUtils>()->NewGUID();
		
		//why taking asset id from file, when we have with assetRef
		const PMString & assetIDStr = dbAssetID->Get();//Utils<IZPAssetUtils>()->GetAssetIDFromLocalFile( inFile );

		PMString newHeadVersionId;
		toReturn = Utils<IZPAMWSUtils>()->UploadVersion_sync( assetIDStr, newVersionID, inFile, newHeadVersionId);
		if ( toReturn != kSuccess )
		{
			CAlert::InformationAlert(kZPWSAssignmentUploadingErrorKey);		//Setting error code will crash app.
			break;
		}

		InterfacePtr<IZPAssetCheckInData> assetCheckInData( inAssignment, UseDefaultIID());
		ASSERT(assetCheckInData);
		
		const PMString & statusID = assetCheckInData->GetStatusID();
		if( statusID.IsEmpty() )
		{
			CAlert::InformationAlert("Data not set.");
			break;
		}

		const PMString & checkInCommentsStr = assetCheckInData->GetDescription();

#pragma message("//TODO: pass old version number, so that server can compare it with latest/head.")
		toReturn = Utils<IZPAMWSUtils>()->CheckInAsset_sync( assetIDStr, newHeadVersionId, statusID, checkInCommentsStr, 0);
		if( toReturn != kSuccess )
		{
			CAlert::ErrorAlert(kZPWSDocumentCheckinErrorKey);		//Setting error code will crash app.
			break;
		}

		this->DoAfterCheckIn_UpdateLocalInfo( dbAssetID );
		this->DoAfterCheckIn( assetCheckInData, dbAssetID );	//Remove CheckIn-Data

		this->DoReleaseLock( dbAssetID );

		Utils<IZPAssetDocUtils>()->CheckAndUpdateAssignmentStoriesStatus( inAssignment );
	} while (kFalse);

	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetAssetFile
//----------------------------------------------------------------------------------------
bool
CZPAMAssignmentCheckInService::GetAssetFile(
	const IManageableAsset *	inManageableAsset,
	IDFile &					oAssetFile) const
{
	return inheritClass::GetAssetFile( inManageableAsset, oAssetFile );
}
