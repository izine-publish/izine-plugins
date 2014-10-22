//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Data/ZPData/CZPStatusList.cpp $
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
#include "ISubject.h"
#include "IStringData.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPApp.h"
#include "IZPStatusList.h"
#include "IZPTitleMgr.h"
#include "IZPTitleStatusMgr.h"

//IZP General includes
#include "CZPStringListData.h"
#include "ZPConstants.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CZPStringListData_<IZPStatusList>
class CZPStatusList : public inheritClass
{
public:
						CZPStatusList(IPMUnknown * boss);
	virtual				~CZPStatusList();

	void				SetStringList(
							const K2Vector<PMString>&	newList);
	const K2Vector<PMString>& GetStringList(
							const bool					inFetchIfNotHave);

	bool				HasData() const;
	void				SetHasData(
							bool						inHasData);
protected:
	bool		mHasData;
private:
};


CREATE_PMINTERFACE(CZPStatusList, kZPStatusListImpl)

CZPStatusList::CZPStatusList(IPMUnknown * boss)
: inheritClass(boss)
, mHasData( false )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPStatusList::~CZPStatusList()
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

	InterfacePtr<IZPTitleStatusMgr> statusMgr( zpApp->GetTitleMgr(), UseDefaultIID() );
	ASSERT(statusMgr);
	
	statusMgr->DecrementReferenceForTitleStatus( mStringList );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// SetStringList
//----------------------------------------------------------------------------------------
void
CZPStatusList::SetStringList(
	const K2Vector<PMString>&	newList)
{
	InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
	ASSERT(selfSubject);

	selfSubject->Change( kZPTitleStatusListWillChange, IZPStatusList::kDefaultIID, nil );

	inheritClass::SetStringList( newList );
	this->SetHasData( true );

	selfSubject->Change( kZPTitleStatusListChanged, IZPStatusList::kDefaultIID, nil );

}

//----------------------------------------------------------------------------------------
// GetStringList
//----------------------------------------------------------------------------------------
const K2Vector<PMString>&
CZPStatusList::GetStringList(
	const bool					inFetchIfNotHave)
{
	if( inFetchIfNotHave && mHasData == false )
	{	//Request for data
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
		InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
		ASSERT( zpApp );

		InterfacePtr<IZPTitleMgr> titleMgr( zpApp->GetTitleMgr(), UseDefaultIID() );
		ASSERT(titleMgr);
		
		InterfacePtr<const IStringData> selfID( this, IID_IZPDATAID );
		ASSERT(selfID);

		const PMString & theTitleID = selfID->Get();
		ASSERT( theTitleID.empty() == false );
		ASSERT( theTitleID.Compare( kFalse, kEmptyGUID ) != 0 );

		titleMgr->FetchStatusForTitle( theTitleID );

	}
	return inheritClass::GetStringList();
}

//----------------------------------------------------------------------------------------
// HasData
//----------------------------------------------------------------------------------------
bool
CZPStatusList::HasData()const
{
	return mHasData;
}

//----------------------------------------------------------------------------------------
// SetHasData
//----------------------------------------------------------------------------------------
void
CZPStatusList::SetHasData(
	bool						inHasData)
{
	mHasData = inHasData;
}
