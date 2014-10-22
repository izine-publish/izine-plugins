//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/DataMgr/CZMAdMgr.cpp $
//	$Revision: 2784 $
//	$Date: 2011-04-11 16:24:21 +0200 (Mon, 11 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 11-4-2011
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
#include "Utils.h"
#include "FileUtils.h"

#include "IZMediaID.h"

//IZP Interfaces
#include "IZMApp.h"
#include "IZMAdMgr.h"
#include "IZMAdFileMgr.h"
#include "IZMAdList.h"
#include "IZMAdInfo.h"

//IZP General includes

#include "IZPLog.h"

#define thisFileLA	enLMA_DBObjects
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

class CZMAdMgr : public CPMUnknown<IZMAdMgr>
{
public:
				CZMAdMgr(IPMUnknown * boss);
	virtual		~CZMAdMgr();

	void				Shutdown();

	void				UpdateAdList(
							const IStringData *			inAdFileID,
							const ZMstAdSList &			inAdList );
	void				UpdateAdList(
							const PMString &			inAdFileID,
							const ZMstAdSList &			inAdList );
	
	//void				AddAd(
	//						const IStringData *			inAdFileID,
	//						const PMString &			inAdID,
	//						const PMString &			inAdName );

	const IStringData *	GetAdByID(
							const PMString &			inAdID ) const;
	
	void				DecrementReferenceForAds(
							const ZMAdIDsList &		inAdIDs );

	//void				DeleteAd(
	//						const PMString &			inAdID ) const;

protected:
	static IStringData * CreateAd(
							const PMString &			inAdID );

	bool				AddUpdateAd(
							const PMString &			inAdID,
							const stAd &				inAdInfo,
							const PMString &			inAdFileID,
							const IStringData **		oAdID );

	void				RemoveFromCache(
							const IStringData *			inAdID );

	void				BroadcastMessage(
							const ClassID &				inTheChange,
							void*						inChangedBy = NULL );

private:
	ZMAdIDMap		mAdMap;
};


CREATE_PMINTERFACE(CZMAdMgr, kZMAdMgrImpl)

//----------------------------------------------------------------------------------------
// MakeAdID
//----------------------------------------------------------------------------------------
PMString
IZMAdMgr::MakeAdID(
	const PMString &				inAdFileID,
	const PMString &				inAdID)
{
	PMString adID = inAdFileID;
	adID.Append( FileUtils::GetDirectorySeperator());
	adID.Append( inAdID);
	return adID;
}



CZMAdMgr::CZMAdMgr(IPMUnknown * boss) :
CPMUnknown<IZMAdMgr>(boss)
{

}

CZMAdMgr::~CZMAdMgr()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// CreateAd
//----------------------------------------------------------------------------------------
IStringData *
CZMAdMgr::CreateAd(
	const PMString &			inAdID)
{
	IStringData * theAdID = ::CreateObject2<IStringData>( kZMDataAdBoss , IID_IZMDATAID );
	ASSERT(theAdID);
#ifdef DEBUG
	InterfacePtr<const IControllingUnknown> controlling( theAdID, UseDefaultIID() );
	IZPLog_Str( thisFileLA, enLT_ObjectCreations, "Ad boss created = 0x%08X", controlling.get() );
	IZPLog_Str_( thisFileLA, enLT_ObjectCreations, "Object ID = %s", inAdID.GrabCString() );
#endif
	theAdID->Set( inAdID );

	return theAdID;
}

//----------------------------------------------------------------------------------------
// DeleteAd
//----------------------------------------------------------------------------------------
//void
//CZMAdMgr::DeleteAd(
//	const PMString &			inAdID) const
//{
//	LogFunctionEnterExit;
//	//Not required.
//}

#pragma mark -
//----------------------------------------------------------------------------------------
// UpdateAdList
//----------------------------------------------------------------------------------------
void
CZMAdMgr::UpdateAdList(
	const PMString &			inAdFileID,
	const ZMstAdSList &			inAdList)
{
	InterfacePtr<const IZMApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<IZMAdFileMgr> adFileMgr( zpApp->GetAdFileMgr(), UseDefaultIID() );
	ASSERT(adFileMgr);

	const IStringData * theAdFileID = adFileMgr->GetAdFileByID( inAdFileID );

	this->UpdateAdList( theAdFileID, inAdList );
}

//----------------------------------------------------------------------------------------
// UpdateAdList
//----------------------------------------------------------------------------------------
void
CZMAdMgr::UpdateAdList(
	const IStringData *			inAdFileID,
	const ZMstAdSList &			inAdList)
{
	ZMAdIDsList adIDsToRemove;
	ZMAdIDsList adIDsNewList;

	InterfacePtr<IZMAdList> adFileAdList( inAdFileID, UseDefaultIID() );

	ASSERT( adFileAdList != nil );	//Updating an adFile that is not cached.
	if ( adFileAdList == nil )
		return;

	adIDsToRemove = adFileAdList->GetStringList( false );
	
	InterfacePtr<ISubject> adFileSubject( adFileAdList, UseDefaultIID() );
	ASSERT(adFileSubject);
	//Send list will change message so that UI does not update with intermediate data.
	adFileSubject->Change( kZMAdListWillChangeMsg, IZMAdList::kDefaultIID, nil );

	//Add/Update the Ad for adFile
	ZMstAdSListCIter	currIter = inAdList.begin();
	ZMstAdSListCIter	endIter = inAdList.end();
	
	const PMString & theAdFileIDStr = inAdFileID->Get();

	while( currIter != endIter )
	{
		const stAd & currAd = **currIter;
		const PMString adID = IZMAdMgr::MakeAdID( theAdFileIDStr, currAd.mId );
		
		//Update the Ad
		const IStringData * theAdID = NULL;
		//(*currIter)->mName.SetTranslatable( kFalse );
		bool newAdCreated = this->AddUpdateAd( adID, currAd, theAdFileIDStr, &theAdID );
		
		//Add to new list.
		adIDsNewList.push_back(adID);
		
		++currIter;
		
		if( newAdCreated == false )
			theAdID->AddRef();	//Retain till this ID is referenced.

		//Increament ref count maintained by this manager.
		InterfacePtr<IIntData> cachedCount( theAdID, IID_IZMCACHEMGRCOUNT );
		ASSERT( cachedCount );
		cachedCount->Set( cachedCount->Get() + 1 );
	}

	//Update the Ad id list
	adFileAdList->SetStringList( adIDsNewList );

	//Remove from cached list. Should we? what if same ad is shared by multiple ad files.
	this->DecrementReferenceForAds( adIDsToRemove );
}

//----------------------------------------------------------------------------------------
// AddAd
//----------------------------------------------------------------------------------------
//void
//CZMAdMgr::AddAd(
//	const IStringData *			inAdFileID,
//	const PMString &			inAdID,
//	const PMString &			inAdName )
//{
//	InterfacePtr<IZMAdList> adFileAdList( inAdFileID, UseDefaultIID() );
//
//	ASSERT( adFileAdList != nil );	//Updating an adFile that is not cached.
//	if ( adFileAdList == nil )
//		return;
//
//	ZMAdIDsList adIDsNewList = adFileAdList->GetStringList( false );
//	//Update the Ad
//	const IStringData * theAdID = NULL;
//	//(*currIter)->mName.SetTranslatable( kFalse );
//	
//	bool newAdCreated = this->AddUpdateAd( inAdID, inAdName, &theAdID );
//	
//	if( !theAdID )
//		return;
//
//	//Add to new list.
//	adIDsNewList.push_back( inAdID );
//
//	//Increament ref count maintained by this manager.
//	InterfacePtr<IIntData> cachedCount( theAdID, IID_IZMCACHEMGRCOUNT );
//	ASSERT( cachedCount );
//	cachedCount->Set( cachedCount->Get() + 1 );
//
//	//Update the Ad id list
//	adFileAdList->SetStringList( adIDsNewList, false );
//}

//----------------------------------------------------------------------------------------
// AddUpdateAd
//----------------------------------------------------------------------------------------
bool
CZMAdMgr::AddUpdateAd(
	const PMString &			inAdID,
	const stAd &				inAdInfo,
	const PMString &			inAdFileID,
	const IStringData **		oAdID)
{
	const IStringData * adID = this->GetAdByID( inAdID );
	bool toReturn = adID == nil;
	if( toReturn )
	{
		adID = mAdMap[inAdID] = CreateAd( inAdID );

		//Ad name does not changes, so setting this only once when it is created.
		InterfacePtr<IStringData> adName( adID, IID_IZMDATANAME );
		ASSERT( adName );
		adName->Set( inAdInfo.mId );

		InterfacePtr<IStringData> adFileID( adID, IID_IZMADFILEID );
		ASSERT( adFileID );
		adFileID->Set( inAdFileID );
	}

	//update data
	InterfacePtr<IZMAdInfo> adInfo( adID, UseDefaultIID() );
	ASSERT( adInfo );
	adInfo->SetAdDetail( inAdInfo );

	if( oAdID )
		*oAdID = adID;

	return toReturn;
}

//----------------------------------------------------------------------------------------
// RemoveFromCache
//----------------------------------------------------------------------------------------
void
CZMAdMgr::RemoveFromCache(
	const IStringData *				inAdID)
{
	if( !inAdID )
		return;

	const PMString & theAdIDStr = inAdID->Get();
#ifdef DEBUG
	InterfacePtr<const IControllingUnknown> controlling( inAdID, UseDefaultIID() );
	IZPLog_Str( thisFileLA, enLT_ObjectCreations, "Ad boss removed from cache = 0x%08X", controlling.get() );
	IZPLog_Str_( thisFileLA, enLT_ObjectCreations, "Object ID = %s", theAdIDStr.GrabCString() );
#endif

	mAdMap.erase( inAdID->Get() );
}

//----------------------------------------------------------------------------------------
// DecrementReferenceForAds
//----------------------------------------------------------------------------------------
void
CZMAdMgr::DecrementReferenceForAds(
	const ZMAdIDsList &		inAdIDs)
{
	ZMAdIDsListCIter idListCurrIter = inAdIDs.begin();
	ZMAdIDsListCIter idListEndIter = inAdIDs.end();
	
	while( idListCurrIter != idListEndIter )
	{
		const IStringData * theAdID = this->GetAdByID( *idListCurrIter );

		//Update reference count maintained by this manager
		InterfacePtr<IIntData> cachedCount( theAdID, IID_IZMCACHEMGRCOUNT );
		ASSERT( cachedCount );
		if( cachedCount->Get() == 1 )
			this->RemoveFromCache( theAdID );
		else
			cachedCount->Set( cachedCount->Get() -1 );

		if( theAdID )
			theAdID->Release();
		++idListCurrIter;
	}
}

//----------------------------------------------------------------------------------------
// GetAdByID
//----------------------------------------------------------------------------------------
const IStringData *
CZMAdMgr::GetAdByID(
	const PMString &			inAdID) const

{
	ZMAdIDMapCIter foundIter = mAdMap.find( inAdID );
	ZMAdIDMapCIter endIter = mAdMap.end();
	if( foundIter != endIter )
		return foundIter->second;
	else
		return nil;
}

//----------------------------------------------------------------------------------------
// BroadcastMessage
//----------------------------------------------------------------------------------------
void
CZMAdMgr::BroadcastMessage(
	const ClassID &				inTheChange,
	void*						inChangedBy)
{
	InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
	ASSERT(selfSubject);
	selfSubject->Change( inTheChange, IZMAdMgr::kDefaultIID, inChangedBy );
}

//----------------------------------------------------------------------------------------
// Shutdown
//----------------------------------------------------------------------------------------
void
CZMAdMgr::Shutdown()
{
	//TODO: 
}

#pragma mark -
