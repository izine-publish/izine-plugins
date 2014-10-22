//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/TasksListTV/CZPTasksTVObserver.cpp $
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
#include "ISubject.h"
#include "IStringData.h"
#include "ITreeViewController.h"
#include "ITreeViewMgr.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes
#include "CObserver.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPUIID.h"
#include "IZPID.h"

//IZP Interfaces
#include "IZPApp.h"
#include "IZPTasksTVDataModel.h"
#include "IZPTaskList.h"
#include "IZPTaskMgr.h"
#include "IZPUITVNodeData.h"
#include "IZPUIDialogFacade.h"

//IZP General includes
#include "ZPTasksTVNodeID.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_TasksTV
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CObserver
class CZPTasksTVObserver : public inheritClass
{
public:
						CZPTasksTVObserver(IPMUnknown * boss);
	virtual				~CZPTasksTVObserver();

	void				AutoAttach();
	void				AutoDetach();
	void				Update(
							const ClassID&				theChange,
							ISubject*					theSubject,
							const PMIID &				protocol,
							void*						changedBy);
protected:
	/**	Handles a change in the selection when the end user 
	 either clicks on a node or clicks off it
	 */
	void				HandleSelectionChanged();
	
	void				HandleEditionTaskListWillChange();
	void				HandleEditionTaskListChange();

	void				HandleTaskInfoChange(
							void*						inTaskSubject );
	
	void				HandleTaskRowDoubleClick(
							ISubject*					inTaskNodeSubject ,		//Node
							void*						changedBy );			//Event

	const PMString &	GetTaskIDFromNode(
							ISubject *					inTaskNodeSubject  ) const;

	/**	Populates the tree model.
	 */
	void				initTree();

	void				RefreshTree();
private:
};


CREATE_PMINTERFACE(CZPTasksTVObserver, kZPTasksTVObserverImpl)

CZPTasksTVObserver::CZPTasksTVObserver(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPTasksTVObserver::~CZPTasksTVObserver()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// AutoAttach
//----------------------------------------------------------------------------------------
void
CZPTasksTVObserver::AutoAttach()
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	// Show/refresh tree when widget made visible
	initTree();
	
	InterfacePtr<ISubject> selfSubject(this, UseDefaultIID());
	
	selfSubject->AttachObserver(this, ITreeViewController::kDefaultIID);

	if( selfSubject->IsAttached( this, IZPTasksTVDataModel::kDefaultIID ) == false )	//Don't Detach it.
		selfSubject->AttachObserver( this, IZPTasksTVDataModel::kDefaultIID );

	InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<const IZPTaskMgr> taskMgr( zpApp->GetTaskMgr(), UseDefaultIID() );
	ASSERT(taskMgr);

	InterfacePtr<ISubject> dataMgrSubject( taskMgr, UseDefaultIID() );
	ASSERT(dataMgrSubject);

	if(!dataMgrSubject->IsAttached(this, IZPTaskMgr::kDefaultIID))	//For TaskInfo change message.
		dataMgrSubject->AttachObserver(this, IZPTaskMgr::kDefaultIID);

}

//----------------------------------------------------------------------------------------
// AutoDetach
//----------------------------------------------------------------------------------------
void
CZPTasksTVObserver::AutoDetach()
{
	InterfacePtr<ISubject> subj(this, UseDefaultIID());
	
	subj->DetachObserver(this, ITreeViewController::kDefaultIID);
}

//----------------------------------------------------------------------------------------
// Update
//----------------------------------------------------------------------------------------
void
CZPTasksTVObserver::Update(
	const ClassID&				theChange,
	ISubject*					theSubject,
	const PMIID &				protocol,
	void*						changedBy)
{
	LogFunctionEnterExit;
	
	switch(theChange.Get())
	{
		case kListSelectionChangedMessage:
		{
			this->HandleSelectionChanged();
			break;
		}
		case kZPUITVHeaderColPositionsChangedMsg:	//Not coming,as we are not listening to header control
		case kZPUITaskTV_ModelChangedMsg:
			this->RefreshTree();
			break;
		case kZPTaskListWillChangeMsg:
			if( protocol == IZPTaskList::kDefaultIID )
				this->HandleEditionTaskListWillChange();
			break;
		case kZPTaskListChangedMsg:	//From edition
		{
			if( protocol == IZPTaskList::kDefaultIID )
				this->HandleEditionTaskListChange();
			break;
		}
		case kZPTaskInfoChangedMsg:	//We are listening to the task manager, so task changed comes in chagned by.
			this->HandleTaskInfoChange( changedBy );
			break;
		case kZPUITaskTV_TaskNodeDoubleClickPressedMsg:
			this->HandleTaskRowDoubleClick( theSubject, changedBy );
			break;
	}
}

//----------------------------------------------------------------------------------------
// initTree
//----------------------------------------------------------------------------------------
void
CZPTasksTVObserver::initTree()
{
	LogFunctionEnterExit;
	
	do
	{
		//Patch: Bug 1706 : Auto refreshment of treeview on reselecting tab
		InterfacePtr<ITreeViewMgr> selfMgr(this, UseDefaultIID());
		ASSERT( selfMgr );

		//Reload
		selfMgr->ChangeRoot( kTrue );
		
		//Patch: Reselect the row if selected
		InterfacePtr<ITreeViewController> tvCont(selfMgr, UseDefaultIID());
		ASSERT (tvCont);
		if (tvCont->AllowsSelection())
		{
			NodeIDList nodeList;
			tvCont->GetSelectedItems(nodeList);
			size_t nodeCount = nodeList.size();
			for(size_t i = 0; i < nodeCount ; ++i)
			{
				const NodeID & currNodeID = nodeList[i];
				tvCont->Deselect(currNodeID, kFalse);
				tvCont->Select(currNodeID, kFalse);
			}
		}

	} while(kFalse);
}

//----------------------------------------------------------------------------------------
// RefreshTree
//----------------------------------------------------------------------------------------
void
CZPTasksTVObserver::RefreshTree()
{
	InterfacePtr<ITreeViewMgr> selfMgr(this, UseDefaultIID());
	ASSERT( selfMgr );

	//Save selection

	//Reload
	selfMgr->ChangeRoot( kTrue );

	//Restore selection

	//Send message so that selection observers handle it even if selected row index don't change.
	this->HandleSelectionChanged();

	//Patch: Reselect the row if selected
	InterfacePtr<ITreeViewController> tvCont(selfMgr, UseDefaultIID());
	ASSERT (tvCont);
	if (tvCont->AllowsSelection())
	{
		NodeIDList nodeList;
		tvCont->GetSelectedItems(nodeList);
		size_t nodeCount = nodeList.size();
		for(size_t i = 0; i < nodeCount ; ++i)
		{
			const NodeID & currNodeID = nodeList[i];
			tvCont->Deselect(currNodeID, kFalse);
			tvCont->Select(currNodeID, kFalse);
		}
	}
}

//----------------------------------------------------------------------------------------
// HandleSelectionChanged
//----------------------------------------------------------------------------------------
void
CZPTasksTVObserver::HandleSelectionChanged()
{
	LogFunctionEnterExit;
	
	do
	{
		InterfacePtr<ITreeViewController> controller(this, UseDefaultIID());
		ASSERT(controller);
		
		NodeIDList selectedItems;
		controller->GetSelectedItems(selectedItems);
		
		InterfacePtr<IControlView> selectedNodeView;
		if(selectedItems.size() == 1)	// We've got single selection only
		{
			const NodeID & firstSelectedNodeID = selectedItems.at(0);

			ZPTreeNodePtr<ZPTasksTVNodeID> nodeID( firstSelectedNodeID );
			ASSERT(nodeID);
			
			InterfacePtr<ITreeViewMgr> selfTreeMgr( this, UseDefaultIID() );
			ASSERT( selfTreeMgr );
			selectedNodeView.reset( selfTreeMgr->QueryWidgetFromNode( firstSelectedNodeID ) );
		}

		InterfacePtr<ISubject> selfSubject(this, UseDefaultIID());
		ASSERT(selfSubject);

		selfSubject->Change( kZPUITasksTV_SelectionChangedMsg, ITreeViewController::kDefaultIID, selectedNodeView.get() );
	} while(kFalse);
}

//----------------------------------------------------------------------------------------
// HandleEditionTaskListWillChange
// Stop updating the Tree till list is changed.
//----------------------------------------------------------------------------------------
void
CZPTasksTVObserver::HandleEditionTaskListWillChange()
{
	InterfacePtr<IZPTasksTVDataModel> selfModel( this, UseDefaultIID() );
	ASSERT(selfModel);

	//TODO:
	//selfModel->SetSkipTaskListUpdates( true );
}

//----------------------------------------------------------------------------------------
// HandleEditionTaskListChange
//----------------------------------------------------------------------------------------
void
CZPTasksTVObserver::HandleEditionTaskListChange()
{
	InterfacePtr<IZPTasksTVDataModel> selfModel( this, UseDefaultIID() );
	ASSERT(selfModel);

	//TODO:
	//selfModel->SetSkipTaskListUpdates( false );
	selfModel->UpdateTaskList();

}

//----------------------------------------------------------------------------------------
// HandleTaskInfoChange
//----------------------------------------------------------------------------------------
void
CZPTasksTVObserver::HandleTaskInfoChange(
	void*					inTaskID )
{
	InterfacePtr<IZPTasksTVDataModel> selfModel( this, UseDefaultIID() );
	ASSERT(selfModel);

	InterfacePtr<const IStringData> dbTaskID( (IPMUnknown*)inTaskID, IID_IZPDATAID );
	ASSERT( dbTaskID );
	selfModel->HandleTaskInfoChange( dbTaskID );
}

//----------------------------------------------------------------------------------------
// HandleTaskRowDoubleClick
//----------------------------------------------------------------------------------------
void
CZPTasksTVObserver::HandleTaskRowDoubleClick(
	ISubject*					inTaskNodeSubject,	//Node
	void*						changedBy)			//Event
{
	do
	{
		InterfacePtr<const IZPUITVNodeData> taskNodeData( inTaskNodeSubject, UseDefaultIID() );
		ASSERT( taskNodeData );
		
		InterfacePtr<const IStringData> taskID( taskNodeData->GetDataObj(), IID_IZPDATAID );
		ASSERT( taskID );
		const PMString & theTaskIDStr = taskID->Get();

		
		//Someday: Opening of task should be with task panel observer.
		//Open task in Update task window.
		InterfacePtr<IZPTasksTVDataModel> selfModel( this, UseDefaultIID() );
		ASSERT(selfModel);
		
		//Task list may change before ShowCreateTaskDialog returns, which causes some asserts.
		//So schedule cmd for this action, if dialog is shown as modal.
		Utils<IZPUIDialogFacade>()->ShowCreateTaskDialog( taskID, selfModel->GetEditionID() );

	} while(kFalse);
}

//----------------------------------------------------------------------------------------
// GetTaskIDFromNode
//----------------------------------------------------------------------------------------
const PMString &
CZPTasksTVObserver::GetTaskIDFromNode(
	ISubject *					inTaskNodeSubject ) const
{
	InterfacePtr<const IZPUITVNodeData> taskNodeData( inTaskNodeSubject, UseDefaultIID() );
	ASSERT( taskNodeData );
	
	InterfacePtr<const IStringData> taskID( taskNodeData->GetDataObj(), IID_IZPDATAID );
	ASSERT( taskID );
	if( taskID )
		return taskID->Get();
	else
		return kNullString;
}

