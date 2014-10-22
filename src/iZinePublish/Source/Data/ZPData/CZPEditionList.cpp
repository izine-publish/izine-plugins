//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Data/ZPData/CZPEditionList.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 1-5-2010
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
#include "IZPEditionList.h"
#include "IZPEditionMgr.h"
#include "IZPTitleMgr.h"

//IZP General includes
#include "CZPStringListData.h"
#include "ZPConstants.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CZPStringListData_<IZPEditionList>
class CZPEditionList : public inheritClass
{
public:
						CZPEditionList(IPMUnknown * boss);
	virtual				~CZPEditionList();

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


CREATE_PMINTERFACE(CZPEditionList, kZPEditionListImpl)

CZPEditionList::CZPEditionList(IPMUnknown * boss)
: inheritClass(boss)
, mHasData( false )
{
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPEditionList::~CZPEditionList()
{
	if( !mHasData )
		return;
	//Is it the right place to decrement the references? Should title manager do this?
	//Decrement edition references.
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<IZPEditionMgr> editionMgr( zpApp->GetEditionMgr(), UseDefaultIID() );
	ASSERT(editionMgr);
	
	editionMgr->DecrementReferenceForEditions( mStringList );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// SetStringList
//----------------------------------------------------------------------------------------
void
CZPEditionList::SetStringList(
	const K2Vector<PMString>& newList)
{
	InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
	ASSERT(selfSubject);

	selfSubject->Change( kZPEditionListWillChange, IZPEditionList::kDefaultIID, nil );

	inheritClass::SetStringList( newList );
	this->SetHasData( true );

	selfSubject->Change( kZPEditionListChanged, IZPEditionList::kDefaultIID, this );
}

//----------------------------------------------------------------------------------------
// GetStringList
//----------------------------------------------------------------------------------------
const K2Vector<PMString>&
CZPEditionList::GetStringList(
	const bool					inFetchIfNotHave)
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	if( inFetchIfNotHave && mHasData == false )
	{	//Request for data
		InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
		ASSERT( zpApp );

		InterfacePtr<IZPTitleMgr> titleMgr( zpApp->GetTitleMgr(), UseDefaultIID() );
		ASSERT(titleMgr);
		
		InterfacePtr<const IStringData> selfID( this, IID_IZPDATAID );
		ASSERT(selfID);

		const PMString & theTitleID = selfID->Get();
		ASSERT( theTitleID.empty() == false );
		ASSERT( theTitleID.Compare( kFalse, kEmptyGUID ) != 0 );

		titleMgr->FetchEditionsForTitle( theTitleID );

	}
	return inheritClass::GetStringList();
}


//----------------------------------------------------------------------------------------
// HasData
//----------------------------------------------------------------------------------------
bool
CZPEditionList::HasData()const
{
	return mHasData;
}

//----------------------------------------------------------------------------------------
// SetHasData
//----------------------------------------------------------------------------------------
void
CZPEditionList::SetHasData(
	bool						inHasData)
{
	mHasData = inHasData;
}

//----------------------------------------------------------------------------------------
// ContainsEditionID
//----------------------------------------------------------------------------------------
bool
CZPEditionList::ContainsEditionID(
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