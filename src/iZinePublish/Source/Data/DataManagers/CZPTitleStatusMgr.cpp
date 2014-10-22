//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Data/DataManagers/CZPTitleStatusMgr.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 5-5-2010
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
#include "ZPTypes.h"

//IZP Interfaces
#include "IZPApp.h"
#include "IZPStatusList.h"
#include "IZPTitleMgr.h"
#include "IZPTitleStatusInfo.h"
#include "IZPTitleStatusMgr.h"

//IZP General includes
#include "ZPConstants.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_DBObjects
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZPTitleStatusMgr>
class CZPTitleStatusMgr : public inheritClass
{
public:
						CZPTitleStatusMgr(IPMUnknown * boss);
	virtual				~CZPTitleStatusMgr();

	void				Shutdown();

	void				UpdateStatusList(
							const ZPTitleStatusInfoSList &	inStatusInfoList );

	const IStringData *	GetStatusForID(
							const PMString &			inStatusID ) const;
	
	const ZPStatusIDsList &
						GetDefaultStatusIDs() const;

	void				DecrementReferenceForTitleStatus (
							const ZPStatusIDsList &			inStatusIDs );
protected:
	static IStringData*	CreateStatus(					//Caller will be owner of obj.
							const PMString &			inStatusID );

	void				UpdateStatusList(
							const PMString &			inTitleID,
							const ZPIDPairSList &		inStatusList,
							const ZPTitleStatusDetailSList &	inStatusDetailList );

	bool				AddUpdateStatus(				//Return true if new object was created.
							const PMString &			inStatusID ,
							const PMString &			inStatusName,
							const ZPTitleStatusDetail &	inStatusDetail,
							const IStringData **		oStatusID );

	void				RemoveFromCache(
							const IStringData *			inStatusID );

	void				BroadcastMessage(
							const ClassID&				inTheChange,
							void *						inChangedBy = NULL );
private:
	ZPStatusIDMap		mStatusMap;
	ZPStatusIDsList		mDefaultStatusIDsList;
};


CREATE_PMINTERFACE(CZPTitleStatusMgr, kZPTitleStatusMgrImpl)

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPTitleStatusMgr::CZPTitleStatusMgr(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPTitleStatusMgr::~CZPTitleStatusMgr()
{
	this->Shutdown();
}

#pragma mark -
//----------------------------------------------------------------------------------------
// CreateStatus
//----------------------------------------------------------------------------------------
IStringData*
CZPTitleStatusMgr::CreateStatus(//Caller will be owner of obj.
	const PMString &			inStatusID)
{
	IStringData * theStatusID = ::CreateObject2<IStringData>( kZPDataTitleStatusBoss , IID_IZPDATAID );
	ASSERT(theStatusID);

#ifdef DEBUG
	InterfacePtr<const IControllingUnknown> controlling( theStatusID, UseDefaultIID() );
	IZPLog_Str( thisFileLA, enLT_ObjectCreations, "Title Status boss created = 0x%08X", controlling.get() );
	IZPLog_Str_( thisFileLA, enLT_ObjectCreations, "Object ID = %s", inStatusID.GrabCString() );
#endif
	theStatusID->Set( inStatusID );

	return theStatusID;
}
#pragma mark -
//----------------------------------------------------------------------------------------
// UpdateStatusList
//----------------------------------------------------------------------------------------
void
CZPTitleStatusMgr::UpdateStatusList(
	const ZPTitleStatusInfoSList &	inStatusInfoList)
{
	//send message title status list will change.
	this->BroadcastMessage( kZPTitleStatusListWillChange );
	
	ZPTitleStatusInfoSList::const_iterator statusInfoCurrIter = inStatusInfoList.begin();
	ZPTitleStatusInfoSList::const_iterator statusInfoEndIter = inStatusInfoList.end();

	while( statusInfoCurrIter != statusInfoEndIter )
	{
		const ZPTitleStatusInfo & currTitleStatusInfo = **statusInfoCurrIter;
		this->UpdateStatusList( currTitleStatusInfo.mTitleID, currTitleStatusInfo.mStatusList, currTitleStatusInfo.mStatusDetailList );

		++statusInfoCurrIter;
	}

	//send message title status list updated.
	this->BroadcastMessage( kZPTitleStatusListChanged );
}

//----------------------------------------------------------------------------------------
// UpdateStatusList
//----------------------------------------------------------------------------------------
void
CZPTitleStatusMgr::UpdateStatusList(
	const PMString &			inTitleID,
	const ZPIDPairSList &		inStatusList,
	const ZPTitleStatusDetailSList &	inStatusDetailList )
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<const IZPTitleMgr> titleMgr( zpApp->GetTitleMgr(), UseDefaultIID() );
	ASSERT(titleMgr);

	const IStringData * theTitle = titleMgr->GetTitleForID( inTitleID );

	ZPStatusIDsList statusIDsToRemove;
	ZPStatusIDsList statusIDsNewList;

	InterfacePtr<IZPStatusList> titleStatusList( theTitle, UseDefaultIID() );
	
	if( theTitle == nil && inTitleID.Compare( kFalse, kEmptyGUID ) == 0 )
	{	//No title associated with these status ids, so these will be common for those that don't have specific status IDs.
		statusIDsToRemove = mDefaultStatusIDsList;
	} else if ( theTitle != nil )
	{
		statusIDsToRemove = titleStatusList->GetStringList();
	}
	else
	{
		ASSERT( theTitle != nil );	//Updating a title that is not cached.
		return;
	}

	//Add/Update the status for title
	ZPIDPairSList::const_iterator	currIter = inStatusList.begin();
	ZPIDPairSList::const_iterator	endIter = inStatusList.end();
	ZPTitleStatusDetailSList::const_iterator currDetailIter = inStatusDetailList.begin();


	while( currIter != endIter )
	{
		const IStringData * theStatus = NULL;
		const PMString & statusID = (*currIter)->mID;
		const ZPTitleStatusDetail *	currStatusDetail = (*currDetailIter);
		//Update the status
		bool newStatusCreated = this->AddUpdateStatus( statusID, (*currIter)->mName, *currStatusDetail, &theStatus );
		
		//Add to new list.
		statusIDsNewList.push_back(statusID);
		
		++currIter;
		++currDetailIter;

		if( newStatusCreated == false )
			theStatus->AddRef();	//Retain till this ID is referenced.

		//Increament ref count maintained by this manager.
		InterfacePtr<IIntData> cachedCount( theStatus, IID_IZPCACHEMGRCOUNT );
		ASSERT( cachedCount );
		cachedCount->Set( cachedCount->Get() + 1 );
	}

	//Update the status id list
	if( theTitle == nil )
		mDefaultStatusIDsList = statusIDsNewList;
	else
	{
		titleStatusList->SetStringList( statusIDsNewList );
	}

	//Remove from cached list.
	//shared object has made AddRef above, so releasing will not have any impact.
	this->DecrementReferenceForTitleStatus( statusIDsToRemove );
}

//----------------------------------------------------------------------------------------
// AddUpdateStatus
//----------------------------------------------------------------------------------------
bool
CZPTitleStatusMgr::AddUpdateStatus(
	const PMString &			inStatusID,
	const PMString &			inStatusName,
	const ZPTitleStatusDetail &	inStatusDetail,
	const IStringData **		oStatusID)
{
	const IStringData * statusID = this->GetStatusForID( inStatusID );
	bool toReturn = statusID == nil;
	if( toReturn )
	{
		statusID = mStatusMap[inStatusID] = CreateStatus( inStatusID );
	}

	InterfacePtr<IStringData> statusName( statusID, IID_IZPDATANAME );
	ASSERT( statusName );
	statusName->Set( inStatusName );

	InterfacePtr<IZPTitleStatusInfo> statusInfo( statusID, UseDefaultIID() );
	ASSERT( statusInfo );
	
	statusInfo->SetIsForLayout( inStatusDetail.mIsLayout );
	statusInfo->SetIsForText( inStatusDetail.mIsText );
	statusInfo->SetState( inStatusDetail.mStateID );
	statusInfo->SetSortIndex( inStatusDetail.mSortIndex);

	if( oStatusID )
		*oStatusID = statusID;

	return toReturn;
}

//----------------------------------------------------------------------------------------
// RemoveFromCache
//----------------------------------------------------------------------------------------
void
CZPTitleStatusMgr::RemoveFromCache(
	const IStringData *			inStatusID)
{
	if( !inStatusID )
		return;

	const PMString & theStatusIDStr = inStatusID->Get();
#ifdef DEBUG
	InterfacePtr<const IControllingUnknown> controlling( inStatusID, UseDefaultIID() );
	IZPLog_Str( thisFileLA, enLT_ObjectCreations, "Status boss removed from cache = 0x%08X", controlling.get() );
	IZPLog_Str_( thisFileLA, enLT_ObjectCreations, "Object ID = %s", theStatusIDStr.GrabCString() );
#endif
	mStatusMap.erase( inStatusID->Get() );
}

//----------------------------------------------------------------------------------------
// DecrementReferenceForTitleStatus
//----------------------------------------------------------------------------------------
void
CZPTitleStatusMgr::DecrementReferenceForTitleStatus (
	const ZPStatusIDsList &			inStatusIDs)
{
	ZPStatusIDsList::const_iterator idListCurrIter = inStatusIDs.begin();
	ZPStatusIDsList::const_iterator idListEndIter = inStatusIDs.end();
	
	while( idListCurrIter != idListEndIter )
	{
		const IStringData * theStatusID = this->GetStatusForID( *idListCurrIter );
		if( theStatusID )
		{
#ifdef DEBUG
			InterfacePtr<const IControllingUnknown> controlling( theStatusID, UseDefaultIID() );
			int theRefCount = controlling->GetRefCount();
			IZPLog_Str( thisFileLA, enLT_ObjectCreations, "Status boss at 0x%08X has refcount = %d", controlling.get(), theRefCount );
#endif

			//Update reference count maintained by this manager
			InterfacePtr<IIntData> cachedCount( theStatusID, IID_IZPCACHEMGRCOUNT );
			ASSERT( cachedCount );
			if( cachedCount->Get() == 1 )
				this->RemoveFromCache( theStatusID );
			else
				cachedCount->Set( cachedCount->Get() -1 );

			theStatusID->Release();
		}
		++idListCurrIter;
	}
}



//----------------------------------------------------------------------------------------
// GetDefaultStatusIDs
//----------------------------------------------------------------------------------------
const ZPStatusIDsList &
CZPTitleStatusMgr::GetDefaultStatusIDs() const
{
	return mDefaultStatusIDsList;
}

//----------------------------------------------------------------------------------------
// GetStatusForID
//----------------------------------------------------------------------------------------
const IStringData *
CZPTitleStatusMgr::GetStatusForID(
	const PMString &			inStatusID) const
{
	ZPStatusIDMapConstIter foundIter = mStatusMap.find( inStatusID );
	ZPStatusIDMapConstIter endIter = mStatusMap.end();
	do
	{
		if( foundIter != endIter )
			break;
	
		//Support use of hardcode constant values
		PMString tempStr( inStatusID );
		tempStr.ToLower();
		foundIter = mStatusMap.find( tempStr );
		if( foundIter != endIter )
			break;

		tempStr.ToUpper();
		foundIter = mStatusMap.find( tempStr );
		if( foundIter != endIter )
			break;
	}while( false );

	if( foundIter != endIter )
		return foundIter->second;
	else
		return nil;
}

//----------------------------------------------------------------------------------------
// BroadcastMessage
//----------------------------------------------------------------------------------------
void
CZPTitleStatusMgr::BroadcastMessage(
	const ClassID&						inTheChange,
	void *								inChangedBy )
{
	InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
	ASSERT(selfSubject);
	selfSubject->Change( inTheChange, IZPTitleStatusMgr::kDefaultIID, inChangedBy );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// Shutdown
//----------------------------------------------------------------------------------------
void
CZPTitleStatusMgr::Shutdown()
{
	this->DecrementReferenceForTitleStatus( mDefaultStatusIDsList );
	mDefaultStatusIDsList.clear();
}

#pragma mark -
//----------------------------------------------------------------------------------------
// CompareStatusIDBySortIndex 
//----------------------------------------------------------------------------------------
bool
CompareStatusIDBySortIndex::operator ()(
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

	InterfacePtr<const IZPTitleStatusMgr> statusMgr( zpApp->GetTitleMgr(), UseDefaultIID() );
	ASSERT(statusMgr);

	InterfacePtr<const IZPTitleStatusInfo> statusInfoFirstVal( statusMgr->GetStatusForID(inFirstVal), UseDefaultIID() );
	ASSERT( statusInfoFirstVal );

	InterfacePtr<const IZPTitleStatusInfo> statusInfoSecondVal( statusMgr->GetStatusForID(inSecondVal), UseDefaultIID() );
	ASSERT( statusInfoSecondVal );

	if (statusInfoFirstVal->GetSortIndex() == statusInfoSecondVal->GetSortIndex())
	{
		InterfacePtr<const IStringData> statusName1( statusInfoFirstVal, IID_IZPDATANAME );
		ASSERT( statusName1 );

		InterfacePtr<const IStringData> statusName2( statusInfoSecondVal, IID_IZPDATANAME );
		ASSERT( statusName2 );

		return statusName1->Get() < statusName2->Get();
	}
	else
		return statusInfoFirstVal->GetSortIndex() < statusInfoSecondVal->GetSortIndex();
}