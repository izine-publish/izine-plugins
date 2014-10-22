//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/EditionAssetsPopup/CZPEditionAssetsPopupController.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 30-9-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IObserver.h"
#include "IStringData.h"
#include "IStringListControlData.h"
#include "ISubject.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes

//OS / ThirdParty includes

#include "IZPUIID.h"
#include "IZPID.h"

//IZP Interfaces
#include "IZPApp.h"
#include "IZPAssetInfo.h"
#include "IZPAssetList.h"
#include "IZPAssetMgr.h"

//IZP General includes
#include "AZPDBObjPopupWithFixedValController.h"
#include "ZPDataHelper.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_UI
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

struct CompareAssetByTypeAndName	//Someday: move to another palce.
{
	bool operator()( const IStringData * inFirstVal, const IStringData * inSecondVal );
};

#define inheritClass AZPDBObjPopupWithFixedValController
class CZPEditionAssetsPopupController : public inheritClass
{
	typedef K2Vector<const IStringData *>				DBAssetsList;
public:
						CZPEditionAssetsPopupController(IPMUnknown * boss);
	virtual				~CZPEditionAssetsPopupController();

	void				AppendDBObjectValues();

	PMString			GetNthDBObjID(
							int							inIndex) const;
	int32				GetIndexOfDBObjID(
							const PMString &			inObjID ) const;
protected:
	void				RemoveListeningEdition();
	void				AddListeningToEdition();

	void				RemoveListeningObject(
							const IPMUnknown *			inObjectToListen,
							const PMIID &				inProtocol);
	void				AddListeningToObject(
							const IPMUnknown *			inObjectToListen,
							const PMIID &				inProtocol);

	void				FilterAndAppendAssets();
	
	DBAssetsList		FilterAssets( 
							const ZPAssetIDsList &		inListToFilter );
	void				SortAssets( 
							DBAssetsList &				inDBAssets );
	void				AppendAssets(
							const DBAssetsList &		inDBAssets );				//Also updates mAssets;

	int					GetAssetTypeMask();
private:
	InterfacePtr<const IStringData> mEdition;		//Edition for which assets are displayed by this popup.

	ZPAssetIDsList		mAssets;					//Asset IDs listed in popup
};


CREATE_PMINTERFACE(CZPEditionAssetsPopupController, kZPEditionAssetsPopupControllerImpl)

CZPEditionAssetsPopupController::CZPEditionAssetsPopupController(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPEditionAssetsPopupController::~CZPEditionAssetsPopupController()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// AppendDBObjectValues
//----------------------------------------------------------------------------------------
void
CZPEditionAssetsPopupController::AppendDBObjectValues()
{
	InterfacePtr<IStringListControlData> iAssetStringListControlData( this, UseDefaultIID());
	ASSERT(iAssetStringListControlData);
	iAssetStringListControlData->Clear();	//Sends change message to self observer
	//TODO:

	InterfacePtr<IStringData> selfEditionID( this, IID_IZPEDITIONIDSTRINGDATA );
	ASSERT( selfEditionID );
	const PMString & theEditionID = selfEditionID->Get();

	this->RemoveListeningEdition();
	mAssets.clear();

	const IStringData * dbEditionID = ZPDataHelper::GetEditionByID( theEditionID );

	if( dbEditionID == nil )
	{
		mEdition.reset(nil);
	}
	else
	{
		dbEditionID->AddRef();
		mEdition.reset(dbEditionID);
		this->AddListeningToEdition();

		this->FilterAndAppendAssets();
	}
}

//----------------------------------------------------------------------------------------
// GetNthDBObjID
//----------------------------------------------------------------------------------------
PMString
CZPEditionAssetsPopupController::GetNthDBObjID(
	int							inIndex) const
{
	PMString toReturn;
	ASSERT( inIndex >= 0 );

	if( inIndex < 0 )
		return toReturn;

	ASSERT( mAssets.size() > inIndex );

	toReturn = mAssets[inIndex];

	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetIndexOfDBObjID
//----------------------------------------------------------------------------------------
int32
CZPEditionAssetsPopupController::GetIndexOfDBObjID(
	const PMString &			inObjID) const
{
	int32 toReturn = -1;

	if( mEdition )
	{
		if( mAssets.size() )
			toReturn = mAssets.Location( inObjID );
	}

	return toReturn;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// FilterAndAppendAssets
//----------------------------------------------------------------------------------------
void
CZPEditionAssetsPopupController::FilterAndAppendAssets()
{
	InterfacePtr<IZPAssetList> editionAssetList( mEdition, UseDefaultIID() );
	
	mAssets.clear();
	if ( editionAssetList != nil )
	{
		const ZPAssetIDsList & assetsToFilter = editionAssetList->GetStringList();
		DBAssetsList theFilteredAssetsList = this->FilterAssets( assetsToFilter );
		this->SortAssets( theFilteredAssetsList );
		this->AppendAssets( theFilteredAssetsList );
		//If asset id list comes empty, then it's observer will be notified when the list is available.
	}
}

//----------------------------------------------------------------------------------------
// FilterAssets
//----------------------------------------------------------------------------------------
CZPEditionAssetsPopupController::DBAssetsList
CZPEditionAssetsPopupController::FilterAssets(
	const ZPAssetIDsList &		inListToFilter)
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<const IZPAssetMgr> assetMgr( zpApp->GetAssetMgr(), UseDefaultIID() );
	ASSERT(assetMgr);

	InterfacePtr<IStringListControlData> selfStringListControlData( this, UseDefaultIID());
	ASSERT(selfStringListControlData);

	ZPAssetIDsList::const_iterator endIter = inListToFilter.end();
	int newListLen = inListToFilter.size();

	int assetFlags[eAssetTypeCount] = { eAssetTypeFlag_IDDocument,
										eAssetTypeFlag_IDTemplate,
										eAssetTypeFlag_ICTemplate,
										eAssetTypeFlag_ICDocument,
										eAssetTypeFlag_ICAssignment,
										eAssetTypeFlag_IDStory };

	int allowedAssetsFlag = this->GetAssetTypeMask();
	int assetTypeMask = 0;
	DBAssetsList toReturn;
	for( ZPAssetIDsList::const_iterator currIter = inListToFilter.begin(); currIter != endIter; ++currIter)
	{
		const PMString & assetID = *currIter;
		
		const IStringData * dbAssetID = assetMgr->GetAssetByID(assetID);
		if( !dbAssetID )
			continue;
		
		InterfacePtr<const IZPAssetInfo> assetInfo( dbAssetID, UseDefaultIID() );
		ASSERT( assetInfo );

		ASSERT( assetInfo->GetType() >= 0 && assetInfo->GetType() < eAssetTypeCount );
		assetTypeMask = assetFlags[assetInfo->GetType()];
		
		if( assetTypeMask & allowedAssetsFlag )
		{
			toReturn.push_back( dbAssetID );
		}
	}
	return toReturn;
}

//----------------------------------------------------------------------------------------
// SortAssets
//----------------------------------------------------------------------------------------
void
CZPEditionAssetsPopupController::SortAssets(
	DBAssetsList &				inDBAssets )
{
	//Sort by Type and Name.
	std::sort( inDBAssets.begin(), inDBAssets.end(), CompareAssetByTypeAndName() );
}

//----------------------------------------------------------------------------------------
// AppendAssets
//----------------------------------------------------------------------------------------
void
CZPEditionAssetsPopupController::AppendAssets(
	const DBAssetsList &		inDBAssets)
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<const IZPAssetMgr> assetMgr( zpApp->GetAssetMgr(), UseDefaultIID() );
	ASSERT(assetMgr);

	InterfacePtr<IStringListControlData> selfStringListControlData( this, UseDefaultIID());
	ASSERT(selfStringListControlData);

	DBAssetsList::const_iterator endIter = inDBAssets.end();
	int newListLen = inDBAssets.size();
	mAssets.reserve( newListLen );

	int prevAssetType = eAssetTypeNone;
	int i=1;
	for( DBAssetsList::const_iterator currIter = inDBAssets.begin(); currIter != endIter; ++currIter,++i)
	{
		const IStringData * dbAssetID = *currIter;
		
		InterfacePtr<const IZPAssetInfo> assetInfo( dbAssetID, UseDefaultIID() );
		ASSERT( assetInfo );

		if( prevAssetType == eAssetTypeNone )
			prevAssetType = assetInfo->GetType();
		else if ( prevAssetType != assetInfo->GetType() )
		{
			prevAssetType = assetInfo->GetType();
			mAssets.push_back("-");
			selfStringListControlData->AddString("-", IStringListControlData::kEnd, kFalse, kFalse );
		}

		InterfacePtr<const IStringData> assetName( dbAssetID, IID_IZPDATANAME );
		ASSERT( assetName );
		const PMString & theAssetName = assetName->Get();

		mAssets.push_back( dbAssetID->Get() );

		if ( i == newListLen )
			selfStringListControlData->AddString(theAssetName);
		else
			selfStringListControlData->AddString(theAssetName, IStringListControlData::kEnd, kFalse, kFalse );
	}
}

#if 0
//----------------------------------------------------------------------------------------
// AppendAssets
//----------------------------------------------------------------------------------------
void
CZPEditionAssetsPopupController::AppendAssets()
{
	InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<const IZPAssetMgr> assetMgr( zpApp->GetAssetMgr(), UseDefaultIID() );
	ASSERT(assetMgr);

	InterfacePtr<IStringListControlData> selfStringListControlData( this, UseDefaultIID());
	ASSERT(selfStringListControlData);

	ZPAssetIDsList::const_iterator endIter = mAssets.end();
	int newListLen = mAssets.size();

	int i=1;
	for( ZPAssetIDsList::const_iterator currIter = mAssets.begin(); currIter != endIter; ++currIter,++i)
	{
		const PMString & assetID = *currIter;
		
		InterfacePtr<const IStringData> assetName( assetMgr->GetAssetByID(assetID), IID_IZPDATANAME );
		ASSERT( assetName );
		const PMString & theAssetName = assetName->Get();

		if ( i == newListLen )
			selfStringListControlData->AddString(theAssetName);
		else
			selfStringListControlData->AddString(theAssetName, IStringListControlData::kEnd, kFalse, kFalse );
	}
}
#endif

#pragma mark -
//----------------------------------------------------------------------------------------
// GetAssetTypeMask
//----------------------------------------------------------------------------------------
int
CZPEditionAssetsPopupController::GetAssetTypeMask()
{
	int allowedAssetsFlag = eAssetTypeFlag_IDDocument |		//Someday: read this value from an int interface on control itself.
							eAssetTypeFlag_IDTemplate |		//This way each control can have different type of assets.
							eAssetTypeFlag_ICTemplate |
							eAssetTypeFlag_ICDocument |
							eAssetTypeFlag_ICAssignment ;

	return allowedAssetsFlag;
}

//----------------------------------------------------------------------------------------
// RemoveListeningEdition
//----------------------------------------------------------------------------------------
void
CZPEditionAssetsPopupController::RemoveListeningEdition()
{
	if( !mEdition )
		return;

	this->RemoveListeningObject( mEdition.get(), IZPAssetList::kDefaultIID );
}

//----------------------------------------------------------------------------------------
// AddListeningToEdition
//----------------------------------------------------------------------------------------
void
CZPEditionAssetsPopupController::AddListeningToEdition()
{
	if( !mEdition )
		return;

	this->AddListeningToObject( mEdition.get(), IZPAssetList::kDefaultIID );
}

//----------------------------------------------------------------------------------------
// RemoveListeningObject
//----------------------------------------------------------------------------------------
void
CZPEditionAssetsPopupController::RemoveListeningObject(
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
CZPEditionAssetsPopupController::AddListeningToObject(
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
#pragma mark -
//----------------------------------------------------------------------------------------
// Compare by Type and Name
//----------------------------------------------------------------------------------------
bool
CompareAssetByTypeAndName::operator ()(const IStringData *inFirstVal, const IStringData *inSecondVal)
{
	ASSERT( inFirstVal != nil && inSecondVal != nil );

	InterfacePtr<const IZPAssetInfo> assetInfo1( inFirstVal, UseDefaultIID() );
	ASSERT( assetInfo1 );

	InterfacePtr<const IZPAssetInfo> assetInfo2( inSecondVal, UseDefaultIID() );
	ASSERT( assetInfo2 );

	if( assetInfo1->GetType() != assetInfo2->GetType() )
	{
		return assetInfo1->GetType() < assetInfo2->GetType();
	}
	else
	{
		InterfacePtr<const IStringData> assetName1( inFirstVal, IID_IZPDATANAME );
		ASSERT( assetName1 );

		InterfacePtr<const IStringData> assetName2( inSecondVal, IID_IZPDATANAME );
		ASSERT( assetName2 );

		return assetName1->Get() < assetName2->Get();
	}
}
