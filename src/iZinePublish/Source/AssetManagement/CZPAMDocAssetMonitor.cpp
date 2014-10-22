//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/CZPAMDocAssetMonitor.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 2-8-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IAMLockable.h"
#include "IAssignmentMgr.h"
#include "IDocument.h"
#include "IManageableAsset.h"
#include "IManagedStatus.h"
#include "IStringData.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes
#include "Utils.h"
#ifdef InDnCS3
#include "LinksPanelID.h"
#endif

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPAMHelper.h"
#include "IZPAssetData.h"
#include "IZPFileUtils.h"

//IZP General includes
#include "AZPAMAssetMonitor.h"
#include "ZPDataHelper.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AssetMonitor
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass AZPAMAssetMonitor
class CZPAMDocAssetMonitor : public inheritClass
{
public:
						CZPAMDocAssetMonitor(IPMUnknown * boss);
	virtual				~CZPAMDocAssetMonitor();

	bool				ProcessAsset(
							IManageableAsset *			inAsset);

	bool16				CanMonitor( IManageableAsset *	asset );
protected:
#if defined(InDnCS5) || defined(InDnCS5_5)
	RsrcID				GetIconForLinkState(
							const ILinkResource::ResourceState	inLinkState ) const;	
#else
	RsrcID				GetIconForLinkState(
							const IDataLink::StateType	inLinkState ) const;
#endif
	RsrcID				GetIconForVersionState(
							const IManagedStatus::VersionState inVersionState ) const;

	const char *		GetAssetMonitorName( ) const;
private:
};


CREATE_PMINTERFACE(CZPAMDocAssetMonitor, kZPAMDocAssetMonitorImpl)

CZPAMDocAssetMonitor::CZPAMDocAssetMonitor(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMDocAssetMonitor::~CZPAMDocAssetMonitor()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetAssetMonitorName
//----------------------------------------------------------------------------------------
const char *
CZPAMDocAssetMonitor::GetAssetMonitorName() const
{
	return "Document";
}

//----------------------------------------------------------------------------------------
// CanMonitor
//----------------------------------------------------------------------------------------
bool16
CZPAMDocAssetMonitor::CanMonitor(
	IManageableAsset * pAsset)
{
	LogFunctionEnterExit;
	bool16 toReturn = inheritClass::CanMonitor( pAsset );
	if( toReturn )
	{	//Check if this document is ours.
		InterfacePtr<const IZPAssetData> assetData( pAsset, UseDefaultIID());
		if( assetData->GetAssetID().IsEmpty() )		//Handle only if our Asset Data is set.
			toReturn = kFalse;
	}

	return toReturn;
}

//----------------------------------------------------------------------------------------
// ProcessAsset
//----------------------------------------------------------------------------------------
bool
CZPAMDocAssetMonitor::ProcessAsset(
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

		InterfacePtr<IDocument> assetDoc( inAsset, UseDefaultIID() );
		if( !assetDoc )
			break;

		// IWorkgroupFacade::GetLockState calls your IAMLockable::VerifyState (because we're passing kTrue)
		//  then IAMLockable::GetLockState() to get the state of the link
		//const int32 currentLockState = Utils<Facade::IWorkgroupFacade>()->GetLockState( assetRef, kTrue, kSuppressUI );

		InterfacePtr<IManagedStatus> documentStatus( inAsset->GetStatus(), UseDefaultIID() );
		InterfacePtr<IAMLockable> documentLock( documentStatus, UseDefaultIID() );
		ASSERT(documentLock);
		if ( !documentLock )
			break; //Should we remove such assets from monitoring list?

#if defined(InDnCS5) || defined(InDnCS5_5)
		documentLock->VerifyState(kSuppressUI, documentLock->GetLockObjectUID());
#else
		documentLock->VerifyState();
#endif

		// update the link's IManagedStatus strings
		//  These will be used by the app UI after we've invalidated the adornments
		bool16 isOnLine = documentStatus->IsOnline();
		IManagedStatus::EditingState editingState = documentStatus->GetEditingState();
		IManagedStatus::VersionState versionState = documentStatus->GetVersionState();
		IManagedStatus::StatusStringTable statusStrings;
		IManagedStatus::StatusIconTable statusIcons;
		documentStatus->GetDisplayStatusData( statusStrings, statusIcons );
		documentLock->GetLockStatusStrings( statusStrings );
		
		InterfacePtr<const IStringData> assetID( documentStatus, IID_IZPDATAID );
		ASSERT( assetID );
		const IStringData * dbAssetID = ZPDataHelper::GetAssetByID( assetID->Get() );

#if defined(InDnCS5) || defined(InDnCS5_5)
		IAMLockable::LockState lockState = documentLock->GetLockState(documentLock->GetLockObjectUID());
#else
		IAMLockable::LockState lockState = documentLock->GetLockState();
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

		documentStatus->Update( isOnLine, newEditingState, newVersionState, statusStrings, statusIcons );

		bool needsUpdate = editingState != newEditingState || versionState != newVersionState;

		//Should we update the link state?
		//kLinkNormal  The link is up-to-date and not missing.  
		//kLinkOutOfDate  The link has been modified but not updated.  
		//kLinkMissing  The link could not be found.  
		//kEmbedded  The file has been embedded in the InDesign document 
		// update the story bars in galley view (InCopy)
		
		IDataBase * db = ::GetDataBase( inAsset );

		//this->InValidateGalleyLines( storyDoc );

		//TODO: Check the Document title & Asset Name, set title if name is not matching.

		if( needsUpdate )
		{
			//Patch: change local file modification date, so that assignment panel shows out-of-date status.
			if( newVersionState == IManagedStatus::enLocalIsOutOfDate && versionState != IManagedStatus::enLocalIsOutOfDate )
			{
				IDFile assetFile;
				if( inAsset->GetWriteFile(assetFile) )
					Utils<IZPFileUtils>()->TouchFile( assetFile );
			}

			//InterfacePtr<IDataLink> assetDL( inAsset, UseDefaultIID() );
			//ASSERT( assetDL );
			//IDataLink::StateType oldLinkState = assetDL->GetStoredState( nil, nil );
			
			//ICommand * refreshLinkCmd = Utils<IZPCommandFacade>()->CreateCommand_DataLinkRefresh( assetDL, newLinkState );
			//fPendingCommands.push_back( refreshLinkCmd );

			InterfacePtr<IAssignmentMgr> assMgr(gSession, UseDefaultIID());
			ASSERT(assMgr);
			assMgr->NotifyPanel(kAssignmentPanelInvalidateNotifyMsg);

			//UIDRef assetRef = ::GetUIDRef( inAsset );
			//UIDRef storyRef = Utils<Facade::IWorkgroupStoryFacade>()->GetStoryUIDRef( assetRef );

			//this->NotifyDocObservers( storyDoc, storyRef );

			// if the state of the story lock has changed, we should update our lock state.
			//this->UpdateTextModelLock( storyRef, lockState );
		}
	}while(kFalse);

	return true;
}

//----------------------------------------------------------------------------------------
// GetIconForLinkState
//----------------------------------------------------------------------------------------
#if defined(InDnCS5) || defined(InDnCS5_5)
RsrcID
CZPAMDocAssetMonitor::GetIconForLinkState(
	const ILinkResource::ResourceState	inLinkState ) const
#else
RsrcID
CZPAMDocAssetMonitor::GetIconForLinkState(
	const IDataLink::StateType	inLinkState) const
#endif
{
	//LogFunctionEnterExit;
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
CZPAMDocAssetMonitor::GetIconForVersionState(
	const IManagedStatus::VersionState inVersionState) const
{
	//LogFunctionEnterExit;
	RsrcID theStatusIcon = kZPLinkedStoryState_MissingIconRsrcID;
	if( inVersionState == IManagedStatus::enLocalIsOutOfDate || inVersionState == IManagedStatus::enVersionConflict )
		theStatusIcon = kZPLinkedStoryState_TextOutOfDateIconRsrcID;
	else
		theStatusIcon = kZPLinkedStoryState_TextNormalIconRsrcID;

	return theStatusIcon;
}

