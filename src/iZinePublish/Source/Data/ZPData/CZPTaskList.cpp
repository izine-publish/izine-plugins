//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Data/ZPData/CZPTaskList.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 6-10-2010
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
#include "IZPTaskList.h"
#include "IZPTaskMgr.h"

//IZP General includes
#include "CZPStringListData.h"
#include "ZPConstants.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CZPStringListData_<IZPTaskList>
class CZPTaskList : public inheritClass
{
public:
						CZPTaskList(IPMUnknown * boss);
	virtual				~CZPTaskList();
	
	void				SetStringList(
							const K2Vector<PMString>&	newList,
							const bool					inHasCompleteList);
	const K2Vector<PMString>& GetStringList(
							const bool					inFetchIfNotHave);

	bool				HasData() const;
	void				SetHasData(
							bool						inHasData);

	void				RequestUpdateFromServer();

	bool				ContainsTaskID(
							const PMString &			inTaskID ) const;

	void				RemoveTaskID(
							const PMString &			inTaskID );

protected:
	bool	mHasData;
private:
};


CREATE_PMINTERFACE(CZPTaskList, kZPTaskListImpl)

CZPTaskList::CZPTaskList(IPMUnknown * boss)
: inheritClass(boss)
, mHasData( false )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPTaskList::~CZPTaskList()
{
	//Is it the right place to decrement the references? Should title manager do this?
	//Decrement task references.
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<IZPTaskMgr> taskMgr( zpApp->GetTaskMgr(), UseDefaultIID() );
	ASSERT(taskMgr);
	
	taskMgr->DecrementReferenceForTasks( mStringList );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// SetStringList
//----------------------------------------------------------------------------------------
void
CZPTaskList::SetStringList(
	const K2Vector<PMString>& newList,
	const bool					inHasCompleteList)
{
	InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
	ASSERT(selfSubject);

	selfSubject->Change( kZPTaskListWillChangeMsg, IZPTaskList::kDefaultIID, nil );

	inheritClass::SetStringList( newList );
	if( inHasCompleteList )
		this->SetHasData( true );

	selfSubject->Change( kZPTaskListChangedMsg, IZPTaskList::kDefaultIID, nil );
}

//----------------------------------------------------------------------------------------
// GetStringList
//----------------------------------------------------------------------------------------
const K2Vector<PMString>&
CZPTaskList::GetStringList(
	const bool					inFetchIfNotHave)
{
	if( inFetchIfNotHave && mHasData == false )
	{	//Request for data
		this->RequestUpdateFromServer();
	}
	return inheritClass::GetStringList();
}

//----------------------------------------------------------------------------------------
// RemoveTaskID
//----------------------------------------------------------------------------------------
void
CZPTaskList::RemoveTaskID(
	const PMString &			inTaskID)
{
	InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
	ASSERT(selfSubject);

	selfSubject->Change( kZPTaskListWillChangeMsg, IZPTaskList::kDefaultIID, nil );

	this->RemoveItem( inTaskID );

	selfSubject->Change( kZPTaskListChangedMsg, IZPTaskList::kDefaultIID, nil );
}

//----------------------------------------------------------------------------------------
// RequestUpdateFromServer
//----------------------------------------------------------------------------------------
void
CZPTaskList::RequestUpdateFromServer()
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<IZPTaskMgr> taskMgr( zpApp->GetTaskMgr(), UseDefaultIID() );
	ASSERT(taskMgr);
	
	InterfacePtr<const IStringData> selfID( this, IID_IZPDATAID );
	ASSERT(selfID);

	const PMString & theEditionID = selfID->Get();
	ASSERT( theEditionID.empty() == false );
	ASSERT( theEditionID.Compare( kFalse, kEmptyGUID ) != 0 );

	taskMgr->FetchTasksForEdition( theEditionID );
}

//----------------------------------------------------------------------------------------
// HasData
//----------------------------------------------------------------------------------------
bool
CZPTaskList::HasData()const
{
	return mHasData;
}

//----------------------------------------------------------------------------------------
// SetHasData
//----------------------------------------------------------------------------------------
void
CZPTaskList::SetHasData(
	bool						inHasData)
{
	mHasData = inHasData;
}

//----------------------------------------------------------------------------------------
// ContainsTaskID
//----------------------------------------------------------------------------------------
bool
CZPTaskList::ContainsTaskID(
	const PMString &			inTaskID) const
{
	if( inTaskID.IsEmpty() )
		return false;

	PMStringList::const_iterator currIter = mStringList.begin();
	PMStringList::const_iterator endIter = mStringList.end();

	while( currIter != endIter )
	{
		if( inTaskID.Compare( kFalse, *currIter ) == 0 )
			return true;
		++currIter;
	}
	return false;
}
