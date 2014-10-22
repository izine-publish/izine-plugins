//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Data/DataManagers/CZPUserMgr.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 1-10-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IIntData.h"
#include "IStringData.h"
#include "ISubject.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPApp.h"
#include "IZPSoapRequest.h"
#include "IZPTitleMgr.h"
#include "IZPUserList.h"
#include "IZPUserMgr.h"

//IZP General includes
#include "AZPSoapRequest.h"
#include "CZPGetUsersForTitleRequest.h"
#include "CZPGetUsersForTitleResponse.h"
#include "ZPConstants.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_DBObjects
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZPUserMgr>
class CZPUserMgr : public inheritClass
{
public:
						CZPUserMgr(IPMUnknown * boss);
	virtual				~CZPUserMgr();

	void				HandleWSResponse(
							const IZPSoapRequest *			inSoapRequest,
							const AZPSoapResponse *			inResponse,
							ZPWSError						inStatus);

	void				Shutdown();

	void				UpdateUserList(
							const PMString &			inTitleID,
							const ZPIDPairSList &		inUserList );

	const IStringData *	GetUserForID(
							const PMString &			inUserID ) const;

	void				DecrementReferenceForUsers (
							const ZPUserIDsList &		inUserIDs );

protected:
	static IStringData * CreateUser(
							const PMString &				inUserID);

	bool				AddUpdateUser(					//Return true if new object was created
							const PMString &				inUserID,
							const PMString &				inUserName,
							const IStringData **			oUserID);

	void				RemoveFromCache(
							const IStringData *				inUserID );

	void				BroadcastMessage(
							const ClassID&				inTheChange,
							void *						inChangedBy = NULL );
private:
	ZPUserIDMap		mUserMap;
};


CREATE_PMINTERFACE(CZPUserMgr, kZPUserMgrImpl)

CZPUserMgr::CZPUserMgr(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPUserMgr::~CZPUserMgr()
{
	this->Shutdown();
}

#pragma mark -
//----------------------------------------------------------------------------------------
// CreateUser
//----------------------------------------------------------------------------------------
IStringData*
CZPUserMgr::CreateUser(//Caller will be owner of obj.
	const PMString &			inUserID)
{
	IStringData * theUserID = ::CreateObject2<IStringData>( kZPDataUserBoss , IID_IZPDATAID );
	ASSERT(theUserID);
#ifdef DEBUG
	InterfacePtr<const IControllingUnknown> controlling( theUserID, UseDefaultIID() );
	IZPLog_Str( thisFileLA, enLT_ObjectCreations, "User boss created = 0x%08X", controlling.get() );
	IZPLog_Str_( thisFileLA, enLT_ObjectCreations, "Object ID = %s", inUserID.GrabCString() );
#endif
	theUserID->Set( inUserID );

	return theUserID;
}

//----------------------------------------------------------------------------------------
// HandleWSResponse
//----------------------------------------------------------------------------------------
void
CZPUserMgr::HandleWSResponse(
	const IZPSoapRequest *			inSoapRequest,
	const AZPSoapResponse *			inResponse,
	ZPWSError						inStatus)
{
	LogFunctionEnterExit;
	
	const AZPSoapRequest * theRequest = inSoapRequest->GetRequestData();
	ASSERT( theRequest->GetRequestType() == eReq_GetUsersForTitle );
	
	const CZPGetUsersForTitleRequest * inUserListRequest = dynamic_cast<const CZPGetUsersForTitleRequest *> (inSoapRequest->GetRequestData() );
	ASSERT( inUserListRequest );
	if( !inUserListRequest )
		return;

	const CZPGetUsersForTitleResponse * inUserListResponse = dynamic_cast<const CZPGetUsersForTitleResponse *> (inResponse );
	ASSERT( inUserListResponse );
	if( !inUserListResponse )
		return;

	this->UpdateUserList( inUserListRequest->GetTitleID(), inUserListResponse->GetTitleUsersList() );
	
}

//----------------------------------------------------------------------------------------
// UpdateUserList
//----------------------------------------------------------------------------------------
void
CZPUserMgr::UpdateUserList(
	const PMString &				inTitleID,
	const ZPIDPairSList &			inUserList)
{
	//send message User list will change.
	this->BroadcastMessage( kZPUserListWillChangeMsg );

#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<IZPTitleMgr> titleMgr( zpApp->GetTitleMgr(), UseDefaultIID() );
	ASSERT(titleMgr);

	const IStringData * theTitle = titleMgr->GetTitleForID( inTitleID );

	ZPUserIDsList userIDsToRemove;
	ZPUserIDsList userIDsNewList;

	InterfacePtr<IZPUserList> titleUserList( theTitle, UseDefaultIID() );
	
	if ( titleUserList != nil )
	{
		userIDsToRemove = titleUserList->GetStringList( false );
	}
	else
	{
		ASSERT( titleUserList != nil );	//Updating a title that is not cached.
		return;
	}

	//Add/Update the User for title
	ZPIDPairSList::const_iterator	currIter = inUserList.begin();
	ZPIDPairSList::const_iterator	endIter = inUserList.end();

	while( currIter != endIter )
	{
		const PMString & userID = (*currIter)->mID;

		//Update the User
		const IStringData * theUser = NULL;
		(*currIter)->mName.SetTranslatable( kFalse );
		bool newUserCreated = this->AddUpdateUser( userID, (*currIter)->mName, &theUser );
		
		//Add to new list.
		userIDsNewList.push_back(userID);
		
		++currIter;

		if( newUserCreated == false )
			theUser->AddRef();	//Retain till this ID is referenced.

		//Increament ref count maintained by this manager.
		InterfacePtr<IIntData> cachedCount( theUser, IID_IZPCACHEMGRCOUNT );
		ASSERT( cachedCount );
		cachedCount->Set( cachedCount->Get() + 1 );
	}

	//Update the user id list
	titleUserList->SetStringList( userIDsNewList );

	//Remove from cached list. Should we? what if same User is shared by multiple titles.
	this->DecrementReferenceForUsers( userIDsToRemove );

	//send message user list updated.
	this->BroadcastMessage( kZPUserListChangedMsg );
}


//----------------------------------------------------------------------------------------
// AddUpdateUser
//----------------------------------------------------------------------------------------
bool
CZPUserMgr::AddUpdateUser(
	const PMString &			inUserID,
	const PMString &			inUserName,
	const IStringData **		oUserID)
{
	const IStringData * userID = this->GetUserForID( inUserID );
	bool toReturn = userID == nil;
	if( toReturn )
	{
		userID = mUserMap[inUserID] = CreateUser( inUserID );
	}

	InterfacePtr<IStringData> userName( userID, IID_IZPDATANAME );
	ASSERT( userName );
	userName->Set( inUserName );
	if( oUserID )
		*oUserID = userID;

	return toReturn;
}

//----------------------------------------------------------------------------------------
// RemoveFromCache
//----------------------------------------------------------------------------------------
void
CZPUserMgr::RemoveFromCache(
	const IStringData *				inUserID)
{
	if( !inUserID )
		return;

	const PMString & theUserIDStr = inUserID->Get();
#ifdef DEBUG
	InterfacePtr<const IControllingUnknown> controlling( inUserID, UseDefaultIID() );
	IZPLog_Str( thisFileLA, enLT_ObjectCreations, "User boss removed from cache = 0x%08X", controlling.get() );
	IZPLog_Str_( thisFileLA, enLT_ObjectCreations, "Object ID = %s", theUserIDStr.GrabCString() );
#endif
	mUserMap.erase( inUserID->Get() );
}

//----------------------------------------------------------------------------------------
// DecrementReferenceForUsers
//----------------------------------------------------------------------------------------
void
CZPUserMgr::DecrementReferenceForUsers (
	const ZPUserIDsList &		inUserIDs)
{
	ZPUserIDsList::const_iterator idListCurrIter = inUserIDs.begin();
	ZPUserIDsList::const_iterator idListEndIter = inUserIDs.end();
	
	while( idListCurrIter != idListEndIter )
	{
		const IStringData * theUserID = this->GetUserForID( *idListCurrIter );
		if( theUserID )
		{
#ifdef DEBUG
			InterfacePtr<const IControllingUnknown> controlling( theUserID, UseDefaultIID() );
			int theRefCount = controlling->GetRefCount();
			IZPLog_Str( thisFileLA, enLT_ObjectCreations, "User boss at 0x%08X has refcount = %d", controlling.get(), theRefCount );
#endif

			//Update reference count maintained by this manager
			InterfacePtr<IIntData> cachedCount( theUserID, IID_IZPCACHEMGRCOUNT );
			ASSERT( cachedCount );
			if( cachedCount->Get() == 1 )
				this->RemoveFromCache( theUserID );
			else
				cachedCount->Set( cachedCount->Get() -1 );
			
			theUserID->Release();
		}
		++idListCurrIter;
	}
}



//----------------------------------------------------------------------------------------
// GetUserForID
//----------------------------------------------------------------------------------------
const IStringData *
CZPUserMgr::GetUserForID(
	const PMString &			inUserID) const
{
	ZPUserIDMapConstIter foundIter = mUserMap.find( inUserID );
	ZPUserIDMapConstIter endIter = mUserMap.end();
	if( foundIter != endIter )
		return foundIter->second;
	else
		return nil;
}

//----------------------------------------------------------------------------------------
// BroadcastMessage
//----------------------------------------------------------------------------------------
void
CZPUserMgr::BroadcastMessage(
	const ClassID&				inTheChange,
	void *						inChangedBy )
{
	InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
	ASSERT(selfSubject);
	selfSubject->Change( inTheChange, IZPUserMgr::kDefaultIID, inChangedBy );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// Shutdown
//----------------------------------------------------------------------------------------
void
CZPUserMgr::Shutdown()
{
	//Remove all the TaskStatus from cache
	ZPUserIDMapConstIter endIter = mUserMap.end();
	ZPUserIDMapConstIter currIter = mUserMap.begin();
	while(currIter != endIter )
	{
		const IStringData * theUserID = currIter->second;
		if( theUserID )
		{
#ifdef DEBUG
			InterfacePtr<const IControllingUnknown> controlling( theUserID, UseDefaultIID() );
			int theRefCount = controlling->GetRefCount();
			IZPLog_Str( thisFileLA, enLT_ObjectCreations, "User boss at 0x%08X has refcount = %d", controlling.get(), theRefCount );
#endif
			theUserID->Release();
		}
		++currIter;
	}
	mUserMap.clear();
}
