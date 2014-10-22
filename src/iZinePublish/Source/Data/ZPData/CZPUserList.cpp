//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Data/ZPData/CZPUserList.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 1-10-2010
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
#include "IZPUserList.h"
#include "IZPUserMgr.h"
#include "IZPTitleMgr.h"

//IZP General includes
#include "ZPConstants.h"
#include "CZPStringListData.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CZPStringListData_<IZPUserList>
class CZPUserList : public inheritClass
{
public:
						CZPUserList(IPMUnknown * boss);
	virtual				~CZPUserList();

	void				SetStringList(
							const K2Vector<PMString>&	newList,
							const bool					inHasCompleteList);
	const K2Vector<PMString>& GetStringList(
							const bool					inFetchIfNotHave);

	bool				HasData() const;
	void				SetHasData(
							bool						inHasData);

	void				RequestUpdateFromServer();

	bool				ContainsUserID(
							const PMString &			inUserID ) const;

protected:
	bool	mHasData;
};


CREATE_PMINTERFACE(CZPUserList, kZPUserListImpl)

CZPUserList::CZPUserList(IPMUnknown * boss)
: inheritClass(boss)
, mHasData( false )
{
	//Is it the right place to decrement the references? Should title manager do this?
	//Decrement user references.
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<IZPUserMgr> userMgr( zpApp->GetUserMgr(), UseDefaultIID() );
	ASSERT(userMgr);
	
	userMgr->DecrementReferenceForUsers( mStringList );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPUserList::~CZPUserList()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// SetStringList
//----------------------------------------------------------------------------------------
void
CZPUserList::SetStringList(
	const K2Vector<PMString>& newList,
	const bool					inHasCompleteList)
{
	InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
	ASSERT(selfSubject);

	selfSubject->Change( kZPUserListWillChangeMsg, IZPUserList::kDefaultIID, nil );

	inheritClass::SetStringList( newList );
	if( inHasCompleteList )
		this->SetHasData( true );

	selfSubject->Change( kZPUserListChangedMsg, IZPUserList::kDefaultIID, nil );
}

//----------------------------------------------------------------------------------------
// GetStringList
//----------------------------------------------------------------------------------------
const K2Vector<PMString>&
CZPUserList::GetStringList(
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
CZPUserList::RequestUpdateFromServer()
{
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

	const PMString & theUserID = selfID->Get();
	ASSERT( theUserID.empty() == false );
	ASSERT( theUserID.Compare( kFalse, kEmptyGUID ) != 0 );

	titleMgr->FetchUsersForTitle( theUserID );
}

//----------------------------------------------------------------------------------------
// HasData
//----------------------------------------------------------------------------------------
bool
CZPUserList::HasData()const
{
	return mHasData;
}

//----------------------------------------------------------------------------------------
// SetHasData
//----------------------------------------------------------------------------------------
void
CZPUserList::SetHasData(
	bool						inHasData)
{
	mHasData = inHasData;
}

//----------------------------------------------------------------------------------------
// ContainsUserID
//----------------------------------------------------------------------------------------
bool
CZPUserList::ContainsUserID(
	const PMString &			inUserID) const
{
	if( inUserID.IsEmpty() )
		return false;

	PMStringList::const_iterator currIter = mStringList.begin();
	PMStringList::const_iterator endIter = mStringList.end();

	while( currIter != endIter )
	{
		if( inUserID.Compare( kFalse, *currIter ) == 0 )
			return true;
		++currIter;
	}
	return false;
}
