//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/Services/AZPAMService.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
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
#include "IAMSPManager.h"
#include "IAssetMonitor.h"
#include "IAssignedStory.h"
#include "IAssignment.h"
#include "IDataLink.h"
#include "IDocument.h"
#include "ILayoutUIUtils.h"		//TODO: will not work on ID server.
#include "ILinkObjectReference.h"
#include "IManageableAsset.h"
#include "IStringData.h"
#include "ITextModel.h"
#include "IWorkgroupStoryFacade.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes
#include "NameInfo.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPAMHelper.h"
#include "IZPAMWSUtils.h"
#include "IZPAssetCheckInData.h"
#include "IZPAssetData.h"
#include "IZPAssetDocUtils.h"
#include "IZPAssetInfo.h"
#include "IZPAssetLocalInfo.h"
#include "IZPAssetsLocalInfoMgr.h"
#include "IZPAssetUtils.h"
#include "IZPCommandFacade.h"
#include "IZPDocUtils.h"
#include "IZPFileUtils.h"
#include "IZPInDnUtils.h"
#include "IZPStoryUtils.h"

//IZP General includes
#include "AZPAMService.h"
#include "ZPDataHelper.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AMService
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#pragma mark -
//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
AZPAMService::AZPAMService(IPMUnknown * boss)
: inheritClass_ZPAMService(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
AZPAMService::~AZPAMService()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetDBAsset
//----------------------------------------------------------------------------------------
const IStringData *
AZPAMService::GetDBAsset(
	const IManageableAsset *	inAsset) const
{
	//LogFunctionEnterExit;
	const IStringData * toReturn = nil;
	ASSERT( ::GetUID( inAsset ).Get() == ::GetDataBase(inAsset)->GetRootUID()
		|| this->IsLinkedStoryAsset(inAsset) );

	InterfacePtr<const IZPAssetData> docAssetData( inAsset, UseDefaultIID() );
	ASSERT( docAssetData );

	const PMString & assetIDStr = docAssetData->GetAssetID();
	const IStringData * docAssetID = nil;
	if( assetIDStr.IsEmpty() == kFalse )
		docAssetID = ZPDataHelper::GetAssetByID( assetIDStr );
	
	toReturn = docAssetID;

	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetDBAsset_FromDoc
//----------------------------------------------------------------------------------------
const IStringData *
AZPAMService::GetDBAsset_FromDoc(
	const IManageableAsset *	inAsset) const
{
	//LogFunctionEnterExit;
	const IStringData * toReturn = nil;
	ASSERT( ::GetUID( inAsset ).Get() == ::GetDataBase(inAsset)->GetRootUID() );

	InterfacePtr<const IZPAssetData> docAssetData( inAsset, UseDefaultIID() );
	ASSERT( docAssetData );

	const PMString & assetIDStr = docAssetData->GetAssetID();
	const IStringData * docAssetID = nil;
	if( assetIDStr.IsEmpty() == kFalse )
		docAssetID = ZPDataHelper::GetAssetByID( assetIDStr );
	
	toReturn = docAssetID;

	return toReturn;
}
//----------------------------------------------------------------------------------------
// GetDBAsset_FromAsgmt
// Warning: Must not be used during check-in service DO.
//----------------------------------------------------------------------------------------
const IStringData *
AZPAMService::GetDBAsset_FromAsgmt(
	const IManageableAsset *	inAsset) const
{
	//LogFunctionEnterExit;
	const IStringData * toReturn = nil;
	
	//We are storing asset data with assignment boss.
	// So get the assignment from the datalink.
	InterfacePtr<const IAssignment> assignment( this->GetAssignment( inAsset ), UseDefaultIID() );
	ASSERT( assignment );
	toReturn = this->GetDBAsset_FromAsgmt( assignment );
	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetDBAsset_FromAsgmt
//----------------------------------------------------------------------------------------
const IStringData *
AZPAMService::GetDBAsset_FromAsgmt(
	const IAssignment *			inAsset) const
{
	//LogFunctionEnterExit;
	const IStringData * toReturn = nil;

	do
	{
		if( !inAsset )
			break;
		InterfacePtr<const IZPAssetData> assignmentAssetData( inAsset, UseDefaultIID() );
		ASSERT( assignmentAssetData );

		const PMString & assetIDStr = assignmentAssetData->GetAssetID();
		const IStringData * assignmentAssetID = nil;
		if( assetIDStr.IsEmpty() == kFalse )
			assignmentAssetID = ZPDataHelper::GetAssetByID( assetIDStr );
		
		toReturn = assignmentAssetID;

	}while(kFalse);

	return toReturn;
}
//----------------------------------------------------------------------------------------
// GetDBAsset_FromIDStory
//----------------------------------------------------------------------------------------
const IStringData *
AZPAMService::GetDBAsset_FromIDStory(
	const IManageableAsset *	inAsset) const
{
	//LogFunctionEnterExit;
	const IStringData * toReturn = nil;

#ifdef DEBUG
	ASSERT( inAsset );
	//Linked story ref may not be retrievable if the link is not yet created
	//ASSERT( this->IsLinkedStoryAsset( inAsset ) );
#endif

	InterfacePtr<const IZPAssetData> idStoryAssetData( inAsset, UseDefaultIID() );
	ASSERT( idStoryAssetData );

	const PMString & assetIDStr = idStoryAssetData->GetAssetID();
	const IStringData * assetID = nil;
	if( assetIDStr.IsEmpty() == kFalse )
		assetID = ZPDataHelper::GetAssetByID( assetIDStr );
	
	toReturn = assetID;

	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetDBAssetInfo_FromDoc
//----------------------------------------------------------------------------------------
const IZPAssetInfo *
AZPAMService::GetDBAssetInfo_FromDoc(
	const IManageableAsset *	inAsset) const
{
	//LogFunctionEnterExit;
	const IZPAssetInfo * toReturn = nil;
	const IStringData * docAssetID = this->GetDBAsset_FromDoc( inAsset );
	
	InterfacePtr<const IZPAssetInfo> dbAssetInfo( docAssetID, UseDefaultIID() );
	ASSERT( dbAssetInfo );
	toReturn = dbAssetInfo;

	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetDocumentForFile
//----------------------------------------------------------------------------------------
const IDocument *
AZPAMService::GetDocumentForFile(
	const IDFile &				inDocAssetFile) const
{
	//LogFunctionEnterExit;
	PMString assetID = Utils<IZPAssetUtils>()->GetAssetIDFromLocalFile( inDocAssetFile );
	ASSERT( assetID.IsEmpty() == kFalse );

	const IDocument * toReturn = Utils<IZPAssetDocUtils>()->GetDocumentForDocAssetID( assetID );
	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetAssignmentForFile
//----------------------------------------------------------------------------------------
IAssignment *
AZPAMService::GetAssignmentForFile(
	const IDFile &				inAssignmentAssetFile) const
{
	//LogFunctionEnterExit;
	//Note: Depends on front document, will not work on ID server.
	IAssignment * toReturn = nil;
	do{
		InterfacePtr<IDocument> iFrontDoc(Utils<ILayoutUIUtils>()->GetFrontDocument(), UseDefaultIID());
		if(!iFrontDoc)
			break;

		InterfacePtr<IAssignment> iAssignment( Utils<IZPDocUtils>()->QueryAssignmentForFilePath( iFrontDoc, inAssignmentAssetFile) );
		if(!iAssignment)
			break;

		toReturn = iAssignment;

	}while( false );
	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetAssignmentAssetForFile
//----------------------------------------------------------------------------------------
IManageableAsset *
AZPAMService::GetAssignmentAssetForFile(
	const IDFile &				inAssignmentAssetFile) const
{
	//LogFunctionEnterExit;
	//Not being used.
	//Note: Depends on front document, will not work on ID server.
	IManageableAsset * toReturn = nil;
	do{
		InterfacePtr<IDocument> iFrontDoc(Utils<ILayoutUIUtils>()->GetFrontDocument(), UseDefaultIID());
		if(!iFrontDoc)
			break;

		InterfacePtr<IAssignment> iAssignment( this->GetAssignmentAssetForFile( inAssignmentAssetFile ), UseDefaultIID() );
		if(!iAssignment)
			break;

		InterfacePtr<IManageableAsset> manageableAsset( Utils<IZPDocUtils>()->QueryManageableAssetForAssignment( iFrontDoc, iAssignment ) );
		ASSERT( manageableAsset );
		toReturn = manageableAsset;

	}while( false );
	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetAssignment
//----------------------------------------------------------------------------------------
IAssignment *
AZPAMService::GetAssignment(
	const IManageableAsset *	inManageableAsset) const
{
	//LogFunctionEnterExit;
	do
	{
		if( !inManageableAsset )
			break;

		IDataBase * assetDB = ::GetDataBase( inManageableAsset );
		
		if( Utils<IZPInDnUtils>()->IsHostAppInCopy()	//In InCopy document is assignment.
		&&	::GetUID( inManageableAsset ) == assetDB->GetRootUID() )
		{
			//Check if this document is an assignment file.
			InterfacePtr<IDocument> assginmentDoc(inManageableAsset, UseDefaultIID());
			ASSERT(assginmentDoc);

			InterfacePtr<IAssignment> iAssignment( Utils<IZPDocUtils>()->QueryAssignmentFromAssignmentDoc( assginmentDoc ) );
			return iAssignment;
		}
		else
		{
			InterfacePtr<const IDocument> theDoc( assetDB, assetDB->GetRootUID(), UseDefaultIID() );
			ASSERT( theDoc );

			InterfacePtr<IAssignment> assignment( Utils<IZPDocUtils>()->QueryAssignmentForAsset( theDoc, inManageableAsset ), UseDefaultIID() );
			ASSERT( assignment );
			return assignment;
		}

	}while(kFalse);

	return nil;
}

//----------------------------------------------------------------------------------------
// GetAssignedStory
//----------------------------------------------------------------------------------------
IAssignedStory *
AZPAMService::GetAssignedStory(
	const IManageableAsset *	inManageableAsset) const
{
	//LogFunctionEnterExit;
	return nil;	//TODO:
}

#if defined(InDnCS5) || defined(InDnCS5_5)
//----------------------------------------------------------------------------------------
// GetServiceDataModelObject
//----------------------------------------------------------------------------------------
UIDRef
AZPAMService::GetServiceDataModelObject()const
{
	UIDRef toReturn = UIDRef::gNull;

	InterfacePtr<IAMServiceData> serviceData( this, UseDefaultIID() );
	ASSERT(serviceData);

	bool16 foundValue = serviceData->Get( IAMService::enModelObject, toReturn );

	return toReturn;
}
#endif

//----------------------------------------------------------------------------------------
// GetAssetFile
//----------------------------------------------------------------------------------------
bool
AZPAMService::GetAssetFile(
	const IManageableAsset *	inManageableAsset,
	IDFile &					oAssetFile) const
{
	//LogFunctionEnterExit;
	//First check service data then asset.
	bool toReturn = false;
	
	do
	{
		InterfacePtr<IAMServiceData> serviceData( this, UseDefaultIID() );
		ASSERT(serviceData);

		toReturn = serviceData->Get( IAMService::enTargetAsset, oAssetFile );
		if( toReturn || !inManageableAsset)
			break;
		
		toReturn = kTrue == (const_cast<IManageableAsset*>(inManageableAsset))->GetWriteFile( oAssetFile );
		if( toReturn )
			break;

	}while(kFalse);

	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetIDStoryFile
//----------------------------------------------------------------------------------------
bool
AZPAMService::GetIDStoryFile(
	const IManageableAsset *	inManageableAsset,
	IDFile &					oIDStoryAssetFile ) const
{	//First check service data then asset.
	// get the local file
	//LogFunctionEnterExit;
	bool toReturn = false;
	
	do
	{
		toReturn = AZPAMService::GetAssetFile( inManageableAsset, oIDStoryAssetFile );	//Don't use this here otherwise it will become recursive.
		if( toReturn )
			break;

#if defined(InDnCS5) || defined(InDnCS5_5)
		InterfacePtr<ILinkResource> assetLinkResource( inManageableAsset, UseDefaultIID() );
		ASSERT( assetLinkResource );
		toReturn = Utils<IZPStoryUtils>()->GetLinkResourceFileLocation( assetLinkResource, oIDStoryAssetFile );
#else
		//We can also use IDataLinkHelper::GetSysFile
		InterfacePtr<IDataLink> assetDL( inManageableAsset, UseDefaultIID() );
		ASSERT( assetDL );
		toReturn = Utils<IZPStoryUtils>()->GetDataLinkFileLocation( assetDL, oIDStoryAssetFile );
#endif
	}while(kFalse);

	return toReturn;
}

//----------------------------------------------------------------------------------------
// IsAssetLockedByCurrentUser
//----------------------------------------------------------------------------------------
bool
AZPAMService::IsAssetLockedByCurrentUser(
	const IStringData *			inAssetID) const
{
	//LogFunctionEnterExit;
	ASSERT( inAssetID );
	bool toReturn = ( Utils<IZPAMHelper>()->IsAssetLockedByCurrentUser( inAssetID, nil, false ) );
	return toReturn;
}

//----------------------------------------------------------------------------------------
// IsAssetLocked
//----------------------------------------------------------------------------------------
bool
AZPAMService::IsAssetLocked(
	//True if locked by anyone.
	const IStringData *			inAssetID) const
{
	//LogFunctionEnterExit;
	ASSERT( inAssetID );
	bool toReturn = ( Utils<IZPAMHelper>()->IsAssetLocked( inAssetID ) );
	return toReturn;
}

//----------------------------------------------------------------------------------------
// IsLinkedStoryAsset
//----------------------------------------------------------------------------------------
bool
AZPAMService::IsLinkedStoryAsset(
	const IManageableAsset *	inManageableAsset) const
{
	//LogFunctionEnterExit;
	bool toReturn = false;
	do
	{
		if( !inManageableAsset )
			break;

#ifdef InDnCS3
		InterfacePtr<ILinkObjectReference> linkObjectReference( inManageableAsset, UseDefaultIID() );
		if( !linkObjectReference )
			break;

		UID linkedObjectUID = linkObjectReference->GetUID();
#endif
		//Check if this data link is of a story
		UIDRef assetRef = ::GetUIDRef( inManageableAsset );
		UIDRef storyRef = Utils<Facade::IWorkgroupStoryFacade>()->GetStoryUIDRef( assetRef );
		if( storyRef != UIDRef::gNull )
			toReturn = kTrue;

#if defined(InDnCS5) || defined(InDnCS5_5)
		if( toReturn == kFalse )
		{
			UIDRef storyRef2 = this->GetServiceDataModelObject();
			if( storyRef2 != UIDRef::gNull )
			{
				InterfacePtr<const ITextModel> storyTextModel( storyRef2, UseDefaultIID() );
				if( storyTextModel )
					toReturn = kTrue;
			}
		}
#endif

		//InterfacePtr<IAssignedStory> assignedStory( ::GetDataBase( linkObjectReference ), linkedObjectUID, UseDefaultIID() );
		//InterfacePtr<IAssignedStory> assignedStory( (IAssignedStory*)linkObjectReference->QueryLinkedObject( IAssignedStory::kDefaultIID) );
		//if( !assignedStory )
		//	break;

		//const IAssignment * storyAssignment = assignedStory->GetAssignment();
		//toReturn = storyAssignment != nil;
	}while(kFalse);
	return toReturn;
}

//----------------------------------------------------------------------------------------
// IsInCopyDocument
//----------------------------------------------------------------------------------------
bool
AZPAMService::IsInCopyDocument(
	const IManageableAsset *	inManageableAsset) const
{
	//LogFunctionEnterExit;
	bool toReturn = false;

	const IStringData * dbAssetID = this->GetDBAsset( inManageableAsset );
	InterfacePtr<const IZPAssetInfo> dbAssetInfo( dbAssetID, UseDefaultIID() );
	if( dbAssetInfo )
	{
		toReturn = dbAssetInfo->GetType() == eAssetType_ICDocument;
	}
	return toReturn;
}

//----------------------------------------------------------------------------------------
// IsInCopyTemplate
//----------------------------------------------------------------------------------------
bool
AZPAMService::IsInCopyTemplate(
	const IManageableAsset *	inManageableAsset) const
{
	//LogFunctionEnterExit;
	bool toReturn = false;

	const IStringData * dbAssetID = this->GetDBAsset( inManageableAsset );
	InterfacePtr<const IZPAssetInfo> dbAssetInfo( dbAssetID, UseDefaultIID() );
	if( dbAssetInfo )
	{
		toReturn = dbAssetInfo->GetType() == eAssetType_ICTemplate;
	}
	return toReturn;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// QueryAssetLocalInfo
//----------------------------------------------------------------------------------------
const IZPAssetLocalInfo *
AZPAMService::QueryAssetLocalInfo(
	const IStringData *			inAssetID) const
{
	//LogFunctionEnterExit;
	ASSERT( inAssetID );

#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IZPAssetsLocalInfoMgr> assetsLocalInfoMgr( gSession, UseDefaultIID() );
	ASSERT( assetsLocalInfoMgr );

	const PMString & theAssetIDStr = inAssetID->Get();
	InterfacePtr<const IZPAssetLocalInfo> assetLocalInfo( assetsLocalInfoMgr->QueryInfoForAsset( theAssetIDStr, true ) );
	
	return assetLocalInfo.forget();
}

//----------------------------------------------------------------------------------------
// UpdateLocalInfo_LocallyModified
//----------------------------------------------------------------------------------------
void
AZPAMService::UpdateLocalInfo_LocallyModified(
	const IStringData *			inAssetID,
	bool						inIsModified) const
{
	LogFunctionEnterExit;
	ASSERT( inAssetID );
	const PMString & theAssetIDStr = inAssetID->Get();
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " Asset ID : %s : modified : %s", theAssetIDStr.GrabCString(), (inIsModified? "true": "false") );

	InterfacePtr<const IZPAssetLocalInfo> assetLocalInfo( this->QueryAssetLocalInfo( inAssetID ) );
	ASSERT( assetLocalInfo );

	Utils<IZPCommandFacade>()->UpdateAssetLocalInfo( assetLocalInfo, nil, &inIsModified , nil );
}

//----------------------------------------------------------------------------------------
// UpdateLocalInfo_VersionNumber
//----------------------------------------------------------------------------------------
	void
	AZPAMService::UpdateLocalInfo_VersionNumber(
	const IStringData *			inAssetID,
	int32						inVersionNumber) const
{
	LogFunctionEnterExit;
	ASSERT( inAssetID );
	const PMString & theAssetIDStr = inAssetID->Get();
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " Asset ID : %s : version number : %d", theAssetIDStr.GrabCString(), inVersionNumber );

	InterfacePtr<const IZPAssetLocalInfo> assetLocalInfo( this->QueryAssetLocalInfo( inAssetID ) );
	ASSERT( assetLocalInfo );

	Utils<IZPCommandFacade>()->UpdateAssetLocalInfo( assetLocalInfo, nil, nil , &inVersionNumber );
}

//----------------------------------------------------------------------------------------
// EnsureLatestVersion
//----------------------------------------------------------------------------------------
ErrorCode
AZPAMService::EnsureLatestVersion(
	const IManageableAsset *	inAsset) const
{
	LogFunctionEnterExit;
	ErrorCode toReturn = kSuccess;

	//This service can be called before opening the document/assignment, so use file instead of pAsset which comes to be nil.
	
	IDFile theAssetFile ;
	
	if( !this->GetAssetFile( inAsset, theAssetFile ) )	// get the local file
	{
		toReturn = kFailure;	//Must never be the case.
		ASSERT( toReturn == kSuccess );
		return toReturn;
	}

	return this->EnsureLatestVersion( inAsset, theAssetFile );
}

//----------------------------------------------------------------------------------------
// EnsureLatestVersion
//----------------------------------------------------------------------------------------
ErrorCode
AZPAMService::EnsureLatestVersion(
	const IManageableAsset *	inAsset,
	const IDFile &				inAssetFile) const
{
	LogFunctionEnterExit;

	ErrorCode toReturn = kSuccess;

	PMString assetID = Utils<IZPAssetUtils>()->GetAssetIDFromLocalFile( inAssetFile );
	ASSERT( assetID.IsEmpty() == kFalse );

	const IStringData * dbAssetID = ZPDataHelper::GetAssetByID( assetID );
	if( dbAssetID )
		toReturn = Utils<IZPAMHelper>()->CheckForHeadAndDownloadIfNeeded( dbAssetID, inAssetFile, false );
	else
		toReturn = kFailure;

	//If file does not exists then error.
	bool fileExists = Utils<IZPFileUtils>()->DoesFileExist( inAssetFile );
	if( !fileExists )
		toReturn = kFailure;

	return toReturn;
}

//----------------------------------------------------------------------------------------
// DoAfterCheckIn_UpdateLocalInfo
//----------------------------------------------------------------------------------------
void
AZPAMService::DoAfterCheckIn_UpdateLocalInfo(
	//Update local asset info after check-in.
	const IStringData *			inDBAssetID)
{
	LogFunctionEnterExit;
	InterfacePtr<const IZPAssetInfo> dbAssetInfo( inDBAssetID, UseDefaultIID() );
	ASSERT( dbAssetInfo );
	this->UpdateLocalInfo_LocallyModified( inDBAssetID, false );	//Remove the locally modified flag.
	this->UpdateLocalInfo_VersionNumber( inDBAssetID, dbAssetInfo->GetVersionNumber() );	//Update local version number, alread updated in DB cache
}

//----------------------------------------------------------------------------------------
// DoAfterCheckIn
//----------------------------------------------------------------------------------------
void
AZPAMService::DoAfterCheckIn(
	const IManageableAsset *	inAsset,
	const IStringData *			inDBAssetID)
{
	LogFunctionEnterExit;
	InterfacePtr<IZPAssetCheckInData> assetCheckInData( inAsset, UseDefaultIID());
	ASSERT(assetCheckInData);

	this->DoAfterCheckIn( assetCheckInData, inDBAssetID );
}

//----------------------------------------------------------------------------------------
// DoAfterCheckIn
//----------------------------------------------------------------------------------------
void
AZPAMService::DoAfterCheckIn(
	//Clear check-in info.
	IZPAssetCheckInData *	inAssetCheckInData,
	const IStringData *			inDBAssetID)
{
	LogFunctionEnterExit;
	InterfacePtr<const IStringData> dbAssetName( inDBAssetID, IID_IZPDATANAME );
	ASSERT( dbAssetName );

	//Update asset info on server if modified.
	//compare asset name and update
	const PMString & assetNewName = inAssetCheckInData->GetAssetName();
	if( !assetNewName.IsEmpty() && assetNewName.Compare( kTrue, dbAssetName->Get() ) )
	{	//update asset name on server.
		InterfacePtr<const IZPAssetInfo> dbAssetInfo( inDBAssetID, UseDefaultIID() );
		const PMString & theVersionID = dbAssetInfo->GetVersionID();
		const PMString & statusID = inAssetCheckInData->GetStatusID();
		ErrorCode toReturn = Utils<IZPAMWSUtils>()->UpdateAsset_sync( inDBAssetID->Get(), assetNewName, kNullString, theVersionID, statusID );
		if( toReturn != kSuccess )
		{
			//??
		}
	}

	//Remove Check-in data stored with asset, so that comments must be asked again.
	inAssetCheckInData->SetDescription( kNullString );
	inAssetCheckInData->SetStatusID( kNullString );
	inAssetCheckInData->SetAssetName( kNullString );
}

//----------------------------------------------------------------------------------------
// CopyParentCheckInData
//----------------------------------------------------------------------------------------
bool
AZPAMService::CopyParentCheckInData(
	IManageableAsset *			inAsset) const
{
	LogFunctionEnterExit;
	//Used for ID Story assets.

	bool toReturn = false;
	InterfacePtr<IZPAssetCheckInData> assetCheckInData( inAsset, UseDefaultIID());
	ASSERT(assetCheckInData);

	//TODO: find assignment for the story and copy its data if set, then try the document data

	IDataBase * assetDB = ::GetDataBase( inAsset );
	InterfacePtr<const IZPAssetData> parentAssetData( assetDB, assetDB->GetRootUID(), UseDefaultIID());
	ASSERT(parentAssetData);

	const IStringData * dbAssetID = ZPDataHelper::GetAssetByID( parentAssetData->GetAssetID() );
	if( !dbAssetID )	//On InCopy parent might be document, which might not be in cache.
	{					//So copy the assignment values.
		InterfacePtr<const IDocument> parentDocument( parentAssetData, UseDefaultIID() );
		ASSERT( parentDocument );
		InterfacePtr<IAssignment> iAssignment( Utils<IZPDocUtils>()->QueryAssignmentFromAssignmentDoc( parentDocument ) );
		if(iAssignment)
		{
			InterfacePtr<const IZPAssetData> assignmentAssetData( iAssignment, UseDefaultIID());
			ASSERT(assignmentAssetData);

			const IStringData * assignmentDBAssetID = ZPDataHelper::GetAssetByID( assignmentAssetData->GetAssetID() );
			dbAssetID = assignmentDBAssetID;
		}
	}
	if( dbAssetID )
		toReturn = this->FillCheckInDataFromDbAsset( assetCheckInData, dbAssetID );

	return toReturn;
}

//----------------------------------------------------------------------------------------
// CopyParentCheckInData
//----------------------------------------------------------------------------------------
bool
AZPAMService::CopyParentCheckInData(
	IAssignment *				inAsset) const
{
	LogFunctionEnterExit;
	bool toReturn = false;
	InterfacePtr<IZPAssetCheckInData> assetCheckInData( inAsset, UseDefaultIID());
	ASSERT(assetCheckInData);

	//TODO: handle ID story

	IDataBase * assetDB = ::GetDataBase( inAsset );
	InterfacePtr<const IZPAssetData> parentAssetData( assetDB, assetDB->GetRootUID(), UseDefaultIID());
	ASSERT(parentAssetData);

	const IStringData * dbAssetID = ZPDataHelper::GetAssetByID( parentAssetData->GetAssetID() );
	ASSERT( dbAssetID );
	if( dbAssetID )
		toReturn = this->FillCheckInDataFromDbAsset( assetCheckInData, dbAssetID );

	return toReturn;
}

//----------------------------------------------------------------------------------------
// FillCheckInDataFromSelf
// Copy self status to check-in Data, used from InDesign
//----------------------------------------------------------------------------------------
bool
AZPAMService::FillCheckInDataFromSelf(
	IAssignment *				inAsset ) const
{
	LogFunctionEnterExit;
	bool toReturn = false;
	InterfacePtr<IZPAssetCheckInData> assetCheckInData( inAsset, UseDefaultIID());
	ASSERT(assetCheckInData);

	InterfacePtr<const IZPAssetData> selfAssetData( inAsset, UseDefaultIID());
	ASSERT(selfAssetData);

	const IStringData * dbAssetID = ZPDataHelper::GetAssetByID( selfAssetData->GetAssetID() );
	ASSERT( dbAssetID );
	if( dbAssetID )
		toReturn = this->FillCheckInDataFromDbAsset( assetCheckInData, dbAssetID, true );
	
	return toReturn;
}

//----------------------------------------------------------------------------------------
// FillCheckInDataFromSelf
//----------------------------------------------------------------------------------------
bool
AZPAMService::FillCheckInDataFromSelf(		//Copy self status to check-in Data,
	IManageableAsset *			inAsset ) const
{
	LogFunctionEnterExit;
	bool toReturn = false;
	InterfacePtr<IZPAssetCheckInData> assetCheckInData( inAsset, UseDefaultIID());
	ASSERT(assetCheckInData);

	InterfacePtr<const IZPAssetData> selfAssetData( inAsset, UseDefaultIID());
	ASSERT(selfAssetData);

	const IStringData * dbAssetID = ZPDataHelper::GetAssetByID( selfAssetData->GetAssetID() );
	ASSERT( dbAssetID );
	if( dbAssetID )
		toReturn = this->FillCheckInDataFromDbAsset( assetCheckInData, dbAssetID, true );
	
	return toReturn;
}

//----------------------------------------------------------------------------------------
// FillCheckInDataFromDbAsset
//----------------------------------------------------------------------------------------
bool
AZPAMService::FillCheckInDataFromDbAsset(
	IZPAssetCheckInData *		ioCheckInData,
	const IStringData *			inDBAssetID,
	bool						inCopyAssetName) const
{
	LogFunctionEnterExit;
	bool toReturn = false;
	if( inDBAssetID )
	{
		InterfacePtr<const IZPAssetInfo> dbAssetInfo( inDBAssetID, UseDefaultIID());
		ASSERT( dbAssetInfo );
		if( dbAssetInfo->GetVersionNumber() > 0 )
		{
			ASSERT( dbAssetInfo->GetStatusID().IsEmpty() == kFalse );

			ioCheckInData->SetStatusID( dbAssetInfo->GetStatusID() );

			if( inCopyAssetName )
			{
				InterfacePtr<const IStringData> dbAssetName( inDBAssetID, IID_IZPDATANAME );
				ASSERT( dbAssetName );

				ioCheckInData->SetAssetName( dbAssetName->Get() );
			}

			toReturn = true;
		}
	}
	return toReturn;
}

//----------------------------------------------------------------------------------------
// CallEnsureLatestVersion
//----------------------------------------------------------------------------------------
void
AZPAMService::CallEnsureLatestVersion(
	IManageableAsset *			pAsset) const
{
	//LogFunctionEnterExit;
	ClassID amProviderClass = pAsset->GetServiceProviderClass();
	if( Utils<IZPAMHelper>()->CallAMServiceVerifyContext( pAsset, amProviderClass, IAMServiceProvider::enEnsureLatestVersion ) )
		Utils<IZPAMHelper>()->CallAMServiceDo( pAsset, amProviderClass, IAMServiceProvider::enEnsureLatestVersion );
}

//----------------------------------------------------------------------------------------
// QueryAssetMonitor
//----------------------------------------------------------------------------------------
IAssetMonitor *
AZPAMService::QueryAssetMonitor(
	ClassID						inServiceProvider) const
{
	//LogFunctionEnterExit;
	IAssetMonitor * toReturn = nil;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	if ( inServiceProvider != kInvalidClass )
	{
		InterfacePtr<IAMSPManager> iAMSPManager( gSession, UseDefaultIID() );
		ASSERT( iAMSPManager );
		
		IAMSystemProvider * amServiceProvider = iAMSPManager->GetSystemProvider( inServiceProvider );
		ASSERT( amServiceProvider );

		toReturn = amServiceProvider->GetAssetMonitor();
		if( toReturn )
			toReturn->AddRef();
	}

	return toReturn;
}




#pragma mark -

//----------------------------------------------------------------------------------------
// DoReleaseLock
//----------------------------------------------------------------------------------------
void
AZPAMService::DoReleaseLock(
	const IStringData *			inAssetID)
{
	LogFunctionEnterExit;

	//Release lock on asset. //TODO: handle the return value
	Utils<IZPAMWSUtils>()->ReleaseLock_sync( inAssetID->Get() );
}

//----------------------------------------------------------------------------------------
// DoAcquireLock
//----------------------------------------------------------------------------------------
ErrorCode
AZPAMService::DoAcquireLock(
	const IStringData *			inAssetID)
{
	LogFunctionEnterExit;
	const PMString & theAssetIDStr = inAssetID->Get();
	ErrorCode toReturn = Utils<IZPAMWSUtils>()->AcquireLock_sync( theAssetIDStr, kNullString, kNullString );
	if( toReturn == kSuccess && !Utils<IZPAMHelper>()->IsAssetLockedByCurrentUser( inAssetID ) )
		toReturn = kFailure;
	return toReturn;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// LogManageableAssetInfo
//----------------------------------------------------------------------------------------
void
AZPAMService::LogManageableAssetInfo(
	const IManageableAsset *	inAsset) const
{
#ifdef DEBUG
	if( inAsset )
	{
		DebugClassUtilsBuffer classBuf;
		const char* className = DebugClassUtils::GetIDName(&classBuf, ::GetClass( inAsset ));
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, " Asset UID = 0x%08X , asset class name = %s", ::GetUID( inAsset ).Get(), className );

		IManageableAsset::AssetType assetType = (const_cast<IManageableAsset*>(inAsset))->GetType();
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, " Asset Type = %d", assetType );

		if( assetType == IManageableAsset::enDocument )
		{
			bool isVersioned = inAsset->IsVersioned();
			IZPLog_Str_( thisFileLA, enLT_DebugInfo, " Asset isVersioned = %hd", isVersioned );
		}
	}
	else
	{
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, " Asset is nil");
	}
#endif
}
//----------------------------------------------------------------------------------------
// LogFilePath
//----------------------------------------------------------------------------------------
void
AZPAMService::LogFilePath(
	const IDFile &				inFile) const
{
#ifdef DEBUG
	PMString pathStr;
	Utils<IZPFileUtils>()->ConvertIDFileToStr( inFile, pathStr );
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "File path : %s ", pathStr.GrabCString() );
#endif
}