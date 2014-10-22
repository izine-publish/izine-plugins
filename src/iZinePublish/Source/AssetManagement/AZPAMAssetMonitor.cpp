//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/AZPAMAssetMonitor.cpp $
//	$Revision: 4097 $
//	$Date: 2013-08-14 11:17:30 +0200 (Wed, 14 Aug 2013) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 21-7-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IAMServiceProvider.h"
#include "IIntData.h"
#include "IObserver.h"
#include "IStringData.h"
#include "ISubject.h"
#include "IWorkspace.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#include "IDocument.h"
#endif

//SDK General includes
#include "K2Vector.tpp"
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPAMWSUtils.h"
#include "IZPAppController.h"
#include "IZPAssetInfo.h"
#include "IZPAssetLockInfo.h"
#include "IZPPrefs.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IZPInDnUtils.h"
#include "IZPCommandFacade.h"
#endif

//IZP General includes
#include "AZPAMAssetMonitor.h"
#include "ZPDataHelper.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AssetMonitor
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#pragma mark -
//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
AZPAMAssetMonitor::AZPAMAssetMonitor(IPMUnknown * boss)
: inheritClass_ZPAMAssetMonitor( boss )
, mCurrentIndex( 0 )
, mLastProcessingTime( 0 )
, mResetIndexPending( false )
{
	IZPLog_ObjC( thisFileLA );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
AZPAMAssetMonitor::~AZPAMAssetMonitor()
{
	IZPLog_ObjD( thisFileLA );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// AssetHasMoved
//----------------------------------------------------------------------------------------
bool16
AZPAMAssetMonitor::AssetHasMoved(
	IManageableAsset * asset)
{
	LogFunctionEnterExit;
	//TODO: find if paths stored with asset is same as current replica folder.
	bool16 toReturn = kFalse;
	//toReturn = kTrue;
	return toReturn;
}

//----------------------------------------------------------------------------------------
// CanMonitor
//----------------------------------------------------------------------------------------
bool16
AZPAMAssetMonitor::CanMonitor(
	const IDFile & file)
{
	LogFunctionEnterExit;
	InterfacePtr<IAMServiceProvider> serviceProvider( this, UseDefaultIID() );
	ASSERT( serviceProvider );
	
	return serviceProvider->CanService( file, nil );
}

//----------------------------------------------------------------------------------------
// CanMonitor
//----------------------------------------------------------------------------------------
bool16
AZPAMAssetMonitor::CanMonitor(
	IManageableAsset * asset)
{
	LogFunctionEnterExit;
	InterfacePtr<IAMServiceProvider> serviceProvider( this, UseDefaultIID() );
	ASSERT( serviceProvider );
	
	return serviceProvider->CanService( asset );
}

//----------------------------------------------------------------------------------------
// StartUp
//----------------------------------------------------------------------------------------
bool16
AZPAMAssetMonitor::StartUp()
{
	LogFunctionEnterExit;
	return kTrue;
}

//----------------------------------------------------------------------------------------
// ShutDown
//----------------------------------------------------------------------------------------
bool16
AZPAMAssetMonitor::ShutDown()
{
	LogFunctionEnterExit;
	return kTrue;
}

//----------------------------------------------------------------------------------------
// StartMonitoring
//----------------------------------------------------------------------------------------
ErrorCode
AZPAMAssetMonitor::StartMonitoring(
	UIDRef assetRef)
{
	LogFunctionEnterExit;
	ErrorCode status = kFailure;

#if defined(InDnCS5) || defined(InDnCS5_5)
	
	bool16 monitorAsset = kTrue;
	IDataBase * db = assetRef.GetDataBase();
	InterfacePtr<IDocument> doc( db, db->GetRootUID(), UseDefaultIID() );
	ASSERT(doc);
		
	//Patch for InCopy open unassigned icml file. Check for valid document as it could be scrap document
	if(Utils<IZPInDnUtils>()->IsHostAppInCopy() && !doc )
	{
		monitorAsset = kFalse;
	}
	else if( doc->IsConverted() ) //Clear asset data if converted document
	{
			Utils<IZPCommandFacade>()->SaveAssetData(nil, doc);		
			monitorAsset = kFalse;
	}	

#endif

	InterfacePtr<IManageableAsset> asset( assetRef, UseDefaultIID() );
#if DEBUG
	DebugClassUtilsBuffer classBuf;
	const char* className = DebugClassUtils::GetIDName(&classBuf, ::GetClass( asset ));	
	bool16 istemp = asset->IsTemporary();
#endif
#if defined(InDnCS5) || defined(InDnCS5_5)
	if ( asset && monitorAsset )
#else 
	if ( asset )
#endif
	{
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Asset Monitor: %s", this->GetAssetMonitorName() );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Asset Monitor: 0x%08X  Database: 0x%08X, UID: 0x%X ", this, assetRef.GetDataBase(), assetRef.GetUID().Get() );
		
		K2Vector<UIDRef>::iterator iter = std::find( fActiveList.begin(), fActiveList.end(), assetRef );
		if( iter == fActiveList.end() )
		{
			fActiveList.push_back(assetRef);
			//Listen to asset
			this->ListenToDBAsset(assetRef);
		}
		
		// do what you need to do to start monitoring
		status = kSuccess;
	}
	return status;
}

//----------------------------------------------------------------------------------------
// DoBackgroundTasks
//----------------------------------------------------------------------------------------
uint32
AZPAMAssetMonitor::DoBackgroundTasks()
{
	//LogFunctionEnterExit;
	if( this->ProcessNextAsset() )
		return 0;
	else
		return 10;
}

//----------------------------------------------------------------------------------------
// GetHandleChangesCommand
//----------------------------------------------------------------------------------------
ICommand *
AZPAMAssetMonitor::GetHandleChangesCommand()
{
	//LogFunctionEnterExit;
	ICommand * toReturn = nil;
	if( fPendingCommands.size() > 0 )
	{
		K2Vector<ICommand *>::iterator iter = fPendingCommands.begin();
		K2Vector<ICommand *>::iterator endIter = fPendingCommands.end();
		while( iter != endIter )
		{
			ICommand * currVal = *iter;
			if( currVal->GetCommandState() == ICommand::kNotDone )
			{	
				//Keep the command in pending list
				//otherwise we get a crash if release else leak if not released.
				toReturn = currVal;
				break;
			}
			else
			{
				fPendingCommands.erase( iter );	//Don't increament the iter.
				currVal->Release();
				endIter = fPendingCommands.end();
			}
		}
	}
	return toReturn;
}

//----------------------------------------------------------------------------------------
// RemovePendingCommandForUIDRef
//----------------------------------------------------------------------------------------
void
AZPAMAssetMonitor::RemovePendingCommandForUIDRef(
	const UIDRef &				inAssetRef)
{
	//LogFunctionEnterExit;
	if( fPendingCommands.size() > 0 )
	{
		K2Vector<ICommand *>::iterator iter = fPendingCommands.begin();
		K2Vector<ICommand *>::iterator endIter = fPendingCommands.end();
		while( iter != endIter )
		{
			ICommand * currVal = *iter;
			const UIDRef & cmdTarget = currVal->GetTarget();
			if( cmdTarget == inAssetRef )
			{
				fPendingCommands.erase( iter );	//Don't increament the iter.
				currVal->Release();
				endIter = fPendingCommands.end();
			}
			else
			{
				++iter;
			}
		}
	}
}

//----------------------------------------------------------------------------------------
// GetNextPendingStatus
//----------------------------------------------------------------------------------------
IManagedStatus *
AZPAMAssetMonitor::GetNextPendingStatus()
{
	//LogFunctionEnterExit;
	//TODO:
	return NULL;
}

//----------------------------------------------------------------------------------------
// IsStatusPending
//----------------------------------------------------------------------------------------
bool16
AZPAMAssetMonitor::IsStatusPending(
	IManageableAsset * asset)
{
	LogFunctionEnterExit;
	//TODO:
	return kFalse;
}

//----------------------------------------------------------------------------------------
// IsMonitoring
//----------------------------------------------------------------------------------------
bool16
AZPAMAssetMonitor::IsMonitoring(
	UIDRef assetRef)
{
	LogFunctionEnterExit;
	
	bool16 isWatching = kFalse;
	
	//	InterfacePtr<IManageableAsset> asset( assetRef, UseDefaultIID() );
	
	K2Vector<UIDRef>::iterator iter = std::find( fActiveList.begin(), fActiveList.end(), assetRef );
	if ( iter != fActiveList.end() )
	{
		isWatching = kTrue;
	}
	
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Asset Monitor: 0x%08X UID: 0x%X , monitoring : %hd", this, assetRef.GetUID().Get(), isWatching );
	
	return isWatching;
}

//----------------------------------------------------------------------------------------
// StopMonitoring
//----------------------------------------------------------------------------------------
ErrorCode
AZPAMAssetMonitor::StopMonitoring(
	UIDRef assetRef)
{
	LogFunctionEnterExit;
	
	ErrorCode status = kFailure;
	
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Asset Monitor: 0x%08X  Database: 0x%08X, UID: 0x%X ", this, assetRef.GetDataBase(), assetRef.GetUID().Get() );
	
	// assetRef is UIDRef of data link..
	//	InterfacePtr<IManageableAsset> asset( assetRef, UseDefaultIID() );
	
	// remove this UIDRef from our active list
	K2Vector<UIDRef>::iterator iter = std::find( fActiveList.begin(), fActiveList.end(), assetRef );
	if ( iter != fActiveList.end() )
	{
		int foundIterIndex = iter - fActiveList.begin();
		if( foundIterIndex <= mCurrentIndex && mCurrentIndex > 0 )
			--mCurrentIndex;	//Reduce the current index, so that current points to the same object as before removal

		//Stop listening to asset
		this->StopListenToDBAsset(assetRef);

		fActiveList.erase( iter );

		this->RemovePendingCommandForUIDRef( assetRef );

		status = kSuccess;
	}
	
	return status;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// ProcessNextAsset
//----------------------------------------------------------------------------------------
bool
AZPAMAssetMonitor::ProcessNextAsset()	//Returns true if more assets are pending.
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	if (mPendingAssetToUpdate.size())
	{
		const PMString & assetId = mPendingAssetToUpdate[0];
		ZPAssetIdUIDRefMapConstIter foundIter = mAssetIdUIDRefMap.find( assetId );
		ZPAssetIdUIDRefMapConstIter endIter = mAssetIdUIDRefMap.end();
		if( foundIter != endIter )
		{
			UIDRef assetRef = foundIter->second;
			InterfacePtr<IManageableAsset> asset( assetRef, UseDefaultIID() );
			ASSERT(asset);
			if( asset )
				this->ProcessAsset( asset );

			IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Removing asset id : %s from pending update list", assetId.GrabCString());
			mPendingAssetToUpdate.erase(mPendingAssetToUpdate.begin());

			return true;	//Come again for processing this list or fActiveList
		}
	}

	int activeListSize = fActiveList.size();
	if( mCurrentIndex < activeListSize )
	{
		UIDRef assetRef = fActiveList.at( mCurrentIndex );
		//Is it valid? Is there any chance that UIDRef is not removed from the active list?
		InterfacePtr<IManageableAsset> asset( assetRef, UseDefaultIID() );
		ASSERT(asset);
		this->ProcessAsset( asset );
		++mCurrentIndex;
	}

	bool toReturn = ( mCurrentIndex < activeListSize );
	
	if (!toReturn && activeListSize > 0 )
	{
		time_t nowTime = time(NULL);
		// check every 30 seconds
#if	0//def DEBUG
		const int timeDelayBetweenRefresh = 10;	//Save debugging time.
#else
		InterfacePtr<const IWorkspace> iWorkspace(gSession->QueryWorkspace());
		
		InterfacePtr<const IZPPrefs> zpPrefs( iWorkspace, UseDefaultIID() );
		ASSERT(zpPrefs);
		const int timeDelayBetweenRefresh = zpPrefs->GetAssetMonitorAutoUpdateDuration();
		ASSERT( timeDelayBetweenRefresh >= 10 );
#endif
		if (nowTime - mLastProcessingTime > timeDelayBetweenRefresh )
		{
			//mCurrentIndex = 0;	//Commented here to set it after a delay of 5 seconds.
			mResetIndexPending = true;
			mLastProcessingTime = nowTime;
			this->FetchInfoFromServer();	//Update cached info for monitored assets.
		}
		if( mResetIndexPending && nowTime - mLastProcessingTime > 5)	//Wait for 5 seconds to get the response from server.
		{
			//Someday: better would be to listen the monitored assets for changes.
			mCurrentIndex = 0;	//Reset for next processing.
			mResetIndexPending = false;
		}
	}
	return toReturn;
}

//----------------------------------------------------------------------------------------
// RefreshNow
//----------------------------------------------------------------------------------------
void
AZPAMAssetMonitor::RefreshNow()
{	//Mostly called after receving data from server.
	LogFunctionEnterExit;
	mCurrentIndex = 0;	//Reset the current index to refresh all the assets status.
}


//----------------------------------------------------------------------------------------
// ProcessAsset
//----------------------------------------------------------------------------------------
bool
AZPAMAssetMonitor::ProcessAsset(
	IManageableAsset *			inAsset)
{
	LogFunctionEnterExit;
	return true;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetAssetID
//----------------------------------------------------------------------------------------
const PMString &
AZPAMAssetMonitor::GetAssetID(
	const UIDRef &				inAssetUIDRef) const
{
	//LogFunctionEnterExit;
	do{
		InterfacePtr<IManageableAsset> asset( inAssetUIDRef, UseDefaultIID() );
		ASSERT(asset);
#if defined(InDnCS5) || defined(InDnCS5_5)
		if(!asset)
			break;
#endif
		InterfacePtr<const IManagedStatus> storyStatus( asset->GetStatus(), UseDefaultIID() );
		ASSERT(storyStatus);
		if ( storyStatus )
		{
			InterfacePtr<const IStringData> assetID( storyStatus, IID_IZPDATAID );
			if( assetID )	//Coming for local image links also.
				return assetID->Get();
		}
	}while(kFalse);
	return kNullString;
}

//----------------------------------------------------------------------------------------
// GetMonitoredAssetIDs
//----------------------------------------------------------------------------------------
void
AZPAMAssetMonitor::GetMonitoredAssetIDs(
	ZPAssetIDsList &			oAssetIDs) const
{
	//LogFunctionEnterExit;
	int activeListSize = fActiveList.size();
	for( int i = 0 ; i < activeListSize ; ++i )
	{
		UIDRef assetRef = fActiveList.at( i );
		//if( !DBUtils::IsValidDataBase ( assetRef.GetDataBase() ) )
		//	continue;
		const PMString & assetID = this->GetAssetID( assetRef );
		if( !assetID.IsEmpty() )
			oAssetIDs.push_back( assetID );
	}
}


//----------------------------------------------------------------------------------------
// FetchInfoFromServer
//	Fetch lock & version info for monitored assets.
//----------------------------------------------------------------------------------------
void
AZPAMAssetMonitor::FetchInfoFromServer()
{
	//Check if logged in.
	if (gZPApp->GetAppState() != eAppState_LoggedIn)
		return;

	LogFunctionEnterExit;

	ZPAssetIDsList	assetIDs;
	this->GetMonitoredAssetIDs( assetIDs );
	if( assetIDs.size() <= 0 )
		return;

	Utils<IZPAMWSUtils>()->FetchAssetsInfo_async( assetIDs );
	Utils<IZPAMWSUtils>()->FetchAssetsLockInfo_async( assetIDs );
}

//----------------------------------------------------------------------------------------
// CalculateLinkState
//----------------------------------------------------------------------------------------
#if defined(InDnCS5) || defined(InDnCS5_5)
ILinkResource::ResourceState
AZPAMAssetMonitor::CalculateLinkState(
	IManagedStatus::VersionState	inVersionState ) const
#else
IDataLink::StateType
AZPAMAssetMonitor::CalculateLinkState(
	IManagedStatus::VersionState	inVersionState) const
#endif
{
	//LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
	ILinkResource::ResourceState newLinkState = ILinkResource::kMissing;
	if(inVersionState == IManagedStatus::enUnknownVersionState)
		newLinkState = ILinkResource::kMissing;
	else if(inVersionState == IManagedStatus::enUpToDate || inVersionState == IManagedStatus::enLocalIsNewer )
		newLinkState = ILinkResource::kAvailable;
	else
		newLinkState = ILinkResource::kPending;	
#else
	IDataLink::StateType newLinkState = IDataLink::kLinkMissing;
	if( inVersionState == IManagedStatus::enUnknownVersionState )
		newLinkState = IDataLink::kLinkMissing;
	else if ( inVersionState == IManagedStatus::enUpToDate || inVersionState == IManagedStatus::enLocalIsNewer )
		newLinkState = IDataLink::kLinkNormal;
	else
		newLinkState = IDataLink::kLinkOutOfDate;
#endif

	return newLinkState;
}

//----------------------------------------------------------------------------------------
// ListenToDBAsset
//----------------------------------------------------------------------------------------
void
AZPAMAssetMonitor::ListenToDBAsset(
	const UIDRef &				inAssetUIDRef)
{
	LogFunctionEnterExit;
	const PMString & assetId = this->GetAssetID(inAssetUIDRef);
	const IStringData * dbAssetID = ZPDataHelper::GetAssetByID( assetId );

	InterfacePtr<ISubject> assetSubject( dbAssetID, UseDefaultIID());
	if( assetSubject )
	{
		InterfacePtr<IObserver> selfObserver( this , UseDefaultIID() );
		if (selfObserver)
		{
			InterfacePtr<IIntData> assetObserverCounter( dbAssetID , IID_IZPASSETMONITOROBSERVECOUNTER );
			assetObserverCounter->Set( assetObserverCounter->Get() + 1 );
			mAssetIdUIDRefMap[assetId] = inAssetUIDRef;		//Add to map to ease getting manageable asset for asset id.
		}
		
		if(selfObserver && !assetSubject->IsAttached( selfObserver, IZPAssetInfo::kDefaultIID ) )
		{
			assetSubject->AttachObserver( selfObserver, IID_IZPDATANAME );
			assetSubject->AttachObserver( selfObserver, IZPAssetInfo::kDefaultIID );
			assetSubject->AttachObserver( selfObserver, IZPAssetLockInfo::kDefaultIID );
			IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Listen for UID : 0x%X, to db asset : 0x%08X, id : %s", inAssetUIDRef.GetUID().Get(), dbAssetID, assetId.GrabCString());
		}
	}
}

//----------------------------------------------------------------------------------------
// StopListenToDBAsset
//----------------------------------------------------------------------------------------
void
AZPAMAssetMonitor::StopListenToDBAsset(
	const UIDRef &				inAssetUIDRef)
{
	LogFunctionEnterExit;
	const PMString & assetId = this->GetAssetID(inAssetUIDRef);
	const IStringData * dbAssetID = ZPDataHelper::GetAssetByID( assetId );

	InterfacePtr<ISubject> assetSubject( dbAssetID, UseDefaultIID());
	if( assetSubject )
	{
		int count = 1;		//Non zero value as default
		InterfacePtr<IObserver> selfObserver( this , UseDefaultIID() );
		if (selfObserver)
		{
			InterfacePtr<IIntData> assetObserverCounter( dbAssetID , IID_IZPASSETMONITOROBSERVECOUNTER );
			ASSERT(assetObserverCounter->Get() > 0);
			if (assetObserverCounter->Get() > 0)
				assetObserverCounter->Set( assetObserverCounter->Get() - 1 );

			count = assetObserverCounter->Get();
		}

		if(selfObserver && count == 0 && assetSubject->IsAttached( selfObserver, IZPAssetInfo::kDefaultIID ) )
		{
			assetSubject->DetachObserver( selfObserver, IID_IZPDATANAME );
			assetSubject->DetachObserver( selfObserver, IZPAssetInfo::kDefaultIID );
			assetSubject->DetachObserver( selfObserver, IZPAssetLockInfo::kDefaultIID );
			IZPLog_Str_( thisFileLA, enLT_DebugInfo, "StopListen for UID : 0x%X, to db asset 0x%08X, id : %s",inAssetUIDRef.GetUID().Get(), dbAssetID, assetId.GrabCString());
			
			mAssetIdUIDRefMap.erase(assetId);
		}
	}
}

//----------------------------------------------------------------------------------------
// AddUpdatedAssetId
//----------------------------------------------------------------------------------------
void
AZPAMAssetMonitor::AddUpdatedAssetId(
	const PMString &			inAssetID)
{
	LogFunctionEnterExit;
	if (!inAssetID.IsEmpty())
	{
		K2Vector<PMString>::iterator iter = std::find( mPendingAssetToUpdate.begin(), mPendingAssetToUpdate.end(), inAssetID );
		if( iter == mPendingAssetToUpdate.end() )
		{
			IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Adding asset id : %s in pending update list", inAssetID.GrabCString());
			mPendingAssetToUpdate.push_back(inAssetID);
		}
	}
}
