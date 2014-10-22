//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Controls/AdsTreeView/CZMAdsTVDataModel.cpp $
//	$Revision: 2913 $
//	$Date: 2011-04-29 16:48:36 +0200 (Fri, 29 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 7-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IObserver.h"
#include "IStringData.h"
#include "ISubject.h"

//SDK General includes
#include "Utils.h"

//OS / ThirdParty includes

#include "iZMediaID.h"

//IZP Interfaces
#include "IZMApp.h"
#include "IZMAdInfo.h"
#include "IZMAdFileList.h"
#include "IZMAdFileMgr.h"
#include "IZMAdMgr.h"
#include "IZMAdsTVDataModel.h"
#include "IZMTitleMgr.h"
#include "IZMAdList.h"

//IZP General includes
#include "ZMDataHelper.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AssetsTV
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

//TODO: should this model be kept with TreeView or Ads Panel?
// It would be better to have model separate from TreeView.

#define inheritClass CPMUnknown<IZMAdsTVDataModel>
class CZMAdsTVDataModel : public inheritClass
{
	//Map of AdFileID and AdIDs 
	typedef std::map<PMString, ZMAdIDsList >	ZPAdFileAdsIDMap;

public:
						CZMAdsTVDataModel(IPMUnknown * boss);
	virtual				~CZMAdsTVDataModel();

//	const PMString &		GetTitleID() const;
	const PMString &		GetEditionID() const;

	const ZMAdFileIDsList *	GetAdFileIDList() const;
	void					SetAdFileIDList(
								const ZMAdFileIDsList *		inAdFileIDList );
	const PMString *		GetNthAdFileID(
								int							inIndex ) const;
	int32					GetAdFileIDIndex(
								const PMString &			inAdFileID ) const;

	const ZMAdIDsList *		GetAdIDList(
								const PMString &			inAdFileID ) const;
	const PMString *		GetNthAdID(
								const PMString &			inAdFileID,
								int							inIndex ) const;
	int32					GetAdIDIndex(
								const PMString &			inAdFileID,
								const PMString &			inAdID ) const;
	const PMString *		GetAdFileIDOfAd(
								const PMString &			inAdID ) const;
	const PMString *		GetAdFileIDOfAd(
								const IStringData *			inAdID ) const;
	
	void					SetAdIDList(
								const PMString &			inAdFileID,
								const ZMAdIDsList &			inAdIDList );
	
	bool					GetSkipAdFileListUpdates() const; //AdFile List will not change if true.
	void					SetSkipAdFileListUpdates(
								bool						inSkipAdFileListChanges );

	void					UpdateData(
								const PMString &			inTitleID,
								const PMString &			inEditionID );

	bool					UpdateAdFileList();
	void					UpdateAdList();

	void					HandleAdInfoChange(
								ISubject*					inAdSubject );

	void					MarkAllAdFileDirty();
//	const FilterAdFnPtr	GetAdFilteringFunction() const;
//	void					SetAdFilteringFunction(
//								FilterAdFnPtr			inFunc );
protected:
	void				RemoveListeningEdition();
	void				AddListeningToEdition();

	void				RemoveListeningAdFiles();
	void				RemoveListeningAdFile(
							const IStringData *			inAdFileID );

	void				RemoveListeningAds();
	void				AddListeningToAd();

	void				RemoveListeningObject(
							const IPMUnknown *			inObjectToListen,
							const PMIID &				inProtocol);
	void				AddListeningToObject(
							const IPMUnknown *			inObjectToListen,
							const PMIID &				inProtocol);

	void				BroadcastMessage(
							const ClassID&						inTheChange,
							void *								inChangedBy = nil) const;
private:
	PMString			mTitleID;
	PMString			mEditionID;

	bool				mSkipAdFileListUpdates;

	//retained here so that it does not get destroyed before its references are removed.
	InterfacePtr<const IStringData> mTitle;	//Title for which editions are displayed by this treeview.
	InterfacePtr<const IStringData> mEdition;	//Edition for which ads are displayed by this treeview.
	
	const ZMAdFileIDsList *			mAdFileIDList;
	ZPAdFileAdsIDMap				mAdFileAds;

//	FilterAdFnPtr					mFilterAd;
};


CREATE_PMINTERFACE(CZMAdsTVDataModel, kZMAdsTVDataModelImpl)

CZMAdsTVDataModel::CZMAdsTVDataModel(IPMUnknown * boss)
: inheritClass(boss)
, mAdFileIDList( nil )
//, mFilterAd ( nil )
, mSkipAdFileListUpdates( false )
{
	LogFunctionEnterExit;
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZMAdsTVDataModel::~CZMAdsTVDataModel()
{
	LogFunctionEnterExit;
}

#pragma mark -

//----------------------------------------------------------------------------------------
// GetTitleID
//----------------------------------------------------------------------------------------
//const PMString &
//CZMAdsTVDataModel::GetTitleID()const
//{
//	return mTitleID;
//}

//----------------------------------------------------------------------------------------
// GetEditionID
//----------------------------------------------------------------------------------------
const PMString &
CZMAdsTVDataModel::GetEditionID()const
{
	return mEditionID;
}

//----------------------------------------------------------------------------------------
// GetAdFileIDList
//----------------------------------------------------------------------------------------
const ZMAdFileIDsList *
CZMAdsTVDataModel::GetAdFileIDList()const
{
	return mAdFileIDList;
}

//----------------------------------------------------------------------------------------
// SetAdFileIDList
//----------------------------------------------------------------------------------------
void
CZMAdsTVDataModel::SetAdFileIDList(
	const ZMAdFileIDsList *		inAdFileIDList)
{
	LogFunctionEnterExit;
	mAdFileIDList = inAdFileIDList;
}

//----------------------------------------------------------------------------------------
// GetNthAdFileID
//----------------------------------------------------------------------------------------
const PMString *
CZMAdsTVDataModel::GetNthAdFileID(
	int							inIndex) const
{
	if( mAdFileIDList )
	{
		ASSERT( inIndex >= 0 && inIndex < mAdFileIDList->size() );
		return & mAdFileIDList->at(inIndex);
	}
	return nil;
}

//----------------------------------------------------------------------------------------
// GetAdFileIDIndex
//----------------------------------------------------------------------------------------
int32
CZMAdsTVDataModel::GetAdFileIDIndex(
	const PMString &			inAdFileID) const
{
	int32 toReturn = -1;

	if( mAdFileIDList )
	{
		ZMAdFileIDsList::const_iterator beginIter = mAdFileIDList->begin();
		ZMAdFileIDsList::const_iterator endIter = mAdFileIDList->end();
		ZMAdFileIDsList::const_iterator foundIter = std::find( beginIter, endIter, inAdFileID );
		if( foundIter != endIter )
		{
			toReturn = foundIter - beginIter;
		}
	}
	return toReturn;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetAdIDList
//----------------------------------------------------------------------------------------
const ZMAdIDsList *
CZMAdsTVDataModel::GetAdIDList(
	const PMString &			inAdFileID) const
{
	ZPAdFileAdsIDMap::const_iterator foundIter = mAdFileAds.find( inAdFileID );
	ZPAdFileAdsIDMap::const_iterator endIter = mAdFileAds.end();
	if( foundIter != endIter )
		return & foundIter->second;
	else
		return nil;
	return nil;
}

//----------------------------------------------------------------------------------------
// GetNthAdID
//----------------------------------------------------------------------------------------
const PMString *
CZMAdsTVDataModel::GetNthAdID(
	const PMString &			inAdFileID,
	int							inIndex) const
{
	const ZMAdIDsList * adList = this->GetAdIDList( inAdFileID );
	if( adList && adList->size() > inIndex && inIndex >= 0 )
		return & adList->at(inIndex);
	return nil;
}

//----------------------------------------------------------------------------------------
// GetAdIDIndex
//----------------------------------------------------------------------------------------
int32
CZMAdsTVDataModel::GetAdIDIndex(
	const PMString &			inAdFileID,
	const PMString &			inAdID) const
{
	const ZMAdIDsList * adList = this->GetAdIDList( inAdFileID );
	int32 toReturn = -1;
	if( adList && adList->size() > 0 )
	{
		ZMAdIDsList::const_iterator beginIter = adList->begin();
		ZMAdIDsList::const_iterator endIter = adList->end();
		ZMAdIDsList::const_iterator foundIter = std::find( beginIter, endIter, inAdID );
		if( foundIter != endIter )
		{
			toReturn = foundIter - beginIter;
		}
	}
	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetAdFileIDOfAd
//----------------------------------------------------------------------------------------
const PMString *
CZMAdsTVDataModel::GetAdFileIDOfAd(
	const PMString &			inAdID) const
{
	InterfacePtr<const IZMApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<const IZMAdMgr> adMgr( zpApp->GetAdMgr(), UseDefaultIID() );
	ASSERT(adMgr);

	const IStringData * adID = adMgr->GetAdByID( inAdID );
	if( adID )
		return this->GetAdFileIDOfAd( adID );
	return nil;
}

//----------------------------------------------------------------------------------------
// GetAdFileIDOfAd
//----------------------------------------------------------------------------------------
const PMString *
CZMAdsTVDataModel::GetAdFileIDOfAd(
	const IStringData *			inAdID) const
{
	
	InterfacePtr<const IStringData> adFileID( inAdID, IID_IZMADFILEID );
	ASSERT(adFileID);
	
	if( adFileID )
		return & adFileID->Get();

	return nil;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// SetAdIDList
//----------------------------------------------------------------------------------------
void
CZMAdsTVDataModel::SetAdIDList(
	const PMString &		inAdFileID,
	const ZMAdIDsList &		inAdIDList)
{
	LogFunctionEnterExit;

	ZMAdIDsList::const_iterator adIDIter = inAdIDList.begin();
	ZMAdIDsList::const_iterator adIDEndIter = inAdIDList.end();

	InterfacePtr<const IZMApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<const IZMAdMgr> adMgr( zpApp->GetAdMgr(), UseDefaultIID() );
	ASSERT(adMgr);

	while( adIDIter != adIDEndIter )
	{
		const PMString & currAdID = *adIDIter;
		
		const IStringData * adID = adMgr->GetAdByID( currAdID );

		if(inAdFileID.CharCount() > 0 )// && ( mFilterAd == nil || mFilterAd(adID) ) )
		{
			mAdFileAds[inAdFileID].push_back( currAdID );
			//Listen to this ad for changes. We need to change model when status of ad changes.
			this->AddListeningToObject( adID, IZMAdInfo::kDefaultIID );
		}
		++adIDIter;
	}
}

//----------------------------------------------------------------------------------------
// GetSkipAdFileListUpdates
//----------------------------------------------------------------------------------------
bool
CZMAdsTVDataModel::GetSkipAdFileListUpdates()const
{
	return mSkipAdFileListUpdates;
}

//----------------------------------------------------------------------------------------
// SetSkipAdFileListUpdates
// AdFile List will not change if true.
//----------------------------------------------------------------------------------------
void
CZMAdsTVDataModel::SetSkipAdFileListUpdates(
	bool						inSkipAdFileListChanges)
{
	LogFunctionEnterExit;
	mSkipAdFileListUpdates = inSkipAdFileListChanges;
}

//----------------------------------------------------------------------------------------
// UpdateData
//----------------------------------------------------------------------------------------
void
CZMAdsTVDataModel::UpdateData(
	const PMString &			inTitleID,
	const PMString &			inEditionID)
{
	LogFunctionEnterExit;
	mTitleID = inTitleID;
	mEditionID = inEditionID;

	const IStringData * theTitle = ZMDataHelper::GetTitleByID( inTitleID );
	
	if( theTitle == nil )
	{
		///	this->RemoveListeningTitle();
		mTitle.reset(nil);
	}
	else
	{
		///bool newTitle = (mTitle.get() != theTitle );
		///if( newTitle )
		///	this->RemoveListeningTitle();
		theTitle->AddRef();
		mTitle.reset(theTitle);
		///if( newTitle )
		///	this->AddListeningToTitle();
	}

	const IStringData * theEdition = ZMDataHelper::GetEditionByID( inEditionID );
	
	ZMAdIDsList emptyList;
	if( theEdition == nil )
	{
		this->RemoveListeningEdition();
		mEdition.reset(nil);
		
		this->RemoveListeningAdFiles();
		this->SetAdFileIDList( nil );
		///this->SetAdIDList( emptyList );
	}
	else
	{
		bool newEdition = mEdition.get() != theEdition;
		if( newEdition )
			this->RemoveListeningEdition();

		this->RemoveListeningAdFiles();

		theEdition->AddRef();
		mEdition.reset(theEdition);
		
		if( newEdition )
			this->AddListeningToEdition();
		this->SetAdFileIDList( nil );
		///this->SetAdIDList( emptyList );
		
	}

	//Update the Ad File list
	if( false == this->UpdateAdFileList() )
		return;	//TODO: should we clear the tree here.?
	
	//Update the Ads list.
	//TODO: update ad list only for expanded nodes
	this->UpdateAdList();

	//Change root.
	//Should we do it here. Model change message automatically does it.
}

//----------------------------------------------------------------------------------------
// UpdateAdFileList
//----------------------------------------------------------------------------------------
bool
CZMAdsTVDataModel::UpdateAdFileList()
{
	bool toReturn = true;
	
	LogFunctionEnterExit;

	if( mSkipAdFileListUpdates )	//Ad File List of this edition is being updated. Don't update now.
		return false;	
	
	InterfacePtr<IZMAdFileList> editionAdFileList( mEdition, UseDefaultIID() );
	
	mAdFileIDList = nil;
	if ( editionAdFileList != nil )
	{
		this->SetAdFileIDList( & editionAdFileList->GetStringList() );
		if( editionAdFileList->HasData() == false )
		{
			ASSERT( editionAdFileList->HasData() );	//all ad file list will have data as it is fetched along with title list
			//toReturn = false;

			///InterfacePtr<const IZMApp> zpApp( gSession, UseDefaultIID() );
			///ASSERT( zpApp );

			///InterfacePtr<IZPTitleAdFileMgr> adFileMgr( zpApp->GetTitleMgr(), UseDefaultIID() );
			///ASSERT(adFileMgr);

			///this->SetAdFileIDList( & adFileMgr->GetDefaultAdFileIDs() );
		}
		//If status id list comes empty, then it's observer will be notified when the list is available.
	}

	if( toReturn )
		this->BroadcastMessage( kZMUIAdsTV_ModelChangedMsg );
	return toReturn;
}

//----------------------------------------------------------------------------------------
// UpdateAdList
//----------------------------------------------------------------------------------------
void
CZMAdsTVDataModel::UpdateAdList()
{
	LogFunctionEnterExit;

	if( mSkipAdFileListUpdates )	//Ad File List of this edition is being updated. Don't update now.
		return;
	bool toReturn = true;
	InterfacePtr<IZMAdFileList> adFileAdList( mEdition, UseDefaultIID() );

	this->RemoveListeningAdFiles();	//Before clearing the list, remove listening to them.
	mAdFileAds.clear();

	InterfacePtr<const IZMApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<IZMAdFileMgr> adFileMgr( zpApp->GetAdFileMgr(), UseDefaultIID() );
	ASSERT(adFileMgr);

	if ( adFileAdList != nil )
	{
		ZMAdFileIDsListCIter currAdFile = mAdFileIDList->begin();
		ZMAdFileIDsListCIter endAdFile = mAdFileIDList->end();

		while( currAdFile != endAdFile )
		{
			const IStringData * adFileID = adFileMgr->GetAdFileByID( *currAdFile );
			ASSERT( adFileID );
			if( adFileID )
			{
				InterfacePtr<IZMAdList> adList( adFileID, UseDefaultIID() );
				ASSERT(adList);
				this->SetAdIDList( *currAdFile, adList->GetStringList() );
			}
			++currAdFile;
		}
		
		if( adFileAdList->HasData() == false )
		{
			

			//assetMgr->FetchAssetsForEdition( mEditionID );	//Already made by GetStringList call
		}
		//If asset id list comes empty, then it's observer will be notified when the list is available.
	}
	if( toReturn )
		this->BroadcastMessage( kZMUIAdsTV_ModelChangedMsg );
}

//----------------------------------------------------------------------------------------
// MarkAllAdFileDirty
//----------------------------------------------------------------------------------------
void
CZMAdsTVDataModel::MarkAllAdFileDirty()
{
	LogFunctionEnterExit;

	InterfacePtr<IZMAdFileList> adFileAdList( mEdition, UseDefaultIID() );

	InterfacePtr<const IZMApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<IZMAdFileMgr> adFileMgr( zpApp->GetAdFileMgr(), UseDefaultIID() );
	ASSERT(adFileMgr);

	if ( adFileAdList != nil )
	{
		ZMAdFileIDsListCIter currAdFile = mAdFileIDList->begin();
		ZMAdFileIDsListCIter endAdFile = mAdFileIDList->end();

		while( currAdFile != endAdFile )
		{
			const IStringData * adFileID = adFileMgr->GetAdFileByID( *currAdFile );
			ASSERT( adFileID );
			if( adFileID )
			{
				InterfacePtr<IZMAdList> adList( adFileID, UseDefaultIID() );
				ASSERT(adList);
				adList->SetHasData(false);	//This will result in parsing the xml file again.
			}
			++currAdFile;
		}
	}
}

//----------------------------------------------------------------------------------------
// HandleAdInfoChange
//----------------------------------------------------------------------------------------
void
CZMAdsTVDataModel::HandleAdInfoChange(
	ISubject*					inAdSubject)
{
	if( mSkipAdFileListUpdates )
		return;

	LogFunctionEnterExit;
	do
	{
		//Find the current status of this asset. If change then update the assets list.
///		InterfacePtr<const IStringData> dbAdID( inAdSubject, IID_IZMDATAID );
///		if( !dbAdID )
///			break;
///
///		const PMString * assetAdFileID = this->GetAdFileIDOfAd( dbAdID );
///		if( !assetAdFileID )
///			break;
///		int assetIndexInList = this->GetAdIDIndex( *assetAdFileID, dbAdID->Get() );
///		if( assetIndexInList != -1 )
///			break;
///
///		//AdFile of the asset has changed, so change the list.
///		this->UpdateAdList();
	}while(kFalse);
}

//----------------------------------------------------------------------------------------
// GetAssetFilteringFunction
//----------------------------------------------------------------------------------------
///const FilterAssetFnPtr
///CZMAdsTVDataModel::GetAssetFilteringFunction()const
///{
///	return mFilterAsset;
///}

//----------------------------------------------------------------------------------------
// SetAssetFilteringFunction
//----------------------------------------------------------------------------------------
///void
///CZMAdsTVDataModel::SetAssetFilteringFunction(
///	FilterAssetFnPtr		inFunc)
///{
///	mFilterAsset = inFunc;
///}

#pragma mark -
//----------------------------------------------------------------------------------------
// RemoveListeningTitle
//----------------------------------------------------------------------------------------
//void
//CZMAdsTVDataModel::RemoveListeningTitle()
//{
//	if( !mTitle )
//		return;
//
//	//In izine publish expandable nodes were on title with status list.
//	//Now these are on edition with ad file list
//	this->RemoveListeningObject( mTitle.get(), IZMAdFileList::kDefaultIID );
//}

//----------------------------------------------------------------------------------------
// AddListeningToTitle
//----------------------------------------------------------------------------------------
//void
//CZMAdsTVDataModel::AddListeningToTitle()
//{
//	if( !mTitle )
//		return;
//
//	this->AddListeningToObject( mTitle, IZMAdFileList::kDefaultIID );
//}

//----------------------------------------------------------------------------------------
// RemoveListeningEdition
//----------------------------------------------------------------------------------------
void
CZMAdsTVDataModel::RemoveListeningEdition()
{
	if( !mEdition )
		return;

	this->RemoveListeningObject( mEdition.get(), IZMAdFileList::kDefaultIID );
}


//----------------------------------------------------------------------------------------
// AddListeningToEdition
//----------------------------------------------------------------------------------------
void
CZMAdsTVDataModel::AddListeningToEdition()
{
	if( !mEdition )
		return;

	this->AddListeningToObject( mEdition.get(), IZMAdFileList::kDefaultIID );
}

//----------------------------------------------------------------------------------------
// RemoveListeningAssets
//----------------------------------------------------------------------------------------
void
CZMAdsTVDataModel::RemoveListeningAdFiles()
{
	InterfacePtr<const IZMApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<const IZMAdFileMgr> assetMgr( zpApp->GetAdFileMgr(), UseDefaultIID() );
	ASSERT(assetMgr);

	ZPAdFileAdsIDMap::iterator currAdFileIter = mAdFileAds.begin();
	ZPAdFileAdsIDMap::iterator endAdFileIter = mAdFileAds.end();
	while( currAdFileIter != endAdFileIter )
	{
		const ZMAdIDsList & currAssetsList = currAdFileIter->second;
		ZMAdIDsList::const_iterator currAssetIter = currAssetsList.begin();
		ZMAdIDsList::const_iterator endAssetIter = currAssetsList.end();
		while( currAssetIter != endAssetIter )
		{
			const PMString & currAssetIDStr = *currAssetIter;
			const IStringData * dbAssetID = assetMgr->GetAdFileByID( currAssetIDStr );

			this->RemoveListeningAdFile( dbAssetID );
			++currAssetIter;
		}
		
		++currAdFileIter;
	}
}

//----------------------------------------------------------------------------------------
// RemoveListeningAd
//----------------------------------------------------------------------------------------
void
CZMAdsTVDataModel::RemoveListeningAdFile(
	const IStringData *			inAdID )
{
	///this->RemoveListeningObject( inAdID, IZMAdInfo::kDefaultIID );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// RemoveListeningObject
//----------------------------------------------------------------------------------------
void
CZMAdsTVDataModel::RemoveListeningObject(
	const IPMUnknown *			inObjectToListen,
	const PMIID &				inProtocol)
{
	InterfacePtr<ISubject> inSubject( inObjectToListen, UseDefaultIID() );
	ASSERT(inSubject);

	InterfacePtr<IObserver> selfObserver( this, UseDefaultIID() );
	ASSERT(selfObserver);

	if( inSubject->IsAttached( selfObserver, inProtocol ) )
		inSubject->DetachObserver( selfObserver, inProtocol );
}

//----------------------------------------------------------------------------------------
// AddListeningToObject
//----------------------------------------------------------------------------------------
void
CZMAdsTVDataModel::AddListeningToObject(
	const IPMUnknown *			inObjectToListen,
	const PMIID &				inProtocol)
{
	InterfacePtr<ISubject> inSubject( inObjectToListen, UseDefaultIID() );
	ASSERT(inSubject);

	InterfacePtr<IObserver> selfObserver( this, UseDefaultIID() );
	ASSERT(selfObserver);

	if( false == inSubject->IsAttached( selfObserver, inProtocol ) )
		inSubject->AttachObserver( selfObserver, inProtocol );
}

//----------------------------------------------------------------------------------------
// BroadcastMessage
//----------------------------------------------------------------------------------------
void
CZMAdsTVDataModel::BroadcastMessage(
	const ClassID&						inTheChange,
	void *								inChangedBy) const
{
	InterfacePtr<ISubject> selfSubject(this, UseDefaultIID());
	ASSERT(selfSubject);
	
	selfSubject->Change( inTheChange, IZMAdsTVDataModel::kDefaultIID );
}
