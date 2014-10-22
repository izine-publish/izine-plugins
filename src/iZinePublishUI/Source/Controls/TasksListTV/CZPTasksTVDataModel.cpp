//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/TasksListTV/CZPTasksTVDataModel.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 27-9-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IObserver.h"
#include "IStringData.h"
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
#include "IZPTasksTVDataModel.h"
#include "IZPTaskList.h"
#include "IZPTaskMgr.h"

//IZP General includes
#include "ZPDataHelper.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZPTasksTVDataModel>
class CZPTasksTVDataModel : public inheritClass
{
public:
						CZPTasksTVDataModel(IPMUnknown * boss);
	virtual				~CZPTasksTVDataModel();

	const PMString &	GetEditionID() const;
	//No setter for EditionID. Use UpdateData instead.

	const ZPTaskIDsList *	GetTaskIDList() const;

	const PMString *	GetNthTaskID(
							int							inIndex ) const;
	int32				GetTaskIDIndex(
							const PMString &			inTaskID ) const;

	void				UpdateData(
							const PMString &			inEditionID );

	void				UpdateTaskList();
	void				SetTasksIDList(
							const ZPTaskIDsList &		inTasksIDList );
	void				HandleTaskInfoChange(
							const IStringData *			inTaskID );

	const FilterTaskFnPtr	GetTaskFilteringFunction() const;
	void				SetTaskFilteringFunction(
							FilterTaskFnPtr				inFunc,
							void *						inFilterFnData );

protected:
	void				RemoveListeningEdition();
	void				AddListeningToEdition();

	void				RemoveListeningObject(
							const IPMUnknown *			inObjectToListen,
							const PMIID &				inProtocol);
	void				AddListeningToObject(
							const IPMUnknown *			inObjectToListen,
							const PMIID &				inProtocol);

	void				BroadcastMessage(
							const ClassID&						inTheChange,
							void *								inChangedBy = nil) const;

private:
	PMString			mEditionID;

	//retained here so that it does not get destroyed before its references are removed.
	InterfacePtr<const IStringData> mEdition;	//Edition for which task are displayed by this treeview.
	
	ZPTaskIDsList		mTaskIDList;

	FilterTaskFnPtr		mFilteringFn;
	void *				mFilteringFnData;
};


CREATE_PMINTERFACE(CZPTasksTVDataModel, kZPTasksTVDataModelImpl)

CZPTasksTVDataModel::CZPTasksTVDataModel(IPMUnknown * boss)
: inheritClass(boss)
, mFilteringFn( nil )
, mFilteringFnData( nil )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPTasksTVDataModel::~CZPTasksTVDataModel()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetEditionID
//----------------------------------------------------------------------------------------
const PMString &
CZPTasksTVDataModel::GetEditionID()const
{
	return mEditionID;
}

//----------------------------------------------------------------------------------------
// GetTaskIDList
//----------------------------------------------------------------------------------------
const ZPTaskIDsList *
CZPTasksTVDataModel::GetTaskIDList()const
{
	return &mTaskIDList;
}

//----------------------------------------------------------------------------------------
// GetNthTaskID
//----------------------------------------------------------------------------------------
const PMString *
CZPTasksTVDataModel::GetNthTaskID(
	int							inIndex) const
{
	ASSERT( inIndex >= 0 && inIndex < mTaskIDList.size() );
	return & mTaskIDList.at(inIndex);
}

//----------------------------------------------------------------------------------------
// GetTaskIDIndex
//----------------------------------------------------------------------------------------
int32
CZPTasksTVDataModel::GetTaskIDIndex(
	const PMString &			inTaskID) const
{
	int32 toReturn = -1;

	ZPTaskIDsList::const_iterator beginIter = mTaskIDList.begin();
	ZPTaskIDsList::const_iterator endIter = mTaskIDList.end();
	ZPTaskIDsList::const_iterator foundIter = std::find( beginIter, endIter, inTaskID );
	if( foundIter != endIter )
	{
		toReturn = foundIter - beginIter;
	}

	return toReturn;
}

//----------------------------------------------------------------------------------------
// UpdateData
//----------------------------------------------------------------------------------------
void
CZPTasksTVDataModel::UpdateData(
	const PMString &			inEditionID)
{
	mEditionID = inEditionID;

	const IStringData * theEdition = ZPDataHelper::GetEditionByID( inEditionID );
	
	ZPTaskIDsList emptyList;
	if( theEdition == nil )
	{
		this->RemoveListeningEdition();
		mEdition.reset(nil);
		this->SetTasksIDList( emptyList );
	}
	else
	{
		this->RemoveListeningEdition();
		theEdition->AddRef();
		mEdition.reset(theEdition);
		this->SetTasksIDList( emptyList );
		this->AddListeningToEdition();
	}

	this->UpdateTaskList();
}

//----------------------------------------------------------------------------------------
// HandleTaskInfoChange
//----------------------------------------------------------------------------------------
void
CZPTasksTVDataModel::HandleTaskInfoChange(
	const IStringData *			inTaskID )
{
	this->UpdateTaskList();
}

//----------------------------------------------------------------------------------------
// UpdateTaskList
//----------------------------------------------------------------------------------------
void
CZPTasksTVDataModel::UpdateTaskList()
{
	bool toReturn = true;
	InterfacePtr<IZPTaskList> edtionTaskList( mEdition, UseDefaultIID() );
	
	if ( edtionTaskList != nil )
	{
		this->SetTasksIDList( edtionTaskList->GetStringList() );
		toReturn = edtionTaskList->HasData();

		//If task id list comes empty, then it's observer will be notified when the list is available.
	}
	
	if( toReturn )
		this->BroadcastMessage( kZPUITaskTV_ModelChangedMsg );
}

//----------------------------------------------------------------------------------------
// SetTasksIDList
//----------------------------------------------------------------------------------------
void
CZPTasksTVDataModel::SetTasksIDList(
	const ZPTaskIDsList &		inTasksIDList)
{
	mTaskIDList.clear();
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	//Filter the task ID list according to filtering criteria.
	ZPTaskIDsList::const_iterator taskIDIter = inTasksIDList.begin();
	ZPTaskIDsList::const_iterator taskIDEndIter = inTasksIDList.end();

	InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<const IZPTaskMgr> taskMgr( zpApp->GetTaskMgr(), UseDefaultIID() );
	ASSERT(taskMgr);

	while( taskIDIter != taskIDEndIter )
	{
		const PMString & currTaskID = *taskIDIter;
		
		const IStringData * taskID = taskMgr->GetTaskByID( currTaskID );

		if(taskID && ( mFilteringFn == nil || mFilteringFn(taskID, mFilteringFnData) ) )
			mTaskIDList.push_back( currTaskID );

		//Listen to this task for changes. We need to change model when any value changes.
		//this->AddListeningToObject( taskID, IZPTaskInfo::kDefaultIID );

		++taskIDIter;
	}
}
#pragma mark -
//----------------------------------------------------------------------------------------
// GetTaskFilteringFunction
//----------------------------------------------------------------------------------------
const FilterTaskFnPtr
CZPTasksTVDataModel::GetTaskFilteringFunction()const
{
	return mFilteringFn;
}

//----------------------------------------------------------------------------------------
// SetTaskFilteringFunction
//----------------------------------------------------------------------------------------
void
CZPTasksTVDataModel::SetTaskFilteringFunction(
	FilterTaskFnPtr				inFunc,
	void *						inFilterFnData)
{
	mFilteringFn = inFunc;
	mFilteringFnData = inFilterFnData;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// RemoveListeningEdition
//----------------------------------------------------------------------------------------
void
CZPTasksTVDataModel::RemoveListeningEdition()
{
	if( !mEdition )
		return;

	this->RemoveListeningObject( mEdition.get(), IZPTaskList::kDefaultIID );
}

//----------------------------------------------------------------------------------------
// AddListeningToEdition
//----------------------------------------------------------------------------------------
void
CZPTasksTVDataModel::AddListeningToEdition()
{
	if( !mEdition )
		return;

	this->AddListeningToObject( mEdition.get(), IZPTaskList::kDefaultIID );
}

//----------------------------------------------------------------------------------------
// RemoveListeningObject
//----------------------------------------------------------------------------------------
void
CZPTasksTVDataModel::RemoveListeningObject(
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
CZPTasksTVDataModel::AddListeningToObject(
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

//----------------------------------------------------------------------------------------
// BroadcastMessage
//----------------------------------------------------------------------------------------
void
CZPTasksTVDataModel::BroadcastMessage(
	const ClassID&						inTheChange,
	void *								inChangedBy) const
{
	InterfacePtr<ISubject> selfSubject(this, UseDefaultIID());
	ASSERT(selfSubject);
	
	selfSubject->Change( inTheChange, IZPTasksTVDataModel::kDefaultIID );
}