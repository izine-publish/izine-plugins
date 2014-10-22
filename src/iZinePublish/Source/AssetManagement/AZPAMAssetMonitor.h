//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/AZPAMAssetMonitor.h $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 21-7-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_AZPAMAssetMonitor_
#define _h_AZPAMAssetMonitor_
#pragma once

#include "IAssetMonitor.h"
#include "IDataLink.h"
#include "IManagedStatus.h"
#include "ZPTypes.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "ILinkResource.h"
#endif

#define inheritClass_ZPAMAssetMonitor CPMUnknown<IAssetMonitor>
class AZPAMAssetMonitor : public inheritClass_ZPAMAssetMonitor
{
public:
	virtual				~AZPAMAssetMonitor();

	/** This method tests if an asset has moved or has been renamed.
		@param asset is a valid pointer to a bound IManageableAsset instance.
		@return kTrue if the asset has been moved or rename; kFalse otherwise.
	*/
	virtual bool16 AssetHasMoved( IManageableAsset * asset );
	
	/** This method does any initialization needed before monitoring begins. This method is called
		once during app startup.
		@return kTrue if the asset monitor is OK for use; kFalse if it may not be used.
	*/
	virtual bool16 StartUp();
	
	/** This method performs any termination needed after monitoring ends. This method is called once
		during app shutdown.
		@return kTrue if shutdown succeeded; kFalse if there was an error.
	*/
	virtual bool16 ShutDown();
	
	/** This method tests if the asset monitor is able to monitor a file for status changes.
		@param file is an IDFile with a path to an existing file.
		@return kTrue if the file can be monitored; kFalse otherwise.
	*/
	virtual bool16 CanMonitor( const IDFile & file );
	
	/** This method tests if the asset monitor is able to monitor an IManageableAsset for status changes.
		@param asset is a valid pointer to an IManageableAsset instance.
		@return kTrue if the asset can be monitored; kFalse otherwise.
	*/	
	virtual bool16 CanMonitor( IManageableAsset * asset );
	
	/** This method provides an ICommand that can be scheduled to process asset status changes. May return
		nil if there are no pending changes to process. An ICommand should be used to process status changes
		if the status processing displays modal dialogs (such as for alerts).
		@return a valid pointer to an ICommand object or nil if there are no changes to process.
	*/
	virtual ICommand * GetHandleChangesCommand();
	
	/** This method retrieves the next pending IManagedStatus instance to process.
		@return a valid pointer to an IManagedStatus instance or nil if there are no remaining instances
			to process.
	*/
	virtual IManagedStatus * GetNextPendingStatus();
	
	/** This method tests if an asset has a pending status that has not yet been processed. The StartWatching()
		method must be called for the asset before this method is called.
		@param asset is an IManageableAsset that is being monitored.
		@return kTrue if the asset has unprocessed status pending; kFalse otherwise. 
	*/
	virtual bool16 IsStatusPending( IManageableAsset * asset );
	
	/** This method is called from the idle task in order to give the asset monitor implementation processing
		time. This method should not display UI or otherwise interrupt the idle loop processing.
		@return the callback interval (for future use).
		@see IIdleTask::RunTask.
	*/
	virtual uint32 DoBackgroundTasks();
		
	/** This method starts monitoring for status changes for an asset. Changes to IManagedStatus
		and IAMLockable states are typically monitored.
		@param assetRef is the UIDRef of a bound IManageableAsset.
		@return kSuccess if monitoring the asset has started; otherwise returns a failure ErrorCode.
	*/
	virtual ErrorCode StartMonitoring( UIDRef assetRef );

	/** This method stops monitoring for status changes for an asset.
		@param assetRef is the UIDRef of a bound IManageableAsset.
		@return kSuccess if monitoring the asset has stopped; otherwise returns a failure ErrorCode.
	*/
	virtual ErrorCode StopMonitoring( UIDRef assetRef );
	
	/** This method tests if an asset is currently being monitored for status changes.
		@param assetRef is the UIDRef of a bound IManageableAsset.
		@return kTrue if the asset is being monitored; kFalse otherwise.
	*/
	virtual bool16 IsMonitoring( UIDRef assetRef );

	virtual void		RefreshNow();

	virtual void		AddUpdatedAssetId(
							const PMString &			inAssetID);

protected:
						AZPAMAssetMonitor(IPMUnknown * boss);

	virtual	bool		ProcessNextAsset();	//Returns true if more assets are pending.
	virtual bool		ProcessAsset(
							IManageableAsset *			inAsset );

	virtual void		RemovePendingCommandForUIDRef(
							const UIDRef &				inAssetRef );

	virtual void		FetchInfoFromServer();	//Fetch lock & version info for monitored assets.

	virtual void		GetMonitoredAssetIDs(
							ZPAssetIDsList &			oAssetIDs ) const;

	virtual const PMString & GetAssetID(
							const UIDRef &				inAssetUIDRef ) const;

#if defined(InDnCS5) || defined(InDnCS5_5)
	virtual ILinkResource::ResourceState CalculateLinkState(
							IManagedStatus::VersionState	inVersionState ) const;	
#else
	virtual IDataLink::StateType CalculateLinkState(
							IManagedStatus::VersionState	inVersionState ) const;
#endif

	virtual const char *	GetAssetMonitorName( ) const = 0;	//Name used for debugging.

	virtual void		ListenToDBAsset(
							const UIDRef &				inAssetUIDRef );

	virtual void		StopListenToDBAsset(
							const UIDRef &				inAssetUIDRef );

private:

protected:
	typedef std::map<PMString, UIDRef>					ZPAssetIdUIDRefMap;
	typedef ZPAssetIdUIDRefMap::const_iterator			ZPAssetIdUIDRefMapConstIter;

	K2Vector<UIDRef>	fActiveList;	// UIDRefs that are being monitored for status change. 
	K2Vector<ICommand *>	fPendingCommands;	//Commands to process one by one.

	ZPAssetIdUIDRefMap		mAssetIdUIDRefMap;	//Stores AssetID and UIDRef, so one asset can be used once only
	//List of asset ID which needs to update document. Mostly when server info change is received, asssetID it is added here
	K2Vector<PMString>		mPendingAssetToUpdate;

	int			mCurrentIndex;	//We process one asset at a time. Check the list size each time before using this value.
	time_t		mLastProcessingTime;
	bool		mResetIndexPending;
};

#endif //_h_AZPAMAssetMonitor_
