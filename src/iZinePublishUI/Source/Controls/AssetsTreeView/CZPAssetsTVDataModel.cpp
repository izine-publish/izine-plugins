//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/AssetsTreeView/CZPAssetsTVDataModel.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 29-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IObserver.h"
#include "IStringData.h"
#include "ISubject.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPUIID.h"
#include "IZPID.h"

//IZP Interfaces
#include "IZPApp.h"
#include "IZPAssetInfo.h"
#include "IZPAssetList.h"
#include "IZPAssetMgr.h"
#include "IZPAssetsTVDataModel.h"
#include "IZPInDnUtils.h"
#include "IZPStatusList.h"
#include "IZPTitleMgr.h"
#include "IZPTitleStatusMgr.h"

//IZP General includes
#include "ZPConstants.h"
#include "ZPDataHelper.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AssetsTV
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

//TODO: should this model be kept with TreeView or Assets Panel?
// It would be better to have model separate from TreeView.

#define inheritClass CPMUnknown<IZPAssetsTVDataModel>
class CZPAssetsTVDataModel : public inheritClass
{
	//Map of TitleStatusID and AssetIDs 
	typedef std::map<PMString, ZPAssetIDsList >	ZPStatusAssetsIDMap;

public:
						CZPAssetsTVDataModel(IPMUnknown * boss);
	virtual				~CZPAssetsTVDataModel();

	const PMString &		GetTitleID() const;
	const PMString &		GetGetEditionID() const;

	const ZPStatusIDsList *	GetStatusIDList() const;
	void					SetStatusIDList(
								const ZPStatusIDsList *		inStatusIDList );
	const PMString *		GetNthStatusID(
								int							inIndex ) const;
	int32					GetStatusIDIndex(
								const PMString &			inStatusID ) const;

	const ZPAssetIDsList *	GetAssetIDList(
								const PMString &			inStatusID ) const;
	const PMString *		GetNthAssetID(
								const PMString &			inStatusID,
								int							inIndex ) const;
	int32					GetAssetIDIndex(
								const PMString &			inStatusID,
								const PMString &			inAssetID ) const;
	const PMString *		GetStatusIDOfAsset(
								const PMString &			inAssetID ) const;
	const PMString *		GetStatusIDOfAsset(
								const IStringData *			inAssetID ) const;
	void					SetAssetsIDList(				//This will filter by status id
								const ZPAssetIDsList &		inAssetIDList );
	void					SetAssetsIDList(				//Not being used.
								const PMString &			inStatusID,
								const ZPAssetIDsList &		inAssetIDList );
	
	bool					GetSkipAssetListUpdates() const; //AssetList will not change if true.
	void					SetSkipAssetListUpdates(
								bool						inSkipAssetListChanges );

	void					UpdateData(
								const PMString &			inTitleID,
								const PMString &			inEditionID );

	bool					UpdateStatusList();
	void					UpdateAssetList();

	void					HandleAssetInfoChange(
								ISubject*					inAssetSubject );

	const FilterAssetFnPtr	GetAssetFilteringFunction() const;
	void					SetAssetFilteringFunction(
								FilterAssetFnPtr			inFunc );
protected:
	void				RemoveListeningTitle();
	void				AddListeningToTitle();

	void				RemoveListeningEdition();
	void				AddListeningToEdition();

	void				RemoveListeningAssets();
	void				RemoveListeningAsset(
							const IStringData *			inDBAssetID );

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

	bool				mSkipAssetListUpdates;

	//retained here so that it does not get destroyed before its references are removed.
	InterfacePtr<const IStringData> mTitle;	//Title for which editions are displayed by this treeview.
	InterfacePtr<const IStringData> mEdition;	//Edition for which assets are displayed by this treeview.
	
	ZPStatusIDsList 				mStatusIDList;
	ZPStatusAssetsIDMap				mStatusAssets;

	FilterAssetFnPtr				mFilterAsset;
};


CREATE_PMINTERFACE(CZPAssetsTVDataModel, kZPAssetsTVDataModelImpl)

CZPAssetsTVDataModel::CZPAssetsTVDataModel(IPMUnknown * boss)
: inheritClass(boss)
//, mStatusIDList( nil )
, mFilterAsset ( nil )
, mSkipAssetListUpdates( false )
{
	if( Utils<IZPInDnUtils>()->IsHostAppInCopy() == kTrue )
		this->SetAssetFilteringFunction( IZPAssetsTVDataModel::CanDisplayAssetInInCopy );
	else
		this->SetAssetFilteringFunction( IZPAssetsTVDataModel::CanDisplayAssetInInDesign );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAssetsTVDataModel::~CZPAssetsTVDataModel()
{
}

#pragma mark -

//----------------------------------------------------------------------------------------
// GetTitleID
//----------------------------------------------------------------------------------------
const PMString &
CZPAssetsTVDataModel::GetTitleID()const
{
	return mTitleID;
}

//----------------------------------------------------------------------------------------
// GetGetEditionID
//----------------------------------------------------------------------------------------
const PMString &
CZPAssetsTVDataModel::GetGetEditionID()const
{
	return mEditionID;
}

//----------------------------------------------------------------------------------------
// GetStatusIDList
//----------------------------------------------------------------------------------------
const ZPStatusIDsList *
CZPAssetsTVDataModel::GetStatusIDList()const
{
	return &mStatusIDList;
}

//----------------------------------------------------------------------------------------
// SetStatusIDList
//----------------------------------------------------------------------------------------
void
CZPAssetsTVDataModel::SetStatusIDList(
	const ZPStatusIDsList *		inStatusIDList)
{
	mStatusIDList.clear();

	if( !inStatusIDList )
		return;

	bool isInCopy = (Utils<IZPInDnUtils>()->IsHostAppInCopy() == kTrue);
	ZPStatusIDsList::const_iterator iter = inStatusIDList->begin();
	ZPStatusIDsList::const_iterator endIter = inStatusIDList->end();
	for (; iter != endIter; ++iter)
	{
		const PMString &statusId = *iter;
		if ((statusId.Compare(false, kTitleStatus_Assigned) == 0))		//Not to display in indesign and incopy
		//|| (!isInCopy && statusId.Compare(false, kTitleStatus_Template) == 0))	//Template status only for incopy
		continue;

		mStatusIDList.push_back(statusId);
	}

	//Sort status id on sort index
	std::sort( mStatusIDList.begin(), mStatusIDList.end(), CompareStatusIDBySortIndex() );
}

//----------------------------------------------------------------------------------------
// GetNthStatusID
//----------------------------------------------------------------------------------------
const PMString *
CZPAssetsTVDataModel::GetNthStatusID(
	int							inIndex) const
{
	if( mStatusIDList.size() )
	{
		ASSERT( inIndex >= 0 && inIndex < mStatusIDList.size() );
		return & mStatusIDList.at(inIndex);
	}
	return nil;
}

//----------------------------------------------------------------------------------------
// GetStatusIDIndex
//----------------------------------------------------------------------------------------
int32
CZPAssetsTVDataModel::GetStatusIDIndex(
	const PMString &			inStatusID) const
{
	int32 toReturn = -1;

	if( mStatusIDList.size() )
	{
		ZPStatusIDsList::const_iterator beginIter = mStatusIDList.begin();
		ZPStatusIDsList::const_iterator endIter = mStatusIDList.end();
		ZPStatusIDsList::const_iterator foundIter = std::find( beginIter, endIter, inStatusID );
		if( foundIter != endIter )
		{
			toReturn = foundIter - beginIter;
		}
	}
	return toReturn;
}


//----------------------------------------------------------------------------------------
// GetAssetIDList
//----------------------------------------------------------------------------------------
const ZPAssetIDsList *
CZPAssetsTVDataModel::GetAssetIDList(
	const PMString &			inStatusID) const
{
	ZPStatusAssetsIDMap::const_iterator foundIter = mStatusAssets.find( inStatusID );
	ZPStatusAssetsIDMap::const_iterator endIter = mStatusAssets.end();
	if( foundIter != endIter )
		return & foundIter->second;
	else
		return nil;
	return nil;
}

//----------------------------------------------------------------------------------------
// GetNthAssetID
//----------------------------------------------------------------------------------------
const PMString *
CZPAssetsTVDataModel::GetNthAssetID(
	const PMString &			inStatusID,
	int							inIndex) const
{
	const ZPAssetIDsList * assetList = this->GetAssetIDList( inStatusID );
	if( assetList && assetList->size() > inIndex && inIndex >= 0 )
		return & assetList->at(inIndex);
	return nil;
}

//----------------------------------------------------------------------------------------
// GetAssetIDIndex
//----------------------------------------------------------------------------------------
int32
CZPAssetsTVDataModel::GetAssetIDIndex(
	const PMString &			inStatusID,
	const PMString &			inAssetID) const
{
	const ZPAssetIDsList * assetList = this->GetAssetIDList( inStatusID );
	int32 toReturn = -1;
	if( assetList && assetList->size() > 0 )
	{
		ZPAssetIDsList::const_iterator beginIter = assetList->begin();
		ZPAssetIDsList::const_iterator endIter = assetList->end();
		ZPAssetIDsList::const_iterator foundIter = std::find( beginIter, endIter, inAssetID );
		if( foundIter != endIter )
		{
			toReturn = foundIter - beginIter;
		}
	}
	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetStatusIDOfAsset
//----------------------------------------------------------------------------------------
const PMString *
CZPAssetsTVDataModel::GetStatusIDOfAsset(
	const PMString &			inAssetID) const
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<const IZPAssetMgr> assetMgr( zpApp->GetAssetMgr(), UseDefaultIID() );
	ASSERT(assetMgr);

	const IStringData * assetID = assetMgr->GetAssetByID( inAssetID );
	return this->GetStatusIDOfAsset( assetID );
}

//----------------------------------------------------------------------------------------
// GetStatusIDOfAsset
//----------------------------------------------------------------------------------------
const PMString *
CZPAssetsTVDataModel::GetStatusIDOfAsset(
	const IStringData *			inAssetID) const
{
	
	InterfacePtr<const IZPAssetInfo> assetInfo( inAssetID, UseDefaultIID() );
	ASSERT(assetInfo);
	
	if( assetInfo )
		return & assetInfo->GetStatusID();

	return nil;
}

//----------------------------------------------------------------------------------------
// SetAssetsIDList
//----------------------------------------------------------------------------------------
void
CZPAssetsTVDataModel::SetAssetsIDList(
	const ZPAssetIDsList &		inAssetIDList)
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	ZPAssetIDsList::const_iterator assetIDIter = inAssetIDList.begin();
	ZPAssetIDsList::const_iterator assetIDEndIter = inAssetIDList.end();

	InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<const IZPAssetMgr> assetMgr( zpApp->GetAssetMgr(), UseDefaultIID() );
	ASSERT(assetMgr);

	while( assetIDIter != assetIDEndIter )
	{
		const PMString & currAssetID = *assetIDIter;
		
		const IStringData * assetID = assetMgr->GetAssetByID( currAssetID );

		const PMString * statusID = this->GetStatusIDOfAsset( assetID );
		ASSERT( statusID );

		if(statusID && ( mFilterAsset == nil || mFilterAsset(assetID) ) )
		{
			mStatusAssets[*statusID].push_back( currAssetID );
			//Listen to this asset for changes. We need to change model when status of asset changes.
			this->AddListeningToObject( assetID, IZPAssetInfo::kDefaultIID );
		}

		++assetIDIter;
	}
}

//----------------------------------------------------------------------------------------
// SetAssetsIDList
//----------------------------------------------------------------------------------------
void
CZPAssetsTVDataModel::SetAssetsIDList(
	const PMString &			inStatusID,
	const ZPAssetIDsList &		inAssetIDList)
{
	mStatusAssets[ inStatusID ] = inAssetIDList;
}

//----------------------------------------------------------------------------------------
// GetSkipAssetListUpdates
//----------------------------------------------------------------------------------------
bool
CZPAssetsTVDataModel::GetSkipAssetListUpdates()const
{
	return mSkipAssetListUpdates;
}

//----------------------------------------------------------------------------------------
// SetSkipAssetListUpdates
// AssetList will not change if true.
//----------------------------------------------------------------------------------------
void
CZPAssetsTVDataModel::SetSkipAssetListUpdates(
	bool						inSkipAssetListChanges)
{
	mSkipAssetListUpdates = inSkipAssetListChanges;
}

//----------------------------------------------------------------------------------------
// UpdateData
//----------------------------------------------------------------------------------------
void
CZPAssetsTVDataModel::UpdateData(
	const PMString &			inTitleID,
	const PMString &			inEditionID)
{
	mTitleID = inTitleID;
	mEditionID = inEditionID;

	const IStringData * theTitle = ZPDataHelper::GetTitleByID( inTitleID );
	
	if( theTitle == nil )
	{
		this->RemoveListeningTitle();
		mTitle.reset(nil);
		this->SetStatusIDList( nil );
	}
	else
	{
		this->RemoveListeningTitle();
		theTitle->AddRef();
		mTitle.reset(theTitle);
		this->SetStatusIDList( nil );
		this->AddListeningToTitle();
	}

	const IStringData * theEdition = ZPDataHelper::GetEditionByID( inEditionID );
	
	ZPAssetIDsList emptyList;
	if( theEdition == nil )
	{
		this->RemoveListeningEdition();
		mEdition.reset(nil);
		this->SetAssetsIDList( emptyList );
	}
	else
	{
		this->RemoveListeningEdition();
		theEdition->AddRef();
		mEdition.reset(theEdition);
		this->SetAssetsIDList( emptyList );
		this->AddListeningToEdition();
	}

	//Update the status list
	if( false == this->UpdateStatusList() )
		return;	//TODO: should we clear the tree here.?
	
	//Update the Assets list.
	this->UpdateAssetList();

	//Change root.
	//Should we do it here. Model change message automatically does it.
}

//----------------------------------------------------------------------------------------
// UpdateStatusList
//----------------------------------------------------------------------------------------
bool
CZPAssetsTVDataModel::UpdateStatusList()
{
	bool toReturn = true;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IZPStatusList> titleStatusList( mTitle, UseDefaultIID() );
	
	mStatusIDList.clear();
	if ( titleStatusList != nil )
	{
		this->SetStatusIDList( & titleStatusList->GetStringList() );
		if( titleStatusList->HasData() == false )
		{
#pragma message("			Uncomment this when status per title is implemented")
			//toReturn = false;

			InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
			ASSERT( zpApp );

			InterfacePtr<IZPTitleStatusMgr> statusMgr( zpApp->GetTitleMgr(), UseDefaultIID() );
			ASSERT(statusMgr);

			this->SetStatusIDList( & statusMgr->GetDefaultStatusIDs() );
		}
		//If status id list comes empty, then it's observer will be notified when the list is available.
	}

	if( toReturn )
		this->BroadcastMessage( kZPUIAssetTV_ModelChangedMsg );
	return toReturn;
}

//----------------------------------------------------------------------------------------
// UpdateAssetList
//----------------------------------------------------------------------------------------
void
CZPAssetsTVDataModel::UpdateAssetList()
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif	
	if( mSkipAssetListUpdates )	//Asset List of this edition is being updated. Don't update now.
		return;
	bool toReturn = true;
	InterfacePtr<IZPAssetList> editionAssetList( mEdition, UseDefaultIID() );
	
	this->RemoveListeningAssets();	//Before clearing the list, remove listening to them.
	mStatusAssets.clear();

	if ( editionAssetList != nil )
	{
		this->SetAssetsIDList( editionAssetList->GetStringList() );
		if( editionAssetList->HasData() == false )
		{
			InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
			ASSERT( zpApp );

			InterfacePtr<IZPAssetMgr> assetMgr( zpApp->GetAssetMgr(), UseDefaultIID() );
			ASSERT(assetMgr);

			//assetMgr->FetchAssetsForEdition( mEditionID );	//Already made by GetStringList call
		}
		//If asset id list comes empty, then it's observer will be notified when the list is available.
	}

	if( toReturn )
		this->BroadcastMessage( kZPUIAssetTV_ModelChangedMsg );
}

//----------------------------------------------------------------------------------------
// HandleAssetInfoChange
//----------------------------------------------------------------------------------------
void
CZPAssetsTVDataModel::HandleAssetInfoChange(
	ISubject*					inAssetSubject)
{
	if( mSkipAssetListUpdates )
		return;

	do
	{
		//Find the current status of this asset. If change then update the assets list.
		InterfacePtr<const IStringData> dbAssetID( inAssetSubject, IID_IZPDATAID );
		if( !dbAssetID )
			break;

		const PMString * assetStatusID = this->GetStatusIDOfAsset( dbAssetID );
		if( !assetStatusID )
			break;
		int assetIndexInList = this->GetAssetIDIndex( *assetStatusID, dbAssetID->Get() );
		if( assetIndexInList != -1 )
			break;

		//Status of the asset has changed, so change the list.
		this->UpdateAssetList();
	}while(kFalse);
}

//----------------------------------------------------------------------------------------
// GetAssetFilteringFunction
//----------------------------------------------------------------------------------------
const FilterAssetFnPtr
CZPAssetsTVDataModel::GetAssetFilteringFunction()const
{
	return mFilterAsset;
}

//----------------------------------------------------------------------------------------
// SetAssetFilteringFunction
//----------------------------------------------------------------------------------------
void
CZPAssetsTVDataModel::SetAssetFilteringFunction(
	FilterAssetFnPtr		inFunc)
{
	mFilterAsset = inFunc;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// RemoveListeningTitle
//----------------------------------------------------------------------------------------
void
CZPAssetsTVDataModel::RemoveListeningTitle()
{
	if( !mTitle )
		return;

	this->RemoveListeningObject( mTitle.get(), IZPStatusList::kDefaultIID );
}

//----------------------------------------------------------------------------------------
// AddListeningToTitle
//----------------------------------------------------------------------------------------
void
CZPAssetsTVDataModel::AddListeningToTitle()
{
	if( !mTitle )
		return;

	this->AddListeningToObject( mTitle, IZPStatusList::kDefaultIID );
}

//----------------------------------------------------------------------------------------
// RemoveListeningEdition
//----------------------------------------------------------------------------------------
void
CZPAssetsTVDataModel::RemoveListeningEdition()
{
	if( !mEdition )
		return;

	this->RemoveListeningObject( mEdition.get(), IZPAssetList::kDefaultIID );
}


//----------------------------------------------------------------------------------------
// AddListeningToEdition
//----------------------------------------------------------------------------------------
void
CZPAssetsTVDataModel::AddListeningToEdition()
{
	if( !mEdition )
		return;

	this->AddListeningToObject( mEdition.get(), IZPAssetList::kDefaultIID );
}

//----------------------------------------------------------------------------------------
// RemoveListeningAssets
//----------------------------------------------------------------------------------------
void
CZPAssetsTVDataModel::RemoveListeningAssets()
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<const IZPAssetMgr> assetMgr( zpApp->GetAssetMgr(), UseDefaultIID() );
	ASSERT(assetMgr);

	ZPStatusAssetsIDMap::iterator currStatusIter = mStatusAssets.begin();
	ZPStatusAssetsIDMap::iterator endStatusIter = mStatusAssets.end();
	while( currStatusIter != endStatusIter )
	{
		const ZPAssetIDsList & currAssetsList = currStatusIter->second;
		ZPAssetIDsList::const_iterator currAssetIter = currAssetsList.begin();
		ZPAssetIDsList::const_iterator endAssetIter = currAssetsList.end();
		while( currAssetIter != endAssetIter )
		{
			const PMString & currAssetIDStr = *currAssetIter;
			const IStringData * dbAssetID = assetMgr->GetAssetByID( currAssetIDStr );

			this->RemoveListeningAsset( dbAssetID );
			++currAssetIter;
		}
		
		++currStatusIter;
	}
}

//----------------------------------------------------------------------------------------
// RemoveListeningAsset
//----------------------------------------------------------------------------------------
void
CZPAssetsTVDataModel::RemoveListeningAsset(
	const IStringData *			inDBAssetID )
{
	this->RemoveListeningObject( inDBAssetID, IZPAssetInfo::kDefaultIID );
}

//----------------------------------------------------------------------------------------
// RemoveListeningObject
//----------------------------------------------------------------------------------------
void
CZPAssetsTVDataModel::RemoveListeningObject(
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
CZPAssetsTVDataModel::AddListeningToObject(
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
CZPAssetsTVDataModel::BroadcastMessage(
	const ClassID&						inTheChange,
	void *								inChangedBy) const
{
	InterfacePtr<ISubject> selfSubject(this, UseDefaultIID());
	ASSERT(selfSubject);
	
	selfSubject->Change( inTheChange, IZPAssetsTVDataModel::kDefaultIID );
}


#pragma mark -
//----------------------------------------------------------------------------------------
// CanDisplayAssetType_InInDesign
//Check if we can display inAssetType asset type.
//----------------------------------------------------------------------------------------
bool
IZPAssetsTVDataModel::CanDisplayAssetInInDesign(
	const IStringData *			inAssetID )
{
	InterfacePtr<const IZPAssetInfo> assetInfo( inAssetID, UseDefaultIID() );
	ASSERT( assetInfo );

	const enAssetType assetType = assetInfo->GetType();
	bool toReturn = CanDisplayAssetTypeIn_InDesign( assetType );
	if( toReturn && assetType == eAssetType_IDStory )
		toReturn = assetInfo->GetStatusID().Compare( kFalse, kTitleStatus_Unassigned ) == 0;	//ID stories only in UnAssigned status.
	return toReturn;
}

//----------------------------------------------------------------------------------------
// CanDisplayAssetType_InInCopy
//Check if we can display inAssetType asset type.
//----------------------------------------------------------------------------------------
bool
IZPAssetsTVDataModel::CanDisplayAssetInInCopy(
	const IStringData *			inAssetID )
{
	InterfacePtr<const IZPAssetInfo> assetInfo( inAssetID, UseDefaultIID() );
	ASSERT( assetInfo );

	const enAssetType assetType = assetInfo->GetType();
	bool toReturn = CanDisplayAssetTypeIn_InCopy( assetType );
	if( toReturn && assetType == eAssetType_IDStory )
		toReturn = assetInfo->GetStatusID().Compare( kFalse, kTitleStatus_Unassigned ) == 0;	//ID stories only in UnAssigned status.
	return toReturn;
}

//----------------------------------------------------------------------------------------
// CanDisplayAssetTypeIn_InDesign
//Check if we can display inAssetType asset type.
//----------------------------------------------------------------------------------------
bool
IZPAssetsTVDataModel::CanDisplayAssetTypeIn_InDesign(
	enAssetType					inAssetType)
{
	bool toReturn = ( (inAssetType == eAssetType_IDDocument )		// InDesign Document
	||( inAssetType == eAssetType_IDTemplate )						// InDesign Template
	||( inAssetType == eAssetType_ICAssignment )					// Assignment
	||( inAssetType == eAssetType_IDStory )							// ID Story, added for Redmine#1838, where story has assigned/unassigned status only.
	||( inAssetType == eAssetType_ICDocument ) );					// InCopy Document

	return toReturn;
}

//----------------------------------------------------------------------------------------
// CanDisplayAssetTypeIn_InCopy
//Check if we can display inAssetType asset type.
//----------------------------------------------------------------------------------------
bool
IZPAssetsTVDataModel::CanDisplayAssetTypeIn_InCopy(
	enAssetType					inAssetType)
{
	bool toReturn = ( ( inAssetType == eAssetType_ICAssignment )		// Assignment
	||( inAssetType == eAssetType_ICDocument )							// InCopy Document
	||( inAssetType == eAssetType_IDStory )								// ID Story, added for Redmine#1838, where story has assigned/unassigned status only
	||( inAssetType == eAssetType_ICTemplate ) );						// InCopy Template

	return toReturn;
}

