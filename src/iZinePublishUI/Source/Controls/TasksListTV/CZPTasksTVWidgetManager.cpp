//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/TasksListTV/CZPTasksTVWidgetManager.cpp $
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
#include "IAssociatedWidgetAttributes.h"
#include "IIntData.h"
#include "IObserver.h"
#include "IStringData.h"
#include "ISubject.h"
#include "IWidgetParent.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes
#include "CoreResTypes.h"
#include "CTreeViewWidgetMgr.h"
#include "LocaleSetting.h"

//OS / ThirdParty includes

#include "IZPUIID.h"
#include "IZPID.h"

//IZP Interfaces
#include "IZPApp.h"
#include "IZPTaskMgr.h"
#include "IZPTVColumnsPref.h"
#include "IZPUITVNodeData.h"
#include "IZPUITVWithHdrNodeWidgetMgr.h"

//IZP General includes
#include "ZPTasksTVNodeID.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_TasksTV
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CTreeViewWidgetMgr
class CZPTasksTVWidgetManager : public inheritClass
{
public:
						CZPTasksTVWidgetManager(IPMUnknown * boss);
	virtual				~CZPTasksTVWidgetManager();

	virtual	IControlView* CreateWidgetForNode(
							const NodeID&				node) const;
	virtual	WidgetID	GetWidgetTypeForNode(
							const NodeID&				node) const;
	virtual	bool16		ApplyNodeIDToWidget(
							const NodeID& node,  IControlView* widget, int32 message = 0) const;
	PMReal				GetNodeWidgetWidth(
							const NodeID&				node) const;

protected:
	RsrcSpec			GetResourceForTaskNode() const;

	bool16				ApplyTaskNodeToWidget(
							const PMString &			inTaskID,
							IControlView *				inNodeWidget) const;

	void				AttachToWidget(
							IControlView *				inNodeWidget) const;

	const IControlView * FindHeaderControlView() const;
				
private:
	InterfacePtr<const IZPTVColumnsPref>	mColumnsPref;
};


CREATE_PMINTERFACE(CZPTasksTVWidgetManager, kZPTasksTVWidgetManagerImpl)

CZPTasksTVWidgetManager::CZPTasksTVWidgetManager(IPMUnknown * boss)
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
CZPTasksTVWidgetManager::~CZPTasksTVWidgetManager()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetWidgetTypeForNode
//----------------------------------------------------------------------------------------
WidgetID
CZPTasksTVWidgetManager::GetWidgetTypeForNode(
	const NodeID& node) const
{
	do
	{
		ZPTreeNodePtr<ZPTasksTVNodeID> nodeID(node);
		ASSERT(nodeID != nil);
		if (nodeID == nil)
			break;

		NodeIDClass::NodeType nodeType = nodeID->GetNodeType();

		return kZPUITasksTVNodeWidgetID;
	}while(false);

	return kInvalidWidgetID;
}

//----------------------------------------------------------------------------------------
// CreateWidgetForNode
//----------------------------------------------------------------------------------------
IControlView*
CZPTasksTVWidgetManager::CreateWidgetForNode(
	const NodeID& node) const
{
	LogFunctionEnterExit;
	
	IControlView* toReturn = NULL;
	do
	{
		ZPTreeNodePtr<ZPTasksTVNodeID> nodeID(node);
		ASSERT(nodeID != nil);
		if (nodeID == nil)
			break;

		const PMString & nodeIDStr = nodeID->GetID();
		
		if (nodeID->GetNodeType() == eTVNodeTask)
		{
			toReturn = (IControlView*) ::CreateObject( ::GetDataBase(this), this->GetResourceForTaskNode(), IID_ICONTROLVIEW);
	
			//InterfacePtr<IZPUITVWithHdrNodeWidgetMgr> nodeWdgtMgr(toReturn, UseDefaultIID());
			//ASSERT(nodeWdgtMgr);
			//nodeWdgtMgr->AddRemoveColumnWidgets( mColumnsPref );	//Commented as it is done during refresh
		}
		
	} while (kFalse);
	
	ASSERT(toReturn);
	return toReturn;
}

//----------------------------------------------------------------------------------------
// ApplyNodeIDToWidget
//----------------------------------------------------------------------------------------
bool16
CZPTasksTVWidgetManager::ApplyNodeIDToWidget(
	const NodeID& node, IControlView* widget, int32 message) const
{
	CTreeViewWidgetMgr::ApplyNodeIDToWidget(node, widget);
	bool16 toReturn = kFalse;
	do
	{
		ZPTreeNodePtr<ZPTasksTVNodeID> nodeID(node);
		ASSERT(nodeID != nil);
		if (nodeID == nil)
			break;

		const PMString & nodeIDStr = nodeID->GetID();
		ClassID theWidgetClass = ::GetClass( widget );

		//Set the row widget width according to header width.
		//TODO:

		if (nodeID->GetNodeType() == eTVNodeTask )//&& theWidgetClass == kZPTasksTVNodeWidgetBoss )
			toReturn = this->ApplyTaskNodeToWidget( nodeIDStr, widget );

	}while( false);

	return toReturn;
}

//----------------------------------------------------------------------------------------
// ApplyTaskNodeToWidget
//----------------------------------------------------------------------------------------
bool16
CZPTasksTVWidgetManager::ApplyTaskNodeToWidget(
	const PMString &			inTaskID,
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

	InterfacePtr<const IZPTaskMgr> taskMgr( zpApp->GetTaskMgr(), UseDefaultIID() );
	ASSERT(taskMgr);

	const IStringData * taskID = taskMgr->GetTaskByID( inTaskID );

	nodeData->SetDataObj(taskID);

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
CZPTasksTVWidgetManager::AttachToWidget(
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
// GetResourceForTaskNode
//----------------------------------------------------------------------------------------
RsrcSpec
CZPTasksTVWidgetManager::GetResourceForTaskNode() const
{
	return RsrcSpec(LocaleSetting::GetLocale(),
				kIZPUIPluginID,
				kViewRsrcType,
				kIZPTasksTVTaskNodeWidgetRsrcID );
}

//----------------------------------------------------------------------------------------
// GetNodeWidgetWidth
//----------------------------------------------------------------------------------------
PMReal
CZPTasksTVWidgetManager::GetNodeWidgetWidth(
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
CZPTasksTVWidgetManager::FindHeaderControlView()const
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
