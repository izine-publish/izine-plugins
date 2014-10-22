//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Data/CZMAdList.cpp $
//	$Revision: 2785 $
//	$Date: 2011-04-11 17:20:43 +0200 (Mon, 11 Apr 2011) $
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
#include "IStringData.h"
#include "ISubject.h"

//SDK General includes

//OS / ThirdParty includes

#include "IZMediaID.h"

//IZP Interfaces
#include "IZMApp.h"
#include "IZMModelController.h"
#include "IZMAdList.h"
#include "IZMAdMgr.h"

//IZP General includes
#include "CZMStringListData.h"
//#include "ZMConstants.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CZMStringListData_<IZMAdList>
class CZMAdList : public inheritClass
{
public:
						CZMAdList(IPMUnknown * boss);
	virtual				~CZMAdList();

	void				SetStringList(
							const K2Vector<PMString>&	newList,
							const bool					inHasCompleteList);
	const K2Vector<PMString>& GetStringList(
							const bool					inFetchIfNotHave);

	bool				HasData() const;
	void				SetHasData(
							bool						inHasData);

	void				RequestUpdateFromServer();

	bool				ContainsAdID(
							const PMString &			inAdID ) const;

protected:
	bool	mHasData;
private:
};


CREATE_PMINTERFACE(CZMAdList, kZMAdListImpl)

CZMAdList::CZMAdList(IPMUnknown * boss)
: inheritClass(boss)
, mHasData( false )
{
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZMAdList::~CZMAdList()
{
	//Is it the right place to decrement the references? Should title manager do this?
	//Decrement Ad references.
	InterfacePtr<const IZMApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );
	if (!zpApp)
		return;

	InterfacePtr<IZMAdMgr> adMgr( zpApp->GetAdMgr(), UseDefaultIID() );
	ASSERT(adMgr);
	
	adMgr->DecrementReferenceForAds( mStringList );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// SetStringList
//----------------------------------------------------------------------------------------
void
CZMAdList::SetStringList(
	const K2Vector<PMString>& newList,
	const bool					inHasCompleteList)
{
	InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
	ASSERT(selfSubject);

	selfSubject->Change( kZMAdListWillChangeMsg, IZMAdList::kDefaultIID, nil );

	inheritClass::SetStringList( newList );
	if( inHasCompleteList )
		this->SetHasData( true );

	selfSubject->Change( kZMAdListChangedMsg, IZMAdList::kDefaultIID, nil );
}

//----------------------------------------------------------------------------------------
// GetStringList
//----------------------------------------------------------------------------------------
const K2Vector<PMString>&
CZMAdList::GetStringList(
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
CZMAdList::RequestUpdateFromServer()
{
	InterfacePtr<IZMModelController> modelController( gSession, UseDefaultIID() );
	ASSERT( modelController );
	
	InterfacePtr<const IStringData> selfID( this, IID_IZMDATAID );
	ASSERT(selfID);

	const PMString & theAdFileID = selfID->Get();
	ASSERT( theAdFileID.empty() == false );
	//ASSERT( theAdFileID.Compare( kFalse, kEmptyGUID ) != 0 );

	modelController->FetchAdsIfUpdated( theAdFileID );
}

//----------------------------------------------------------------------------------------
// HasData
//----------------------------------------------------------------------------------------
bool
CZMAdList::HasData()const
{
	return mHasData;
}

//----------------------------------------------------------------------------------------
// SetHasData
//----------------------------------------------------------------------------------------
void
CZMAdList::SetHasData(
	bool						inHasData)
{
	mHasData = inHasData;
}

//----------------------------------------------------------------------------------------
// ContainsAdID
//----------------------------------------------------------------------------------------
bool
CZMAdList::ContainsAdID(
	const PMString &			inAdID) const
{
	if( inAdID.IsEmpty() )
		return false;

	PMStringList::const_iterator currIter = mStringList.begin();
	PMStringList::const_iterator endIter = mStringList.end();

	while( currIter != endIter )
	{
		if( inAdID.Compare( kFalse, *currIter ) == 0 )
			return true;
		++currIter;
	}
	return false;
}
