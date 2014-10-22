//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Controls/TreeViewHeader/CZMTVHeaderController.cpp $
//	$Revision: 2770 $
//	$Date: 2011-04-11 12:00:23 +0200 (Mon, 11 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 7-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IAssociatedWidgetAttributes.h"
#include "IIntData.h"
#include "IPanelControlData.h"
#include "ISubject.h"
#include "ITextControlData.h"
#include "ITreeViewMgr.h"
#include "IWidgetParent.h"

//SDK General includes
#include "CoreResTypes.h"
#include "LocaleSetting.h"
#include "RsrcSpec.h"

//OS / ThirdParty includes

#include "iZMediaID.h"

//IZP Interfaces
#include "IZMTVHeaderController.h"
#include "IZMTVColumnsInfo.h"
#include "IZMTVColumnsPref.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_TVHeader
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZMTVHeaderController>
class CZMTVHeaderController : public inheritClass
{
public:
						CZMTVHeaderController(IPMUnknown * boss);
	virtual				~CZMTVHeaderController();

	void				InitHeaderControlsInfo();

	void				CreateHeaderWidgets();

	void				HandleTreeViewScrolled(
							const IPMUnknown *					inTreeView );
	void				HandleTreeViewResized(
							const IPMUnknown *					inTreeView );

	void				ToggleColumnVisibility(
							const int							inColID );

	void				UpdateAssociatedTreeView();

	void				BroadcastMessage(
							const ClassID&						inTheChange,
							void *								inChangedBy = nil) const;

protected:
	bool				CreateColumnHeaderWidget(
							IPanelControlData *					inControlStripPanelData,
							IControlView * &					oControlView,
							const WidgetID &					inWidgetID );

	const IControlView * FindTVControlView()const;
private:
	bool		mInitialized;
};


CREATE_PMINTERFACE(CZMTVHeaderController, kZMTVHeaderControllerImpl)

CZMTVHeaderController::CZMTVHeaderController(IPMUnknown * boss)
: inheritClass(boss)
, mInitialized( false )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZMTVHeaderController::~CZMTVHeaderController()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// InitHeaderControlsInfo
//----------------------------------------------------------------------------------------
void
CZMTVHeaderController::InitHeaderControlsInfo()
{
	LogFunctionEnterExit;

	if( !mInitialized )
	{
		mInitialized = true;

		InterfacePtr<IZMTVColumnsInfo> selfColumnInfo( this, UseDefaultIID() );
		ASSERT( selfColumnInfo );

		InterfacePtr<const IIntData> selfTreeType( this, IID_IZMINTDATA );
		ASSERT( selfTreeType );

		selfColumnInfo->InitCoumnsInfo( selfTreeType->Get() );	//Someday: ReadWrite these from resource
	}

	//Update the column prefs.
	InterfacePtr<IZMTVColumnsPref> selfPrefs( this, UseDefaultIID() );
	ASSERT( selfPrefs );

	selfPrefs->AddRemoveColumnPrefs();	//Add new columns if any
	selfPrefs->ValidatePositionIndexes();
	selfPrefs->ValidateSize();
	selfPrefs->RecalculatePositions();
}

//----------------------------------------------------------------------------------------
// CreateHeaderWidgets
//----------------------------------------------------------------------------------------
void
CZMTVHeaderController::CreateHeaderWidgets()
{
	LogFunctionEnterExit;

	InterfacePtr<const IPanelControlData> selfPanelCtrlData( this, UseDefaultIID() );
	ASSERT( selfPanelCtrlData );

	IControlView * headerControlStrip = selfPanelCtrlData->FindWidget( kZMUITVHdrControlStripWidgetID );	//We can also get the 0th child as it is only child
	ASSERT( headerControlStrip );
	if( !headerControlStrip )
		return;

	InterfacePtr<IPanelControlData> stripPanelCtrlData( headerControlStrip, UseDefaultIID() );
	ASSERT( stripPanelCtrlData );

	InterfacePtr<IZMTVColumnsInfo> selfColumnInfo( this, UseDefaultIID() );
	ASSERT( selfColumnInfo );

	const WidgetID & stripWidgetStartID = selfColumnInfo->GetColumnWidgetStartID();

	InterfacePtr<IZMTVColumnsPref> selfPrefs( this, UseDefaultIID() );
	ASSERT( selfPrefs );

	//Update the Header strip width
	PMRect stripRect = headerControlStrip->GetFrame();
	stripRect.SetWidth( selfPrefs->GetTotalWidthOfVisibleColumns() );
	headerControlStrip->SetFrame( stripRect );

	//Hide all the Column header widgets, the are not currently in visible list
	int stripControlCount = stripPanelCtrlData->Length();
	int i = 0;
	int currColID = 0;
	WidgetID currWidgetID;
	IControlView * columnHeaderWidgetView = nil;
	for( ; i < stripControlCount ; ++i )
	{
		columnHeaderWidgetView = stripPanelCtrlData->GetWidget( i );
		currWidgetID = columnHeaderWidgetView->GetWidgetID();
		currColID = currWidgetID - stripWidgetStartID;

		if( !selfPrefs->IsColumnVisible( currColID ) )
			columnHeaderWidgetView->Hide();
	}

	//For visible columns, show header widget, Create if not already created.
	int theVisibleColCount = selfPrefs->GetVisibleColumnCount();
	for( int i = 0; i < theVisibleColCount; ++i )
	{
		const ZMTVColumnPref & currColPref = selfPrefs->GetColumnPrefByVisibleIndex( i );
		currWidgetID = stripWidgetStartID + currColPref.mColID;

		bool isNewCreated = this->CreateColumnHeaderWidget( stripPanelCtrlData, columnHeaderWidgetView, currWidgetID );

		ASSERT( columnHeaderWidgetView );
		if( !columnHeaderWidgetView )
			continue;

		//Set the Column Header widget size & position
		PMRect	theColHeaderRect(currColPref.mColumnLeft, 0, currColPref.mColumnLeft + currColPref.mWidth, stripRect.Bottom() - stripRect.Top() );
		columnHeaderWidgetView->SetFrame( theColHeaderRect );

		//Set col header label
		if( isNewCreated )
		{
			const ZMTVColumnInfo & colInfo = selfColumnInfo->GetColumnInfoForColID( currColPref.mColID );
			InterfacePtr<ITextControlData> colHeaderTextCtrlData( columnHeaderWidgetView, UseDefaultIID() );
			colHeaderTextCtrlData->SetString( colInfo.mColDispName );
		}

		//Show if not currently visible
		if( !columnHeaderWidgetView->IsVisible() )
			columnHeaderWidgetView->Show();
	}
}

//----------------------------------------------------------------------------------------
// CreateColumnHeaderWidget
//----------------------------------------------------------------------------------------
bool
CZMTVHeaderController::CreateColumnHeaderWidget(
	IPanelControlData *					inControlStripPanelData,
	IControlView * &					oControlView,
	const WidgetID &					inWidgetID)
{
	do
	{
		oControlView = inControlStripPanelData->FindWidget( inWidgetID, 2 );
		if( !oControlView )
		{
			InterfacePtr<IControlView> newTxtWidget(static_cast<IControlView*>
									( ::CreateObject( ::GetDataBase(this),
									  RsrcSpec( LocaleSetting::GetLocale(), kIZMediaPluginID, kViewRsrcType, kZMUITVHeaderColumnWidgetRsrcID),
									  IControlView::kDefaultIID ) ));
			ASSERT(newTxtWidget);
			if(!newTxtWidget)
				break;
			
			inControlStripPanelData->AddWidget( newTxtWidget );
			newTxtWidget->SetWidgetID( inWidgetID );
			oControlView = newTxtWidget;
			return true;
		}
	}while(kFalse);
	return false;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// HandleTreeViewScrolled
//----------------------------------------------------------------------------------------
void
CZMTVHeaderController::HandleTreeViewScrolled(
	const IPMUnknown *					inTreeView )
{
	do{
		InterfacePtr<const IPanelControlData> selfPanelCtrlData( this, UseDefaultIID() );
		ASSERT( selfPanelCtrlData );

		IControlView * headerControlStrip = selfPanelCtrlData->FindWidget( kZMUITVHdrControlStripWidgetID );	//We can also get the 0th child as it is only child
		ASSERT( headerControlStrip );
		if( !headerControlStrip )
			break;
		
		InterfacePtr<const IPanelControlData> treePanelControlData( inTreeView, UseDefaultIID());
		ASSERT(treePanelControlData);
		if(!treePanelControlData)
			break;

		IControlView* treeScrollerControlView = treePanelControlData->FindWidget(kTreeScrollerWidgetID);
		if( !treeScrollerControlView )
			break;
	
		//InterfacePtr<const IPanelControlData> treeScrollerPanelControlData( treeScrollerControlView, UseDefaultIID());
		//ASSERT(treeScrollerPanelControlData);
		//if(!treeScrollerPanelControlData)
		//	break;

		//if( treeScrollerPanelControlData->Length() <= 0 )
		//	break;
		//IControlView* theNodeWidget = treeScrollerPanelControlData->GetWidget( 0 );
		//if(!theNodeWidget)
		//	break;

		PMRect theScrollerRect = treeScrollerControlView->GetFrame();
		//PMRect theScrollerBBox = treeScrollerControlView->GetBBox();
		//PMRect theNodeRect (theNodeWidget->GetFrame() );//GetBBox ()) ;
		//PMRect theNodeBBox = theNodeWidget->GetBBox();
		PMRect headerRect = headerControlStrip->GetFrame();
		ASSERT( headerRect.IsEmpty() == kFalse );
		if(headerRect.IsEmpty())
			break;
		
		headerRect.MoveTo(theScrollerRect.Left(), headerRect.Top() );
		headerControlStrip->SetFrame(headerRect);
		headerControlStrip->Invalidate();
	}
	while(kFalse);
}

//----------------------------------------------------------------------------------------
// HandleTreeViewResized
//----------------------------------------------------------------------------------------
void
CZMTVHeaderController::HandleTreeViewResized(
	const IPMUnknown *					inTreeView )
{
	this->HandleTreeViewScrolled( inTreeView );
}

//----------------------------------------------------------------------------------------
// ToggleColumnVisibility
//----------------------------------------------------------------------------------------
void
CZMTVHeaderController::ToggleColumnVisibility(
	const int							inColID)
{
	LogFunctionEnterExit;
	InterfacePtr<IZMTVColumnsPref> selfPrefs( this, UseDefaultIID() );
	ASSERT( selfPrefs );

	selfPrefs->ToggleColumnVisibility( inColID );
	selfPrefs->RecalculatePositions();
	this->CreateHeaderWidgets();
	this->BroadcastMessage( kZMUITVHeaderColPositionsChangedMsg );
	this->UpdateAssociatedTreeView();
}

//----------------------------------------------------------------------------------------
// UpdateAssociatedTreeView
//----------------------------------------------------------------------------------------
void
CZMTVHeaderController::UpdateAssociatedTreeView()
{
	LogFunctionEnterExit;
	const IControlView * tvControlView = this->FindTVControlView();
	if( !tvControlView )
		return;
	
	InterfacePtr<ITreeViewMgr> tvMgr( tvControlView, UseDefaultIID());
	ASSERT( tvMgr );

	tvMgr->ChangeRoot( kTrue );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// BroadcastMessage
//----------------------------------------------------------------------------------------
void
CZMTVHeaderController::BroadcastMessage(
	const ClassID&						inTheChange,
	void *								inChangedBy ) const
{
	InterfacePtr<ISubject> selfSubject(this, UseDefaultIID());
	ASSERT(selfSubject);
	
	selfSubject->Change( inTheChange, IZMTVHeaderController::kDefaultIID );
}

//----------------------------------------------------------------------------------------
// FindHeaderControlView
//----------------------------------------------------------------------------------------
const IControlView *
CZMTVHeaderController::FindTVControlView()const
{
	InterfacePtr<const IWidgetParent> selfWParent(this, UseDefaultIID());
	ASSERT( selfWParent );
	
	const IPMUnknown * parentOfTreeViewHdr = selfWParent->GetParent();

	InterfacePtr<const IPanelControlData> hdrParentPanelCtrlData( parentOfTreeViewHdr, UseDefaultIID() );
	ASSERT( hdrParentPanelCtrlData );

	InterfacePtr<const IAssociatedWidgetAttributes> selfLinkedWidgetID( this, IID_ILINKEDWIDGETATTRIBUTES );
	ASSERT( selfLinkedWidgetID );

	ASSERT( selfLinkedWidgetID->GetAssociatedWidgetID() != kInvalidWidgetID );

	const IControlView * tvControlView = hdrParentPanelCtrlData->FindWidget( selfLinkedWidgetID->GetAssociatedWidgetID() );
	return tvControlView;
}