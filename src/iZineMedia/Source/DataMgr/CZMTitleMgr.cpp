//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/DataMgr/CZMTitleMgr.cpp $
//	$Revision: 2711 $
//	$Date: 2011-04-06 15:20:51 +0200 (Wed, 06 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 4-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
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

#include "IZMediaID.h"

//IZP Interfaces
#include "IZMEditionMgr.h"
#include "IZMTitleMgr.h"
#include "IZMEditionList.h"
#include "IZMModelController.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_DBObjects
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

class CZMTitleMgr : public CPMUnknown<IZMTitleMgr>
{
public:
						CZMTitleMgr(IPMUnknown * boss);
	virtual				~CZMTitleMgr();

	void				Shutdown();

	void				UpdateTitleList(
							const ZMstTitleSList &		inTitleList);
	
	const IStringData *	GetTitleForID(
							const PMString &			inTitleID ) const;

	void				FetchEditionsForTitle(
							const PMString &			inTitleID );

	const IStringData *	GetTitleForEditionID(
							const PMString &			inEditionID ) const;
protected:
	void				GetAllTitleIDs(
							ZMTitleIDsList &			oKeys ) const;
	
	static IStringData*	CreateTitle(					//Caller will be owner of obj.
							const PMString &			inTitleID );

	void				BroadcastMessage(
							const ClassID&				inTheChange,
							void *						inChangedBy = NULL );
private:
	ZMTitleIDMap		mTitleMap;
};


CREATE_PMINTERFACE(CZMTitleMgr, kZMTitleMgrImpl)

CZMTitleMgr::CZMTitleMgr(IPMUnknown * boss) :
CPMUnknown<IZMTitleMgr>(boss)
{

}

CZMTitleMgr::~CZMTitleMgr()
{
	this->Shutdown();
}

#pragma mark -
//----------------------------------------------------------------------------------------
// CreateTitle
//----------------------------------------------------------------------------------------
IStringData*
CZMTitleMgr::CreateTitle(
	const PMString &			inTitleID)
{
	IStringData* theTitleID = ::CreateObject2<IStringData>( kZMDataTitleBoss , IID_IZMDATAID );
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
CZMTitleMgr::UpdateTitleList(
	const ZMstTitleSList &		inTitleList)
{
	LogFunctionEnterExit;

	//send message title list will change.
	this->BroadcastMessage( kZMTitleListWillChange );

	InterfacePtr<IZMEditionMgr> editionMgr( this, UseDefaultIID() );
	ASSERT(editionMgr);

	//Create copy of exiting title IDs list
	ZMTitleIDsList oldKeys;
	this->GetAllTitleIDs( oldKeys );

	//Update/Add title names
	ZMstTitleSListCIter newTitleCurrIter = inTitleList.begin();
	ZMstTitleSListCIter newTitleEndIter = inTitleList.end();
	ZMTitleIDsListIter oldKeysEndIter;
	ZMTitleIDsListIter oldKeysFoundIter;

	int i = 0;
	while( newTitleCurrIter != newTitleEndIter )
	{
		const stTitle & newTitle = **newTitleCurrIter;
		const PMString & newTitleID = newTitle.mName;
		const IStringData * titleID = this->GetTitleForID( newTitleID );
		if( titleID == nil )
		{
			titleID = mTitleMap[newTitleID] = CreateTitle( newTitleID );

			//Title name & ID are same, so setting this only once when it is created.
			InterfacePtr<IStringData> titleName( titleID, IID_IZMDATANAME );
			ASSERT( titleName );
			PMString theTitleName = newTitleID;
			theTitleName.SetTranslatable( kFalse );
			titleName->Set( theTitleName );
		}

		//Update the Edition list for this title.
		editionMgr->UpdateEditionList( titleID, newTitle.mEditions );

		oldKeysEndIter = oldKeys.end();
		oldKeysFoundIter = std::find(oldKeys.begin(), oldKeysEndIter, newTitleID );
		if( oldKeysFoundIter != oldKeysEndIter )
			oldKeys.erase( oldKeysFoundIter );

		++newTitleCurrIter;
	}

	//Remove not listed titles
	oldKeysEndIter = oldKeys.end();
	ZMTitleIDsListIter oldKeysCurrIter = oldKeys.begin();
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
	this->BroadcastMessage( kZMTitleListChanged );
}


//----------------------------------------------------------------------------------------
// GetTitleForID
//----------------------------------------------------------------------------------------
const IStringData *
CZMTitleMgr::GetTitleForID(
	const PMString &			inTitleID) const
{
	ZMTitleIDMapConstIter foundIter = mTitleMap.find( inTitleID );
	ZMTitleIDMapConstIter endIter = mTitleMap.end();
	if( foundIter != endIter )
		return foundIter->second;
	else
		return nil;
}

//----------------------------------------------------------------------------------------
// GetTitleForEditionID
//----------------------------------------------------------------------------------------
const IStringData *
CZMTitleMgr::GetTitleForEditionID(
	const PMString &				inEditionID) const
{
	ZMTitleIDMapConstIter currIter = mTitleMap.begin();
	ZMTitleIDMapConstIter endIter = mTitleMap.end();
	while( currIter != endIter )
	{
		const IStringData * titleID = currIter->second;
		InterfacePtr<const IZMEditionList> titleEditionList( titleID , UseDefaultIID() );
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
CZMTitleMgr::GetAllTitleIDs(
	ZMTitleIDsList &			oKeys) const
{
	ZMTitleIDMapConstIter currIter = mTitleMap.begin();
	ZMTitleIDMapConstIter endIter = mTitleMap.end();
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
CZMTitleMgr::BroadcastMessage(
	const ClassID&						inTheChange,
	void *								inChangedBy )
{
	InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
	ASSERT(selfSubject);
	selfSubject->Change( inTheChange, IZMTitleMgr::kDefaultIID, inChangedBy );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// FetchEditionsForTitle
//----------------------------------------------------------------------------------------
void
CZMTitleMgr::FetchEditionsForTitle(
	const PMString &			inTitleID)
{
	InterfacePtr<IZMModelController> configController( gSession, UseDefaultIID() );
	ASSERT( configController );
	configController->FetchEditions( inTitleID );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// Shutdown
//----------------------------------------------------------------------------------------
void
CZMTitleMgr::Shutdown()
{
	//Remove all the titles
	ZMTitleIDMapConstIter endIter = mTitleMap.end();
	ZMTitleIDMapConstIter currIter = mTitleMap.begin();
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
