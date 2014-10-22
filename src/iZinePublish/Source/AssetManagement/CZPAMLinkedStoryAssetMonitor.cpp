//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/CZPAMLinkedStoryAssetMonitor.cpp $
//	$Revision: 4096 $
//	$Date: 2013-04-01 09:20:00 +0200 (Mon, 01 Apr 2013) $
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
#include "IAssetMonitor.h"
#include "IAssignmentMgr.h"
#include "IBoolData.h"
#include "IGalleyUtils.h"
#include "IInCopyBridgeUtils.h"
#include "IItemLockData.h"
#include "ILayoutUtils.h"
#include "IStoryNeedsRelinkData.h"
#include "IStringData.h"
#include "ISubject.h"
#include "ITextLines.h"
#include "IWorkgroupFacade.h"
#include "IWorkgroupStoryFacade.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#include "ILiveEditFacade.h"
#include "FileUtils.h"
#endif

//SDK General includes
#include "LazyNotificationData.h"
#if defined(InDnCS5) || defined(InDnCS5_5)	//TODO: CS5 Porting
#include "LinksID.h"
#else
#include "LinksPanelID.h"
#endif
#include "ListLazyNotificationData.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPAMHelper.h"
#include "IZPCommandFacade.h"
#include "IZPFileUtils.h"
#include "IZPStoryUtils.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IZPInDnUtils.h"
#endif

//IZP General includes
#include "AZPAMAssetMonitor.h"
#include "StInDesignUtils.h"
#include "ZPDataHelper.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AssetMonitor
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass AZPAMAssetMonitor
class CZPAMLinkedStoryAssetMonitor : public inheritClass
{
public:
						CZPAMLinkedStoryAssetMonitor(IPMUnknown * boss);
	virtual				~CZPAMLinkedStoryAssetMonitor();
protected:
	bool				ProcessAsset(
							IManageableAsset *			inAsset);

	void				InValidateGalleyLines(
							IDocument *					inStoryDoc );

	bool16				CanMonitor( IManageableAsset *	asset );

#if defined(InDnCS5) || defined(InDnCS5_5)
	RsrcID				GetIconForLinkState(
							const ILinkResource::ResourceState	inLinkState ) const;	
#else
	RsrcID				GetIconForLinkState(
							const IDataLink::StateType	inLinkState ) const;
#endif
	RsrcID				GetIconForVersionState(
							const IManagedStatus::VersionState inVersionState ) const;

	void				NotifyDocObservers(
							const IDocument*			inDocument,
							UIDRef						inStoryRef) const;
	void				UpdateTextModelLock(
							UIDRef						inStoryRef,
							IAMLockable::LockState		inLockState ) const;
	
	const char *		GetAssetMonitorName( ) const;
#if defined(InDnCS5) || defined(InDnCS5_5)
	ErrorCode			StopMonitoring(UIDRef assetRef);
#endif
private:
};


CREATE_PMINTERFACE(CZPAMLinkedStoryAssetMonitor, kZPAMLinkedStoryAssetMonitorImpl)

CZPAMLinkedStoryAssetMonitor::CZPAMLinkedStoryAssetMonitor(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMLinkedStoryAssetMonitor::~CZPAMLinkedStoryAssetMonitor()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetAssetMonitorName
//----------------------------------------------------------------------------------------
const char *
CZPAMLinkedStoryAssetMonitor::GetAssetMonitorName() const
{
	return "Linked Story";
}

//----------------------------------------------------------------------------------------
// CanMonitor
//----------------------------------------------------------------------------------------
bool16
CZPAMLinkedStoryAssetMonitor::CanMonitor(
	IManageableAsset * pAsset)
{
	bool16 toReturn = inheritClass::CanMonitor( pAsset );
	if( toReturn )
	{	
		IDataBase * db = ::GetDataBase( pAsset );
		InterfacePtr<IDocument> storyDoc( db, db->GetRootUID(), UseDefaultIID() );
		ASSERT( storyDoc );
		
		if( !storyDoc || ::GetClass( storyDoc ) != kDocBoss )
		{	//Don't process this as it is not on a normal document, but on scrap document.
			toReturn = kFalse;
		}
		else
		{	//Check if this link is from any replica folder.
			IDFile linkedFilePath;
#if defined(InDnCS5) || defined(InDnCS5_5)
			InterfacePtr<ILinkResource> assetLinkResource( pAsset, UseDefaultIID() );
			ASSERT( assetLinkResource );
			if( !Utils<IZPStoryUtils>()->GetLinkResourceFileLocation( assetLinkResource, linkedFilePath ) //Don't use asset write file
#else
			InterfacePtr<IDataLink> assetDL( pAsset, UseDefaultIID() );
			ASSERT( assetDL );
			if( !Utils<IZPStoryUtils>()->GetDataLinkFileLocation( assetDL, linkedFilePath ) //Don't use asset write file
#endif
			|| ( !Utils<IZPAMHelper>()->IsPathOfAnyReplicaFolder( linkedFilePath ) ) )
			{
				toReturn = kFalse;
			}
#if defined(InDnCS5) || defined(InDnCS5_5)		//update the stored asset ref if required; UpdatestoredassetRef service automatically called in CS3, but not in CS5
			if(toReturn)
			{
				IDFile newFilePath;			
				PMString oldPath, newPath;
				
				const PMString & assetId = this->GetAssetID(::GetUIDRef( pAsset ));
				const IStringData * dbAssetID = ZPDataHelper::GetAssetByID( assetId );
				Utils<IZPAMHelper>()->GetAssetFilePath( dbAssetID, newFilePath );
				
				FileUtils::IDFileToPMString(linkedFilePath, oldPath);
				FileUtils::IDFileToPMString(newFilePath, newPath);
				
				//if( !oldPath.IsEqual(newPath, kFalse) && Utils<IZPInDnUtils>()->IsHostAppInCopy())
				if( !oldPath.IsEqual(newPath, kFalse)  && Utils<IZPInDnUtils>()->IsHostAppInCopy())
					Utils<Facade::IWorkgroupFacade>()->UpdateStoredAssetReference(::GetUIDRef( pAsset ));
			}
#endif			
		}
	}

	return toReturn;
}

//----------------------------------------------------------------------------------------
// ProcessAsset
//----------------------------------------------------------------------------------------
bool
CZPAMLinkedStoryAssetMonitor::ProcessAsset(
	IManageableAsset *			inAsset)
{
	//Check Current editing state and update the Managed status.
	//Also check the local version & remote version status.
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
	const IExecutionContext* ec = GetExecutionContext();
	ISession* gSession = ec->GetSession();
#endif
	do
	{
		if( !inAsset )
			break;

		IDataBase * db = ::GetDataBase( inAsset );
		InterfacePtr<IDocument> storyDoc( db, db->GetRootUID(), UseDefaultIID() );
		if( !storyDoc )
			break;

		if( ::GetClass( storyDoc ) != kDocBoss )	//Don't process this as it is not on a normal document, but on scrap document.
			break;

		// IWorkgroupFacade::GetLockState calls your IAMLockable::VerifyState (because we're passing kTrue)
		//  then IAMLockable::GetLockState() to get the state of the link
		//const int32 currentLockState = Utils<Facade::IWorkgroupFacade>()->GetLockState( assetRef, kTrue, kSuppressUI );

		InterfacePtr<IManagedStatus> storyStatus( inAsset->GetStatus(), UseDefaultIID() );
		InterfacePtr<IAMLockable> storyLock( storyStatus, UseDefaultIID() );
		ASSERT(storyLock);
		if ( !storyLock )
			break; //Should we remove such assets from monitoring list?

#if defined(InDnCS5) || defined(InDnCS5_5)
		storyLock->VerifyState(kSuppressUI, storyLock->GetLockObjectUID());
#else
		storyLock->VerifyState();
#endif


		// update the link's IManagedStatus strings
		//  These will be used by the app UI after we've invalidated the adornments
		bool16 isOnLine = storyStatus->IsOnline();
		IManagedStatus::EditingState editingState = storyStatus->GetEditingState();
		IManagedStatus::VersionState versionState = storyStatus->GetVersionState();
		IManagedStatus::StatusStringTable statusStrings;
		IManagedStatus::StatusIconTable statusIcons;
		storyStatus->GetDisplayStatusData( statusStrings, statusIcons );
		storyLock->GetLockStatusStrings( statusStrings );
		
		InterfacePtr<const IStringData> assetID( storyStatus, IID_IZPDATAID );
		ASSERT( assetID );
		const IStringData * dbAssetID = ZPDataHelper::GetAssetByID( assetID->Get() );

#if defined(InDnCS5) || defined(InDnCS5_5)
		IAMLockable::LockState lockState = storyLock->GetLockState(storyLock->GetLockObjectUID());
#else
		IAMLockable::LockState lockState = storyLock->GetLockState();
#endif

		IManagedStatus::VersionState newVersionState = Utils<IZPAMHelper>()->GetAssetVersionState( dbAssetID );
		IManagedStatus::EditingState newEditingState = Utils<IZPAMHelper>()->GetEditingState( lockState, newVersionState );

#if defined(InDnCS5) || defined(InDnCS5_5)
		ILinkResource::ResourceState newLinkState = this->CalculateLinkState( versionState );
#else
		IDataLink::StateType newLinkState = this->CalculateLinkState( versionState );
#endif
		//RsrcID newStatusIcon = this->GetIconForVersionState( newVersionState );
		RsrcID newStatusIcon = this->GetIconForLinkState( newLinkState );
		//statusIcons[IManagedStatus::enContent] = newStatusIcon;

		
		StRetainModifiedFlag retainDirtyFlag( db );	//Don't mark document dirty if lock/version state changes.

		storyStatus->Update( isOnLine, newEditingState, newVersionState, statusStrings, statusIcons );

		bool needsUpdate = editingState != newEditingState || versionState != newVersionState;

		//Should we update the link state?
		//kLinkNormal  The link is up-to-date and not missing.  
		//kLinkOutOfDate  The link has been modified but not updated.  
		//kLinkMissing  The link could not be found.  
		//kEmbedded  The file has been embedded in the InDesign document 
		// update the story bars in galley view (InCopy)
		
		
		
		this->InValidateGalleyLines( storyDoc );

		if( needsUpdate )
		{
			//Patch: change local file modification date, so that assignment panel shows out-of-date status.
			if( newVersionState == IManagedStatus::enLocalIsOutOfDate && versionState != IManagedStatus::enLocalIsOutOfDate )
			{
				IDFile assetFile;
				if( inAsset->GetWriteFile(assetFile) )
					Utils<IZPFileUtils>()->TouchFile( assetFile );
			}
#ifdef InDnCS3
			InterfacePtr<IDataLink> assetDL( inAsset, UseDefaultIID() );
			ASSERT( assetDL );
			IDataLink::StateType oldLinkState = assetDL->GetStoredState( nil, nil );
#endif

#if 0		//TODO: This is causing random crash. Find a solutions for this.
			ICommand * refreshLinkCmd = Utils<IZPCommandFacade>()->CreateCommand_DataLinkRefresh( assetDL, newLinkState );
			UIDList itemList(::GetUIDRef(storyDoc));
			refreshLinkCmd->SetItemList( itemList );
			
			fPendingCommands.push_back( refreshLinkCmd );
#endif

			UIDRef assetRef = ::GetUIDRef( inAsset );
			UIDRef storyRef = Utils<Facade::IWorkgroupStoryFacade>()->GetStoryUIDRef( assetRef );

			// if the state of the story lock has changed, we should update our lock state.
			this->UpdateTextModelLock( storyRef, lockState );
#if defined(InDnCS5) || defined(InDnCS5_5)
			UIDList itemList(assetRef.GetDataBase());
			itemList.Append(storyRef.GetUID());
			Utils<Facade::ILiveEditFacade>()->SynchronizeLocks(itemList);
#else
			Utils<Facade::IWorkgroupStoryFacade>()->SyncStoryPIAccessToStoryLock( storyRef );
#endif


			this->NotifyDocObservers( storyDoc, storyRef );

			//CmdUtils::ProcessCommand( refreshLinkCmd );
			//refreshLinkCmd->Release();

			//Update Assignments panel
			InterfacePtr<IAssignmentMgr> assMgr(gSession, UseDefaultIID());
			ASSERT(assMgr);
			assMgr->NotifyPanel(kAssignmentPanelInvalidateNotifyMsg);
		}
	}while(kFalse);

	return true;
}

//----------------------------------------------------------------------------------------
// InValidateGalleyLines
//----------------------------------------------------------------------------------------
void
CZPAMLinkedStoryAssetMonitor::InValidateGalleyLines(
	IDocument *					inStoryDoc)
{
	LogFunctionEnterExit;
	InterfacePtr<IGalleyUtils> galleyUtils( Utils<IGalleyUtils>().QueryUtilInterface() );
	if ( galleyUtils )
	{
		IControlView *galleyView = galleyUtils->FindGalleyView( inStoryDoc );
		if( galleyView )
		{
			InterfacePtr<ITextLines> textLines( galleyView, ITextLines::kDefaultIID );
			if ( textLines )
				textLines->InvalidateLine(0);
		}
	}
}

//----------------------------------------------------------------------------------------
// GetIconForLinkState
//----------------------------------------------------------------------------------------
#if defined(InDnCS5) || defined(InDnCS5_5)
RsrcID
CZPAMLinkedStoryAssetMonitor::GetIconForLinkState(
	const ILinkResource::ResourceState	inLinkState) const
#else
RsrcID
CZPAMLinkedStoryAssetMonitor::GetIconForLinkState(
	const IDataLink::StateType	inLinkState) const
#endif
{
	//#define kEmbeddedStatusIconResID		1006
#if defined(InDnCS5) || defined(InDnCS5_5)
	RsrcID theStatusIcon = kLinksUnknownStatusIconResID;	//TODO: CS5: Implement functionality
	if( inLinkState == ILinkResource::kAvailable )
		theStatusIcon = kLinksStatusOKResID;
	else if( inLinkState == ILinkResource::kPending )
		theStatusIcon = kLinksOutOfDateStatusIconResID;
	else if( inLinkState == ILinkResource::kMissing )
		theStatusIcon = kLinksMissingStatusIconResID;
	else if( inLinkState == ILinkResource::kInaccessible)
		theStatusIcon = kLinksInaccessibleStatusIconResID;		
#else
	RsrcID theStatusIcon = kEmbeddedStatusIconResID;
	
	if( inLinkState == IDataLink::kLinkNormal )
		theStatusIcon = kStatusOKResID;
	else if( inLinkState == IDataLink::kLinkOutOfDate )
		theStatusIcon = kOutOfDateStatusIconResID;
	else if( inLinkState == IDataLink::kLinkMissing )
		theStatusIcon = kMissingStatusIconResID;
#endif

	return theStatusIcon;
}

//----------------------------------------------------------------------------------------
// GetIconForVersionState
//----------------------------------------------------------------------------------------
RsrcID
CZPAMLinkedStoryAssetMonitor::GetIconForVersionState(
	const IManagedStatus::VersionState inVersionState) const
{
	RsrcID theStatusIcon = kZPLinkedStoryState_MissingIconRsrcID;
	if( inVersionState == IManagedStatus::enLocalIsOutOfDate || inVersionState == IManagedStatus::enVersionConflict )
		theStatusIcon = kZPLinkedStoryState_TextOutOfDateIconRsrcID;
	else
		theStatusIcon = kZPLinkedStoryState_TextNormalIconRsrcID;

	return theStatusIcon;
}

//----------------------------------------------------------------------------------------
// NotifyDocObservers
//----------------------------------------------------------------------------------------
void
CZPAMLinkedStoryAssetMonitor::NotifyDocObservers(
	const IDocument*			inDocument,
	UIDRef						inStoryRef) const
{
	LogFunctionEnterExit;
#if 1
	Utils<ILayoutUtils>()->InvalidateViews((IDocument*) inDocument );
#else
	InterfacePtr<IStoryNeedsRelinkData> storyRelinkData( inStoryRef, IID_ISTORYNEEDSRELINK );
	IDataBase* storyDB = inStoryRef.GetDataBase();
	if ( inDocument && storyDB && storyRelinkData && ::GetDataBase( inDocument ) == storyDB )
	{
		InterfacePtr<ISubject> storySubject( inStoryRef, IID_ISUBJECT );
		InterfacePtr<ISubject> docSubject( inDocument, IID_ISUBJECT );
		if( docSubject && storySubject )
		{
			LazyNotificationData *cookie = new ListLazyNotificationData<UID>;
			ListLazyNotificationData<UID> *listCookie = static_cast<ListLazyNotificationData<UID> *>( cookie );
			listCookie->ItemChanged( inStoryRef.GetUID());	
			docSubject->ModelChange( ::GetClass(storySubject), IID_ISTORYNEEDSRELINK, nil, cookie );
		}

		//Update layout
		//> kRefreshCmdBoss @ kDocBoss (IID_IDATALINK)
		//...kLayoutWidgetBoss (IID_ILAYOUTPAGEITEMOBSERVER)
		//...kLayoutWidgetBoss (IID_ILAYOUTMASTERPAGEOBSERVER)
		//if( docSubject )
			//docSubject->Change( kRefreshCmdBoss, IID_IDATALINK );
		//	docSubject->Change( kSetItemLockDataCmdBoss, IID_IITEMLOCK_DOCUMENT );
	}
#endif
}

//----------------------------------------------------------------------------------------
// UpdateTextModelLock
//----------------------------------------------------------------------------------------
void
CZPAMLinkedStoryAssetMonitor::UpdateTextModelLock(
	UIDRef						inStoryRef,
	IAMLockable::LockState		inLockState) const
{
	LogFunctionEnterExit;

	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Story UID: 0x%X  Lock State: %d ", inStoryRef.GetUID().Get(), inLockState );
#if 1
	//Utils<IInCopyBridgeUtils>()->SetTextModelLock( inStoryRef );
#if defined(InDnCS5) || defined(InDnCS5_5)
	UIDList storyList(inStoryRef.GetDataBase());
	storyList.Append(inStoryRef.GetUID());
	Utils<Facade::ILiveEditFacade>()->SynchronizeLocks(storyList);
#else
	IDFile file;
	Utils<IInCopyBridgeUtils>()->SyncTextModelLock( inStoryRef, file );
#endif
#else
	//
	// Note that IItemLockData state is the opposite of IAMLockable
	// IAMLockable is locked when item is checked out (editable)
	// IItemLockData is locked when item is checked in (non-editable)
	bool16 locked;
	if ( inLockState == IAMLockable::enCheckedOut || inLockState == IAMLockable::enLocked )
		locked = kFalse;
	else
		locked = kTrue;
		
	InterfacePtr<IItemLockData> ild( inStoryRef, UseDefaultIID());
	bool16 storyLocked = ((ild != NULL) && ild->GetInsertLock(kFalse));
	
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Story locked: %hd, newLock: %hd, insert lock: %hd, attr lock: %hd ", storyLocked, locked, ild->GetInsertLock(), ild->GetAttributeLock() );

	if ( locked != storyLocked )
	{
		// story lock state must be changed
		InterfacePtr<ICommand>setStoryLockStateCmd( CmdUtils::CreateCommand(kSetItemLockDataCmdBoss) );
		setStoryLockStateCmd->SetUndoability( ICommand::kAutoUndo );
		setStoryLockStateCmd->SetItemList( UIDList(inStoryRef) );
		InterfacePtr<IBoolData> lockData( setStoryLockStateCmd, UseDefaultIID() );
		lockData->Set( locked );
	
		InterfacePtr<IBoolData> invalFlag( setStoryLockStateCmd, IID_IDOINVALFLAG );
		invalFlag->Set( kTrue );
		InterfacePtr<IBoolData> doNotify( setStoryLockStateCmd, IID_IDONOTIFYFLAG );
		doNotify->Set( kTrue );
	
		CmdUtils::ProcessCommand( setStoryLockStateCmd );
	}
	
	IDFile file;
	//Utils<IInCopyBridgeUtils>()->SetTextModelLock( inStoryRef );
	Utils<IInCopyBridgeUtils>()->SyncTextModelLock( inStoryRef, file );
#endif
}

#if defined(InDnCS5) || defined(InDnCS5_5)
ErrorCode CZPAMLinkedStoryAssetMonitor::StopMonitoring(UIDRef assetRef)
{
	ErrorCode err = kFailure;
	err = inheritClass::StopMonitoring(assetRef);
	return err;
}
#endif