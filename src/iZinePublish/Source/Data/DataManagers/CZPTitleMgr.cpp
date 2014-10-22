//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Data/DataManagers/CZPTitleMgr.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 29-3-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IStringData.h"
#include "ISubject.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPApp.h"
#include "IZPAppController.h"
#include "IZPConfigController.h"
#include "IZPTitleMgr.h"
#include "IZPEditionList.h"
#include "IZPWSUtils.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_DBObjects
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

class CZPTitleMgr : public CPMUnknown<IZPTitleMgr>
{
public:
						CZPTitleMgr(IPMUnknown * boss);
	virtual				~CZPTitleMgr();

	void				Shutdown();

	void				UpdateTitleList(
							const ZPPMStringSList &		inTitleIDList,
							const ZPPMStringSList &		inTitleNames);
	
	const IStringData *	GetTitleForID(
							const PMString &			inTitleID ) const;

	void				FetchEditionsForTitle(
							const PMString &			inTitleID );
	void				FetchStatusForTitle(
							const PMString &			inTitleID );
	void				FetchUsersForTitle(
							const PMString &			inTitleID );

	const IStringData *	GetTitleForEditionID(
							const PMString &			inEditionID ) const;
protected:
	void				GetAllTitleIDs(
							ZPTitleIDsList &			oKeys ) const;
	
	static IStringData*	CreateTitle(					//Caller will be owner of obj.
							const PMString &			inTitleID );

	void				BroadcastMessage(
							const ClassID&				inTheChange,
							void *						inChangedBy = NULL );
private:
	ZPTitleIDMap		mTitleMap;
};


CREATE_PMINTERFACE(CZPTitleMgr, kZPTitleMgrImpl)

CZPTitleMgr::CZPTitleMgr(IPMUnknown * boss) :
CPMUnknown<IZPTitleMgr>(boss)
{

}

CZPTitleMgr::~CZPTitleMgr()
{
	this->Shutdown();
}

#pragma mark -
//----------------------------------------------------------------------------------------
// CreateTitle
//----------------------------------------------------------------------------------------
IStringData*
CZPTitleMgr::CreateTitle(
	const PMString &			inTitleID)
{
	IStringData* theTitleID = ::CreateObject2<IStringData>( kZPDataTitleBoss , IID_IZPDATAID );
	ASSERT(theTitleID);
#ifdef DEBUG
	InterfacePtr<const IControllingUnknown> controlling( theTitleID, UseDefaultIID() );
	IZPLog_Str( thisFileLA, enLT_ObjectCreations, "Title boss created = 0x%08X", controlling.get() );
	IZPLog_Str_( thisFileLA, enLT_ObjectCreations, "Object ID = %s", inTitleID.GrabCString() );
#endif
	theTitleID->Set( inTitleID );

	return theTitleID;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// UpdateTitleList
//----------------------------------------------------------------------------------------
void
CZPTitleMgr::UpdateTitleList(
	const ZPPMStringSList &		inTitleIDList,
	const ZPPMStringSList &		inTitleNames)
{
	LogFunctionEnterExit;

	//send message title list will change.
	this->BroadcastMessage( kZPTitleListWillChange );
	
	//Create copy of exiting title IDs list
	ZPTitleIDsList oldKeys;
	this->GetAllTitleIDs( oldKeys );

	//Update/Add title names
	ZPPMStringSList::const_iterator newKeysCurrIter = inTitleIDList.begin();
	ZPPMStringSList::const_iterator newKeysEndIter = inTitleIDList.end();
	ZPTitleIDsList::iterator oldKeysEndIter;
	ZPTitleIDsList::iterator oldKeysFoundIter;

	int i = 0;
	while( newKeysCurrIter != newKeysEndIter )
	{
		const PMString & newTitleID = **newKeysCurrIter;
		const IStringData * titleID = this->GetTitleForID( newTitleID );
		if( titleID == nil )
		{
			titleID = mTitleMap[newTitleID] = CreateTitle( newTitleID );
		}

		InterfacePtr<IStringData> titleName( titleID, IID_IZPDATANAME );
		ASSERT( titleName );
		PMString & theTitleName = *inTitleNames.at(i);
		theTitleName.SetTranslatable( kFalse );
		titleName->Set( theTitleName );

		oldKeysEndIter = oldKeys.end();
		oldKeysFoundIter = std::find(oldKeys.begin(), oldKeysEndIter, newTitleID );
		if( oldKeysFoundIter != oldKeysEndIter )
			oldKeys.erase( oldKeysFoundIter );

		++i;
		++newKeysCurrIter;
	}

	//Remove not listed titles
	oldKeysEndIter = oldKeys.end();
	ZPTitleIDsList::iterator oldKeysCurrIter = oldKeys.begin();
	while(oldKeysCurrIter != oldKeysEndIter )
	{
		const IStringData * titleID = this->GetTitleForID( *oldKeysCurrIter );
		if( titleID )
		{
#ifdef DEBUG
			InterfacePtr<const IControllingUnknown> controlling( titleID, UseDefaultIID() );
			IZPLog_Str( thisFileLA, enLT_ObjectCreations, "Title boss removed from cache = 0x%08X", controlling.get() );
			IZPLog_Str_( thisFileLA, enLT_ObjectCreations, "Object ID = %s", (*oldKeysCurrIter).GrabCString() );
#endif
			titleID->Release();
		}
		mTitleMap.erase( *oldKeysCurrIter );
		++oldKeysCurrIter;
	}

	//send message title list updated.
	this->BroadcastMessage( kZPTitleListChanged );
}


//----------------------------------------------------------------------------------------
// GetTitleForID
//----------------------------------------------------------------------------------------
const IStringData *
CZPTitleMgr::GetTitleForID(
	const PMString &			inTitleID) const
{
	ZPTitleIDMapConstIter foundIter = mTitleMap.find( inTitleID );
	ZPTitleIDMapConstIter endIter = mTitleMap.end();
	if( foundIter != endIter )
		return foundIter->second;
	else
		return nil;
}

//----------------------------------------------------------------------------------------
// GetTitleForEditionID
//----------------------------------------------------------------------------------------
const IStringData *
CZPTitleMgr::GetTitleForEditionID(
	const PMString &				inEditionID) const
{
	ZPTitleIDMapConstIter currIter = mTitleMap.begin();
	ZPTitleIDMapConstIter endIter = mTitleMap.end();
	while( currIter != endIter )
	{
		const IStringData * titleID = currIter->second;
		InterfacePtr<const IZPEditionList> titleEditionList( titleID , UseDefaultIID() );
		ASSERT( titleEditionList );
		if( titleEditionList->ContainsEditionID( inEditionID ) )
			return titleID;
		++currIter;
	}
	return nil;
}

//----------------------------------------------------------------------------------------
// GetAllTitleIDs
//----------------------------------------------------------------------------------------
void
CZPTitleMgr::GetAllTitleIDs(
	ZPTitleIDsList &			oKeys) const
{
	ZPTitleIDMapConstIter currIter = mTitleMap.begin();
	ZPTitleIDMapConstIter endIter = mTitleMap.end();
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
CZPTitleMgr::BroadcastMessage(
	const ClassID&						inTheChange,
	void *								inChangedBy )
{
	InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
	ASSERT(selfSubject);
	selfSubject->Change( inTheChange, IZPTitleMgr::kDefaultIID, inChangedBy );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// FetchEditionsForTitle
//----------------------------------------------------------------------------------------
void
CZPTitleMgr::FetchEditionsForTitle(
	const PMString &			inTitleID)
{
	ASSERT(gZPApp);
	InterfacePtr<IZPConfigController> configController( gZPApp, UseDefaultIID() );
	ASSERT( configController );
	configController->FetchEditions( inTitleID );
}

//----------------------------------------------------------------------------------------
// FetchStatusForTitle
//----------------------------------------------------------------------------------------
void
CZPTitleMgr::FetchStatusForTitle(
	const PMString &			inTitleID)
{
#pragma message ("FetchStatusForTitle")
	//TODO;
}

//----------------------------------------------------------------------------------------
// FetchUsersForTitle
//----------------------------------------------------------------------------------------
void
CZPTitleMgr::FetchUsersForTitle(
	const PMString &			inTitleID)
{
	Utils<IZPWSUtils>()->FetchTitleUsersList_async( inTitleID );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// Shutdown
//----------------------------------------------------------------------------------------
void
CZPTitleMgr::Shutdown()
{
	//Remove all the titles
	ZPTitleIDMap::const_iterator endIter = mTitleMap.end();
	ZPTitleIDMap::const_iterator currIter = mTitleMap.begin();
	while(currIter != endIter )
	{
		IStringData * theTitleID = currIter->second;
		if( theTitleID )
		{
#ifdef DEBUG
			InterfacePtr<const IControllingUnknown> controlling( theTitleID, UseDefaultIID() );
			int theRefCount = controlling->GetRefCount();
			IZPLog_Str( thisFileLA, enLT_ObjectCreations, "Title boss at 0x%08X has refcount = %d", controlling.get(), theRefCount );
#endif
			theTitleID->Release();
		}
		++currIter;
	}
	mTitleMap.clear();
}

#pragma mark -
//----------------------------------------------------------------------------------------
// CompareTitleIDByName 
//----------------------------------------------------------------------------------------
bool
CompareTitleIDByName::operator ()(
	const PMString &			inFirstVal, 
	const PMString &			inSecondVal)
{
	ASSERT( inFirstVal.IsEmpty() == false && inSecondVal.IsEmpty() == false);

#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	const IZPTitleMgr * titleMgr = zpApp->GetTitleMgr();
	ASSERT(titleMgr);

	InterfacePtr<const IStringData> titleName1( titleMgr->GetTitleForID(inFirstVal), IID_IZPDATANAME );
	ASSERT( titleName1 );

	InterfacePtr<const IStringData> titleName2( titleMgr->GetTitleForID(inSecondVal), IID_IZPDATANAME );
	ASSERT( titleName2 );

	if( titleName1 && titleName2 )
	{
		const PMString & name1 = titleName1->Get();
		const PMString & name2 = titleName2->Get();
		bool toReturn = name1.Compare(false, name2) > 0;//name1 < name2;
#ifdef DEBUG
		//IZPLog_Str( thisFileLA, enLT_DebugInfo, "Compare \"%s\" and \"%s\" , result %hd", name1.GrabCString(), name2.GrabCString(), (short int) toReturn );
#endif
		return !toReturn;
	}
	return false;
}
