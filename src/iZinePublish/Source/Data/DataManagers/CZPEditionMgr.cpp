//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Data/DataManagers/CZPEditionMgr.cpp $
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
#include "IIntData.h"
#include "IStringData.h"
#include "ISubject.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPApp.h"
#include "IZPAssetList.h"
#include "IZPEditionList.h"
#include "IZPEditionMgr.h"
#include "IZPTaskList.h"
#include "IZPTitleMgr.h"

//IZP General includes
#include "ZPConstants.h"

#include "IZPLog.h"

#define thisFileLA	enLMA_DBObjects
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

class CZPEditionMgr : public CPMUnknown<IZPEditionMgr>
{
public:
						CZPEditionMgr(IPMUnknown * boss);
	virtual				~CZPEditionMgr();

	void				Shutdown();

	void				UpdateEditionList(
							const ZPTitleEditionInfoSList &	inEditionInfoList );

	const IStringData *	GetEditionForID(
							const PMString &				inEditionID ) const;
	
	const IStringData *	GetEditionForAssetID(
							const PMString &				inAssetID ) const;
	const IStringData *	GetEditionForTaskID(
							const PMString &				inTaskID ) const;

	void				DecrementReferenceForEditions (
							const ZPEditionIDsList &		inEditionIDs );
	
protected:
	static IStringData * CreateEdition(
							const PMString &				inEditionID);

	void				UpdateEditionList(
							const PMString &				inTitleID,
							const ZPIDPairSList &			inEditionList );

	bool				AddUpdateEdition(					//Return true if new object was created
							const PMString &				inEditionID,
							const PMString &				inEditionName,
							const IStringData **			oEditionID);

	void				RemoveFromCache(
							const IStringData *				inEditionID );

	void				BroadcastMessage(
							const ClassID&				inTheChange,
							void *						inChangedBy = NULL );
private:
	ZPEditionIDMap		mEditionMap;
};


CREATE_PMINTERFACE(CZPEditionMgr, kZPEditionMgrImpl)

#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPEditionMgr::CZPEditionMgr(IPMUnknown * boss) :
CPMUnknown<IZPEditionMgr>(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPEditionMgr::~CZPEditionMgr()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// CreateEdition
//----------------------------------------------------------------------------------------
IStringData*
CZPEditionMgr::CreateEdition(//Caller will be owner of obj.
	const PMString &			inEditionID)
{
	IStringData * theEditionID = ::CreateObject2<IStringData>( kZPDataTitleEditionBoss , IID_IZPDATAID );
	ASSERT(theEditionID);
#ifdef DEBUG
	InterfacePtr<const IControllingUnknown> controlling( theEditionID, UseDefaultIID() );
	IZPLog_Str( thisFileLA, enLT_ObjectCreations, "Edition boss created = 0x%08X", controlling.get() );
	IZPLog_Str_( thisFileLA, enLT_ObjectCreations, "Object ID = %s", inEditionID.GrabCString() );
#endif
	theEditionID->Set( inEditionID );

	return theEditionID;
}

//----------------------------------------------------------------------------------------
// UpdateEditionList
//----------------------------------------------------------------------------------------
void
CZPEditionMgr::UpdateEditionList(
	const ZPTitleEditionInfoSList &	inEditionInfoList)
{
	//send message edition list will change.
	this->BroadcastMessage( kZPEditionListWillChange );

	ZPTitleEditionInfoSList::const_iterator editionInfoCurrIter = inEditionInfoList.begin();
	ZPTitleEditionInfoSList::const_iterator editionInfoEndIter = inEditionInfoList.end();

	while( editionInfoCurrIter != editionInfoEndIter )
	{
		const ZPTitleEditionInfo & currTitleEditionInfo = **editionInfoCurrIter;
		this->UpdateEditionList( currTitleEditionInfo.mTitleID, currTitleEditionInfo.mEditionList );

		++editionInfoCurrIter;
	}
	//send message edition list updated.
	this->BroadcastMessage( kZPEditionListChanged );
}

//----------------------------------------------------------------------------------------
// UpdateEditionList
//----------------------------------------------------------------------------------------
void
CZPEditionMgr::UpdateEditionList(
	const PMString &			inTitleID,
	const ZPIDPairSList &		inEditionList)
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<IZPTitleMgr> titleMgr( zpApp->GetTitleMgr(), UseDefaultIID() );
	ASSERT(titleMgr);

	const IStringData * theTitle = titleMgr->GetTitleForID( inTitleID );

	ZPEditionIDsList editionIDsToRemove;
	ZPEditionIDsList editionIDsNewList;

	InterfacePtr<IZPEditionList> titleEditionList( theTitle, UseDefaultIID() );
	
	if ( titleEditionList != nil )
	{
		editionIDsToRemove = titleEditionList->GetStringList( false );
	}
	else
	{
		ASSERT( titleEditionList != nil );	//Updating a title that is not cached.
		return;
	}

	//Add/Update the Edition for title
	ZPIDPairSList::const_iterator	currIter = inEditionList.begin();
	ZPIDPairSList::const_iterator	endIter = inEditionList.end();

	while( currIter != endIter )
	{
		const PMString & editionID = (*currIter)->mID;

		//Update the Edition
		const IStringData * theEdition = NULL;
		(*currIter)->mName.SetTranslatable( kFalse );
		bool newEditionCreated = this->AddUpdateEdition( editionID, (*currIter)->mName, &theEdition );
		
		//Add to new list.
		editionIDsNewList.push_back(editionID);
		
		++currIter;

		if( newEditionCreated == false )
			theEdition->AddRef();	//Retain till this ID is referenced.

		//Increament ref count maintained by this manager.
		InterfacePtr<IIntData> cachedCount( theEdition, IID_IZPCACHEMGRCOUNT );
		ASSERT( cachedCount );
		cachedCount->Set( cachedCount->Get() + 1 );
	}

	//Update the edition id list
	titleEditionList->SetStringList( editionIDsNewList );

	//Remove from cached list. Should we? what if same Edition is shared by multiple titles.
	this->DecrementReferenceForEditions( editionIDsToRemove );
	
}

//----------------------------------------------------------------------------------------
// AddUpdateEdition
//----------------------------------------------------------------------------------------
bool
CZPEditionMgr::AddUpdateEdition(
	const PMString &			inEditionID,
	const PMString &			inEditionName,
	const IStringData **		oEditionID)
{
	const IStringData * editionID = this->GetEditionForID( inEditionID );
	bool toReturn = editionID == nil;
	if( toReturn )
	{
		editionID = mEditionMap[inEditionID] = CreateEdition( inEditionID );
	}

	InterfacePtr<IStringData> editionName( editionID, IID_IZPDATANAME );
	ASSERT( editionName );
	editionName->Set( inEditionName );
	if( oEditionID )
		*oEditionID = editionID;

	return toReturn;
}

//----------------------------------------------------------------------------------------
// RemoveFromCache
//----------------------------------------------------------------------------------------
void
CZPEditionMgr::RemoveFromCache(
	const IStringData *				inEditionID)
{
	if( !inEditionID )
		return;

	const PMString & theEditionIDStr = inEditionID->Get();
#ifdef DEBUG
	InterfacePtr<const IControllingUnknown> controlling( inEditionID, UseDefaultIID() );
	IZPLog_Str( thisFileLA, enLT_ObjectCreations, "Edition boss removed from cache = 0x%08X", controlling.get() );
	IZPLog_Str_( thisFileLA, enLT_ObjectCreations, "Object ID = %s", theEditionIDStr.GrabCString() );
#endif
	mEditionMap.erase( inEditionID->Get() );
}

//----------------------------------------------------------------------------------------
// DecrementReferenceForEditions
//----------------------------------------------------------------------------------------
void
CZPEditionMgr::DecrementReferenceForEditions (
	const ZPEditionIDsList &		inEditionIDs)
{
	ZPEditionIDsList::const_iterator idListCurrIter = inEditionIDs.begin();
	ZPEditionIDsList::const_iterator idListEndIter = inEditionIDs.end();
	
	while( idListCurrIter != idListEndIter )
	{
		const IStringData * theEditionID = this->GetEditionForID( *idListCurrIter );
		if( theEditionID )
		{
#ifdef DEBUG
			InterfacePtr<const IControllingUnknown> controlling( theEditionID, UseDefaultIID() );
			int theRefCount = controlling->GetRefCount();
			IZPLog_Str( thisFileLA, enLT_ObjectCreations, "Edition boss at 0x%08X has refcount = %d", controlling.get(), theRefCount );
#endif

			//Update reference count maintained by this manager
			InterfacePtr<IIntData> cachedCount( theEditionID, IID_IZPCACHEMGRCOUNT );
			ASSERT( cachedCount );
			if( cachedCount->Get() == 1 )
				this->RemoveFromCache( theEditionID );
			else
				cachedCount->Set( cachedCount->Get() -1 );
			
			theEditionID->Release();
		}
		++idListCurrIter;
	}
}



//----------------------------------------------------------------------------------------
// GetEditionForID
//----------------------------------------------------------------------------------------
const IStringData *
CZPEditionMgr::GetEditionForID(
	const PMString &			inEditionID) const
{
	ZPEditionIDMapConstIter foundIter = mEditionMap.find( inEditionID );
	ZPEditionIDMapConstIter endIter = mEditionMap.end();
	if( foundIter != endIter )
		return foundIter->second;
	else
		return nil;
}

//----------------------------------------------------------------------------------------
// GetEditionForAssetID
//----------------------------------------------------------------------------------------
const IStringData *
CZPEditionMgr::GetEditionForAssetID(
	const PMString &				inAssetID) const
{
	ZPEditionIDMapConstIter currIter = mEditionMap.begin();
	ZPEditionIDMapConstIter endIter = mEditionMap.end();
	while( currIter != endIter )
	{
		const IStringData * editionID = currIter->second;
		InterfacePtr<const IZPAssetList> editionAssetList( editionID , UseDefaultIID() );
		ASSERT( editionAssetList );
		if( editionAssetList->ContainsAssetID( inAssetID ) )
			return editionID;
		++currIter;
	}
	return nil;
}

//----------------------------------------------------------------------------------------
// GetEditionForTaskID
//----------------------------------------------------------------------------------------
const IStringData *
CZPEditionMgr::GetEditionForTaskID(
	const PMString &				inTaskID) const
{
	ZPEditionIDMapConstIter currIter = mEditionMap.begin();
	ZPEditionIDMapConstIter endIter = mEditionMap.end();
	while( currIter != endIter )
	{
		const IStringData * dbEditionID = currIter->second;
		InterfacePtr<const IZPTaskList> editionTaskList( dbEditionID , UseDefaultIID() );
		ASSERT( editionTaskList );
		if( editionTaskList->ContainsTaskID( inTaskID ) )
			return dbEditionID;
		++currIter;
	}
	return nil;
}

//----------------------------------------------------------------------------------------
// BroadcastMessage
//----------------------------------------------------------------------------------------
void
CZPEditionMgr::BroadcastMessage(
	const ClassID&				inTheChange,
	void *						inChangedBy )
{
	InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
	ASSERT(selfSubject);
	selfSubject->Change( inTheChange, IZPEditionMgr::kDefaultIID, inChangedBy );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// Shutdown
//----------------------------------------------------------------------------------------
void
CZPEditionMgr::Shutdown()
{
	//TODO:
}

#pragma mark -
//----------------------------------------------------------------------------------------
// CompareEditionIDByName 
//----------------------------------------------------------------------------------------
bool
CompareEditionIDByName::operator ()(
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

	const IZPEditionMgr * editionMgr = zpApp->GetEditionMgr();
	ASSERT(editionMgr);

	InterfacePtr<const IStringData> editionName1( editionMgr->GetEditionForID(inFirstVal), IID_IZPDATANAME );
	ASSERT( editionName1 );

	InterfacePtr<const IStringData> editionName2( editionMgr->GetEditionForID(inSecondVal), IID_IZPDATANAME );
	ASSERT( editionName2 );

	if( editionName1 && editionName2 )
	{
		const PMString & name1 = editionName1->Get();
		const PMString & name2 = editionName2->Get();
		bool toReturn = name1.Compare(false, name2) > 0;//name1 < name2;
#ifdef DEBUG
		//IZPLog_Str( thisFileLA, enLT_DebugInfo, "Compare \"%s\" and \"%s\" , result %hd", name1.GrabCString(), name2.GrabCString(), (short int) toReturn );
#endif
		return !toReturn;
	}
	return false;
}
