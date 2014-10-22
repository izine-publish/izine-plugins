//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Controls/AdsTreeView/CZMAdsTVWidgetManager.cpp $
//	$Revision: 2788 $
//	$Date: 2011-04-11 17:26:11 +0200 (Mon, 11 Apr 2011) $
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
#include "IControlView.h"
#include "IIntData.h"
#include "IObserver.h"
#include "IPanelControlData.h"
#include "IStringData.h"
#include "ISubject.h"
#include "ITextControlData.h"
#include "ITreeViewHierarchyAdapter.h"
#include "IWidgetParent.h"

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

#include "iZMediaID.h"

//IZP Interfaces
#include "IZMApp.h"
#include "IZMAdMgr.h"
#include "IZMAdFileMgr.h"
#include "IZMTitleMgr.h"
#include "IZMTVColumnsPref.h"
#include "IZMUITVNodeData.h"
#include "IZMUITVNodeWidgetMgr.h"
#include "IZMUITVWithHdrNodeWidgetMgr.h"

//IZP General includes
#include "ZMAdsTVNodeID.h"

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
class CZMAdsTVWidgetManager : public inheritClass
{
public:
						CZMAdsTVWidgetManager(IPMUnknown * boss);
	virtual				~CZMAdsTVWidgetManager();

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

	RsrcSpec			GetResourceForAdFileNode() const;
	RsrcSpec			GetResourceForAdNode() const;

	bool16				ApplyAdFileNodeToWidget(
							const PMString &			inAdFileID,
							IControlView *				inNodeWidget ) const;
	bool16				ApplyAdNodeToWidget(
							const PMString &			inAdID,
							IControlView *				inNodeWidget ) const;
	void				ShowHideExpanderWidget(
							IControlView *				inNodeWidget,
							bool						inShow ) const;

	const IControlView * FindHeaderControlView() const;
private:
	
	InterfacePtr<const IZMTVColumnsPref>	mColumnsPref;
};


CREATE_PMINTERFACE(CZMAdsTVWidgetManager, kZMAdsTVWidgetManagerImpl)

CZMAdsTVWidgetManager::CZMAdsTVWidgetManager(IPMUnknown * boss)
: inheritClass(boss)
{
	const IControlView * headerControlView = this->FindHeaderControlView();

	InterfacePtr<const IZMTVColumnsPref> headerColPrefs( headerControlView, UseDefaultIID() );
	ASSERT( headerColPrefs );
	headerColPrefs->AddRef();
	mColumnsPref.reset( headerColPrefs );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZMAdsTVWidgetManager::~CZMAdsTVWidgetManager()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetWidgetTypeForNode
//----------------------------------------------------------------------------------------
WidgetID
CZMAdsTVWidgetManager::GetWidgetTypeForNode(
	const NodeID& node) const
{
	do
	{
		ZMTreeNodePtr<ZMAdsTVNodeID> nodeID(node);
		ASSERT(nodeID != nil);
		if (nodeID == nil)
			break;

		NodeIDClass::NodeType nodeType = nodeID->GetNodeType();

		if (nodeID->GetNodeType() == eTVNodeAd)
		{
			return kZMUIAdsTVAdNodeWidgetID;
		}
		else if (nodeID->GetNodeType() == eTVNodeAdFile)
		{
			return kZMUIAdsTVAdFileNodeWidgetID;
		}
	}while(false);

	return kInvalidWidgetID;
}

//----------------------------------------------------------------------------------------
// CreateWidgetForNode
//----------------------------------------------------------------------------------------
IControlView*
CZMAdsTVWidgetManager::CreateWidgetForNode(
	const NodeID& node) const
{
	LogFunctionEnterExit;
	
	IControlView* toReturn = NULL;
	do
	{
		ZMTreeNodePtr<ZMAdsTVNodeID> nodeID(node);
		ASSERT(nodeID != nil);
		if (nodeID == nil)
			break;

		const PMString & nodeIDStr = nodeID->GetID();
		
		if (nodeID->GetNodeType() == eTVNodeAd)
		{
			toReturn = (IControlView*) ::CreateObject( ::GetDataBase(this), this->GetResourceForAdNode(), IID_ICONTROLVIEW);
			
			//InterfacePtr<IZMUITVWithHdrNodeWidgetMgr> nodeWdgtMgr(toReturn, UseDefaultIID());
			//ASSERT(nodeWdgtMgr);
			//nodeWdgtMgr->AddRemoveColumnWidgets( mColumnsPref );	//Commented as it is done during refresh
		}
		else if (nodeID->GetNodeType() == eTVNodeAdFile)
			toReturn = (IControlView*) ::CreateObject( ::GetDataBase(this), this->GetResourceForAdFileNode(), IID_ICONTROLVIEW);
	} while (kFalse);
	
	ASSERT(toReturn);
	return toReturn;
}

//----------------------------------------------------------------------------------------
// ApplyNodeIDToWidget
//----------------------------------------------------------------------------------------
bool16
CZMAdsTVWidgetManager::ApplyNodeIDToWidget(
	const NodeID& node, IControlView* widget, int32 message) const
{
	bool16 toReturn = kFalse;
	do
	{
		ZMTreeNodePtr<ZMAdsTVNodeID> nodeID(node);
		ASSERT(nodeID != nil);
		if (nodeID == nil)
			break;

		const PMString & nodeIDStr = nodeID->GetID();
		ClassID theWidgetClass = ::GetClass( widget );

		if (nodeID->GetNodeType() == eTVNodeAd && theWidgetClass == kZMAdsTVAdNodeWidgetBoss )
			toReturn = this->ApplyAdNodeToWidget( nodeIDStr, widget );
		else if (nodeID->GetNodeType() == eTVNodeAdFile && theWidgetClass == kZMAdsTVAdFileNodeWidgetBoss )
			toReturn = this->ApplyAdFileNodeToWidget( nodeIDStr, widget );

	}while( false);
	CTreeViewWidgetMgr::ApplyNodeIDToWidget(node, widget);

	return toReturn;
}

//----------------------------------------------------------------------------------------
// ApplyAdFileNodeToWidget
//----------------------------------------------------------------------------------------
bool16
CZMAdsTVWidgetManager::ApplyAdFileNodeToWidget(
	const PMString &			inAdFileID,
	IControlView *				inNodeWidget) const
{
	InterfacePtr<const IZMApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<const IZMAdFileMgr> adFileMgr( zpApp->GetAdFileMgr(), UseDefaultIID() );
	ASSERT(adFileMgr);

	const IStringData * adFileID = adFileMgr->GetAdFileByID( inAdFileID );
	ASSERT( adFileID );

	//store statusID with node and then applyit.
	InterfacePtr<IZMUITVNodeData> nodeData(inNodeWidget, UseDefaultIID());
	ASSERT(nodeData);
	nodeData->SetDataObj( adFileID );

	InterfacePtr<IZMUITVWithHdrNodeWidgetMgr> nodeWdgtMgr(inNodeWidget, UseDefaultIID());
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
// ApplyAdNodeToWidget
//----------------------------------------------------------------------------------------
bool16
CZMAdsTVWidgetManager::ApplyAdNodeToWidget(
	const PMString &			inAdID,
	IControlView *				inNodeWidget) const
{
	InterfacePtr<IZMUITVNodeData> nodeData(inNodeWidget, UseDefaultIID());
	ASSERT(nodeData);

	InterfacePtr<const IZMApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<const IZMAdMgr> adMgr( zpApp->GetAdMgr(), UseDefaultIID() );
	ASSERT(adMgr);

	const IStringData * adID = adMgr->GetAdByID( inAdID );

	nodeData->SetDataObj(adID);

	InterfacePtr<IZMUITVWithHdrNodeWidgetMgr> nodeWdgtMgr(inNodeWidget, UseDefaultIID());
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
CZMAdsTVWidgetManager::AttachToWidget(
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
// GetResourceForAdFileNode
//----------------------------------------------------------------------------------------
RsrcSpec
CZMAdsTVWidgetManager::GetResourceForAdFileNode() const
{
	return	RsrcSpec(LocaleSetting::GetLocale(),
				kIZMediaPluginID,
				kViewRsrcType,
				kIZMAdsTVAdFileNodeWidgetRsrcID);
}


//----------------------------------------------------------------------------------------
// GetResourceForAdNode
//----------------------------------------------------------------------------------------
RsrcSpec
CZMAdsTVWidgetManager::GetResourceForAdNode() const
{
	return RsrcSpec(LocaleSetting::GetLocale(),
				kIZMediaPluginID,
				kViewRsrcType,
				kIZMAdsTVAdNodeWidgetRsrcID );
}


#pragma mark -
//----------------------------------------------------------------------------------------
// SetColumnText
//----------------------------------------------------------------------------------------
void
CZMAdsTVWidgetManager::SetColumnText(
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
CZMAdsTVWidgetManager::EnableColumn(
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
CZMAdsTVWidgetManager::DisableColumn(
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
CZMAdsTVWidgetManager::ShowHideExpanderWidget(
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
CZMAdsTVWidgetManager::ShowColumn(
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
CZMAdsTVWidgetManager::HideColumn(
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
CZMAdsTVWidgetManager::SetIcon(
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
CZMAdsTVWidgetManager::GetIndentForNode(
	const NodeID& node) const
{
	PMReal toReturn(0);
	do
	{
		ZMTreeNodePtr<ZMAdsTVNodeID> nodeID(node);
		ASSERT(nodeID != nil);
		if (nodeID == nil)
			break;

		if (nodeID->GetNodeType() == eTVNodeAd )
			toReturn = 10.0;
	}while(kFalse);

	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetNodeWidgetWidth
//----------------------------------------------------------------------------------------
PMReal
CZMAdsTVWidgetManager::GetNodeWidgetWidth(
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
CZMAdsTVWidgetManager::FindHeaderControlView()const
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
