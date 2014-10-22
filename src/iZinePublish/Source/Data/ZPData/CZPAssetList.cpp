//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Data/ZPData/CZPAssetList.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 24-5-2010
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

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPApp.h"
#include "IZPConfigController.h"
#include "IZPAssetList.h"
#include "IZPAssetMgr.h"

//IZP General includes
#include "CZPStringListData.h"
#include "ZPConstants.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CZPStringListData_<IZPAssetList>
class CZPAssetList : public inheritClass
{
public:
						CZPAssetList(IPMUnknown * boss);
	virtual				~CZPAssetList();

	void				SetStringList(
							const K2Vector<PMString>&	newList,
							const bool					inHasCompleteList);
	const K2Vector<PMString>& GetStringList(
							const bool					inFetchIfNotHave);

	bool				HasData() const;
	void				SetHasData(
							bool						inHasData);

	void				RequestUpdateFromServer();

	bool				ContainsAssetID(
							const PMString &			inAssetID ) const;

protected:
	bool	mHasData;
private:
};


CREATE_PMINTERFACE(CZPAssetList, kZPAssetListImpl)

CZPAssetList::CZPAssetList(IPMUnknown * boss)
: inheritClass(boss)
, mHasData( false )
{
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAssetList::~CZPAssetList()
{
	//Is it the right place to decrement the references? Should title manager do this?
	//Decrement asset references.
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<IZPAssetMgr> assetMgr( zpApp->GetAssetMgr(), UseDefaultIID() );
	ASSERT(assetMgr);
	
	assetMgr->DecrementReferenceForAssets( mStringList );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// SetStringList
//----------------------------------------------------------------------------------------
void
CZPAssetList::SetStringList(
	const K2Vector<PMString>& newList,
	const bool					inHasCompleteList)
{
	InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
	ASSERT(selfSubject);

	selfSubject->Change( kZPAssetListWillChangeMsg, IZPAssetList::kDefaultIID, nil );

	inheritClass::SetStringList( newList );
	if( inHasCompleteList )
		this->SetHasData( true );

	selfSubject->Change( kZPAssetListChangedMsg, IZPAssetList::kDefaultIID, nil );
}

//----------------------------------------------------------------------------------------
// GetStringList
//----------------------------------------------------------------------------------------
const K2Vector<PMString>&
CZPAssetList::GetStringList(
	const bool					inFetchIfNotHave)
{
	if( inFetchIfNotHave && mHasData == false )
	{	//Request for data
		this->RequestUpdateFromServer();
	}
	return inheritClass::GetStringList();
}

//----------------------------------------------------------------------------------------
// RequestUpdateFromServer
//----------------------------------------------------------------------------------------
void
CZPAssetList::RequestUpdateFromServer()
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<IZPAssetMgr> assetMgr( zpApp->GetAssetMgr(), UseDefaultIID() );
	ASSERT(assetMgr);
	
	InterfacePtr<const IStringData> selfID( this, IID_IZPDATAID );
	ASSERT(selfID);

	const PMString & theEditionID = selfID->Get();
	ASSERT( theEditionID.empty() == false );
	ASSERT( theEditionID.Compare( kFalse, kEmptyGUID ) != 0 );

	assetMgr->FetchAssetsForEdition( theEditionID );
}

//----------------------------------------------------------------------------------------
// HasData
//----------------------------------------------------------------------------------------
bool
CZPAssetList::HasData()const
{
	return mHasData;
}

//----------------------------------------------------------------------------------------
// SetHasData
//----------------------------------------------------------------------------------------
void
CZPAssetList::SetHasData(
	bool						inHasData)
{
	mHasData = inHasData;
}

//----------------------------------------------------------------------------------------
// ContainsAssetID
//----------------------------------------------------------------------------------------
bool
CZPAssetList::ContainsAssetID(
	const PMString &			inAssetID) const
{
	if( inAssetID.IsEmpty() )
		return false;

	PMStringList::const_iterator currIter = mStringList.begin();
	PMStringList::const_iterator endIter = mStringList.end();

	while( currIter != endIter )
	{
		if( inAssetID.Compare( kFalse, *currIter ) == 0 )
			return true;
		++currIter;
	}
	return false;
}
