//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Panels/Tasks/CZPTasksPanelObserver.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 23-9-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IBoolData.h"
#include "IBooleanControlData.h"
#include "IControlView.h"
#include "IDropDownListController.h"
#include "IPanelControlData.h"
#include "IStringData.h"
#include "IStringListControlData.h"
#include "ISubject.h"
#include "ITextControlData.h"
#include "ITreeViewController.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes
#include "CWidgetObserver.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPUIID.h"
#include "IZPID.h"

//IZP Interfaces
#include "IZPAppController.h"
#include "IZPDBObjController.h"
#include "IZPDefaultSession.h"
#include "IZPLoginController.h"
#include "IZPTaskInfo.h"
#include "IZPTaskList.h"
#include "IZPTasksPanelHelper.h"
#include "IZPTasksTVDataModel.h"
#include "IZPUIHistoryPanelController.h"
#include "IZPUITVNodeData.h"
#include "IZPUserCredentials.h"
#include "IZPWSResponseHandler.h"

//IZP General includes
#include "AZPDBObjPopupController.h"
#include "ZPDataHelper.h"
#include "ZPTreeNodePtr.h"
#include "ZPTasksTVNodeID.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_PanelTasks
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

struct TaskFilterCriteria
{
	PMString mDocumentID;
	PMString mStatusID;
	PMString mAssignedToUserID;
	PMString mCategoryID;
	PMString mSpread;
	PMString mPage;
};

#define inheritClass CWidgetObserver
class CZPTasksPanelObserver : public inheritClass
{
public:
						CZPTasksPanelObserver(IPMUnknown * boss);
	virtual				~CZPTasksPanelObserver();

	void				AutoAttach();
	void				AutoDetach();
	void				Update(
							const ClassID&				theChange,
							ISubject*					theSubject,
							const PMIID &				protocol,
							void*						changedBy);
protected:
	void				DoAttachDetach(
							const WidgetID&				widgetID,
							const PMIID&				pmiid,
							bool16						attaching);

	void				HandlePopupListChange(
							const ClassID&				theChange,
							ISubject*					theSubject,
							const PMIID &				protocol,
							void*						changedBy );
	void				HandlePopupSelectionChange(
							const ClassID&				theChange,
							ISubject*					theSubject,
							const PMIID &				protocol,
							void*						changedBy );
	void				HandleTreeSelectionChange(
							const ClassID&				theChange,
							ISubject*					theSubject,
							const PMIID &				protocol,
							void*						changedBy );

	PMString			GetSelectedTitleID() const;
	PMString			GetSelectedEditionID() const;
	
	void				EnableDisableControls(
							bool						inEnable );

	void				HandleUserLoginDone();

	IControlView *		GetWidgetView(
							const WidgetID &			inWidgetID ) const;
	PMString			GetEditBoxText(
							const WidgetID&				inWidgetID ) const;
	
	void				RefreshEditionPopup();
	void				RefreshAssignedToDocPopup();
	void				RefreshAssignedToUserPopup();
	void				RefreshTasksTreeView();

	void				DoPanelRefresh();
	void				DoHistory();

	void				Initialize();

	static bool			FilterTask(
							const IStringData *			inTaskID,
							void *						inFilterFnData );
	void				UpdateFilteringCriteria();
private:

	bool			mCheckLoginState;
	TaskFilterCriteria	mFilterData;
};


CREATE_PMINTERFACE(CZPTasksPanelObserver, kZPTasksPanelObserverImpl)

CZPTasksPanelObserver::CZPTasksPanelObserver(IPMUnknown * boss)
: inheritClass(boss)
, mCheckLoginState( true )
{
	this->Initialize();
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPTasksPanelObserver::~CZPTasksPanelObserver()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// AutoAttach
//----------------------------------------------------------------------------------------
void
CZPTasksPanelObserver::AutoAttach()
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	do
	{
		ASSERT( gZPApp );
		InterfacePtr<ISubject> loginControllerSubject( gZPApp, UseDefaultIID() );
		ASSERT(loginControllerSubject);
		if(!loginControllerSubject->IsAttached(this, IZPLoginController::kDefaultIID))
		{
			loginControllerSubject->AttachObserver(this, IZPLoginController::kDefaultIID);
		}

		this->DoAttachDetach(kZPUITasksPanelRefreshButtonWidgetID, IBooleanControlData::kDefaultIID, kTrue);
		this->DoAttachDetach( kZPUITasksPanelBtnHistoryWidgetID, IBooleanControlData::kDefaultIID, kTrue);
		this->DoAttachDetach( kZPUITasksPanelTitleDropDownWidgetID, IStringListControlData::kDefaultIID, kTrue);
		this->DoAttachDetach( kZPUITasksPanelEditionDropDownWidgetID, IStringListControlData::kDefaultIID, kTrue);
		this->DoAttachDetach( kZPUITasksPanelPopAsgnToUserWidgetID, IStringListControlData::kDefaultIID, kTrue);
		this->DoAttachDetach( kZPUITasksPanelPopAsgnToDocWidgetID, IStringListControlData::kDefaultIID, kTrue);
		this->DoAttachDetach( kZPUITasksPanelPopCategoryWidgetID, IStringListControlData::kDefaultIID, kTrue);
		this->DoAttachDetach( kZPUITasksPanelPopStatusWidgetID, IStringListControlData::kDefaultIID, kTrue);
		this->DoAttachDetach( kZPUITasksPanelLstTasksWidgetID, ITreeViewController::kDefaultIID, kTrue);
		
		this->DoAttachDetach( kZPUITasksPanelTxtSpreadWidgetID, ITextControlData::kDefaultIID, kTrue);
		this->DoAttachDetach( kZPUITasksPanelTxtPageWidgetID, ITextControlData::kDefaultIID, kTrue);

		if( mCheckLoginState )
		{
			InterfacePtr<const IZPDefaultSession> defaultSession( gSession, UseDefaultIID() );
			ASSERT( defaultSession );

			const IZPUserCredentials * defaultCredentials = defaultSession->GetUserCredentials();

			InterfacePtr<const IBoolData> isValidData( defaultCredentials, IID_IZPISVALID );
			
			if( isValidData && isValidData->Get())
				this->HandleUserLoginDone();
			mCheckLoginState = false;
		}
	}while(kFalse);
}

//----------------------------------------------------------------------------------------
// AutoDetach
//----------------------------------------------------------------------------------------
void
CZPTasksPanelObserver::AutoDetach()
{
	this->DoAttachDetach(kZPUITasksPanelRefreshButtonWidgetID, IBooleanControlData::kDefaultIID, kFalse);
	this->DoAttachDetach( kZPUITasksPanelBtnHistoryWidgetID, IBooleanControlData::kDefaultIID, kFalse);
	//Don't Detach these widgets, as they broadcast messages even when not visible in UI.
	//kZPUITasksPanelTitleDropDownWidgetID
	//kZPUITasksPanelEditionDropDownWidgetID
	//kZPUITasksPanelPopAsgnToUserWidgetID

	this->DoAttachDetach( kZPUITasksPanelLstTasksWidgetID, ITreeViewController::kDefaultIID, kFalse);
	this->DoAttachDetach( kZPUITasksPanelTxtSpreadWidgetID, ITextControlData::kDefaultIID, kFalse);
	this->DoAttachDetach( kZPUITasksPanelTxtPageWidgetID, ITextControlData::kDefaultIID, kFalse);
}

//----------------------------------------------------------------------------------------
// Update
//----------------------------------------------------------------------------------------
void
CZPTasksPanelObserver::Update(
	const ClassID&				theChange,
	ISubject*					theSubject,
	const PMIID &				protocol,
	void*						changedBy)
{
	LogFunctionEnterExit;
	
	do
	{
		if ( theChange == kZPLogin_LoggedInMsg && protocol == IZPLoginController::kDefaultIID )
		{
			this->HandleUserLoginDone();
		}
		else if( protocol == IZPLoginController::kDefaultIID && theChange != kZPLogin_CanLogoutMsg )
		{	//disable controls if state is not logged in, 
			this->EnableDisableControls( false );
		}
		else if ( theChange == kZPUITasksTV_SelectionChangedMsg )
		{
			this->HandleTreeSelectionChange( theChange, theSubject, protocol, changedBy );
		}
		else if ( theChange == kZPUIDBObjDropDown_ListLengthChangedMsg )
		{
			this->HandlePopupListChange( theChange, theSubject, protocol, changedBy );
		}
		else if ( theChange == kZPUIDBObjDropDown_SelectionChangedMsg )
		{
			this->HandlePopupSelectionChange( theChange, theSubject, protocol, changedBy );
		}
		else if (theChange == kTextChangeStateMessage)
		{
			InterfacePtr<IControlView> controlView(theSubject, UseDefaultIID());
			ASSERT(controlView);
			if(!controlView)
				break;
					
			// Get the button ID from the view.
			WidgetID widgetID = controlView->GetWidgetID();
			switch( widgetID.Get() )
			{
				case kZPUITasksPanelTxtPageWidgetID:
				case kZPUITasksPanelTxtSpreadWidgetID:
					this->RefreshTasksTreeView();
				break;
			}
		}
		else if (theChange == kTrueStateMessage)
		{
			InterfacePtr<IControlView> controlView(theSubject, UseDefaultIID());
			ASSERT(controlView);
			if(!controlView)
				break;
					
			// Get the button ID from the view.
			WidgetID widgetID = controlView->GetWidgetID();
			switch( widgetID.Get() )
			{
				case kZPUITasksPanelRefreshButtonWidgetID:
					this->DoPanelRefresh();
				break;
				case kZPUITasksPanelBtnHistoryWidgetID:
					this->DoHistory();
				break;
			}
		}
	}while(false);
}

//----------------------------------------------------------------------------------------
// DoAttachDetach
//----------------------------------------------------------------------------------------
void
CZPTasksPanelObserver::DoAttachDetach(
	const WidgetID&				widgetID,
	const PMIID&				pmiid,
	bool16						attaching)
{
	do
	{
		InterfacePtr<const IPanelControlData> selfPanelControlData(this, UseDefaultIID());
		ASSERT(selfPanelControlData);
		if (!selfPanelControlData)
		{
			break;
		}
		IControlView* widgetView = selfPanelControlData->FindWidget(widgetID);
		ASSERT(widgetView);
		if (!widgetView)
		{
			break;
		}
		InterfacePtr<ISubject> widgetSubject(widgetView, UseDefaultIID());
		ASSERT(widgetSubject);
		if (!widgetSubject)
		{
			break;
		}
		
		bool isAttached = widgetSubject->IsAttached(this, pmiid);
		if (attaching && !isAttached)
				widgetSubject->AttachObserver(this, pmiid);
		else if( !attaching && isAttached )
				widgetSubject->DetachObserver(this, pmiid);
	} while (kFalse);
}
#pragma mark -
//----------------------------------------------------------------------------------------
// Initialize
//----------------------------------------------------------------------------------------
void
CZPTasksPanelObserver::Initialize()
{
	LogFunctionEnterExit;
	//Clear the controls here.
	this->EnableDisableControls( false );	//Controls comes as enabled after app crash.
}

//----------------------------------------------------------------------------------------
// HandleUserLoginDone
//----------------------------------------------------------------------------------------
void
CZPTasksPanelObserver::HandleUserLoginDone()
{
	this->EnableDisableControls( true );
}

//----------------------------------------------------------------------------------------
// HandlePopupListChange
//----------------------------------------------------------------------------------------
void
CZPTasksPanelObserver::HandlePopupListChange(
	const ClassID&				theChange,
	ISubject*					theSubject,
	const PMIID &				protocol,
	void*						changedBy)
{
	LogFunctionEnterExit;
	InterfacePtr<IControlView> controlView(theSubject, UseDefaultIID());
	ASSERT(controlView);

	WidgetID theChangedWidgetID = controlView->GetWidgetID();
	if( theChangedWidgetID.Get() == kZPUITasksPanelTitleDropDownWidgetID )
	{	//Refresh the edition popup.
		//this->RefreshEditionPopup();	//Commented as It should be done on selection change only.
		//Refresh the Assigned to user popup
		//this->RefreshAssignedToUserPopup();	//Commented as It should be done on selection change only.
	}
	else if( theChangedWidgetID.Get() == kZPUITasksPanelEditionDropDownWidgetID )
	{	//Refresh the Tasks treeview
		//this->RefreshTasksTreeView();	//Commented as It should be done on selection change only.
	}
}

//----------------------------------------------------------------------------------------
// HandlePopupSelectionChange
//----------------------------------------------------------------------------------------
void
CZPTasksPanelObserver::HandlePopupSelectionChange(
	const ClassID&				theChange,
	ISubject*					theSubject,
	const PMIID &				protocol,
	void*						changedBy)
{
	LogFunctionEnterExit;

	InterfacePtr<IControlView> controlView(theSubject, UseDefaultIID());
	ASSERT(controlView);

	WidgetID theChangedWidgetID = controlView->GetWidgetID();
	if( theChangedWidgetID.Get() == kZPUITasksPanelTitleDropDownWidgetID )
	{
		this->RefreshEditionPopup();
		this->RefreshAssignedToUserPopup();
		//TODO: we should disable tree view till editions popup is not refreshed.
		// as the tree view might be showing data of some other title.
	}
	else if( theChangedWidgetID.Get() == kZPUITasksPanelEditionDropDownWidgetID
		|| theChangedWidgetID.Get() == kZPUITasksPanelPopAsgnToUserWidgetID
		|| theChangedWidgetID.Get() == kZPUITasksPanelPopAsgnToDocWidgetID
		|| theChangedWidgetID.Get() == kZPUITasksPanelPopCategoryWidgetID
		|| theChangedWidgetID.Get() == kZPUITasksPanelPopStatusWidgetID )
	{	
		if( theChangedWidgetID.Get() == kZPUITasksPanelEditionDropDownWidgetID )
			this->RefreshAssignedToDocPopup();
		//Refresh the Tasks treeview
		this->RefreshTasksTreeView();
	}
}

//----------------------------------------------------------------------------------------
// HandleTreeSelectionChange
//----------------------------------------------------------------------------------------
void
CZPTasksPanelObserver::HandleTreeSelectionChange(
	const ClassID&				theChange,
	ISubject*					theSubject,
	const PMIID &				protocol,
	void*						changedBy)
{
	//Update the task description
	
	InterfacePtr<IControlView> historyBtnControlView( this->GetWidgetView(kZPUITasksPanelBtnHistoryWidgetID), UseDefaultIID());
	ASSERT(historyBtnControlView);
	do
	{
		IControlView * nodeControlView = reinterpret_cast<IControlView*>(changedBy);

		if( !nodeControlView )
			break;

		InterfacePtr<const IZPUITVNodeData> nodeData( nodeControlView, UseDefaultIID());
		ASSERT(nodeData);
		const IStringData * taskID = static_cast<const IStringData *>(nodeData->GetDataObj());
		
		if( !taskID )
			break;

		InterfacePtr<const IZPTaskInfo> taskInfo( taskID, UseDefaultIID());
		ASSERT(taskInfo);

		//Update the task description
		this->SetTextControlData( kZPUITasksPanelTxtDescriptionWidgetID, taskInfo->GetDescription() );

		//Enable History btn
		historyBtnControlView->Enable();

		return;
	}while(kFalse);

	//Clear the task description
	this->SetTextControlData( kZPUITasksPanelTxtDescriptionWidgetID, kNullString );

	//Disable History btn
	historyBtnControlView->Disable();
}
#pragma mark -
//----------------------------------------------------------------------------------------
// DoPanelRefresh
//----------------------------------------------------------------------------------------
void
CZPTasksPanelObserver::DoPanelRefresh()
{
	//Refresh only the tasks list for now.
	//Someday: refresh the edition, title & user list as well.
	const PMString theEditionID = this->GetSelectedEditionID();

	if( theEditionID.IsEmpty() )
		return;	//Should we clear the tree here?
	const IStringData * editionID = ZPDataHelper::GetEditionByID( theEditionID );

	if( !editionID )
		return;	//Should we clear the tree here?
	InterfacePtr<IZPDBObjController> editionController( editionID, UseDefaultIID() );
	ASSERT( editionController );
	//Fetch Task List here.
	editionController->RequestUpdateFromServer( IZPTaskList::kDefaultIID );
}

//----------------------------------------------------------------------------------------
// RefreshEditionPopup
//----------------------------------------------------------------------------------------
void
CZPTasksPanelObserver::RefreshEditionPopup()
{
	LogFunctionEnterExit;

	const PMString theTitleID = this->GetSelectedTitleID();
	//if( theTitleID.empty() == kFalse )
	{
		InterfacePtr<IStringData> editionTitleID( this->GetWidgetView(kZPUITasksPanelEditionDropDownWidgetID), IID_IZPTITLEIDSTRINGDATA );
		ASSERT(editionTitleID);
		editionTitleID->Set( theTitleID );
		
		InterfacePtr<AZPDBObjPopupController> editionController( editionTitleID, IZPDBObjPopupController::kDefaultIID );
		ASSERT( editionController );

		editionController->Refresh();
	}
}

//----------------------------------------------------------------------------------------
// RefreshAssignedToDocPopup
//----------------------------------------------------------------------------------------
void
CZPTasksPanelObserver::RefreshAssignedToDocPopup()
{
	LogFunctionEnterExit;

	const PMString theEditionID = this->GetSelectedEditionID();
	
	IControlView * assetsPopupControlView = this->GetWidgetView( kZPUITasksPanelPopAsgnToDocWidgetID );
	ASSERT( assetsPopupControlView );

	InterfacePtr<IStringData> assetsPopEditionID( assetsPopupControlView, IID_IZPEDITIONIDSTRINGDATA );
	ASSERT( assetsPopEditionID );
	assetsPopEditionID->Set( theEditionID );

	InterfacePtr<AZPDBObjPopupController> assetsPopController( assetsPopupControlView, IZPDBObjPopupController::kDefaultIID );
	ASSERT( assetsPopController );

	assetsPopController->Refresh();
}

//----------------------------------------------------------------------------------------
// RefreshAssignedToUserPopup
//----------------------------------------------------------------------------------------
void
CZPTasksPanelObserver::RefreshAssignedToUserPopup()
{
	LogFunctionEnterExit;

	const PMString theTitleID = this->GetSelectedTitleID();
	//if( theTitleID.empty() == kFalse )
	{
		InterfacePtr<IStringData> userTitleID( this->GetWidgetView(kZPUITasksPanelPopAsgnToUserWidgetID), IID_IZPTITLEIDSTRINGDATA );
		ASSERT(userTitleID);
		userTitleID->Set( theTitleID );
		
		InterfacePtr<AZPDBObjPopupController> userController( userTitleID, IZPDBObjPopupController::kDefaultIID );
		ASSERT( userController );

		userController->Refresh();
	}
}

//----------------------------------------------------------------------------------------
// RefreshTasksTreeView
//----------------------------------------------------------------------------------------
void
CZPTasksPanelObserver::RefreshTasksTreeView()
{
	LogFunctionEnterExit;

	const PMString theEditionID = this->GetSelectedEditionID();
	//TODO: Get other filtering criteria values.
	
	IControlView * treeControlView = this->GetWidgetView( kZPUITasksPanelLstTasksWidgetID );
	InterfacePtr<IZPTasksTVDataModel> treeViewModel( treeControlView, UseDefaultIID() );
	ASSERT( treeViewModel );

	//Disable filtering. Redmine#1764, all filtering controls are made hidden.
	//if( treeViewModel->GetTaskFilteringFunction() == nil )
	//	treeViewModel->SetTaskFilteringFunction( FilterTask, this );

	this->UpdateFilteringCriteria();
	treeViewModel->UpdateData( theEditionID );
}

//----------------------------------------------------------------------------------------
// DoHistory
//----------------------------------------------------------------------------------------
void
CZPTasksPanelObserver::DoHistory()
{
	IControlView * treeControlView = this->GetWidgetView( kZPUITasksPanelLstTasksWidgetID );
	ASSERT( treeControlView );
	InterfacePtr<const ITreeViewController> tvController( treeControlView, UseDefaultIID());
	ASSERT( tvController );
	NodeIDList selectedNodes;
	tvController->GetSelectedItems( selectedNodes );
	ASSERT( selectedNodes.Length() == 1 );
	ZPTreeNodePtr<ZPTasksTVNodeID> nodeID(selectedNodes[0]);
	ASSERT(nodeID != nil);
	if (nodeID != nil)
	{
		const PMString & theTaskID = nodeID->GetID();//("55a7be8e-8c82-4a07-a467-f54497b50c10");

		IZPUIHistoryPanelController * historyController = IZPUIHistoryPanelController::GetHistoryController();
		ASSERT( historyController );

		historyController->ShowHistoryForTask( theTaskID );
	}
}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetSelectedTitleID
//----------------------------------------------------------------------------------------
PMString
CZPTasksPanelObserver::GetSelectedTitleID()const
{
	InterfacePtr<IZPTasksPanelHelper> taskPanelHelper(this, UseDefaultIID());
	return taskPanelHelper->DBObjPopGetSelectedID( kZPUITasksPanelTitleDropDownWidgetID );
}

//----------------------------------------------------------------------------------------
// GetSelectedEditionID
//----------------------------------------------------------------------------------------
PMString
CZPTasksPanelObserver::GetSelectedEditionID()const
{
	InterfacePtr<IZPTasksPanelHelper> taskPanelHelper(this, UseDefaultIID());
	return taskPanelHelper->DBObjPopGetSelectedID( kZPUITasksPanelEditionDropDownWidgetID );
}

//----------------------------------------------------------------------------------------
// EnableDisableControls
//----------------------------------------------------------------------------------------
void
CZPTasksPanelObserver::EnableDisableControls(
	bool						inEnable)
{
	LogFunctionEnterExit;

	InterfacePtr<IControlView> selfControlView( this, UseDefaultIID());
	ASSERT(selfControlView);

	if( inEnable )
		selfControlView->Enable();
	else
		selfControlView->Disable();
}

//----------------------------------------------------------------------------------------
// GetWidgetView
//----------------------------------------------------------------------------------------
IControlView *
CZPTasksPanelObserver::GetWidgetView(
	const WidgetID &			inWidgetID) const
{
	InterfacePtr<const IPanelControlData> selfPanelControlData(this, UseDefaultIID());
	ASSERT(selfPanelControlData);
	if (selfPanelControlData)
	{
		IControlView* widgetView = selfPanelControlData->FindWidget( inWidgetID );
		ASSERT(widgetView);
		return widgetView;
	}
	return NULL;
}

//----------------------------------------------------------------------------------------
// GetEditBoxText
//----------------------------------------------------------------------------------------
PMString
CZPTasksPanelObserver::GetEditBoxText(
	const WidgetID& inWidgetID ) const
{
	InterfacePtr<const ITextControlData> textControlData( this->GetWidgetView(inWidgetID), UseDefaultIID());
	ASSERT(textControlData);
	
	return (textControlData->GetString());
}

#pragma mark -
//----------------------------------------------------------------------------------------
// FilterTask
//----------------------------------------------------------------------------------------
bool
CZPTasksPanelObserver::FilterTask(
	const IStringData *			inTaskID,
	void *						inFilterFnData)
{
	do
	{
		if( !inTaskID )
			break;
		
		CZPTasksPanelObserver * thisPtr = reinterpret_cast<CZPTasksPanelObserver*>( inFilterFnData );
		const TaskFilterCriteria & criteria = thisPtr->mFilterData;

		InterfacePtr<const IZPTaskInfo> dbTaskInfo( inTaskID, UseDefaultIID() );
		ASSERT( dbTaskInfo );

		if( !dbTaskInfo )
			break;

		bool toReturn = true;
		//Task Status
		if( criteria.mStatusID.IsEmpty() == kFalse && criteria.mStatusID != kIZPPopupItemAnyKey && criteria.mStatusID != dbTaskInfo->GetStatusID() )
			break;

		//Task assigned to User
		if( criteria.mAssignedToUserID == kIZPPopupItemAnyKey ) {
			if( dbTaskInfo->GetAssignedToUserID().IsEmpty() )
				break;
		}
		else if( criteria.mAssignedToUserID == kIZPPopupItemNoneKey ) {
			if ( dbTaskInfo->GetAssignedToUserID().IsEmpty() == kFalse )
			break;
		}
		else if( criteria.mAssignedToUserID.IsEmpty() == kFalse && criteria.mAssignedToUserID != dbTaskInfo->GetAssignedToUserID() )
			break;

		//Task assigned document
		if( criteria.mDocumentID == kIZPPopupItemAnyKey ) {
			if( dbTaskInfo->GetDocumentID().IsEmpty() )
				break;
		}
		else if( criteria.mDocumentID == kIZPPopupItemNoneKey ) {
			if ( dbTaskInfo->GetDocumentID().IsEmpty() == kFalse )
			break;
		}
		else if( criteria.mDocumentID.IsEmpty() == kFalse && criteria.mDocumentID != dbTaskInfo->GetDocumentID() )
			break;

		//Task Category
		//Task assigned to User
		if( criteria.mCategoryID == kIZPPopupItemAnyKey ) {
			if( dbTaskInfo->GetCategoryID().IsEmpty() )
				break;
		}
		else if( criteria.mCategoryID == kIZPPopupItemNoneKey ) {
			if ( dbTaskInfo->GetCategoryID().IsEmpty() == kFalse )
			break;
		}
		else if( criteria.mCategoryID.IsEmpty() == kFalse && criteria.mCategoryID != dbTaskInfo->GetCategoryID() )
			break;


		//Task spread
		if( criteria.mSpread.IsEmpty() == kFalse && criteria.mSpread != dbTaskInfo->GetSpread() )
			break;

		//Task page
		if( criteria.mPage.IsEmpty() == kFalse && criteria.mPage != dbTaskInfo->GetPage() )
			break;
		return true;
	}while(kFalse);

	return false;
}

//----------------------------------------------------------------------------------------
// UpdateFilteringCriteria
//----------------------------------------------------------------------------------------
void
CZPTasksPanelObserver::UpdateFilteringCriteria()
{
	LogFunctionEnterExit;

	InterfacePtr<IZPTasksPanelHelper> taskPanelHelper(this, UseDefaultIID());
	mFilterData.mDocumentID			= taskPanelHelper->DBObjPopGetSelectedID( kZPUITasksPanelPopAsgnToDocWidgetID, true );
	mFilterData.mStatusID			= taskPanelHelper->DBObjPopGetSelectedID( kZPUITasksPanelPopStatusWidgetID, true );
	mFilterData.mAssignedToUserID	= taskPanelHelper->DBObjPopGetSelectedID( kZPUITasksPanelPopAsgnToUserWidgetID, true );
	mFilterData.mCategoryID			= taskPanelHelper->DBObjPopGetSelectedID( kZPUITasksPanelPopCategoryWidgetID, true );
	mFilterData.mSpread				= this->GetEditBoxText( kZPUITasksPanelTxtSpreadWidgetID );
	mFilterData.mPage				= this->GetEditBoxText( kZPUITasksPanelTxtPageWidgetID );

	if( mFilterData.mDocumentID == kIZPPopupItemAllKey )
		mFilterData.mDocumentID = kNullString;

	if( mFilterData.mAssignedToUserID == kIZPPopupItemAllKey )
		mFilterData.mAssignedToUserID = kNullString;
	else if( mFilterData.mAssignedToUserID == kIZPPopupItemSelfKey )
	{	//Set the criteria to logged in user ID.
		mFilterData.mAssignedToUserID = ZPDataHelper::GetCurrUserID();
	}

	if( mFilterData.mCategoryID == kIZPPopupItemAllKey )
		mFilterData.mCategoryID = kNullString;

	mFilterData.mSpread.StripWhiteSpace( PMString::kLeadingAndTrailingWhiteSpace );
	mFilterData.mPage.StripWhiteSpace( PMString::kLeadingAndTrailingWhiteSpace );
}
