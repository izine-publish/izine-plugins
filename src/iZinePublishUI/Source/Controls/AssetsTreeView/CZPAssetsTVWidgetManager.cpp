//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/AssetsTreeView/CZPAssetsTVWidgetManager.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 28-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IAssociatedWidgetAttributes.h"
#include "IControlView.h"
#include "IIntData.h"
#include "IObserver.h"
#include "IPanelControlData.h"
#include "IStringData.h"
#include "ISubject.h"
#include "ITextControlData.h"
#include "ITreeViewHierarchyAdapter.h"
#include "IWidgetParent.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes
#include "CAlert.h"
#include "CoreResTypes.h"
#include "CreateObject.h"
#include "CTreeViewWidgetMgr.h"
#include "IconRsrcDefs.h"
#include "InterfaceColorDefines.h"
#include "LocaleSetting.h"
#include "RsrcSpec.h"
#include "SysControlIds.h"

//OS / ThirdParty includes

#include "IZPUIID.h"
#include "IZPID.h"

//IZP Interfaces
#include "IZPApp.h"
#include "IZPAssetMgr.h"
#include "IZPTitleMgr.h"
#include "IZPTitleStatusMgr.h"
#include "IZPTVColumnsPref.h"
#include "IZPUITVNodeData.h"
#include "IZPUITVNodeWidgetMgr.h"
#include "IZPUITVWithHdrNodeWidgetMgr.h"

//IZP General includes
#include "ZPAssetsTVNodeID.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AssetsTV
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

/**
 Implements ITreeViewWidgetMgr, providing methods to create and describe the widgets for 
 nodes in the tree. This interface is similar to a widget factory in that this is where
 you will create widgets for the nodes in the tree.
 
 For further reading on ITreeViewWidgetMgr interface, see the Widgets chapter
 of the User Interface technote.
 
 
 @ingroup paneltreeview
 
 */
#define inheritClass CTreeViewWidgetMgr
class CZPAssetsTVWidgetManager : public inheritClass
{
public:
						CZPAssetsTVWidgetManager(IPMUnknown * boss);
	virtual				~CZPAssetsTVWidgetManager();

	virtual	IControlView* CreateWidgetForNode(
							const NodeID&				node) const;
	virtual	WidgetID	GetWidgetTypeForNode(
							const NodeID&				node) const;
	virtual	bool16		ApplyNodeIDToWidget(
							const NodeID& node,  IControlView* widget, int32 message = 0) const;
	PMReal				GetIndentForNode(
							const NodeID&				node) const;
	PMReal				GetNodeWidgetWidth(
							const NodeID&				node) const;
	
protected:
	void				SetColumnText(
							IControlView* widget, WidgetID widgetid, const PMString value) const;
	
	void				SetIcon(
							IControlView*				inNodeWidget,
							WidgetID					inWidgetid,
							const PluginID &			inIconPluginID,
							const RsrcID &				inIconID) const;

	void				ShowColumn(
							IControlView* widget, WidgetID widgetid) const;
	void				HideColumn(
							IControlView* widget, WidgetID widgetid) const;

	void				EnableColumn(
							IControlView* widget, WidgetID widgetid) const;
	void				DisableColumn(
							IControlView* widget, WidgetID widgetid) const;

	void				AttachToWidget(
							IControlView *				inNodeWidget ) const;

	RsrcSpec			GetResourceForStatusNode() const;
	RsrcSpec			GetResourceForAssetNode() const;

	bool16				ApplyStatusNodeToWidget(
							const PMString &			inStatusID,
							IControlView *				inNodeWidget ) const;
	bool16				ApplyAssetNodeToWidget(
							const PMString &			inAssetID,
							IControlView *				inNodeWidget ) const;
	void				ShowHideExpanderWidget(
							IControlView *				inNodeWidget,
							bool						inShow ) const;

	const IControlView * FindHeaderControlView() const;
private:
	
	InterfacePtr<const IZPTVColumnsPref>	mColumnsPref;
};


CREATE_PMINTERFACE(CZPAssetsTVWidgetManager, kZPAssetsTVWidgetManagerImpl)

CZPAssetsTVWidgetManager::CZPAssetsTVWidgetManager(IPMUnknown * boss)
: inheritClass(boss)
{
	const IControlView * headerControlView = this->FindHeaderControlView();

	InterfacePtr<const IZPTVColumnsPref> headerColPrefs( headerControlView, UseDefaultIID() );
	ASSERT( headerColPrefs );
	headerColPrefs->AddRef();
	mColumnsPref.reset( headerColPrefs );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAssetsTVWidgetManager::~CZPAssetsTVWidgetManager()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetWidgetTypeForNode
//----------------------------------------------------------------------------------------
WidgetID
CZPAssetsTVWidgetManager::GetWidgetTypeForNode(
	const NodeID& node) const
{
	do
	{
		ZPTreeNodePtr<ZPAssetsTVNodeID> nodeID(node);
		ASSERT(nodeID != nil);
		if (nodeID == nil)
			break;

		NodeIDClass::NodeType nodeType = nodeID->GetNodeType();

		if (nodeID->GetNodeType() == eTVNodeAsset)
		{
			return kZPUIAssetsTVAssetNodeWidgetID;
		}
		else if (nodeID->GetNodeType() == eTVNodeStatus)
		{
			return kZPUIAssetsTVStatusNodeWidgetID;
		}
	}while(false);

	return kInvalidWidgetID;
}

//----------------------------------------------------------------------------------------
// CreateWidgetForNode
//----------------------------------------------------------------------------------------
IControlView*
CZPAssetsTVWidgetManager::CreateWidgetForNode(
	const NodeID& node) const
{
	LogFunctionEnterExit;
	
	IControlView* toReturn = NULL;
	do
	{
		ZPTreeNodePtr<ZPAssetsTVNodeID> nodeID(node);
		ASSERT(nodeID != nil);
		if (nodeID == nil)
			break;

		const PMString & nodeIDStr = nodeID->GetID();
		
		if (nodeID->GetNodeType() == eTVNodeAsset)
		{
			toReturn = (IControlView*) ::CreateObject( ::GetDataBase(this), this->GetResourceForAssetNode(), IID_ICONTROLVIEW);
			
			//InterfacePtr<IZPUITVWithHdrNodeWidgetMgr> nodeWdgtMgr(toReturn, UseDefaultIID());
			//ASSERT(nodeWdgtMgr);
			//nodeWdgtMgr->AddRemoveColumnWidgets( mColumnsPref );	//Commented as it is done during refresh
		}
		else if (nodeID->GetNodeType() == eTVNodeStatus)
			toReturn = (IControlView*) ::CreateObject( ::GetDataBase(this), this->GetResourceForStatusNode(), IID_ICONTROLVIEW);
	} while (kFalse);
	
	ASSERT(toReturn);
	return toReturn;
}

//----------------------------------------------------------------------------------------
// ApplyNodeIDToWidget
//----------------------------------------------------------------------------------------
bool16
CZPAssetsTVWidgetManager::ApplyNodeIDToWidget(
	const NodeID& node, IControlView* widget, int32 message) const
{
	bool16 toReturn = kFalse;
	do
	{
		ZPTreeNodePtr<ZPAssetsTVNodeID> nodeID(node);
		ASSERT(nodeID != nil);
		if (nodeID == nil)
			break;

		const PMString & nodeIDStr = nodeID->GetID();
		ClassID theWidgetClass = ::GetClass( widget );

		if (nodeID->GetNodeType() == eTVNodeAsset && theWidgetClass == kZPAssetsTVAssetNodeWidgetBoss )
			toReturn = this->ApplyAssetNodeToWidget( nodeIDStr, widget );
		else if (nodeID->GetNodeType() == eTVNodeStatus && theWidgetClass == kZPAssetsTVStatusNodeWidgetBoss )
			toReturn = this->ApplyStatusNodeToWidget( nodeIDStr, widget );

	}while( false);
	CTreeViewWidgetMgr::ApplyNodeIDToWidget(node, widget);

	return toReturn;
}

//----------------------------------------------------------------------------------------
// ApplyStatusNodeToWidget
//----------------------------------------------------------------------------------------
bool16
CZPAssetsTVWidgetManager::ApplyStatusNodeToWidget(
	const PMString &			inStatusID,
	IControlView *				inNodeWidget) const
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<const IZPTitleStatusMgr> titleStatusMgr( zpApp->GetTitleMgr(), UseDefaultIID() );
	ASSERT(titleStatusMgr);

	const IStringData * statusID = titleStatusMgr->GetStatusForID( inStatusID );

	//store statusID with node and then applyit.
	InterfacePtr<IZPUITVNodeData> nodeData(inNodeWidget, UseDefaultIID());
	ASSERT(nodeData);
	nodeData->SetDataObj( statusID );

	InterfacePtr<const IZPUITVNodeWidgetMgr> nodeWdgtMgr(inNodeWidget, UseDefaultIID());
	ASSERT(nodeWdgtMgr);
	nodeWdgtMgr->Refresh();

	//TODO: move below code to node widget mgr
	InterfacePtr<const IStringData> statusName( statusID, IID_IZPDATANAME );
	ASSERT( statusName );
	
	PMString theStatusName;
	if( statusName )
		theStatusName = statusName->Get( );

	InterfacePtr<const ITreeViewHierarchyAdapter> selfAdapter(this, UseDefaultIID());
	ASSERT(selfAdapter);
		
	int childCount = selfAdapter->GetNumChildren(ZPAssetsTVNodeID::Create( eTVNodeStatus, inStatusID )) ;

	char buffer[100];
	sprintf(buffer, "%s (%d)", theStatusName.GrabCString(), childCount);
	
#if defined(InDnCS5) || defined(InDnCS5_5)
	SetColumnText( inNodeWidget, kZPUIAssetsTVNodeNameWidgetID, PMString(buffer, PMString::kDontTranslateDuringCall));
#else
	SetColumnText( inNodeWidget, kZPUIAssetsTVNodeNameWidgetID, PMString(buffer, -1, PMString::kNoTranslate));
#endif

	//TODO: when resource for both the node types is different, then
	// why do we need to show hide the expander widget.
	this->ShowHideExpanderWidget( inNodeWidget, childCount > 0 );
	
	//This icon is fixed, then why are we setting this each time.
	SetIcon(inNodeWidget, kZPUIAssetsTVNodeIconWidgetID, kApplicationRsrcPluginID, kGenericFolderRolloverIconID);

	//Move the status name widget in-align to asset name widget. i.e eTVColAssets_AssetName column.
	const ZPTVColumnPref & nameColPref = mColumnsPref->GetColumnPrefByColID( eTVColAssets_AssetName );

	InterfacePtr<const IPanelControlData> nodePanelCtrlData( inNodeWidget, UseDefaultIID() );
	ASSERT( nodePanelCtrlData );
	IControlView * nameColumnWidgetView = nodePanelCtrlData->FindWidget( kZPUIAssetsTVNodeNameWidgetID );
	ASSERT( nameColumnWidgetView );
	if( nameColumnWidgetView )
	{
		//Set the Column widget width & position
		PMRect	theNameColRect = nameColumnWidgetView->GetFrame();
		PMRect	theNameColBBoxRect = nameColumnWidgetView->GetBBox();
		PMRect	theNodeBBoxRect = inNodeWidget->GetBBox();
		PMReal theLeftPosition = nameColPref.mColumnLeft - theNameColBBoxRect.Left() + theNodeBBoxRect.Left() + theNameColRect.Left();
		if( theNameColRect.Left() != theLeftPosition )//nameColPref.mColumnLeft )
		{
			theNameColRect.MoveTo( theLeftPosition, theNameColRect.Top() );
			nameColumnWidgetView->SetFrame( theNameColRect );
		}
	}

	//Attach self observer to node.
	this->AttachToWidget( inNodeWidget );

	return kTrue;
}

//----------------------------------------------------------------------------------------
// ApplyAssetNodeToWidget
//----------------------------------------------------------------------------------------
bool16
CZPAssetsTVWidgetManager::ApplyAssetNodeToWidget(
	const PMString &			inAssetID,
	IControlView *				inNodeWidget) const
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IZPUITVNodeData> nodeData(inNodeWidget, UseDefaultIID());
	ASSERT(nodeData);

	InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<const IZPAssetMgr> assetMgr( zpApp->GetAssetMgr(), UseDefaultIID() );
	ASSERT(assetMgr);

	const IStringData * assetID = assetMgr->GetAssetByID( inAssetID );

	nodeData->SetDataObj(assetID);

	InterfacePtr<IZPUITVWithHdrNodeWidgetMgr> nodeWdgtMgr(inNodeWidget, UseDefaultIID());
	ASSERT(nodeWdgtMgr);

	InterfacePtr<IIntData> nodeWdgtIntData( inNodeWidget, UseDefaultIID() );
	ASSERT( nodeWdgtIntData );
	
	if( nodeWdgtIntData->Get() != mColumnsPref->GetVersion() )
	{
		nodeWdgtIntData->Set( mColumnsPref->GetVersion() );
		nodeWdgtMgr->AddRemoveColumnWidgets( mColumnsPref );
		nodeWdgtMgr->RepositionColumnWidgets( mColumnsPref );
	}
	nodeWdgtMgr->Refresh();

	//Attach self observer to node.
	this->AttachToWidget( inNodeWidget );

	return kTrue;
}

//----------------------------------------------------------------------------------------
// AttachToWidget
//----------------------------------------------------------------------------------------
void
CZPAssetsTVWidgetManager::AttachToWidget(
	IControlView *				inNodeWidget) const
{
	InterfacePtr<ISubject> nodeSubject(inNodeWidget, UseDefaultIID());
	ASSERT( nodeSubject );

	InterfacePtr<IObserver> selfObserver( this, UseDefaultIID());
	ASSERT( selfObserver );
	
	if( !nodeSubject->IsAttached( selfObserver, IID_IEVENTHANDLER ) )
		nodeSubject->AttachObserver( selfObserver, IID_IEVENTHANDLER );	//Double click
	if( !nodeSubject->IsAttached( selfObserver, IID_IOBSERVER ) )		//Other messages like column button pressed
		nodeSubject->AttachObserver( selfObserver, IID_IOBSERVER );
}	

//----------------------------------------------------------------------------------------
// GetResourceForStatusNode
//----------------------------------------------------------------------------------------
RsrcSpec
CZPAssetsTVWidgetManager::GetResourceForStatusNode() const
{
	return	RsrcSpec(LocaleSetting::GetLocale(),
				kIZPUIPluginID,
				kViewRsrcType,
				kIZPAssetsTVStatusNodeWidgetRsrcID);
}


//----------------------------------------------------------------------------------------
// GetResourceForAssetNode
//----------------------------------------------------------------------------------------
RsrcSpec
CZPAssetsTVWidgetManager::GetResourceForAssetNode() const
{
	return RsrcSpec(LocaleSetting::GetLocale(),
				kIZPUIPluginID,
				kViewRsrcType,
				kIZPAssetsTVAssetNodeWidgetRsrcID );
}


#pragma mark -
//----------------------------------------------------------------------------------------
// SetColumnText
//----------------------------------------------------------------------------------------
void
CZPAssetsTVWidgetManager::SetColumnText(
	IControlView* widget, WidgetID widgetid, const PMString value) const
{
	do {
		InterfacePtr<const IPanelControlData> panelControlData(widget, UseDefaultIID());
		ASSERT(panelControlData);
		if(panelControlData==nil) {
			break;
		}
		
		IControlView* controlView = panelControlData->FindWidget( widgetid );
		ASSERT(controlView);
		if (controlView==nil)
			break;

		InterfacePtr<ITextControlData> textControlData( controlView, UseDefaultIID() );
		ASSERT(textControlData);

		PMString strValue(value);
		strValue.SetTranslatable(kFalse);
		
		textControlData->SetString(strValue);

	} while (kFalse);
}

//----------------------------------------------------------------------------------------
// EnableColumn
//----------------------------------------------------------------------------------------
void
CZPAssetsTVWidgetManager::EnableColumn(
	IControlView* widget, WidgetID widgetid) const
{
	do {
		InterfacePtr<const IPanelControlData> panelControlData(widget, UseDefaultIID());
		ASSERT(panelControlData);
		if(panelControlData==nil) {
			break;
		}
		
		IControlView* controlView = panelControlData->FindWidget( widgetid );
		ASSERT(controlView);
		if (controlView==nil)
			break;
		
		controlView->Enable();
		
	} while (kFalse);
}

//----------------------------------------------------------------------------------------
// DisableColumn
//----------------------------------------------------------------------------------------
void
CZPAssetsTVWidgetManager::DisableColumn(
	IControlView* widget, WidgetID widgetid) const
{
	do {
		InterfacePtr<const IPanelControlData> panelControlData(widget, UseDefaultIID());
		ASSERT(panelControlData);
		if(panelControlData==nil) {
			break;
		}
		
		IControlView* controlView = panelControlData->FindWidget( widgetid );
		ASSERT(controlView);
		if (controlView==nil)
			break;
		
		controlView->Disable();
		
	} while (kFalse);
}

//----------------------------------------------------------------------------------------
// ShowHideExpanderWidget
//----------------------------------------------------------------------------------------
void
CZPAssetsTVWidgetManager::ShowHideExpanderWidget(
	IControlView *				inNodeWidget,
	bool						inShow) const
{
	// If this node doesn't have any children, don't show expander Widget
	InterfacePtr<const IPanelControlData> panelControlData(inNodeWidget, UseDefaultIID());
	ASSERT(panelControlData);
		
	// Set expander widget to hidden state if this node has no kids
	IControlView* expanderWidget = panelControlData->FindWidget( kTreeNodeExpanderWidgetID );
	ASSERT(expanderWidget);
		
	if( inShow )
		expanderWidget->Show();
	else
		expanderWidget->Hide();
}

//----------------------------------------------------------------------------------------
// ShowColumn
//----------------------------------------------------------------------------------------
void
CZPAssetsTVWidgetManager::ShowColumn(
	IControlView* widget, WidgetID widgetid) const
{
	do {
		InterfacePtr<const IPanelControlData> panelControlData(widget, UseDefaultIID());
		ASSERT(panelControlData);
		if(panelControlData==nil) {
			break;
		}
		
		IControlView* controlView = panelControlData->FindWidget( widgetid );
		ASSERT(controlView);
		
		controlView->Show();
	} while (kFalse);
	
}

//----------------------------------------------------------------------------------------
// HideColumn
//----------------------------------------------------------------------------------------
void
CZPAssetsTVWidgetManager::HideColumn(
	IControlView* widget, WidgetID widgetid) const
{
	do {
		InterfacePtr<const IPanelControlData> panelControlData(widget, UseDefaultIID());
		ASSERT(panelControlData);
		if(panelControlData==nil) {
			break;
		}
		
		IControlView* controlView = panelControlData->FindWidget( widgetid );
		ASSERT(controlView);
		
		controlView->Hide();
	} while (kFalse);
	
}

//----------------------------------------------------------------------------------------
// SetIcon
//----------------------------------------------------------------------------------------
void
CZPAssetsTVWidgetManager::SetIcon(
	IControlView*				inNodeWidget,
	WidgetID					inWidgetid,
	const PluginID &			inIconPluginID,
	const RsrcID &				inIconID) const
{
	InterfacePtr<const IPanelControlData> panelControlData( inNodeWidget, UseDefaultIID());
	ASSERT(panelControlData);
	
	IControlView* controlView = panelControlData->FindWidget( inWidgetid );
	ASSERT(controlView);
	
	controlView->SetRsrcPluginID( inIconPluginID );
	controlView->SetRsrcID( inIconID );
}

//----------------------------------------------------------------------------------------
// GetIndentForNode
//----------------------------------------------------------------------------------------
PMReal
CZPAssetsTVWidgetManager::GetIndentForNode(
	const NodeID& node) const
{
	PMReal toReturn(0);
	do
	{
		ZPTreeNodePtr<ZPAssetsTVNodeID> nodeID(node);
		ASSERT(nodeID != nil);
		if (nodeID == nil)
			break;

		if (nodeID->GetNodeType() == eTVNodeAsset )
			toReturn = 10.0;
	}while(kFalse);

	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetNodeWidgetWidth
//----------------------------------------------------------------------------------------
PMReal
CZPAssetsTVWidgetManager::GetNodeWidgetWidth(
	const NodeID&				node) const
{
	PMReal toReturn = mColumnsPref->GetTotalWidthOfVisibleColumns();
	return toReturn;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// FindHeaderControlView
//----------------------------------------------------------------------------------------
const IControlView *
CZPAssetsTVWidgetManager::FindHeaderControlView()const
{
	InterfacePtr<const IWidgetParent> selfWParent(this, UseDefaultIID());
	ASSERT( selfWParent );
	
	const IPMUnknown * parentOfTreeView = selfWParent->GetParent();

	InterfacePtr<const IPanelControlData> tvParentPanelCtrlData( parentOfTreeView, UseDefaultIID() );
	ASSERT( tvParentPanelCtrlData );

	InterfacePtr<const IAssociatedWidgetAttributes> selfLinkedWidgetID( this, IID_ILINKEDWIDGETATTRIBUTES );
	ASSERT( selfLinkedWidgetID );

	ASSERT( selfLinkedWidgetID->GetAssociatedWidgetID() != kInvalidWidgetID );

	const IControlView * headerControlView = tvParentPanelCtrlData->FindWidget( selfLinkedWidgetID->GetAssociatedWidgetID() );
	return headerControlView;
}
