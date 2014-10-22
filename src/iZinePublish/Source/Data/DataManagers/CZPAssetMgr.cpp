//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Data/DataManagers/CZPAssetMgr.cpp $
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
#include "Utils.h"

#include "IZPID.h"

//IZP Interfaces
#include "IZPApp.h"
#include "IZPAMWSUtils.h"
#include "IZPAssetInfo.h"
#include "IZPAssetList.h"
#include "IZPAssetLockInfo.h"
#include "IZPAssetMgr.h"
#include "IZPEditionMgr.h"
#include "IZPInDnUtils.h"
#include "IZPSoapRequest.h"
#include "IZPTitleStatusInfo.h"
#include "IZPTitleStatusMgr.h"
#include "IZPWSController.h"

//IZP General includes
#include "CZPDeleteAssetRequest.h"
#include "CZPFetchAssetListRequest.h"
#include "CZPFetchAssetListResponse.h"
#include "CZPFetchAssetsResponse.h"
#include "CZPFetchLockInfoRequest.h"
#include "CZPFetchLockInfoResponse.h"
#include "ZPConstants.h"
#include "ZPDataHelper.h"

#include "IZPLog.h"

#define thisFileLA	enLMA_DBObjects
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

class CZPAssetMgr : public CPMUnknown<IZPAssetMgr>
{
public:
				CZPAssetMgr(IPMUnknown * boss);
	virtual		~CZPAssetMgr();

	void				Shutdown();

	void				UpdateAssetList(
							const IStringData *			inEditionID,
							const ZPIDPairSList &		inAssetList,
							const ZPAssetDetailSList &	inAssetInfoList );
	
	void				UpdateAssetsLockInfo(
							const ZPAssetLockInfoSList &	inLocksInfo );
	void				UpdateAssetLockInfo(
							const ZPAssetLockInfo &		inAssetLockInfo );

	void				UpdateAsset(
							const IStringData *			inAssetID,
							const PMString *			inAssetName,
							const ZPAssetDetailInfo *	inAssetInfo ) const;

	void				UpdateAssetName(
							const IStringData *			inDbAssetID,
							const PMString &			inAssetName ) const;

	void				UpdateAssetName(
							const PMString &			inAssetID,
							const PMString &			inAssetName ) const;

	void				UpdateAssetStatus(
							const PMString &			inAssetID,
							const PMString &			inStatusID ) const;

	void				AddAsset(
							const IStringData *			inEditionID,
							const PMString &			inAssetID,
							const PMString *			inAssetName,
							const ZPAssetDetailInfo *	inAssetInfo );

	const IStringData *	GetAssetByID(
							const PMString &			inAssetID ) const;
	
	void				DecrementReferenceForAssets(
							const ZPAssetIDsList &		inAssetIDs );

	void				FetchAssetsForEdition(
							const PMString &			inEditionID ) const;
	void				FetchAssetsLockInfo(
							const ZPAssetIDsList &		inAssetIDs ) const;

	void				DeleteAsset(
							const PMString &			inAssetID ) const;

	void				HandleWSResponse(
							const IZPSoapRequest *		inSoapRequest,
							const AZPSoapResponse *		inResponse,
							ZPWSError					inStatus);
protected:
	static IStringData * CreateAsset(
							const PMString &			inAssetID );

	bool				AddUpdateAsset(
							const PMString &			inAssetID,
							const PMString &			inAssetName,
							const ZPAssetDetailInfo *	inAssetDetails,
							const IStringData **		oAssetID );

	void				RemoveFromCache(
							const IStringData *			inAssetID );

	void				HandleResponse_FetchAssetList(
							const AZPSoapRequest *		inRequest,
							const AZPSoapResponse *		inResponse );
	void				HandleResponse_FetchAssets(
							const AZPSoapRequest *		inRequest,
							const AZPSoapResponse *		inResponse );
	void				HandleResponse_FetchLockInfo(
							const AZPSoapRequest *		inRequest,
							const AZPSoapResponse *		inResponse );

	void				BroadcastMessage(
							const ClassID &				inTheChange,
							void*						inChangedBy = NULL );

private:
	ZPAssetIDMap		mAssetMap;
};


CREATE_PMINTERFACE(CZPAssetMgr, kZPAssetMgrImpl)

CZPAssetMgr::CZPAssetMgr(IPMUnknown * boss) :
CPMUnknown<IZPAssetMgr>(boss)
{

}

CZPAssetMgr::~CZPAssetMgr()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// CreateAsset
//----------------------------------------------------------------------------------------
IStringData *
CZPAssetMgr::CreateAsset(
	const PMString &			inAssetID)
{
	LogFunctionEnterExit;
	IStringData * theAssetID = ::CreateObject2<IStringData>( kZPDataAssetBoss , IID_IZPDATAID );
	ASSERT(theAssetID);
#ifdef DEBUG
	InterfacePtr<const IControllingUnknown> controlling( theAssetID, UseDefaultIID() );
	IZPLog_Str( thisFileLA, enLT_ObjectCreations, "Asset boss created = 0x%08X", controlling.get() );
	IZPLog_Str_( thisFileLA, enLT_ObjectCreations, "Object ID = %s", inAssetID.GrabCString() );
#endif
	theAssetID->Set( inAssetID );

	return theAssetID;
}

//----------------------------------------------------------------------------------------
// FetchAssetsForEdition
//----------------------------------------------------------------------------------------
void
CZPAssetMgr::FetchAssetsForEdition(
	const PMString &			inEditionID) const
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IZPWSController> wsController( gSession , UseDefaultIID() );
	ASSERT(wsController);

	NewWSRequest( CZPFetchAssetListRequest )

	ASSERT( ZPDataHelper::IsValidGUID(inEditionID ) );
	
	theRequest->SetEditionID( inEditionID );


	bool16 isInCopyHost = Utils<IZPInDnUtils>()->IsHostAppInCopy();
	//TODO: Get type from other function, also for InDn server.
	K2Vector<PMString> types;
	//Now we are caching all assets of an edition, so fetch all types
	types.insert(types.end(), kAssetType_InCopyAssignmentID);
	types.insert(types.end(), kAssetType_InCopyDocumentID);
	if( isInCopyHost )
	{
		types.insert(types.end(), kAssetType_InCopyTemplateID);
	}
	else
	{
		types.insert(types.end(), kAssetType_InDesignTemplateID);
		types.insert(types.end(), kAssetType_InDesignDocumentID);
	}
	types.insert(types.end(), kAssetType_InDesignStoryID);
	
	theRequest->SetAssetTypes( types );

	wsController->AddRequest( soapRequest );
}

//----------------------------------------------------------------------------------------
// FetchAssetsLockInfo
//----------------------------------------------------------------------------------------
void
CZPAssetMgr::FetchAssetsLockInfo(
	const ZPAssetIDsList &		inAssetIDs) const
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	if( inAssetIDs.size() <= 0 )
		return;

	InterfacePtr<IZPWSController> wsController( gSession , UseDefaultIID() );
	ASSERT(wsController);

	AZPSoapRequest * theRequest = Utils<IZPAMWSUtils>()->CreateFetchLockInfoRequest( inAssetIDs );
	NewWSRequestWithData( theRequest )

	wsController->AddRequest( soapRequest );
}

//----------------------------------------------------------------------------------------
// DeleteAsset
//----------------------------------------------------------------------------------------
void
CZPAssetMgr::DeleteAsset(
	const PMString &			inAssetID) const
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IZPWSController> wsController( gSession , UseDefaultIID() );
	ASSERT(wsController);

	NewWSRequest( CZPDeleteAssetRequest )

	//Someday: save the asset to delete with this request, so that anyone that needs info from old can be fetched till its handling is over.
	theRequest->SetAssetID( inAssetID );

	wsController->AddRequest( soapRequest );
}


//----------------------------------------------------------------------------------------
// HandleWSResponse
//----------------------------------------------------------------------------------------
void
CZPAssetMgr::HandleWSResponse(
	const IZPSoapRequest *			inSoapRequest,
	const AZPSoapResponse *			inResponse,
	ZPWSError						inStatus)
{
	LogFunctionEnterExit;
	
	const AZPSoapRequest * theRequest = inSoapRequest->GetRequestData();
	switch( theRequest->GetRequestType() )
	{
	case eReq_FetchAssetList:
		this->HandleResponse_FetchAssetList( theRequest, inResponse );
		break;
	case eReq_FetchAssets:
		this->HandleResponse_FetchAssets( theRequest, inResponse );
		break;
	case eReq_FetchLockInfo:
		this->HandleResponse_FetchLockInfo( theRequest, inResponse );
		break;
	}
}

//----------------------------------------------------------------------------------------
// HandleResponse_FetchAssetList
//----------------------------------------------------------------------------------------
void
CZPAssetMgr::HandleResponse_FetchAssetList(
	const AZPSoapRequest *			inRequest,
	const AZPSoapResponse *			inResponse)
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<IZPEditionMgr> editionMgr( zpApp->GetEditionMgr(), UseDefaultIID() );
	ASSERT(editionMgr);

	const CZPFetchAssetListRequest * theRequest = dynamic_cast<const CZPFetchAssetListRequest*>(inRequest);
	ASSERT(theRequest);
	
	const CZPFetchAssetListResponse * theResponse = dynamic_cast<const CZPFetchAssetListResponse*>(inResponse);
	ASSERT(theResponse);
	

	const IStringData * editionID = editionMgr->GetEditionForID( theRequest->GetEditionID() );
	ASSERT( editionID );

	if( !editionID )
		return;

	this->UpdateAssetList( editionID, theResponse->GetAssetList(), theResponse->GetAssetDetailList() );

}

//----------------------------------------------------------------------------------------
// HandleResponse_FetchAssets
//----------------------------------------------------------------------------------------
void
CZPAssetMgr::HandleResponse_FetchAssets(
	const AZPSoapRequest *			inRequest,
	const AZPSoapResponse *			inResponse)
{
	LogFunctionEnterExit;
	const CZPFetchAssetsResponse * theResponse = dynamic_cast<const CZPFetchAssetsResponse*>(inResponse);
	ASSERT(theResponse);

	const ZPAssetInfoSList &	newAssetsInfoList = theResponse->GetAssets();

	ZPAssetInfoSList::const_iterator currIter = newAssetsInfoList.begin();
	ZPAssetInfoSList::const_iterator endIter = newAssetsInfoList.end();

	const IStringData * editionID = nil;
	PMString prevEditionID;	//For optimization, as editionID will be same in all response.

	while(currIter != endIter)
	{
		const ZPAssetInfo * newAssetInfo = *currIter;
		//update the asset info with the Asset.
		const IStringData * dbAssetID = this->GetAssetByID( newAssetInfo->mAsset.mID );
		
		if( dbAssetID )
			this->UpdateAsset( dbAssetID, &newAssetInfo->mAsset.mName, &newAssetInfo->mAssetDetailInfo);
		else
		{
			if( editionID == nil || prevEditionID.Compare( kFalse, newAssetInfo->mEditionID ) != 0 )
			{	//Update the cached pointer to edition
				prevEditionID = newAssetInfo->mEditionID;
				editionID = ZPDataHelper::GetEditionByID( prevEditionID );
			}

			this->AddAsset( editionID, newAssetInfo->mAsset.mID, &(newAssetInfo->mAsset.mName), &(newAssetInfo->mAssetDetailInfo));
		}
		++currIter;
	}
}

//----------------------------------------------------------------------------------------
// HandleResponse_FetchLockInfo
//----------------------------------------------------------------------------------------
void
CZPAssetMgr::HandleResponse_FetchLockInfo(
	const AZPSoapRequest *		inRequest,
	const AZPSoapResponse *		inResponse)
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<IZPEditionMgr> editionMgr( zpApp->GetEditionMgr(), UseDefaultIID() );
	ASSERT(editionMgr);

	const CZPFetchLockInfoRequest * theRequest = dynamic_cast<const CZPFetchLockInfoRequest*>(inRequest);
	ASSERT(theRequest);
	
	const CZPFetchLockInfoResponse * theResponse = dynamic_cast<const CZPFetchLockInfoResponse*>(inResponse);
	ASSERT(theResponse);

	const ZPAssetLockInfoSList & theLocks = theResponse->GetLockInfoList();
	this->UpdateAssetsLockInfo( theLocks );
}
#pragma mark -
//----------------------------------------------------------------------------------------
// UpdateAssetList
//----------------------------------------------------------------------------------------
void
CZPAssetMgr::UpdateAssetList(
	const IStringData *			inEditionID,
	const ZPIDPairSList &		inAssetList,
	const ZPAssetDetailSList &	inAssetInfoList)
{
	LogFunctionEnterExit;
	ZPAssetIDsList assetIDsToRemove;
	ZPAssetIDsList assetIDsNewList;

	InterfacePtr<IZPAssetList> editionAssetList( inEditionID, UseDefaultIID() );

	ASSERT( editionAssetList != nil );	//Updating an edition that is not cached.
	if ( editionAssetList == nil )
		return;

	assetIDsToRemove = editionAssetList->GetStringList( false );
	
	InterfacePtr<ISubject> editionSubject( editionAssetList, UseDefaultIID() );
	ASSERT(editionSubject);
	//Send list will change message so that UI does not update with intermediate data.
	editionSubject->Change( kZPAssetListWillChangeMsg, IZPAssetList::kDefaultIID, nil );

	//Add/Update the Asset for edition
	ZPIDPairSList::const_iterator	currIter = inAssetList.begin();
	ZPIDPairSList::const_iterator	endIter = inAssetList.end();
	ZPAssetDetailSList::const_iterator	currDetailsIter = inAssetInfoList.begin();

	while( currIter != endIter )
	{
		const PMString & assetID = (*currIter)->mID;

		//Update the Asset
		const IStringData * theAsset = NULL;
		//(*currIter)->mName.SetTranslatable( kFalse );
		bool newAssetCreated = this->AddUpdateAsset( assetID, (*currIter)->mName, *currDetailsIter, &theAsset );
		
		//Add to new list.
		assetIDsNewList.push_back(assetID);
		
		++currIter;
		++currDetailsIter;

		if( newAssetCreated == false )
			theAsset->AddRef();	//Retain till this ID is referenced.

		//Increament ref count maintained by this manager.
		InterfacePtr<IIntData> cachedCount( theAsset, IID_IZPCACHEMGRCOUNT );
		ASSERT( cachedCount );
		cachedCount->Set( cachedCount->Get() + 1 );
	}

	//Update the asset id list
	editionAssetList->SetStringList( assetIDsNewList );

	//Fetch Lock info for these assets
	this->FetchAssetsLockInfo( assetIDsNewList );

	//Remove from cached list. Should we? what if same Edition is shared by multiple titles.
	this->DecrementReferenceForAssets( assetIDsToRemove );
}

//----------------------------------------------------------------------------------------
// AddAsset
//----------------------------------------------------------------------------------------
void
CZPAssetMgr::AddAsset(
	const IStringData *			inEditionID,
	const PMString &			inAssetID,
	const PMString *			inAssetName,
	const ZPAssetDetailInfo *	inAssetInfo)
{
	LogFunctionEnterExit;
	InterfacePtr<IZPAssetList> editionAssetList( inEditionID, UseDefaultIID() );

	ASSERT( editionAssetList != nil );	//Updating an edition that is not cached.
	if ( editionAssetList == nil )
		return;

	ZPAssetIDsList assetIDsNewList = editionAssetList->GetStringList( false );
	//Update the Asset
	const IStringData * theAsset = NULL;
	//(*currIter)->mName.SetTranslatable( kFalse );
	ASSERT(inAssetName);

	bool newAssetCreated = this->AddUpdateAsset( inAssetID, *inAssetName, inAssetInfo, &theAsset );
	
	if( !theAsset )
		return;

	//Add to new list.
	assetIDsNewList.push_back( inAssetID );

	//Increament ref count maintained by this manager.
	InterfacePtr<IIntData> cachedCount( theAsset, IID_IZPCACHEMGRCOUNT );
	ASSERT( cachedCount );
	cachedCount->Set( cachedCount->Get() + 1 );

	//Update the asset id list
	editionAssetList->SetStringList( assetIDsNewList, false );
}

//----------------------------------------------------------------------------------------
// UpdateAssetsLockInfo
//----------------------------------------------------------------------------------------
void
CZPAssetMgr::UpdateAssetsLockInfo(
	const ZPAssetLockInfoSList &	inLocksInfo)
{
	LogFunctionEnterExit;
	ZPAssetLockInfoSList::const_iterator	currIter = inLocksInfo.begin();
	ZPAssetLockInfoSList::const_iterator	endIter = inLocksInfo.end();

	while( currIter != endIter )
	{
		const ZPAssetLockInfo * theAssetLockInfo = *currIter;
		if( theAssetLockInfo )
			this->UpdateAssetLockInfo( *theAssetLockInfo );
		++currIter;
	}
}

//----------------------------------------------------------------------------------------
// UpdateAssetLockInfo
//----------------------------------------------------------------------------------------
void
CZPAssetMgr::UpdateAssetLockInfo(
	const ZPAssetLockInfo &		inAssetLockInfo)
{
	LogFunctionEnterExit;
	const IStringData * theAsset = this->GetAssetByID( inAssetLockInfo.mAssetID );
	if( !theAsset )
		return;	//This may be new asset, unsaved.

	InterfacePtr<IZPAssetLockInfo> assetLockInfo( theAsset, UseDefaultIID() );
	ASSERT( assetLockInfo );

	bool infoChanged = false;
	
	if( assetLockInfo->GetLockID().Compare( kTrue, inAssetLockInfo.mLockID ) != 0 
		|| assetLockInfo->GetLockID().IsEmpty() )	//This fixes issue with lock status being set to unavailable after first check-in
	{
		infoChanged = true;
		assetLockInfo->SetLockID( inAssetLockInfo.mLockID );
	}

	if( assetLockInfo->GetDocumentID().Compare( kTrue, inAssetLockInfo.mDocumentID ) != 0)
	{
		infoChanged = true;
		assetLockInfo->SetDocumentID( inAssetLockInfo.mDocumentID );
	}
	
	if( assetLockInfo->GetDocumentName().Compare( kTrue, inAssetLockInfo.mDocumentName ) != 0)
	{
		infoChanged = true;
		assetLockInfo->SetDocumentName( inAssetLockInfo.mDocumentName );
	}
	
	if( assetLockInfo->GetUserID().Compare( kTrue, inAssetLockInfo.mUserID ) != 0)
	{
		infoChanged = true;
		assetLockInfo->SetUserID( inAssetLockInfo.mUserID );
	}
	
	if( assetLockInfo->GetUserName().Compare( kTrue, inAssetLockInfo.mUserName ) != 0)
	{
		infoChanged = true;
		assetLockInfo->SetUserName( inAssetLockInfo.mUserName );
	}

	if( assetLockInfo->GetAppName().Compare( kTrue, inAssetLockInfo.mApplicationName ) != 0)
	{
		infoChanged = true;
		assetLockInfo->SetAppName( inAssetLockInfo.mApplicationName );
	}

	if( infoChanged )
		assetLockInfo->BroadcastMessage();
}



//----------------------------------------------------------------------------------------
// AddUpdateAsset
//----------------------------------------------------------------------------------------
bool
CZPAssetMgr::AddUpdateAsset(
	const PMString &			inAssetID,
	const PMString &			inAssetName,
	const ZPAssetDetailInfo *	inAssetDetails,
	const IStringData **		oAssetID)
{
	LogFunctionEnterExit;
	const IStringData * assetID = this->GetAssetByID( inAssetID );
	bool toReturn = assetID == nil;
	if( toReturn )
	{
		assetID = mAssetMap[inAssetID] = CreateAsset( inAssetID );
	}

	this->UpdateAsset( assetID, &inAssetName, inAssetDetails );

	if( oAssetID )
		*oAssetID = assetID;

	return toReturn;
}

//----------------------------------------------------------------------------------------
// UpdateAsset
//----------------------------------------------------------------------------------------
void
CZPAssetMgr::UpdateAsset(
	const IStringData *			inAssetID,
	const PMString *			inAssetName,
	const ZPAssetDetailInfo *	inAssetDetails) const
{
	LogFunctionEnterExit;
	if( inAssetName )
	{
		this->UpdateAssetName( inAssetID, *inAssetName );
	}
	
	if( inAssetDetails )
	{
		InterfacePtr<IZPAssetInfo> assetDetail( inAssetID, UseDefaultIID() );
		ASSERT( assetDetail );
		assetDetail->SetAssetDetail( *inAssetDetails );
	}
	//else?
}

//----------------------------------------------------------------------------------------
// UpdateAssetName
//----------------------------------------------------------------------------------------
void
CZPAssetMgr::UpdateAssetName(
	const IStringData *			inDbAssetID,
	const PMString &			inAssetName) const
{
	LogFunctionEnterExit;
	if( inDbAssetID && inAssetName.IsEmpty() == false )
	{
		InterfacePtr<IStringData> assetName( inDbAssetID, IID_IZPDATANAME );
		ASSERT( assetName );
		if (assetName->Get().Compare(kFalse, inAssetName) == 0)
			return;

		const PMString oldName = assetName->Get();
		InterfacePtr<ISubject> assetSubject( inDbAssetID, UseDefaultIID() );
		ASSERT(assetSubject);

		if(oldName.IsEmpty() == kFalse)
			assetSubject->Change( kZPDataNameWillChangeMsg, IID_IZPDATANAME, (void *)&inAssetName );

		assetName->Set( inAssetName );

		if(oldName.IsEmpty() == kFalse)
			assetSubject->Change( kZPDataNameChangedMsg, IID_IZPDATANAME, nil );
	}
}

//----------------------------------------------------------------------------------------
// UpdateAssetName
//----------------------------------------------------------------------------------------
void
CZPAssetMgr::UpdateAssetName(
	const PMString &			inAssetID,
	const PMString &			inAssetName) const
{
	LogFunctionEnterExit;
	const IStringData *dbAssetID = this->GetAssetByID(inAssetID);
	this->UpdateAssetName(dbAssetID, inAssetName);
}

//----------------------------------------------------------------------------------------
// UpdateAssetStatus
//----------------------------------------------------------------------------------------
void
CZPAssetMgr::UpdateAssetStatus(
	const PMString &			inAssetID,
	const PMString &			inStatusID) const
{
	LogFunctionEnterExit;
	if (inStatusID != kNullString)
	{
		const IStringData * assetID = GetAssetByID(inAssetID);
		InterfacePtr<IZPAssetInfo> assetDetail( assetID, UseDefaultIID() );
		ASSERT( assetDetail );
		ZPAssetDetailInfo & assetDetailInfo = assetDetail->GetAssetDetail();
		if( inStatusID.Compare( kTrue, assetDetailInfo.mStatusID ) != 0 )
		{
			assetDetailInfo.mStatusID = inStatusID;
		
			InterfacePtr<ISubject> assetSubject( assetDetail, UseDefaultIID() );
			ASSERT(assetSubject);

			assetSubject->Change( kZPAssetInfoChangedMsg, IZPAssetInfo::kDefaultIID, nil );
		}
	}
}

//----------------------------------------------------------------------------------------
// RemoveFromCache
//----------------------------------------------------------------------------------------
void
CZPAssetMgr::RemoveFromCache(
	const IStringData *				inAssetID)
{
	LogFunctionEnterExit;
	if( !inAssetID )
		return;

	const PMString & theAssetIDStr = inAssetID->Get();
#ifdef DEBUG
	InterfacePtr<const IControllingUnknown> controlling( inAssetID, UseDefaultIID() );
	IZPLog_Str( thisFileLA, enLT_ObjectCreations, "Asset boss removed from cache = 0x%08X", controlling.get() );
	IZPLog_Str_( thisFileLA, enLT_ObjectCreations, "Object ID = %s", theAssetIDStr.GrabCString() );
#endif

	mAssetMap.erase( inAssetID->Get() );
}

//----------------------------------------------------------------------------------------
// DecrementReferenceForAssets
//----------------------------------------------------------------------------------------
void
CZPAssetMgr::DecrementReferenceForAssets(
	const ZPAssetIDsList &		inAssetIDs)
{
	LogFunctionEnterExit;
	ZPAssetIDsList::const_iterator idListCurrIter = inAssetIDs.begin();
	ZPAssetIDsList::const_iterator idListEndIter = inAssetIDs.end();
	
	while( idListCurrIter != idListEndIter )
	{
		const IStringData * theAssetID = this->GetAssetByID( *idListCurrIter );

		//Update reference count maintained by this manager
		InterfacePtr<IIntData> cachedCount( theAssetID, IID_IZPCACHEMGRCOUNT );
		ASSERT( cachedCount );
		if( cachedCount->Get() == 1 )
			this->RemoveFromCache( theAssetID );
		else
			cachedCount->Set( cachedCount->Get() -1 );

		if( theAssetID )
			theAssetID->Release();
		++idListCurrIter;
	}
}

//----------------------------------------------------------------------------------------
// GetAssetByID
//----------------------------------------------------------------------------------------
const IStringData *
CZPAssetMgr::GetAssetByID(
	const PMString &			inAssetID) const
{
	LogFunctionEnterExit;
	ZPAssetIDMapConstIter foundIter = mAssetMap.find( inAssetID );
	ZPAssetIDMapConstIter endIter = mAssetMap.end();
	if( foundIter != endIter )
		return foundIter->second;
	else
		return nil;
}

//----------------------------------------------------------------------------------------
// BroadcastMessage
//----------------------------------------------------------------------------------------
void
CZPAssetMgr::BroadcastMessage(
	const ClassID &				inTheChange,
	void*						inChangedBy)
{
	LogFunctionEnterExit;
	InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
	ASSERT(selfSubject);
	selfSubject->Change( inTheChange, IZPAssetMgr::kDefaultIID, inChangedBy );
}

//----------------------------------------------------------------------------------------
// Shutdown
//----------------------------------------------------------------------------------------
void
CZPAssetMgr::Shutdown()
{
	//TODO: 
}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetAssetState
//----------------------------------------------------------------------------------------
enTitleStatusState
IZPAssetMgr::GetAssetState(
	const IStringData *			inDBAssetID)
{
	LogFunctionEnterExit;
	enTitleStatusState toReturn = eTitleStatusState_UnKnown;
	do
	{
		if( !inDBAssetID )
			break;

		InterfacePtr<const IZPAssetInfo> assetInfo( inDBAssetID, UseDefaultIID() );
		ASSERT( assetInfo );

		const PMString & statusIDStr = assetInfo->GetStatusID();

		const IStringData * dbStatusID = ZPDataHelper::GetTitleStatusByID( statusIDStr );
		if( !dbStatusID )
			break;

		InterfacePtr<const IZPTitleStatusInfo> statusInfo( dbStatusID, UseDefaultIID() );
		ASSERT( statusInfo );

		toReturn = statusInfo->GetState();

	}while(kFalse);

	return toReturn;
}
