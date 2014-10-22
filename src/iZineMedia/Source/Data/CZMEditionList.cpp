//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Data/CZMEditionList.cpp $
//	$Revision: 3028 $
//	$Date: 2011-05-18 08:09:30 +0200 (Wed, 18 May 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 4-4-2011
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
#include "IZMEditionList.h"
#include "IZMEditionMgr.h"
#include "IZMTitleMgr.h"

//IZP General includes
#include "CZMStringListData.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CZMStringListData_<IZMEditionList>
class CZMEditionList : public inheritClass
{
public:
						CZMEditionList(IPMUnknown * boss);
	virtual				~CZMEditionList();

	void				SetStringList(
							const K2Vector<PMString>&	newList);
	const K2Vector<PMString>& GetStringList(
							const bool					inFetchIfNotHave);

	bool				HasData() const;
	void				SetHasData(
							bool						inHasData);

	bool				ContainsEditionID(
							const PMString &			inEditionID ) const;
protected:
	bool	mHasData;
private:
};


CREATE_PMINTERFACE(CZMEditionList, kZMEditionListImpl)

CZMEditionList::CZMEditionList(IPMUnknown * boss)
: inheritClass(boss)
, mHasData( false )
{
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZMEditionList::~CZMEditionList()
{
	if( !mHasData )
		return;
	//Is it the right place to decrement the references? Should title manager do this?
	//Decrement edition references.
	InterfacePtr<const IZMApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );
	if (!zpApp)
		return;

	InterfacePtr<IZMEditionMgr> editionMgr( zpApp->GetEditionMgr(), UseDefaultIID() );
	ASSERT(editionMgr);
	
	editionMgr->DecrementReferenceForEditions( mStringList );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// SetStringList
//----------------------------------------------------------------------------------------
void
CZMEditionList::SetStringList(
	const K2Vector<PMString>& newList)
{
	InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
	ASSERT(selfSubject);

	selfSubject->Change( kZMEditionListWillChange, IZMEditionList::kDefaultIID, nil );

	inheritClass::SetStringList( newList );
	this->SetHasData( true );

	selfSubject->Change( kZMEditionListChanged, IZMEditionList::kDefaultIID, this );
}

//----------------------------------------------------------------------------------------
// GetStringList
//----------------------------------------------------------------------------------------
const K2Vector<PMString>&
CZMEditionList::GetStringList(
	const bool					inFetchIfNotHave)
{
	if( inFetchIfNotHave && mHasData == false )
	{	//Request for data
		InterfacePtr<const IZMApp> zpApp( gSession, UseDefaultIID() );
		ASSERT( zpApp );

		InterfacePtr<IZMTitleMgr> titleMgr( zpApp->GetTitleMgr(), UseDefaultIID() );
		ASSERT(titleMgr);
		
		InterfacePtr<const IStringData> selfID( this, IID_IZMDATAID );
		ASSERT(selfID);

		const PMString & theTitleID = selfID->Get();
		ASSERT( theTitleID.empty() == false );
		//ASSERT( theTitleID.Compare( kFalse, kEmptyGUID ) != 0 );

		titleMgr->FetchEditionsForTitle( theTitleID );

	}
	return inheritClass::GetStringList();
}


//----------------------------------------------------------------------------------------
// HasData
//----------------------------------------------------------------------------------------
bool
CZMEditionList::HasData()const
{
	return mHasData;
}

//----------------------------------------------------------------------------------------
// SetHasData
//----------------------------------------------------------------------------------------
void
CZMEditionList::SetHasData(
	bool						inHasData)
{
	mHasData = inHasData;
}

//----------------------------------------------------------------------------------------
// ContainsEditionID
//----------------------------------------------------------------------------------------
bool
CZMEditionList::ContainsEditionID(
	const PMString &			inEditionID) const
{
	if( inEditionID.IsEmpty() )
		return false;

	PMStringList::const_iterator currIter = mStringList.begin();
	PMStringList::const_iterator endIter = mStringList.end();

	while( currIter != endIter )
	{
		if( inEditionID.Compare( kFalse, *currIter ) == 0 )
			return true;
		++currIter;
	}
	return false;
}