//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/DataMgr/CZMEditionMgr.cpp $
//	$Revision: 2749 $
//	$Date: 2011-04-08 11:32:30 +0200 (Fri, 08 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 5-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IIntData.h"
#include "IStringData.h"
#include "ISubject.h"

//SDK General includes
#include "FileUtils.h"

#include "IZMediaID.h"

//IZP Interfaces
#include "IZMApp.h"
#include "IZMAdFileList.h"
#include "IZMEditionList.h"
#include "IZMEditionMgr.h"
#include "IZMTitleMgr.h"
#include "IZMAdFileMgr.h"
#include "IZMModelController.h"

//IZP General includes

#include "IZPLog.h"

#define thisFileLA	enLMA_DBObjects
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

class CZMEditionMgr : public CPMUnknown<IZMEditionMgr>
{
public:
						CZMEditionMgr(IPMUnknown * boss);
	virtual				~CZMEditionMgr();

	void				Shutdown();

	void				UpdateEditionList(
							const PMString &				inTitleID,
							const ZMstEditionSList &		inEditionList );
	void				UpdateEditionList(
							const IStringData *				inTitleID,
							const ZMstEditionSList &		inEditionList );

	const IStringData *	GetEditionForID(
							const PMString &				inEditionID ) const;
	
	const IStringData *	GetEditionForAdFileID(
							const PMString &				inAdFileID ) const;
	
	void				DecrementReferenceForEditions (
							const ZMEditionIDsList &		inEditionIDs );
	
	void				FetchAdFileListForEdition(
							const PMString &				inEditinoID );
protected:
	static IStringData * CreateEdition(
							const PMString &				inEditionID);

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
	ZMEditionIDMap		mEditionMap;
};


CREATE_PMINTERFACE(CZMEditionMgr, kZMEditionMgrImpl)

#pragma mark -
//----------------------------------------------------------------------------------------
// MakeEditionID
//----------------------------------------------------------------------------------------
PMString
IZMEditionMgr::MakeEditionID(
	const PMString &				inTitleID,
	const PMString &				inEditionName)
{
	PMString editionID = inTitleID;
	editionID.Append( FileUtils::GetDirectorySeperator());
	editionID.Append( inEditionName);
	return editionID;
}

#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZMEditionMgr::CZMEditionMgr(IPMUnknown * boss) :
CPMUnknown<IZMEditionMgr>(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZMEditionMgr::~CZMEditionMgr()
{

}
#pragma mark -
//----------------------------------------------------------------------------------------
// CreateEdition
//----------------------------------------------------------------------------------------
IStringData*
CZMEditionMgr::CreateEdition(//Caller will be owner of obj.
	const PMString &			inEditionID)
{
	IStringData * theEditionID = ::CreateObject2<IStringData>( kZMDataTitleEditionBoss , IID_IZMDATAID );
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
CZMEditionMgr::UpdateEditionList(
	const PMString & 				inTitleID,
	const ZMstEditionSList &		inEditionList )
{
	InterfacePtr<const IZMApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<IZMTitleMgr> titleMgr( zpApp->GetTitleMgr(), UseDefaultIID() );
	ASSERT(titleMgr);

	const IStringData * theTitle = titleMgr->GetTitleForID( inTitleID );

	this->UpdateEditionList( theTitle, inEditionList );
}

//----------------------------------------------------------------------------------------
// UpdateEditionList
//----------------------------------------------------------------------------------------
void
CZMEditionMgr::UpdateEditionList(
	const IStringData *			inTitleID,
	const ZMstEditionSList &	inEditionList)
{
	//send message edition list will change.
	this->BroadcastMessage( kZMEditionListWillChange );

	InterfacePtr<IZMAdFileMgr> adFileMgr( this, UseDefaultIID() );
	ASSERT(adFileMgr);

	ZMEditionIDsList editionIDsToRemove;
	ZMEditionIDsList editionIDsNewList;

	InterfacePtr<IZMEditionList> titleEditionList( inTitleID, UseDefaultIID() );
	
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
	ZMstEditionSListCIter	currIter = inEditionList.begin();
	ZMstEditionSListCIter	endIter = inEditionList.end();

	const PMString & titleID = inTitleID->Get();
	while( currIter != endIter )
	{
		const stEdition & currEdition = **currIter;
		PMString editionID = IZMEditionMgr::MakeEditionID( titleID, currEdition.mName);

		//Update the Edition
		const IStringData * theEditionID = NULL;
		(*currIter)->mName.SetTranslatable( kFalse );
		bool newEditionCreated = this->AddUpdateEdition( editionID, currEdition.mName, &theEditionID );
		
		//Add to new list.
		editionIDsNewList.push_back(editionID);
		
		//update the Ad file list
		adFileMgr->UpdateAdFileList( theEditionID, currEdition.mXmlFiles );

		++currIter;

		if( newEditionCreated == false )
			theEditionID->AddRef();	//Retain till this ID is referenced.

		//Increament ref count maintained by this manager.
		InterfacePtr<IIntData> cachedCount( theEditionID, IID_IZMCACHEMGRCOUNT );
		ASSERT( cachedCount );
		cachedCount->Set( cachedCount->Get() + 1 );
	}

	//Update the edition id list
	titleEditionList->SetStringList( editionIDsNewList );

	//Remove from cached list. Should we? what if same Edition is shared by multiple titles.
	this->DecrementReferenceForEditions( editionIDsToRemove );

	//send message edition list updated.
	this->BroadcastMessage( kZMEditionListChanged );
}

//----------------------------------------------------------------------------------------
// AddUpdateEdition
//----------------------------------------------------------------------------------------
bool
CZMEditionMgr::AddUpdateEdition(
	const PMString &			inEditionID,
	const PMString &			inEditionName,
	const IStringData **		oEditionID)
{
	const IStringData * editionID = this->GetEditionForID( inEditionID );
	bool toReturn = editionID == nil;
	if( toReturn )
	{
		editionID = mEditionMap[inEditionID] = CreateEdition( inEditionID );

		//Edition name does not changes, so setting this only once when it is created.
		InterfacePtr<IStringData> editionName( editionID, IID_IZMDATANAME );
		ASSERT( editionName );
		editionName->Set( inEditionName );
	}

	if( oEditionID )
		*oEditionID = editionID;

	return toReturn;
}

//----------------------------------------------------------------------------------------
// RemoveFromCache
//----------------------------------------------------------------------------------------
void
CZMEditionMgr::RemoveFromCache(
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
CZMEditionMgr::DecrementReferenceForEditions (
	const ZMEditionIDsList &		inEditionIDs)
{
	ZMEditionIDsListCIter idListCurrIter = inEditionIDs.begin();
	ZMEditionIDsListCIter idListEndIter = inEditionIDs.end();
	
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
			InterfacePtr<IIntData> cachedCount( theEditionID, IID_IZMCACHEMGRCOUNT );
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
CZMEditionMgr::GetEditionForID(
	const PMString &			inEditionID) const
{
	ZMEditionIDMapCIter foundIter = mEditionMap.find( inEditionID );
	ZMEditionIDMapCIter endIter = mEditionMap.end();
	if( foundIter != endIter )
		return foundIter->second;
	else
		return nil;
}

//----------------------------------------------------------------------------------------
// GetEditionForAdFileID
//----------------------------------------------------------------------------------------
const IStringData *
CZMEditionMgr::GetEditionForAdFileID(
	const PMString &				inAdFileID) const
{
	ZMEditionIDMapCIter currIter = mEditionMap.begin();
	ZMEditionIDMapCIter endIter = mEditionMap.end();
	while( currIter != endIter )
	{
		const IStringData * editionID = currIter->second;
		InterfacePtr<const IZMAdFileList> editionAdFileList( editionID , UseDefaultIID() );
		ASSERT( editionAdFileList );
		if( editionAdFileList->ContainsAdFileID( inAdFileID ) )
			return editionID;
		++currIter;
	}
	return nil;
}

//----------------------------------------------------------------------------------------
// BroadcastMessage
//----------------------------------------------------------------------------------------
void
CZMEditionMgr::BroadcastMessage(
	const ClassID&				inTheChange,
	void *						inChangedBy )
{
	InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
	ASSERT(selfSubject);
	selfSubject->Change( inTheChange, IZMEditionMgr::kDefaultIID, inChangedBy );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// FetchAdFileListForEdition
//----------------------------------------------------------------------------------------
void
CZMEditionMgr::FetchAdFileListForEdition(
	const PMString &				inEditinoID)
{
	InterfacePtr<IZMModelController> configController( gSession, UseDefaultIID() );
	ASSERT( configController );
	configController->FetchAdFilesForEdition( inEditinoID );
}

//----------------------------------------------------------------------------------------
// Shutdown
//----------------------------------------------------------------------------------------
void
CZMEditionMgr::Shutdown()
{
	//TODO:
}
