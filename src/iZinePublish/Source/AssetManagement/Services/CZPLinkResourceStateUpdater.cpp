//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/Services/CZPLinkResourceStateUpdater.cpp $
//	$Revision: 3923 $
//	$Date: 2012-02-16 05:25:15 +0100 (Thu, 16 Feb 2012) $
//	$Author: aman.alam $
//	
//	Creator: Aman Alam
//	Created: 14-2-2012
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "ILinkManager.h"
#include "ILinkResourceStateUpdateCmdData.h"
#include "ILinkResourceStateUpdater.h"

//SDK General includes
#include "URI.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<ILinkResourceStateUpdater>
class CZPLinkResourceStateUpdater : public inheritClass
{
public:
						CZPLinkResourceStateUpdater(IPMUnknown * boss);
	virtual				~CZPLinkResourceStateUpdater();

	/**	
		Delegate to UpdateResourceState
		@see ILinkResourceHandler::UpdateResourceStateSync
	 */
	virtual ErrorCode UpdateResourceStateSync(const UIDRef& resourceRef, bool bNotify);
	/**	
		Delegate to UpdateResourceState
		@see ILinkResourceHandler::UpdateResourceStateAsync
	 */
	virtual ErrorCode UpdateResourceStateAsync(const UIDRef& resourceRef);

	/**	
		When you enqueue an AsyncWorkPacket, you supply an AsyncWorkPacketJobSpec that can identify
		your packet.  When link manager needs to cancel the resource update state request for whatever reason, 
		we search all outstanding AsyncWorkPackets for matches with the AsyncWorkPacketJobSpec when we enqueued it.
		Then we cancel the job.
		
		@see ILinkResourceHandler::CancelUpdateResourceState
	 */
	virtual void CancelUpdateResourceState(const UIDRef& resourceRef);

	/**	@see ILinkResourceHandler::ResolveResource
	 */
	virtual ErrorCode ResolveResource(const UIDRef& resourceRef, const URI& relativeURI, bool bIgnoreStamp);
protected:
private:

	// Prevent copy construction and assignment.
	CZPLinkResourceStateUpdater(const CZPLinkResourceStateUpdater&);
	CZPLinkResourceStateUpdater& operator=(const CZPLinkResourceStateUpdater&);

	ErrorCode UpdateResourceState(const UIDRef& resourceRef, bool bNotify, bool bSynchronous) const;
};


CREATE_PMINTERFACE(CZPLinkResourceStateUpdater, kZPLinkResourceStateUpdaterImpl)

CZPLinkResourceStateUpdater::CZPLinkResourceStateUpdater(IPMUnknown * boss)
: inheritClass(boss)
{
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPLinkResourceStateUpdater::~CZPLinkResourceStateUpdater()
{
}

#pragma mark -
//----------------------------------------------------------------------------------------
// UpdateResourceStateSync
//----------------------------------------------------------------------------------------
ErrorCode
CZPLinkResourceStateUpdater::UpdateResourceStateSync(
	const UIDRef& resourceRef, bool bNotify)
{
	return UpdateResourceState(resourceRef, bNotify, true);
}

//----------------------------------------------------------------------------------------
// UpdateResourceStateAsync
//----------------------------------------------------------------------------------------
ErrorCode
CZPLinkResourceStateUpdater::UpdateResourceStateAsync(
	const UIDRef& resourceRef)
{
	return UpdateResourceState(resourceRef, true, false);
}

//----------------------------------------------------------------------------------------
// CancelUpdateResourceState
//----------------------------------------------------------------------------------------
void
CZPLinkResourceStateUpdater::CancelUpdateResourceState(
	const UIDRef& resourceRef)
{
	IDataBase* db = resourceRef.GetDataBase();
	InterfacePtr<ILinkManager> iLinkMgr(db, db->GetRootUID(), UseDefaultIID());
	if (!iLinkMgr)
	{
		ASSERT_FAIL("CZPLinkResourceStateUpdater::CancelUpdateResourceState() - ILinkManager is nil!");
		return;
	}
	
	UID resourceUID = resourceRef.GetUID();
	
	// check to make sure we have a valid resource to queue
	InterfacePtr<ILinkResource> iResource(iLinkMgr->QueryResourceByUID(resourceUID));
	if (!iResource)
	{
		ASSERT_FAIL("CZPLinkResourceStateUpdater::CancelUpdateResourceState() - ILinkResource is nil!");
		return;
	}
	
	ClassID providerId = iLinkMgr->GetResourceProviderByScheme(iResource->GetURI().GetComponent(URI::kScheme));
	if (!providerId.IsValid())
	{
		ASSERT_FAIL("CZPLinkResourceStateUpdater::CancelUpdateResourceState() - resource provider id is invalid!");
		return;
	}
/*	
	CusDtLnkLinkResourceStatePacket job(ILinkManager::kAsynchronous,
										   false,
										   db,
										   resourceUID,
										   iResource->GetURI(),
										   iResource->GetState(),
											iResource->GetStamp(),
											iResource->GetModTime(),
											iResource->GetSize());
	
	// first check to see if a job is already pending
	InterfacePtr<ICusDtLnkStateUpdateProcessor> stateUpdateProcessor(GetExecutionContextSession(), UseDefaultIID());
	if (stateUpdateProcessor) {
		if (stateUpdateProcessor->IsJobPending(job))
			stateUpdateProcessor->RemoveJob(job);
	}*/
}

//----------------------------------------------------------------------------------------
// ResolveResource
//----------------------------------------------------------------------------------------
ErrorCode
CZPLinkResourceStateUpdater::ResolveResource(
	const UIDRef& resourceRef, const URI& relativeURI, bool bIgnoreStamp)
{
	#pragma unused(resourceRef, relativeUIR, bIgnoreStamp)
	return kFailure;
}

//----------------------------------------------------------------------------------------
// UpdateResourceState
//----------------------------------------------------------------------------------------
ErrorCode
CZPLinkResourceStateUpdater::UpdateResourceState(
	const UIDRef& resourceRef, bool bNotify, bool bSynchronous) const
{
	IDataBase* db = resourceRef.GetDataBase();
	InterfacePtr<ILinkManager> iLinkMgr(db, db->GetRootUID(), UseDefaultIID());
	if (!iLinkMgr) {
		ASSERT_FAIL("CZPLinkResourceStateUpdater::UpdateResourceState() - ILinkManager is nil!");
		return kFailure;
	}
	
	UID resourceUID = resourceRef.GetUID();
	
	// check to make sure we have a valid resource to queue
	InterfacePtr<ILinkResource> iResource(iLinkMgr->QueryResourceByUID(resourceUID));
	if (!iResource) {
		ASSERT_FAIL("CZPLinkResourceStateUpdater::UpdateResourceState() - ILinkResource is nil!");
		return kFailure;
	}
	
	ClassID providerId = iLinkMgr->GetResourceProviderByScheme(iResource->GetURI().GetComponent(URI::kScheme));
	if (!providerId.IsValid()) {
		ASSERT_FAIL("CZPLinkResourceStateUpdater::UpdateResourceState() - resource provider id is invalid!");
		return kFailure;
	}
/*
	InterfacePtr<ICusDtLnkStateUpdateProcessor> stateUpdateProcessor(GetExecutionContextSession(), UseDefaultIID());
	if (!stateUpdateProcessor) {
		ASSERT_FAIL("CZPLinkResourceStateUpdater::UpdateResourceState() - stateUpdateProcessor is nil!");
		return kFailure;
	}
	
	CusDtLnkLinkResourceStatePacket job(bSynchronous ? ILinkManager::kSynchronous : ILinkManager::kAsynchronous,
										  bNotify,
										  db,
										  resourceUID,
										  iResource->GetURI(),
										  iResource->GetState(),
										  iResource->GetStamp(),
										  iResource->GetModTime(),
										  iResource->GetSize());
	// first check to see if a job is already pending
	if (stateUpdateProcessor->IsJobPending(job)) {
		if (bSynchronous)
			// cancel the pending packet, so we can process it synchronously
			stateUpdateProcessor->RemoveJob(job);
		else
			// allow the pending packet to complete processing asynchronously
			return kSuccess;
	}
	
	stateUpdateProcessor->AddJob(job);
	
	if (bSynchronous) {
		if (stateUpdateProcessor) {
			stateUpdateProcessor->UpdateStateForJob(job);
		}
	}*/
	
	return kSuccess;
}