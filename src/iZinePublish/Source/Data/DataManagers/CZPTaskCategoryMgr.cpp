//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Data/DataManagers/CZPTaskCategoryMgr.cpp $
//	$Revision: 2302 $
//	$Date: 2010-10-14 14:18:41 +0200 (Thu, 14 Oct 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 14-10-2010
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
#include "IZPTaskCategoryMgr.h"
#include "IZPWSUtils.h"
#include "IZPSoapRequest.h"

//IZP General includes
#include "AZPSoapRequest.h"
#include "CZPGetTaskCategoryListResponse.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_DBObjects
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZPTaskCategoryMgr>
class CZPTaskCategoryMgr : public inheritClass
{
public:
						CZPTaskCategoryMgr(IPMUnknown * boss);
	virtual				~CZPTaskCategoryMgr();

	void				Shutdown();

	void				HandleWSResponse(
							const IZPSoapRequest *			inSoapRequest,
							const AZPSoapResponse *			inResponse,
							ZPWSError						inCategory);

	void				UpdateTaskCategoryList(
							const ZPIDPairSList &		inTaskCategoryList );
	const IStringData *	GetTaskCategoryForID(
							const PMString &			inTaskCategoryID ) const;

	void				GetAllTaskCategoryIDs(
							ZPTaskCategoryIDsList &		oKeys,
							const bool					inFetchIfNotHave) const;

	bool				HasData() const;
	void				SetHasData(
							bool						inHasData);
protected:
	static const IStringData * CreateTaskCategory(
							const PMString &			inTaskCategoryID );

	void				BroadcastMessage(
							const ClassID&				inTheChange,
							void *						inChangedBy = NULL );
private:
	ZPTaskCategoryIDMap	mCategoryMap;
	bool				mHasData;
};


CREATE_PMINTERFACE(CZPTaskCategoryMgr, kZPTaskCategoryMgrImpl)

CZPTaskCategoryMgr::CZPTaskCategoryMgr(IPMUnknown * boss)
: inheritClass(boss)
, mHasData( false )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPTaskCategoryMgr::~CZPTaskCategoryMgr()
{
	this->Shutdown();
}

#pragma mark -
//----------------------------------------------------------------------------------------
// CreateTaskCategory
//----------------------------------------------------------------------------------------
const IStringData *
CZPTaskCategoryMgr::CreateTaskCategory(
	const PMString &			inTaskCategoryID)
{
	IStringData* theCategoryID = ::CreateObject2<IStringData>( kZPDataTaskCategoryBoss , IID_IZPDATAID );
	ASSERT(theCategoryID);
#ifdef DEBUG
	InterfacePtr<const IControllingUnknown> controlling( theCategoryID, UseDefaultIID() );
	IZPLog_Str( thisFileLA, enLT_ObjectCreations, "Task Category boss created = 0x%08X", controlling.get() );
	IZPLog_Str_( thisFileLA, enLT_ObjectCreations, "Object ID = %s", inTaskCategoryID.GrabCString() );
#endif
	theCategoryID->Set( inTaskCategoryID );

	return theCategoryID;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// HandleWSResponse
//----------------------------------------------------------------------------------------
void
CZPTaskCategoryMgr::HandleWSResponse(
	const IZPSoapRequest *			inSoapRequest,
	const AZPSoapResponse *			inResponse,
	ZPWSError						inCategory)
{
	LogFunctionEnterExit;
	
	const AZPSoapRequest * theRequest = inSoapRequest->GetRequestData();
	ASSERT( theRequest->GetRequestType() == eReq_GetTaskCategoryList );
	
	const CZPGetTaskCategoryListResponse * inTaskCategoryResponse = dynamic_cast<const CZPGetTaskCategoryListResponse *> (inResponse );
	ASSERT( inTaskCategoryResponse );
	if( !inTaskCategoryResponse )
		return;

	this->UpdateTaskCategoryList( inTaskCategoryResponse->GetTaskCategoryList() );
	
}

//----------------------------------------------------------------------------------------
// UpdateTaskCategoryList
//----------------------------------------------------------------------------------------
void
CZPTaskCategoryMgr::UpdateTaskCategoryList(
	const ZPIDPairSList &		inTaskCategoryList)
{
	LogFunctionEnterExit;

	mHasData = true;	//Even if there are not values in list.

	//send message TaskCategory list will change.
	this->BroadcastMessage( kZPTaskCategoryListWillChange );
	
	//Create copy of exiting task Category IDs list
	ZPTaskCategoryIDsList oldKeys;
	this->GetAllTaskCategoryIDs( oldKeys, false );

	//Add TaskCategory or Update names
	ZPIDPairSList::const_iterator newKeysCurrIter = inTaskCategoryList.begin();
	ZPIDPairSList::const_iterator newKeysEndIter = inTaskCategoryList.end();
	ZPTaskCategoryIDsList::iterator oldKeysEndIter;
	ZPTaskCategoryIDsList::iterator oldKeysFoundIter;

	int i = 0;
	while( newKeysCurrIter != newKeysEndIter )
	{
		const ZPIDPair currTaskCategory = **newKeysCurrIter;
		const IStringData * dbTaskCategoryID = this->GetTaskCategoryForID( currTaskCategory.mID );
		if( dbTaskCategoryID == nil )
		{
			dbTaskCategoryID = mCategoryMap[currTaskCategory.mID] = CreateTaskCategory( currTaskCategory.mID );
		}

		InterfacePtr<IStringData> dbCategoryName( dbTaskCategoryID, IID_IZPDATANAME );
		ASSERT( dbCategoryName );
		PMString theCategoryName = currTaskCategory.mName;
		theCategoryName.SetTranslatable( kFalse );
		dbCategoryName->Set( theCategoryName );

		oldKeysEndIter = oldKeys.end();
		oldKeysFoundIter = std::find(oldKeys.begin(), oldKeysEndIter, currTaskCategory.mID );
		if( oldKeysFoundIter != oldKeysEndIter )
			oldKeys.erase( oldKeysFoundIter );

		++i;
		++newKeysCurrIter;
	}

	//Remove not listed task Category
	oldKeysEndIter = oldKeys.end();
	ZPTaskCategoryIDsList::iterator oldKeysCurrIter = oldKeys.begin();
	while(oldKeysCurrIter != oldKeysEndIter )
	{
		const IStringData * dbCategoryID = this->GetTaskCategoryForID( *oldKeysCurrIter );
		if( dbCategoryID )
		{
#ifdef DEBUG
			InterfacePtr<const IControllingUnknown> controlling( dbCategoryID, UseDefaultIID() );
			IZPLog_Str( thisFileLA, enLT_ObjectCreations, "Task Category boss removed from cache = 0x%08X", controlling.get() );
			IZPLog_Str_( thisFileLA, enLT_ObjectCreations, "Object ID = %s", (*oldKeysCurrIter).GrabCString() );
#endif
			dbCategoryID->Release();
		}
		mCategoryMap.erase( *oldKeysCurrIter );
		++oldKeysCurrIter;
	}

	//send message TaskCategory list updated.
	this->BroadcastMessage( kZPTaskCategoryListChanged );
}

//----------------------------------------------------------------------------------------
// GetTaskCategoryForID
//----------------------------------------------------------------------------------------
const IStringData *
CZPTaskCategoryMgr::GetTaskCategoryForID(
	const PMString &			inTaskCategoryID) const
{
	ZPTaskCategoryIDMapConstIter foundIter = mCategoryMap.find( inTaskCategoryID );
	ZPTaskCategoryIDMapConstIter endIter = mCategoryMap.end();
	if( foundIter != endIter )
		return foundIter->second;
	else
		return nil;
}

//----------------------------------------------------------------------------------------
// GetAllTaskCategoryIDs
//----------------------------------------------------------------------------------------
void
CZPTaskCategoryMgr::GetAllTaskCategoryIDs(
	ZPTaskCategoryIDsList &		oKeys,
	const bool					inFetchIfNotHave) const
{
	if( !mHasData && inFetchIfNotHave )
		Utils<IZPWSUtils>()->FetchTaskCategoryList_async();

	ZPTaskCategoryIDMapConstIter currIter = mCategoryMap.begin();
	ZPTaskCategoryIDMapConstIter endIter = mCategoryMap.end();
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
CZPTaskCategoryMgr::BroadcastMessage(
	const ClassID&						inTheChange,
	void *								inChangedBy )
{
	InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
	ASSERT(selfSubject);
	selfSubject->Change( inTheChange, IZPTaskCategoryMgr::kDefaultIID, inChangedBy );
}


#pragma mark -

//----------------------------------------------------------------------------------------
// Shutdown
//----------------------------------------------------------------------------------------
void
CZPTaskCategoryMgr::Shutdown()
{
//Remove all the TaskCategory from cache
	ZPTaskCategoryIDMapConstIter endIter = mCategoryMap.end();
	ZPTaskCategoryIDMapConstIter currIter = mCategoryMap.begin();
	while(currIter != endIter )
	{
		const IStringData * theCategoryID = currIter->second;
		if( theCategoryID )
		{
#ifdef DEBUG
			InterfacePtr<const IControllingUnknown> controlling( theCategoryID, UseDefaultIID() );
			int theRefCount = controlling->GetRefCount();
			IZPLog_Str( thisFileLA, enLT_ObjectCreations, "Task Category boss at 0x%08X has refcount = %d", controlling.get(), theRefCount );
#endif
			theCategoryID->Release();
		}
		++currIter;
	}
	mCategoryMap.clear();
}
