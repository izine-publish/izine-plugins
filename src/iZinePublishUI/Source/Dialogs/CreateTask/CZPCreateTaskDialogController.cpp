//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Dialogs/CreateTask/CZPCreateTaskDialogController.cpp $
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
#include "IDropDownListController.h"
#include "IStringData.h"
#include "IPanelControlData.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes
#include "CDialogController.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPUIID.h"
#include "IZPID.h"

//IZP Interfaces
#include "IZPApp.h"
#include "IZPTaskInfo.h"
#include "IZPTitleMgr.h"
#include "IZPWSUtils.h"

//IZP General includes
#include "AZPDBObjPopupController.h"
#include "ZPDataHelper.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_Dialogs
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CDialogController
class CZPCreateTaskDialogController : public inheritClass
{
public:
						CZPCreateTaskDialogController(IPMUnknown * boss);
	virtual				~CZPCreateTaskDialogController();

	/** Initialize each widget in the dialog with its default value.
	 Called when the dialog is opened.
	 */
	void				InitializeDialogFields(IActiveContext* dlgContext);
	
	/** Validate the values in the widgets.
	 By default, the widget with ID kOKButtonWidgetID causes
	 ValidateFields to be called. When all widgets are valid,
	 ApplyFields will be called.
	 @return kDefaultWidgetId if all widget values are valid, WidgetID of the widget to select otherwise.
	 
	 */
	WidgetID			ValidateDialogFields(IActiveContext* myContext);
	
	/** Retrieve the values from the widgets and act on them.
	 @param widgetId identifies the widget on which to act.
	 */
	void				ApplyDialogFields(IActiveContext* myContext, const WidgetID& widgetId);
protected:
	void				InitDialog( 
							const IStringData *			inTaskSubject,
							const IZPTaskInfo *			inTaskInfo );

	void				DBObjPopSetSelectedID(
							WidgetID					inWidgetID,
							const PMString &			inSelectedID );
	void				DBObjPopRefresh(
							WidgetID					inWidgetID );
	PMString			DBObjPopGetSelectedID(
							WidgetID					inWidgetID,
							bool						inEmptyIfNotCurrentlySelected = false );

	IControlView *		GetWidgetView(
							const WidgetID &			inWidgetID ) const;
private:
	InterfacePtr<const IStringData>		mTaskToUpdate;	//cached task id to update, if not nil
};


CREATE_PMINTERFACE(CZPCreateTaskDialogController, kZPCreateTaskDialogControllerImpl)

CZPCreateTaskDialogController::CZPCreateTaskDialogController(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPCreateTaskDialogController::~CZPCreateTaskDialogController()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// InitializeDialogFields
//----------------------------------------------------------------------------------------
void
CZPCreateTaskDialogController::InitializeDialogFields(
	IActiveContext* dlgContext)
{
	LogFunctionEnterExit;

	inheritClass::InitializeDialogFields(dlgContext);

	InterfacePtr<const IStringData> selfTaskID( this, IID_IZPDATAID );
	ASSERT( selfTaskID );
	
	//get task object from cache.
	const IStringData * dbTaskID = ZPDataHelper::GetTaskByID( selfTaskID->Get() );

	if( dbTaskID )
		dbTaskID->AddRef();
	mTaskToUpdate.reset( dbTaskID );

	//TODO: better take this from dlg itself, so that duplicating task will be easy.
	//InterfacePtr<const IZPTaskInfo> dbTaskInfo( dbTaskID, UseDefaultIID() );
	InterfacePtr<const IZPTaskInfo> selfTaskInfo( this, UseDefaultIID() );
	//InterfacePtr<const IStringData> dbTaskSubject( dbTaskID, IID_IZPDATANAME );
	InterfacePtr<const IStringData> selfTaskSubject( this, IID_IZPDATANAME );


	this->InitDialog( selfTaskSubject, selfTaskInfo );

	if( dbTaskID )
		this->SetTextControlData( kOKButtonWidgetID, kIZPCreateTaskDlgUpdateBtnKey );
	else
		this->SetTextControlData( kOKButtonWidgetID, kIZPCreateTaskDlgCreateBtnKey );
}

//----------------------------------------------------------------------------------------
// ValidateDialogFields
//----------------------------------------------------------------------------------------
WidgetID
CZPCreateTaskDialogController::ValidateDialogFields(
	IActiveContext* myContext)
{
	WidgetID toReturn = inheritClass::ValidateDialogFields(myContext);

	do
	{
		PMString	theSubject;
		theSubject			= this->GetTextControlData( kZPUICreateTaskDlgTxtSubjectWidgetID );
		if( theSubject.IsEmpty() )
		{	toReturn = kZPUICreateTaskDlgTxtSubjectWidgetID;
			break;
		}

		PMString	theTitleID;
		theTitleID			= this->DBObjPopGetSelectedID( kZPUICreateTaskDlgPopTitleWidgetID, true );
		if( theTitleID.IsEmpty() )
		{	toReturn = kZPUICreateTaskDlgPopTitleWidgetID;
			break;
		}
		
		PMString	theEditionID;
		theEditionID		= this->DBObjPopGetSelectedID( kZPUICreateTaskDlgPopEditionWidgetID, true );
		if( theEditionID.IsEmpty() )
		{	toReturn = kZPUICreateTaskDlgPopEditionWidgetID;
			break;
		}
		
		if( mTaskToUpdate )
		{	//Check, title/edition cannot be changed.
			InterfacePtr<const IStringData> selfTaskEdition( this, IID_IZPEDITIONIDSTRINGDATA );
			ASSERT( selfTaskEdition );
			const PMString & oldEditionID = selfTaskEdition->Get();
			if( oldEditionID != theEditionID )
				break;
		}
		
		PMString	theStatusID;
		theStatusID			= this->DBObjPopGetSelectedID( kZPUICreateTaskDlgPopStatusWidgetID, true );
		if( theStatusID.IsEmpty() )
		{	toReturn = kZPUICreateTaskDlgPopStatusWidgetID;
			break;
		}

	}while(kFalse);

	return toReturn;
}

//----------------------------------------------------------------------------------------
// ApplyDialogFields
//----------------------------------------------------------------------------------------
void
CZPCreateTaskDialogController::ApplyDialogFields(
	IActiveContext* myContext, const WidgetID& widgetId)
{
	PMString	theSubject;
	PMString	theDescription;
	PMString	theDocumentID;
	PMString	theAssignedToUserID;
	PMString	theStatusID;
	PMString	theCategoryID;
	PMString	theSpread;
	PMString	thePage;

	PMString	theTitleID;
	PMString	theEditionID;
	
	theSubject			= this->GetTextControlData( kZPUICreateTaskDlgTxtSubjectWidgetID );
	theDescription		= this->GetTextControlData( kZPUICreateTaskDlgTxtDescriptionWidgetID );
	theSpread			= this->GetTextControlData( kZPUICreateTaskDlgTxtSpreadWidgetID );
	thePage				= this->GetTextControlData( kZPUICreateTaskDlgTxtPageWidgetID );

	theTitleID			= this->DBObjPopGetSelectedID( kZPUICreateTaskDlgPopTitleWidgetID, true );
	theEditionID		= this->DBObjPopGetSelectedID( kZPUICreateTaskDlgPopEditionWidgetID, true );
	theDocumentID		= this->DBObjPopGetSelectedID( kZPUICreateTaskDlgPopAsgnToDocWidgetID, true );
	theCategoryID		= this->DBObjPopGetSelectedID( kZPUICreateTaskDlgPopCategoryWidgetID, true );
	theStatusID			= this->DBObjPopGetSelectedID( kZPUICreateTaskDlgPopStatusWidgetID, true );
	theAssignedToUserID	= this->DBObjPopGetSelectedID( kZPUICreateTaskDlgPopAsgnToUserWidgetID, true );

	if( theDocumentID == kIZPPopupItemEmptyKey )
		theDocumentID = kNullString;

	if( theAssignedToUserID == kIZPPopupItemEmptyKey )
		theAssignedToUserID = kNullString;

	if( theCategoryID == kIZPPopupItemEmptyKey )
		theCategoryID = kNullString;

	theSubject.StripWhiteSpace( PMString::kLeadingAndTrailingWhiteSpace );
	theDescription.StripWhiteSpace( PMString::kLeadingAndTrailingWhiteSpace );
	theSpread.StripWhiteSpace( PMString::kLeadingAndTrailingWhiteSpace );
	thePage.StripWhiteSpace( PMString::kLeadingAndTrailingWhiteSpace );

	if( mTaskToUpdate )	//Update task
	{
		const PMString & theTaskID = mTaskToUpdate->Get();

		InterfacePtr<const IZPTaskInfo> dbTaskInfo( mTaskToUpdate, UseDefaultIID() );


		PMString theComments;
		theComments 	= this->GetTextControlData( kZPUICreateTaskDlgTxtCommentsWidgetID );
		Utils<IZPWSUtils>()->UpdateTask( theTaskID, theTitleID, theEditionID, theSubject,
			theDescription, theDocumentID, theAssignedToUserID, theStatusID, theCategoryID, theSpread, thePage, dbTaskInfo->GetCurrentVersion(), theComments );
	}
	else	//Create New Task.
	{
		PMString newTaskID;
		Utils<IZPWSUtils>()->CreateNewTask( newTaskID, theTitleID, theEditionID, theSubject,
			theDescription, theDocumentID, theAssignedToUserID, theStatusID, theCategoryID, theSpread, thePage );
	}
}

#pragma mark -
//----------------------------------------------------------------------------------------
// InitDialog
//----------------------------------------------------------------------------------------
void
CZPCreateTaskDialogController::InitDialog(
	const IStringData *			inTaskSubject,
	const IZPTaskInfo *			inTaskInfo)
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	PMString subjectStr;
	if( inTaskSubject )
		subjectStr = inTaskSubject->Get();
	
	this->SetTextControlData( kZPUICreateTaskDlgTxtSubjectWidgetID, subjectStr );

	PMString	theDescription;
	PMString	theDocumentID;
	PMString	theAssignedToUserID;
	PMString	theStatusID;
	PMString	theCategoryID;
	PMString	theSpread;
	PMString	thePage;

	if( inTaskInfo )
	{
		theDescription		= inTaskInfo->GetDescription();
		theDocumentID		= inTaskInfo->GetDocumentID();
		theAssignedToUserID	= inTaskInfo->GetAssignedToUserID();
		theStatusID			= inTaskInfo->GetStatusID();
		theCategoryID		= inTaskInfo->GetCategoryID();
		theSpread			= inTaskInfo->GetSpread();
		thePage				= inTaskInfo->GetPage();
	}

	//Text control data
	this->SetTextControlData( kZPUICreateTaskDlgTxtDescriptionWidgetID, theDescription );
	this->SetTextControlData( kZPUICreateTaskDlgTxtSpreadWidgetID, theSpread );
	this->SetTextControlData( kZPUICreateTaskDlgTxtPageWidgetID, thePage );
	this->SetTextControlData( kZPUICreateTaskDlgTxtCommentsWidgetID, kNullString );	//Always empty.

	InterfacePtr<const IStringData> selfTaskEdition( this, IID_IZPEDITIONIDSTRINGDATA );
	ASSERT( selfTaskEdition );
	const PMString & editionID = selfTaskEdition->Get();

	PMString theTitleID = kNullString;
	if( editionID.IsEmpty() == kFalse )
	{
		InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
		ASSERT( zpApp );
		InterfacePtr<const IZPTitleMgr> theTitleMgr( zpApp->GetTitleMgr(), UseDefaultIID() );
		ASSERT(theTitleMgr);
		const IStringData * dbTitleID = theTitleMgr->GetTitleForEditionID( editionID );
		if( dbTitleID )
			theTitleID = dbTitleID->Get();
	}

	//Set the dependency values
	InterfacePtr<IStringData> editionTitleID( this->GetWidgetView(kZPUICreateTaskDlgPopEditionWidgetID ), IID_IZPTITLEIDSTRINGDATA );
	ASSERT(editionTitleID);
	editionTitleID->Set( theTitleID );

	InterfacePtr<IStringData> docEditionID( this->GetWidgetView(kZPUICreateTaskDlgPopAsgnToDocWidgetID ), IID_IZPEDITIONIDSTRINGDATA );
	ASSERT(docEditionID);
	docEditionID->Set( editionID );

	//Selected IDs
	//Set the selected IDs before filling
	this->DBObjPopSetSelectedID( kZPUICreateTaskDlgPopTitleWidgetID, theTitleID );
	this->DBObjPopSetSelectedID( kZPUICreateTaskDlgPopEditionWidgetID, editionID );
	this->DBObjPopSetSelectedID( kZPUICreateTaskDlgPopAsgnToDocWidgetID, theDocumentID );
	this->DBObjPopSetSelectedID( kZPUICreateTaskDlgPopCategoryWidgetID, theCategoryID );
	this->DBObjPopSetSelectedID( kZPUICreateTaskDlgPopStatusWidgetID, theStatusID );
	this->DBObjPopSetSelectedID( kZPUICreateTaskDlgPopAsgnToUserWidgetID, theAssignedToUserID );
	
	//Fill title dropdown
	this->DBObjPopRefresh( kZPUICreateTaskDlgPopTitleWidgetID );
	this->DBObjPopRefresh( kZPUICreateTaskDlgPopEditionWidgetID );
	this->DBObjPopRefresh( kZPUICreateTaskDlgPopAsgnToDocWidgetID );
	this->DBObjPopRefresh( kZPUICreateTaskDlgPopCategoryWidgetID );
	this->DBObjPopRefresh( kZPUICreateTaskDlgPopStatusWidgetID );
	this->DBObjPopRefresh( kZPUICreateTaskDlgPopAsgnToUserWidgetID );

	//Disable comments
	IControlView * commentsView = this->GetWidgetView( kZPUICreateTaskDlgTxtCommentsWidgetID );
	this->SetTextControlData( kZPUICreateTaskDlgTxtCommentsWidgetID, kNullString );
	if( mTaskToUpdate )
		commentsView->Enable();
	else
		commentsView->Disable();
}

//----------------------------------------------------------------------------------------
// DBObjPopSetSelectedID
//----------------------------------------------------------------------------------------
void
CZPCreateTaskDialogController::DBObjPopSetSelectedID(
	WidgetID					inWidgetID,
	const PMString &			inSelectedID)
{
	InterfacePtr<IStringData> selectedID( this->GetWidgetView(inWidgetID), IID_IZPUISELECTEDID );
	ASSERT(selectedID);
	selectedID->Set( inSelectedID );
}

//----------------------------------------------------------------------------------------
// DBObjPopRefresh
//----------------------------------------------------------------------------------------
void
CZPCreateTaskDialogController::DBObjPopRefresh(
	WidgetID					inWidgetID)
{
	InterfacePtr<AZPDBObjPopupController> popupController( this->GetWidgetView(inWidgetID), IZPDBObjPopupController::kDefaultIID );
	ASSERT( popupController );
	popupController->Refresh();
}

//----------------------------------------------------------------------------------------
// GetWidgetView
//----------------------------------------------------------------------------------------
IControlView *
CZPCreateTaskDialogController::GetWidgetView(
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
// DBObjPopGetSelectedID
//----------------------------------------------------------------------------------------
PMString
CZPCreateTaskDialogController::DBObjPopGetSelectedID(
	WidgetID					inWidgetID,
	bool						inEmptyIfNotCurrentlySelected )
{
	InterfacePtr<const IStringData> selectedID( this->GetWidgetView(inWidgetID), IID_IZPUISELECTEDID );
	ASSERT(selectedID);

	InterfacePtr<IDropDownListController> popupListController( selectedID, UseDefaultIID());
	ASSERT(popupListController);
	
	int32 selectedIndex = popupListController->GetSelected();
	if( !inEmptyIfNotCurrentlySelected || selectedIndex >= 0 )
		return selectedID->Get();
	
	return kNullString;
}
