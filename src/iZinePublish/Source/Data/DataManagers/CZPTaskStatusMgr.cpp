//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Data/DataManagers/CZPTaskStatusMgr.cpp $
//	$Revision: 2208 $
//	$Date: 2010-09-30 16:51:25 +0200 (Thu, 30 Sep 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 23-9-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IStringData.h"
#include "ISubject.h"

//SDK General includes
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPTaskStatusMgr.h"
#include "IZPWSUtils.h"
#include "IZPSoapRequest.h"

//IZP General includes
#include "AZPSoapRequest.h"
#include "CZPGetTaskStatusListResponse.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_DBObjects
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZPTaskStatusMgr>
class CZPTaskStatusMgr : public inheritClass
{
public:
						CZPTaskStatusMgr(IPMUnknown * boss);
	virtual				~CZPTaskStatusMgr();

	void				Shutdown();

	void				HandleWSResponse(
							const IZPSoapRequest *			inSoapRequest,
							const AZPSoapResponse *			inResponse,
							ZPWSError						inStatus);

	void				UpdateTaskStatusList(
							const ZPIDPairSList &		inTaskStatusList );
	const IStringData *	GetTaskStatusForID(
							const PMString &			inTaskStatusID ) const;

	void				GetAllTaskStatusIDs(
							ZPTaskStatusIDsList &		oKeys,
							const bool					inFetchIfNotHave) const;

	bool				HasData() const;
	void				SetHasData(
							bool						inHasData);
protected:
	static const IStringData * CreateTaskStatus(
							const PMString &			inTaskStatusID );

	void				BroadcastMessage(
							const ClassID&				inTheChange,
							void *						inChangedBy = NULL );
private:
	ZPTaskStatusIDMap	mStatusMap;
	bool				mHasData;
};


CREATE_PMINTERFACE(CZPTaskStatusMgr, kZPTaskStatusMgrImpl)

CZPTaskStatusMgr::CZPTaskStatusMgr(IPMUnknown * boss)
: inheritClass(boss)
, mHasData( false )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPTaskStatusMgr::~CZPTaskStatusMgr()
{
	this->Shutdown();
}

#pragma mark -
//----------------------------------------------------------------------------------------
// CreateTaskStatus
//----------------------------------------------------------------------------------------
const IStringData *
CZPTaskStatusMgr::CreateTaskStatus(
	const PMString &			inTaskStatusID)
{
	IStringData* theStatusID = ::CreateObject2<IStringData>( kZPDataTaskStatusBoss , IID_IZPDATAID );
	ASSERT(theStatusID);
#ifdef DEBUG
	InterfacePtr<const IControllingUnknown> controlling( theStatusID, UseDefaultIID() );
	IZPLog_Str( thisFileLA, enLT_ObjectCreations, "Task Status boss created = 0x%08X", controlling.get() );
	IZPLog_Str_( thisFileLA, enLT_ObjectCreations, "Object ID = %s", inTaskStatusID.GrabCString() );
#endif
	theStatusID->Set( inTaskStatusID );

	return theStatusID;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// HandleWSResponse
//----------------------------------------------------------------------------------------
void
CZPTaskStatusMgr::HandleWSResponse(
	const IZPSoapRequest *			inSoapRequest,
	const AZPSoapResponse *			inResponse,
	ZPWSError						inStatus)
{
	LogFunctionEnterExit;
	
	const AZPSoapRequest * theRequest = inSoapRequest->GetRequestData();
	ASSERT( theRequest->GetRequestType() == eReq_GetTaskStatusList );
	
	const CZPGetTaskStatusListResponse * inTaskStatusResponse = dynamic_cast<const CZPGetTaskStatusListResponse *> (inResponse );
	ASSERT( inTaskStatusResponse );
	if( !inTaskStatusResponse )
		return;

	this->UpdateTaskStatusList( inTaskStatusResponse->GetTaskStatusList() );
	
}

//----------------------------------------------------------------------------------------
// UpdateTaskStatusList
//----------------------------------------------------------------------------------------
void
CZPTaskStatusMgr::UpdateTaskStatusList(
	const ZPIDPairSList &		inTaskStatusList)
{
	LogFunctionEnterExit;

	mHasData = true;	//Even if there are not values in list.

	//send message TaskStatus list will change.
	this->BroadcastMessage( kZPTaskStatusListWillChange );
	
	//Create copy of exiting task status IDs list
	ZPTaskStatusIDsList oldKeys;
	this->GetAllTaskStatusIDs( oldKeys, false );

	//Add TaskStatus or Update names
	ZPIDPairSList::const_iterator newKeysCurrIter = inTaskStatusList.begin();
	ZPIDPairSList::const_iterator newKeysEndIter = inTaskStatusList.end();
	ZPTaskStatusIDsList::iterator oldKeysEndIter;
	ZPTaskStatusIDsList::iterator oldKeysFoundIter;

	int i = 0;
	while( newKeysCurrIter != newKeysEndIter )
	{
		const ZPIDPair currTaskStatus = **newKeysCurrIter;
		const IStringData * dbTaskStatusID = this->GetTaskStatusForID( currTaskStatus.mID );
		if( dbTaskStatusID == nil )
		{
			dbTaskStatusID = mStatusMap[currTaskStatus.mID] = CreateTaskStatus( currTaskStatus.mID );
		}

		InterfacePtr<IStringData> dbStatusName( dbTaskStatusID, IID_IZPDATANAME );
		ASSERT( dbStatusName );
		PMString theStatusName = currTaskStatus.mName;
		theStatusName.SetTranslatable( kFalse );
		dbStatusName->Set( theStatusName );

		oldKeysEndIter = oldKeys.end();
		oldKeysFoundIter = std::find(oldKeys.begin(), oldKeysEndIter, currTaskStatus.mID );
		if( oldKeysFoundIter != oldKeysEndIter )
			oldKeys.erase( oldKeysFoundIter );

		++i;
		++newKeysCurrIter;
	}

	//Remove not listed task status
	oldKeysEndIter = oldKeys.end();
	ZPTaskStatusIDsList::iterator oldKeysCurrIter = oldKeys.begin();
	while(oldKeysCurrIter != oldKeysEndIter )
	{
		const IStringData * dbStatusID = this->GetTaskStatusForID( *oldKeysCurrIter );
		if( dbStatusID )
		{
#ifdef DEBUG
			InterfacePtr<const IControllingUnknown> controlling( dbStatusID, UseDefaultIID() );
			IZPLog_Str( thisFileLA, enLT_ObjectCreations, "Task Status boss removed from cache = 0x%08X", controlling.get() );
			IZPLog_Str_( thisFileLA, enLT_ObjectCreations, "Object ID = %s", (*oldKeysCurrIter).GrabCString() );
#endif
			dbStatusID->Release();
		}
		mStatusMap.erase( *oldKeysCurrIter );
		++oldKeysCurrIter;
	}

	//send message TaskStatus list updated.
	this->BroadcastMessage( kZPTaskStatusListChanged );
}

//----------------------------------------------------------------------------------------
// GetTaskStatusForID
//----------------------------------------------------------------------------------------
const IStringData *
CZPTaskStatusMgr::GetTaskStatusForID(
	const PMString &			inTaskStatusID) const
{
	ZPTaskStatusIDMapConstIter foundIter = mStatusMap.find( inTaskStatusID );
	ZPTaskStatusIDMapConstIter endIter = mStatusMap.end();
	if( foundIter != endIter )
		return foundIter->second;
	else
		return nil;
}

//----------------------------------------------------------------------------------------
// GetAllTaskStatusIDs
//----------------------------------------------------------------------------------------
void
CZPTaskStatusMgr::GetAllTaskStatusIDs(
	ZPTaskStatusIDsList &		oKeys,
	const bool					inFetchIfNotHave) const
{
	if( !mHasData && inFetchIfNotHave )
		Utils<IZPWSUtils>()->FetchTaskStatusList_async();

	ZPTaskStatusIDMapConstIter currIter = mStatusMap.begin();
	ZPTaskStatusIDMapConstIter endIter = mStatusMap.end();
	while( currIter != endIter )
	{
		oKeys.push_back(currIter->first);
		++currIter;
	}
}

//----------------------------------------------------------------------------------------
// BroadcastMessage
//----------------------------------------------------------------------------------------
void
CZPTaskStatusMgr::BroadcastMessage(
	const ClassID&						inTheChange,
	void *								inChangedBy )
{
	InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
	ASSERT(selfSubject);
	selfSubject->Change( inTheChange, IZPTaskStatusMgr::kDefaultIID, inChangedBy );
}


#pragma mark -

//----------------------------------------------------------------------------------------
// Shutdown
//----------------------------------------------------------------------------------------
void
CZPTaskStatusMgr::Shutdown()
{
//Remove all the TaskStatus from cache
	ZPTaskStatusIDMapConstIter endIter = mStatusMap.end();
	ZPTaskStatusIDMapConstIter currIter = mStatusMap.begin();
	while(currIter != endIter )
	{
		const IStringData * theStatusID = currIter->second;
		if( theStatusID )
		{
#ifdef DEBUG
			InterfacePtr<const IControllingUnknown> controlling( theStatusID, UseDefaultIID() );
			int theRefCount = controlling->GetRefCount();
			IZPLog_Str( thisFileLA, enLT_ObjectCreations, "Task Status boss at 0x%08X has refcount = %d", controlling.get(), theRefCount );
#endif
			theStatusID->Release();
		}
		++currIter;
	}
	mStatusMap.clear();
}
