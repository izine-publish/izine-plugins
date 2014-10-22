//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/CZPAMLockable.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 8-7-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IAMLockable.h"
#include "IAMServiceProvider.h"
#include "IManagedStatus.h"
#include "IManageableAsset.h"
#include "IStringData.h"
#include "IWorkgroupStoryFacade.h"

//SDK General includes

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPAMHelper.h"
#include "IZPAssetInfo.h"
#include "IZPAssetLockInfo.h"

//IZP General includes
#include "ZPDataHelper.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AM
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

/**
		enUndefined = 0,	// Default before object initialization or if locking is not supported.
		enUnavailable,		// Cannot be locked at this time; e.g., new asset not yet saved.
		enAvailable,		// Can be locked (currently unlocked).
		enLocked,			// Asset is locked other than here.
		enCheckedOut		// Locked here (note that all other instances of IAMLockable will report enLocked).
*/
#ifdef DEBUG
static const char * gLockStates[] = {
						"enUndefined",
						"enUnavailable",
						"enAvailable",
						"enLocked",
						"enCheckedOut"
					};
#endif

#define inheritClass CPMUnknown<IAMLockable>
class CZPAMLockable : public inheritClass
{
public:
						CZPAMLockable(IPMUnknown * boss);
	virtual				~CZPAMLockable();

	bool16				IsLocked();
#if defined(InDnCS5) || defined(InDnCS5_5)
	bool16				IsLocked(
							const PMString & userName);
	bool16				HasLock(
							UID objectUID = kInvalidUID);

	IAMLockable::LockState GetLockState(
							UID objectUID = kInvalidUID);
#else
	bool16				HasLock();

	IAMLockable::LockState GetLockState();
#endif

	void				GetLockStatusStrings(
							std::map<int32, PMString> & stringTable );
#if defined(InDnCS5) || defined(InDnCS5_5)
	bool16				Lock(
							UIFlags uiFlags = kFullUI, bool16 allowOverride = kFalse, const PMString *userName = nil, UID objectUID = kInvalidUID);
	bool16				Relock(
							UIFlags uiFlags = kFullUI, bool16 allowOverride = kFalse, UID objectUID = kInvalidUID);
	bool16				Unlock(
							UIFlags uiFlags = kFullUI, bool16 allowOverride = kFalse, UID objectUID = kInvalidUID);
	IAMLockable::LockState VerifyState(
							UIFlags uiFlags = kFullUI, UID objectUID = kInvalidUID);
	UID					GetLockObjectUID();
#else
	bool16				Lock(
							UIFlags uiFlags, bool16 allowOverride, const PMString *checkedOutTo );
	bool16				Relock(
							UIFlags uiFlags, bool16 allowOverride );
	bool16				Unlock(
							UIFlags uiFlags, bool16 allowOverride );
	IAMLockable::LockState VerifyState(
							UIFlags uiFlags = kFullUI );
#endif
protected:
	const IStringData *	GetDBAsset();
	IManageableAsset *	QueryAsset();
private:
	IAMLockable::LockState fLockState;
	
	PMString fLockOwnerName;
	PMString fCurrentUserName;
	
	PMString fCheckedOutBy;
	PMString fCheckedOutDoc;
	PMString fCheckedOutApp;
#if defined(InDnCS5) || defined(InDnCS5_5)
	UID		 fObjectUID;
#endif
};


CREATE_PMINTERFACE(CZPAMLockable, kZPAMLockableImpl)

CZPAMLockable::CZPAMLockable(IPMUnknown * boss)
: inheritClass(boss)
{
	IZPLog_ObjC( thisFileLA );
	fLockState = IAMLockable::enUndefined;  // init to 'checked-in'
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMLockable::~CZPAMLockable()
{
	IZPLog_ObjD( thisFileLA );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetLockStatusStrings
//----------------------------------------------------------------------------------------
void
CZPAMLockable::GetLockStatusStrings(
	std::map<int32, PMString> & stringTable)
{
	LogFunctionEnterExit;
	
	if ( !fCheckedOutBy.IsEmpty() )
	{
		fCheckedOutBy.SetTranslatable( kFalse );
		stringTable.insert( std::pair<int32, PMString>( IManagedStatus::enCheckedOutBy, fCheckedOutBy ) );
	}
	
	if ( !fCheckedOutDoc.IsEmpty() )
	{
		fCheckedOutDoc.SetTranslatable( kFalse );
		stringTable.insert( std::pair<int32, PMString>( IManagedStatus::enCheckedOutDoc, fCheckedOutDoc ) );
	}
	
	if ( !fCheckedOutApp.IsEmpty() )
	{
		fCheckedOutApp.SetTranslatable( kFalse );
		stringTable.insert( std::pair<int32, PMString>( IManagedStatus::enCheckedOutApp, fCheckedOutApp ) );
	}
	
	if ( !fLockOwnerName.IsEmpty() )
		stringTable.insert( std::pair<int32, PMString>( IManagedStatus::enLockOwner, fLockOwnerName ) );
	
	if ( !fCurrentUserName.IsEmpty() )
		stringTable.insert( std::pair<int32, PMString>( IManagedStatus::enCurrentUser, fCurrentUserName ) );
	
	if ( !fCurrentUserName.IsEmpty() )
		stringTable.insert( std::pair<int32, PMString>( IManagedStatus::enCurrentClient, fCurrentUserName ) );
}


//----------------------------------------------------------------------------------------
// IsLocked
//----------------------------------------------------------------------------------------
bool16
CZPAMLockable::IsLocked()
{
	LogFunctionEnterExit;
	
	bool16 isLocked = kFalse;
#ifdef DEBUG
	IAMLockable::LockState oldCachedState = fLockState;
#endif
	//if ( fLockState == IAMLockable::enUndefined )
	// not yet initialized
#if defined(InDnCS5) || defined(InDnCS5_5)
	VerifyState( kSuppressUI, GetLockObjectUID() );
#else
	VerifyState( kSuppressUI );	
#endif

#ifdef DEBUG
	if( fLockState != oldCachedState )
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Lock state changed from %d to %d", oldCachedState, fLockState);
#endif	
	if ( fLockState == IAMLockable::enCheckedOut || fLockState==IAMLockable::enLocked )
	{
		isLocked =  kTrue;
	}
	
	// retrieve lock state from server?
	
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "IsLocked = %s\n",(isLocked?"true":"false"));
	
	return isLocked;
}

#if defined(InDnCS5) || defined(InDnCS5_5)
//----------------------------------------------------------------------------------------
// IsLocked
//----------------------------------------------------------------------------------------
bool16
CZPAMLockable::IsLocked(
	const PMString & userName)
{
	LogFunctionEnterExit;
	//TODO: CS5: Implement functionality
	bool16 isLocked = kFalse;

	VerifyState( kSuppressUI, GetLockObjectUID() );	
	
	if(userName.IsEqual(fLockOwnerName))
	{
		if( fLockState == IAMLockable::enCheckedOut || fLockState == IAMLockable::enLocked )
		{
			isLocked =  kTrue;
		}
	}

	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "IsLocked = %s\n",(isLocked?"true":"false"));
	
	return isLocked;
}
#endif

//----------------------------------------------------------------------------------------
// HasLock
//----------------------------------------------------------------------------------------
#if defined(InDnCS5) || defined(InDnCS5_5)
bool16
CZPAMLockable::HasLock(
	UID objectUID)	//TODO: CS5: Implement functionality
#else
bool16
CZPAMLockable::HasLock()
#endif
{
	LogFunctionEnterExit;
	
	bool16 hasLock = kFalse;

#ifdef DEBUG
	IAMLockable::LockState oldCachedState = fLockState;
#endif
	//if ( fLockState == IAMLockable::enUndefined )
	
	// not yet initialized
#if defined(InDnCS5) || defined(InDnCS5_5)
	VerifyState( kSuppressUI, objectUID );
#else
	VerifyState( kSuppressUI );	
#endif

#ifdef DEBUG
	if( fLockState != oldCachedState )
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Lock state changed from %d to %d", oldCachedState, fLockState);
#endif	
	
	if ( fLockState == IAMLockable::enCheckedOut )
	{
		hasLock = kTrue;
	}
	
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "HasLock = %s\n",(hasLock?"true":"false"));
	
	return hasLock;
}

//----------------------------------------------------------------------------------------
// GetLockState
//----------------------------------------------------------------------------------------
#if defined(InDnCS5) || defined(InDnCS5_5)
IAMLockable::LockState
CZPAMLockable::GetLockState(
	UID objectUID)	//TODO: CS5: Implement functionality
#else
IAMLockable::LockState
CZPAMLockable::GetLockState()
#endif
{
	//LogFunctionEnterExit;
#ifdef DEBUG
	IAMLockable::LockState oldCachedState = fLockState;
#endif
	//if ( fLockState == IAMLockable::enUndefined || fLockState  == IAMLockable::enUnavailable )
	
	// not yet initialized
#if defined(InDnCS5) || defined(InDnCS5_5)
	VerifyState( kSuppressUI, objectUID );
#else
	VerifyState( kSuppressUI );	
#endif

#ifdef DEBUG
	if( fLockState != oldCachedState )
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Lock state changed from %d to %d", oldCachedState, fLockState);
#endif	
	return fLockState;
}

//----------------------------------------------------------------------------------------
// Lock
//----------------------------------------------------------------------------------------
#if defined(InDnCS5) || defined(InDnCS5_5)
bool16
CZPAMLockable::Lock(
	UIFlags uiFlags, bool16 allowOverride, const PMString *checkedOutTo, UID objectUID)	//TODO: CS5: Implement functionality
#else
bool16
CZPAMLockable::Lock(
	UIFlags uiFlags, bool16 allowOverride, const PMString *checkedOutTo )
#endif
{
	LogFunctionEnterExit;
	
	bool16 lockSucceeded = kFalse;
	
	do
	{
		InterfacePtr<IManageableAsset> asset( this->QueryAsset());
		ASSERT(asset);
		if( !asset )
			break;
		ClassID amProviderClass = asset->GetServiceProviderClass();
		lockSucceeded = Utils<IZPAMHelper>()->CallAMServiceDo( asset, amProviderClass, IAMServiceProvider::enCheckOut );
		
#if defined(InDnCS5) || defined(InDnCS5_5)
		VerifyState( kSuppressUI, objectUID );
#else
		VerifyState( kSuppressUI );			
#endif
		
	}while (kFalse);
	
	return lockSucceeded;
}

//----------------------------------------------------------------------------------------
// Relock
//----------------------------------------------------------------------------------------
#if defined(InDnCS5) || defined(InDnCS5_5)
bool16
CZPAMLockable::Relock(
	UIFlags uiFlags, bool16 allowOverride, UID objectUID)	//TODO: CS5: Implement functionality
#else
bool16
CZPAMLockable::Relock( UIFlags uiFlags, bool16 allowOverride )
#endif
{
	LogFunctionEnterExit;
	
	bool16 lockSucceeded = kFalse;
	
	do
	{
		InterfacePtr<IManagedStatus> managedStatus( this, UseDefaultIID() );
		if ( !managedStatus )
			break;
		
#if defined(InDnCS5) || defined(InDnCS5_5)
		this->VerifyState( kSuppressUI, objectUID);
#else
		this->VerifyState();
#endif
	
		
		//if ( fLockState == IAMLockable::enLocked || fLockState == IAMLockable::enCheckedOut )
		{
			UIDRef storyRef = Utils<Facade::IWorkgroupStoryFacade>()->GetStoryUIDRef( managedStatus->GetAsset() );
			
			IZPLog_Str_( thisFileLA, enLT_DebugInfo, "story UID=0x%08X", storyRef.GetUID().Get() );
			
			//
			// Relock is called when the user updates a checked-out link
			// or when a doc with locked stories is opened
			//
			// In both cases the story is already locked but the metadata or story UID associated
			//  with the asset may have changed.  Relock gives you a chance to update this metadata
			//
			// We have no such metadata in this example so we do nothing but verify
			//  that the asset is indeed already locked
			
			InterfacePtr<IManageableAsset> asset( this->QueryAsset());
			ASSERT(asset);
			if( !asset )
				break;
#if 0		//Comment otherwise assignment acquire lock for all stories irrespective of prefs setting
			ClassID amProviderClass = asset->GetServiceProviderClass();
			lockSucceeded = Utils<IZPAMHelper>()->CallAMServiceDo( asset, amProviderClass, IAMServiceProvider::enCheckOut );
			VerifyState( kSuppressUI );
#else
			lockSucceeded = true;
#endif
		}
	}while (kFalse);
	
	return lockSucceeded;
}

//----------------------------------------------------------------------------------------
// Unlock
//----------------------------------------------------------------------------------------
#if defined(InDnCS5) || defined(InDnCS5_5)
bool16
CZPAMLockable::Unlock(
	UIFlags uiFlags, bool16 allowOverride, UID objectUID)	//TODO: CS5: Implement functionality
#else
bool16
CZPAMLockable::Unlock( UIFlags uiFlags, bool16 allowOverride )
#endif
{
	LogFunctionEnterExit;
	
	bool16 unlockSucceeded = kFalse;
	
	do
	{
		InterfacePtr<IManageableAsset> asset( this->QueryAsset());
		ASSERT(asset);
		
		if( !asset )
			break;
		ClassID amProviderClass = asset->GetServiceProviderClass();
		unlockSucceeded = Utils<IZPAMHelper>()->CallAMServiceDo( asset, amProviderClass, IAMServiceProvider::enCancelCheckOut );

#if defined(InDnCS5) || defined(InDnCS5_5)
		VerifyState( kSuppressUI, objectUID );
#else
		VerifyState( kSuppressUI );
#endif
		
		unlockSucceeded = fLockState != IAMLockable::enCheckedOut;
	}while (kFalse);
	
	return unlockSucceeded;
}

//----------------------------------------------------------------------------------------
// VerifyState
//----------------------------------------------------------------------------------------
#if defined(InDnCS5) || defined(InDnCS5_5)
IAMLockable::LockState 
CZPAMLockable::VerifyState(
	UIFlags uiFlags, UID objectUID)	//TODO: CS5: Implement functionality
#else
IAMLockable::LockState
CZPAMLockable::VerifyState( UIFlags uiFlags )
#endif
{
	LogFunctionEnterExit;
	
	do
	{
		fCheckedOutBy	= kNullString;
		fCheckedOutDoc	= kNullString;
		fCheckedOutApp	= kNullString;

		const IStringData * dbAsset = this->GetDBAsset();
		if( !dbAsset )
		{
			fLockState = IAMLockable::enUndefined;
			break;
		}
		
		InterfacePtr<const IZPAssetLockInfo> dbAssetLockInfo( dbAsset, UseDefaultIID());
		ASSERT( dbAssetLockInfo );
		
		fLockState		= dbAssetLockInfo->GetLockState();		
		fCheckedOutDoc	= dbAssetLockInfo->GetDocumentName();
		fCheckedOutApp	= dbAssetLockInfo->GetAppName();
		fCheckedOutBy	= dbAssetLockInfo->GetUserName();
#if defined(InDnCS5) || defined(InDnCS5_5)
		fObjectUID		= objectUID;
#endif

		#ifdef DEBUG
			//InterfacePtr<const IStringData> dbAssetName( dbAsset, IID_IZPDATANAME);
			//IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Asset Name = %s\n", dbAssetName->Get().GrabCString() );
		#endif
	} while (kFalse);
	
#ifdef DEBUG
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "state = %s\n", gLockStates[fLockState] );
#endif
	
	return fLockState;
}

#if defined(InDnCS5) || defined(InDnCS5_5)
//----------------------------------------------------------------------------------------
// GetLockObjectUID
//----------------------------------------------------------------------------------------
UID CZPAMLockable::GetLockObjectUID()
{
	LogFunctionEnterExit;
	//TODO: CS5: Implement functionality
	UID objectUID = kInvalidUID;
	InterfacePtr<IManagedStatus> managedStatus( this, UseDefaultIID() );
	ASSERT( managedStatus );
	if( managedStatus )
	{
		UIDRef  assetRef(managedStatus->GetAsset());
		objectUID = assetRef.GetUID();
	}

	return objectUID;
}
#endif

//----------------------------------------------------------------------------------------
// GetDBAsset
//----------------------------------------------------------------------------------------
const IStringData *
CZPAMLockable::GetDBAsset()
{
	//LogFunctionEnterExit;
	InterfacePtr<const IStringData> selfAssetID( this, IID_IZPDATAID );
	ASSERT( selfAssetID );

	const PMString & assetIDStr = selfAssetID->Get();
	const IStringData * toReturn = ZPDataHelper::GetAssetByID( assetIDStr );
	return toReturn;
}

//----------------------------------------------------------------------------------------
// QueryAsset
//----------------------------------------------------------------------------------------
IManageableAsset *
CZPAMLockable::QueryAsset()
{
	//LogFunctionEnterExit;
	InterfacePtr<IManagedStatus> managedStatus( this, UseDefaultIID() );
	ASSERT( managedStatus );
	if( managedStatus )
	{
		UIDRef  assetRef(managedStatus->GetAsset());
		
		InterfacePtr<IManageableAsset> asset(assetRef,UseDefaultIID());
		return asset.forget();
	}
	return nil;
}
