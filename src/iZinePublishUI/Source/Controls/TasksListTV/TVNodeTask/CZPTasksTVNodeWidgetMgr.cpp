//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/TasksListTV/TVNodeTask/CZPTasksTVNodeWidgetMgr.cpp $
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
#include "IPanelControlData.h"
#include "IStringData.h"
#include "ITextControlData.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes
#include "CoreResTypes.h"
#include "RsrcSpec.h"
#include "LocaleSetting.h"

//OS / ThirdParty includes

#include "IZPUIID.h"
#include "IZPID.h"

//IZP Interfaces
#include "IZPApp.h"
#include "IZPAssetMgr.h"
#include "IZPTaskInfo.h"
#include "IZPTaskMgr.h"
#include "IZPUserMgr.h"
#include "IZPTaskCategoryMgr.h"
#include "IZPTaskStatusMgr.h"
#include "IZPTVColumnsPref.h"
#include "IZPUITVNodeData.h"
#include "IZPUITVWithHdrNodeWidgetMgr.h"

//IZP General includes
#include "ZPUITypes.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZPUITVWithHdrNodeWidgetMgr>
class CZPTasksTVNodeWidgetMgr : public inheritClass
{
public:
						CZPTasksTVNodeWidgetMgr(IPMUnknown * boss);
	virtual				~CZPTasksTVNodeWidgetMgr();

						//Updates the UI controls of this node.
	void				Refresh() const;

	void				AddRemoveColumnWidgets(
							const IZPTVColumnsPref *	inColPrefs );
	void				RepositionColumnWidgets(
							const IZPTVColumnsPref *	inColPrefs ) const;

protected:
	bool				CreateColumnWidget(	//As all the columns are text, so no need to pass the column type
							IPanelControlData *			inControlPanelData,
							IControlView * &			oControlView,
							const WidgetID &			inWidgetID );

	void				SetColumnText(
							WidgetID					widgetID,
							const PMString &			inValue) const;

	PMString			GetColumnText(
							const IPMUnknown *			inTaskID,
							const IZPTaskInfo *			inTaskInfo,
							enTVColTasks				inColID ) const;

	PMString			GetDispName_Status(
							const IZPTaskInfo *			inTaskInfo ) const;
	PMString			GetDispName_Category(
							const IZPTaskInfo *			inTaskInfo ) const;
	PMString			GetDispName_Document(
							const IZPTaskInfo *			inTaskInfo ) const;
	PMString			GetDispName_User(
							const IZPTaskInfo *			inTaskInfo,
							enTVColTasks				inColID ) const;
private:

	InterfacePtr<const IZPTaskMgr> mTaskMgr;	//Cache it as all the mgr are on this boss.
};


CREATE_PMINTERFACE(CZPTasksTVNodeWidgetMgr, kZPTasksTVNodeWidgetMgrImpl)

CZPTasksTVNodeWidgetMgr::CZPTasksTVNodeWidgetMgr(IPMUnknown * boss)
: inheritClass(boss)
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	mTaskMgr.reset( zpApp->GetTaskMgr() );
	ASSERT( mTaskMgr );
	mTaskMgr->AddRef();
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPTasksTVNodeWidgetMgr::~CZPTasksTVNodeWidgetMgr()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// AddRemoveColumnWidgets
// Should be called when column count changes, when any column is shown/hidden.
// Or when new row widget is created.
//----------------------------------------------------------------------------------------
void
CZPTasksTVNodeWidgetMgr::AddRemoveColumnWidgets(
	const IZPTVColumnsPref *	inColPrefs)
{
	InterfacePtr<IControlView> selfControlView( this, UseDefaultIID() );
	ASSERT( selfControlView );

	InterfacePtr<IPanelControlData> selfPanelCtrlData( this, UseDefaultIID() );
	ASSERT( selfPanelCtrlData );

	const WidgetID & widgetStartID = kZPUITasksTVNodeSubjectWidgetID;	//First column widget ID

	//Update the row width
	PMRect rowRect = selfControlView->GetFrame();
	rowRect.SetWidth( inColPrefs->GetTotalWidthOfVisibleColumns() );
	selfControlView->SetFrame( rowRect );

	//Hide all the Column header widgets, the are not currently in visible list
	int currControlCount = selfPanelCtrlData->Length();
	int i = 0;
	int currColID = 0;
	WidgetID currWidgetID;
	IControlView * columnWidgetView = nil;
	for( ; i < currControlCount ; ++i )
	{
		columnWidgetView = selfPanelCtrlData->GetWidget( i );
		currWidgetID = columnWidgetView->GetWidgetID();
		currColID = currWidgetID - widgetStartID;

		if( !inColPrefs->IsColumnVisible( currColID ) )
			columnWidgetView->Hide();
	}

	//For visible columns, show column widget, Create if not already created.
	int theVisibleColCount = inColPrefs->GetVisibleColumnCount();
	for( int i = 0; i < theVisibleColCount; ++i )
	{
		const ZPTVColumnPref & currColPref = inColPrefs->GetColumnPrefByVisibleIndex( i );
		currWidgetID = widgetStartID + currColPref.mColID;

		bool isNewCreated = this->CreateColumnWidget( selfPanelCtrlData, columnWidgetView, currWidgetID );
		ASSERT( columnWidgetView );
		if( !columnWidgetView )
			continue;

		if( isNewCreated )
		{
			//Set the Column widget size & position
			PMRect	theColRect(currColPref.mColumnLeft, 0, currColPref.mColumnLeft + currColPref.mWidth, rowRect.Bottom() - rowRect.Top() );
			columnWidgetView->SetFrame( theColRect );
		}

		//Show if not currently visible
		if( !columnWidgetView->IsVisible() )
			columnWidgetView->Show();
	}
}

//----------------------------------------------------------------------------------------
// RepositionColumnWidgets
// Should be called when Any column moves or resizes.
//----------------------------------------------------------------------------------------
void
CZPTasksTVNodeWidgetMgr::RepositionColumnWidgets(
	const IZPTVColumnsPref *	inColPrefs) const
{
	InterfacePtr<IControlView> selfControlView( this, UseDefaultIID() );
	ASSERT( selfControlView );

	InterfacePtr<const IPanelControlData> selfPanelCtrlData( this, UseDefaultIID() );
	ASSERT( selfPanelCtrlData );

	const WidgetID & widgetStartID = kZPUITasksTVNodeSubjectWidgetID;	//First column widget ID

	//Update the row width
	PMRect rowRect = selfControlView->GetFrame();
	rowRect.SetWidth( inColPrefs->GetTotalWidthOfVisibleColumns() );
	selfControlView->SetFrame( rowRect );

	//For visible columns, reposition the column widget
	int i = 0;
	WidgetID currWidgetID;
	IControlView * columnWidgetView = nil;
	int theVisibleColCount = inColPrefs->GetVisibleColumnCount();
	for( int i = 0; i < theVisibleColCount; ++i )
	{
		const ZPTVColumnPref & currColPref = inColPrefs->GetColumnPrefByVisibleIndex( i );
		currWidgetID = widgetStartID + currColPref.mColID;

		columnWidgetView = selfPanelCtrlData->FindWidget( currWidgetID );
		ASSERT( columnWidgetView );
		if( !columnWidgetView )
			continue;
		ASSERT( columnWidgetView->IsVisible() );

		//Set the Column widget width & position
		PMRect	theColRect = columnWidgetView->GetFrame();
		theColRect.Left() = currColPref.mColumnLeft;
		theColRect.SetWidth( currColPref.mWidth );
		columnWidgetView->SetFrame( theColRect );
	}
}

//----------------------------------------------------------------------------------------
// Refresh
//----------------------------------------------------------------------------------------
void
CZPTasksTVNodeWidgetMgr::Refresh()const
{
	InterfacePtr<const IZPUITVNodeData> nodeData(this, UseDefaultIID());
	ASSERT(nodeData);
	const IPMUnknown * taskID = nodeData->GetDataObj();

	if( !taskID )
		return;

	InterfacePtr<const IZPTaskInfo> taskInfo( taskID, UseDefaultIID() );
	ASSERT( taskInfo );

	InterfacePtr<const IPanelControlData> selfPanelCtrlData( this, UseDefaultIID() );
	ASSERT( selfPanelCtrlData );

	const WidgetID & widgetStartID = kZPUITasksTVNodeSubjectWidgetID;	//First column widget ID

	//Update each column text
	int currControlCount = selfPanelCtrlData->Length();
	int i = 0;
	enTVColTasks currColID = eTVColTasks_Subject;
	WidgetID currWidgetID;
	IControlView * columnWidgetView = nil;
	PMString currColText;
	for( ; i < currControlCount ; ++i )
	{
		columnWidgetView = selfPanelCtrlData->GetWidget( i );
		currWidgetID = columnWidgetView->GetWidgetID();
		currColID = (enTVColTasks)(currWidgetID - widgetStartID);
		ASSERT( currColID >= eTVColTasks_Subject && currColID < eTVColTasksCount );
		currColText = this->GetColumnText( taskID, taskInfo, currColID );
		this->SetColumnText( currWidgetID, currColText );
	}
}

//----------------------------------------------------------------------------------------
// CreateColumnWidget
//As all the columns are text, so no need to pass the column type
//----------------------------------------------------------------------------------------
bool
CZPTasksTVNodeWidgetMgr::CreateColumnWidget(
	IPanelControlData *					inControlPanelData,
	IControlView * &					oControlView,
	const WidgetID &					inWidgetID)
{
	do
	{
		oControlView = inControlPanelData->FindWidget( inWidgetID, 2 );
		if( !oControlView )
		{
			InterfacePtr<IControlView> newTxtWidget(static_cast<IControlView*>
									( ::CreateObject( ::GetDataBase(this),
									  RsrcSpec( LocaleSetting::GetLocale(), kIZPUIPluginID, kViewRsrcType, kZPUITVColumnTxtWidgetRsrcID),
									  IControlView::kDefaultIID ) ));
			ASSERT(newTxtWidget);
			if(!newTxtWidget)
				break;
			
			inControlPanelData->AddWidget( newTxtWidget );
			newTxtWidget->SetWidgetID( inWidgetID );
			oControlView = newTxtWidget;
			return true;
		}
	}while(kFalse);
	return false;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetColumnText
//----------------------------------------------------------------------------------------
PMString
CZPTasksTVNodeWidgetMgr::GetColumnText(
	const IPMUnknown *			inTaskID,
	const IZPTaskInfo *			inTaskInfo,
	enTVColTasks				inColID) const
{
	PMString toReturn;
	switch( inColID )
	{
	case eTVColTasks_Subject:
		{
			InterfacePtr<const IStringData> taskSubject( inTaskID, IID_IZPDATANAME );
			ASSERT(taskSubject);
			toReturn = taskSubject->Get();
			break;
		}
	case eTVColTasks_Status:
		{	
			toReturn = this->GetDispName_Status( inTaskInfo );
			break;
		}
	case eTVColTasks_AssignedTo:
		{
			toReturn = this->GetDispName_User( inTaskInfo, inColID );
			break;
		}
	case eTVColTasks_Document:
		{
			toReturn = this->GetDispName_Document( inTaskInfo );
			break;
		}
	case eTVColTasks_Category:
		{
			toReturn = this->GetDispName_Category( inTaskInfo );
			break;
		}
	case eTVColTasks_Spread:
		{
			toReturn = inTaskInfo->GetSpread();
			break;
		}
	case eTVColTasks_Page:
		{
			toReturn = inTaskInfo->GetPage();
			break;
		}
	case eTVColTasks_UpdatedBy:
		{
			//toReturn = inTaskInfo->Get();
			break;
		}
	case eTVColTasks_UpdatedOn:
		{
			//toReturn = inTaskInfo->Get();
			break;
		}
	case eTVColTasks_Creator:
		{
			toReturn = kNullString;
			break;
		}
	case eTVColTasks_CreationDate:
		{
			toReturn = kNullString;
			break;
		}
	}
	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetDispName_Status
//----------------------------------------------------------------------------------------
PMString
CZPTasksTVNodeWidgetMgr::GetDispName_Status(
	const IZPTaskInfo *			inTaskInfo) const
{
	const PMString & theStatusID = inTaskInfo->GetStatusID();
	if( !theStatusID.IsEmpty() )
	{
		InterfacePtr<const IZPTaskStatusMgr> taskStatusMgr( mTaskMgr, UseDefaultIID() );
		ASSERT( taskStatusMgr );

		const IStringData * dbTaskStatusID = taskStatusMgr->GetTaskStatusForID( theStatusID );
		if( dbTaskStatusID )
		{
			InterfacePtr<const IStringData> dispName( dbTaskStatusID, IID_IZPDATANAME );
			return dispName->Get();
		}
	}
	return kNullString;
}

//----------------------------------------------------------------------------------------
// GetDispName_Category
//----------------------------------------------------------------------------------------
PMString
CZPTasksTVNodeWidgetMgr::GetDispName_Category(
	const IZPTaskInfo *			inTaskInfo) const
{
	const PMString & theCategoryID = inTaskInfo->GetCategoryID();
	if( !theCategoryID.IsEmpty() )
	{
		InterfacePtr<const IZPTaskCategoryMgr> taskCategoryMgr( mTaskMgr, UseDefaultIID() );
		ASSERT( taskCategoryMgr );

		const IStringData * dbTaskCategoryID = taskCategoryMgr->GetTaskCategoryForID( theCategoryID );
		if( dbTaskCategoryID )
		{
			InterfacePtr<const IStringData> dispName( dbTaskCategoryID, IID_IZPDATANAME );
			return dispName->Get();
		}
	}
	return kNullString;
}

//----------------------------------------------------------------------------------------
// GetDispName_Document
//----------------------------------------------------------------------------------------
PMString
CZPTasksTVNodeWidgetMgr::GetDispName_Document(
	const IZPTaskInfo *			inTaskInfo) const
{
	const PMString & theAssetID = inTaskInfo->GetDocumentID();
	if( !theAssetID.IsEmpty() )
	{
		InterfacePtr<const IZPAssetMgr> assetMgr( mTaskMgr, UseDefaultIID() );
		ASSERT( assetMgr );

		const IStringData * dbAssetID = assetMgr->GetAssetByID( theAssetID );
		if( dbAssetID )
		{
			InterfacePtr<const IStringData> dispName( dbAssetID, IID_IZPDATANAME );
			return dispName->Get();
		}
	}
	return kNullString;
}

//----------------------------------------------------------------------------------------
// GetDispName_User
//----------------------------------------------------------------------------------------
PMString
CZPTasksTVNodeWidgetMgr::GetDispName_User(
	const IZPTaskInfo *			inTaskInfo,
	enTVColTasks				inColID) const
{
	//TODO: use inColID
	const PMString & theUserID = inTaskInfo->GetAssignedToUserID();
	if( !theUserID.IsEmpty() )
	{
		InterfacePtr<const IZPUserMgr> userMgr( mTaskMgr, UseDefaultIID() );
		ASSERT( userMgr );

		const IStringData * dbUserID = userMgr->GetUserForID( theUserID );
		if( dbUserID )
		{
			InterfacePtr<const IStringData> dispName( dbUserID, IID_IZPDATANAME );
			return dispName->Get();
		}
	}
	return kNullString;
}

//----------------------------------------------------------------------------------------
// SetColumnText
//----------------------------------------------------------------------------------------
void
CZPTasksTVNodeWidgetMgr::SetColumnText(
	WidgetID					widgetID,
	const PMString &			inValue) const
{
	do {
		InterfacePtr<const IPanelControlData> selfPanelControlData(this, UseDefaultIID());
		ASSERT(selfPanelControlData);
		if(selfPanelControlData==nil) {
			break;
		}
		
		IControlView* controlView = selfPanelControlData->FindWidget( widgetID );
		ASSERT(controlView);
		if (controlView==nil)
			break;

		InterfacePtr<ITextControlData> textControlData( controlView, UseDefaultIID() );
		ASSERT(textControlData);

		PMString strValue(inValue);
		strValue.SetTranslatable(kFalse);
		
		textControlData->SetString(strValue);

	} while (kFalse);
}
