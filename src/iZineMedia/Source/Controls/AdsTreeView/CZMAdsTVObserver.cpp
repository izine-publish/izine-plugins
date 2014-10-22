//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Controls/AdsTreeView/CZMAdsTVObserver.cpp $
//	$Revision: 2759 $
//	$Date: 2011-04-08 14:22:51 +0200 (Fri, 08 Apr 2011) $
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
#include "IApplication.h"
#include "IControlView.h"
#include "IEvent.h"
#include "IPanelControlData.h"
#include "IPanelMgr.h"
#include "ISubject.h"
#include "IStringData.h"
#include "ITreeNodeIDData.h"
#include "ITreeViewController.h"
#include "ITreeViewMgr.h"
#include "IWidgetParent.h"
#include "IWorkspace.h"

//SDK General includes
#include "CAlert.h"
#include "CObserver.h"
#include "K2Vector.tpp" // For NodeIDList to compile
#include "Utils.h"

//OS / ThirdParty includes

#include "iZMediaID.h"

//IZP Interfaces
#include "IZMApp.h"
#include "IZMAdFileList.h"
#include "IZMAdFileMgr.h"
#include "IZMAdsTVDataModel.h"
#include "IZMUITVNodeData.h"

//IZP General includes
#include "ZMAdsTVNodeID.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AssetsTV
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

/** Implements IObserver; initialises the tree when an IObserver::AutoAttach message sent
 and listens for when the node selection changes.
 When the selection changes and there's a non-empty selection list, it takes the first item
 and previews the asset if it can do so (for instance, if it's a GIF, JPEG etc).
 It does this by setting a path on a data interface of the custom-view panel widget and
 invalidating the widget, forcing it to redraw to create a rendering of the new asset.
 
 The class is derived from CObserver, and overrides the
 AutoAttach(), AutoDetach(), and Update() methods.
 This class implements the IObserver interface using the CObserver helper class,
 and is listening along the IID_ITREEVIEWCONTROLLER protocol for changes in the tree data model.
 
 @ingroup paneltreeview
 
 */
#define inheritClass CObserver
class CZMAdsTVObserver : public inheritClass
{
public:
						CZMAdsTVObserver(IPMUnknown * boss);
	virtual				~CZMAdsTVObserver();

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
	void				handleSelectionChanged();
	
	void				HandleEditionAdFileListWillChange();
	void				HandleEditionAdFileListChange();

	void				HandleAdInfoChange(
							ISubject*					inAdSubject );
	
	void				HandleAdFileRowDoubleClick(
							ISubject*					inAdFileNodeSubject ,	//Node
							void*						changedBy );			//Event

	void				HandleAdRowDoubleClick(		//Someday: this handling should be with panel observer
							ISubject*					inAdNodeSubject ,	//Node
							void*						changedBy );			//Event
	
	const PMString &	GetAdIDFromNode(
							ISubject *					inAdNodeSubject  ) const;

	/**	Populates the tree model.
	 */
	void				initTree();

	void				RefreshTree();
private:
};


CREATE_PMINTERFACE(CZMAdsTVObserver, kZMAdsTVObserverImpl)

CZMAdsTVObserver::CZMAdsTVObserver(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZMAdsTVObserver::~CZMAdsTVObserver()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// AutoAttach
//----------------------------------------------------------------------------------------
void
CZMAdsTVObserver::AutoAttach()
{
	// Show/refresh tree when widget made visible
	initTree();
	
	InterfacePtr<ISubject> selfSubject(this, UseDefaultIID());
	
	selfSubject->AttachObserver(this, ITreeViewController::kDefaultIID);

	if( selfSubject->IsAttached( this, IZMAdsTVDataModel::kDefaultIID ) == false )	//Don't Detach it.
		selfSubject->AttachObserver( this, IZMAdsTVDataModel::kDefaultIID );

}

//----------------------------------------------------------------------------------------
// AutoDetach
//----------------------------------------------------------------------------------------
void
CZMAdsTVObserver::AutoDetach()
{
	InterfacePtr<ISubject> subj(this, UseDefaultIID());
	
	subj->DetachObserver(this, ITreeViewController::kDefaultIID);
}

//----------------------------------------------------------------------------------------
// Update
//----------------------------------------------------------------------------------------
void
CZMAdsTVObserver::Update(
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
			this->handleSelectionChanged();
			break;
		}
		case kZMUIAdsTV_ModelChangedMsg:
			this->RefreshTree();
			break;
		case kZMAdFileListWillChangeMsg:
			if( protocol == IZMAdFileList::kDefaultIID )
				this->HandleEditionAdFileListWillChange();
			break;
		case kZMAdFileListChangedMsg:	//From edition
		{
			if( protocol == IZMAdFileList::kDefaultIID )
				this->HandleEditionAdFileListChange();
			break;
		}
		case kZMAdInfoChangedMsg:
			this->HandleAdInfoChange( theSubject );
			break;
//		case kZMUIAdsTV_AdNodeDoubleClickPressedMsg:
//			this->HandleAdRowDoubleClick( theSubject, changedBy );
//			break;
		case kZMUIAdsTV_AdFileNodeDoubleClickPressedMsg:
			this->HandleAdFileRowDoubleClick( theSubject, changedBy );
			break;
	}
}

//----------------------------------------------------------------------------------------
// initTree
//----------------------------------------------------------------------------------------
void
CZMAdsTVObserver::initTree()
{
	LogFunctionEnterExit;
	
	do
	{
	} while(kFalse);
}

//----------------------------------------------------------------------------------------
// RefreshTree
//----------------------------------------------------------------------------------------
void
CZMAdsTVObserver::RefreshTree()
{
	InterfacePtr<ITreeViewMgr> selfMgr(this, UseDefaultIID());
	ASSERT( selfMgr );

	//Save selection

	//Reload
	selfMgr->ChangeRoot( kTrue );

	//Restore selection
}


//----------------------------------------------------------------------------------------
// handleSelectionChanged
//----------------------------------------------------------------------------------------
void
CZMAdsTVObserver::handleSelectionChanged()
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

			ZMTreeNodePtr<ZMAdsTVNodeID> nodeID( firstSelectedNodeID );
			ASSERT(nodeID);
			
			InterfacePtr<ITreeViewMgr> selfTreeMgr( this, UseDefaultIID() );
			ASSERT( selfTreeMgr );
			selectedNodeView.reset( selfTreeMgr->QueryWidgetFromNode( firstSelectedNodeID ) );
		}

		InterfacePtr<ISubject> selfSubject(this, UseDefaultIID());
		ASSERT(selfSubject);

		selfSubject->Change( kZMUIAdsTV_SelectionChangedMsg, ITreeViewController::kDefaultIID, selectedNodeView.get() );
	} while(kFalse);
}

//----------------------------------------------------------------------------------------
// HandleEditionAdFileListWillChange
// Stop updating the Tree till list is changed.
//----------------------------------------------------------------------------------------
void
CZMAdsTVObserver::HandleEditionAdFileListWillChange()
{
	InterfacePtr<IZMAdsTVDataModel> selfModel( this, UseDefaultIID() );
	ASSERT(selfModel);

	selfModel->SetSkipAdFileListUpdates( true );
}

//----------------------------------------------------------------------------------------
// HandleEditionAdFileListChange
//----------------------------------------------------------------------------------------
void
CZMAdsTVObserver::HandleEditionAdFileListChange()
{
	InterfacePtr<IZMAdsTVDataModel> selfModel( this, UseDefaultIID() );
	ASSERT(selfModel);

	selfModel->SetSkipAdFileListUpdates( false );
	selfModel->UpdateAdFileList();

}

//----------------------------------------------------------------------------------------
// HandleAdInfoChange
//----------------------------------------------------------------------------------------
void
CZMAdsTVObserver::HandleAdInfoChange(
	ISubject*					inAdSubject )
{
	InterfacePtr<IZMAdsTVDataModel> selfModel( this, UseDefaultIID() );
	ASSERT(selfModel);

	selfModel->HandleAdInfoChange( inAdSubject );
}

//----------------------------------------------------------------------------------------
// HandleAdFileRowDoubleClick
//----------------------------------------------------------------------------------------
void
CZMAdsTVObserver::HandleAdFileRowDoubleClick(
	ISubject*					inAdFileNodeSubject,	//Node
	void*						changedBy)			//Event
{
	//IEvent * theEvent = reinterpret_cast<IEvent*>(changedBy);
	InterfacePtr<ITreeViewMgr> treeViewMgr( this, UseDefaultIID() );
	ASSERT(treeViewMgr);

	InterfacePtr<const IZMUITVNodeData> adFileNodeData( inAdFileNodeSubject, UseDefaultIID() );
	ASSERT( adFileNodeData );
	
	InterfacePtr<const IStringData> adFileID( adFileNodeData->GetDataObj(), IID_IZMDATAID );
	ASSERT(adFileID);
	const PMString & theAdFileIDStr = adFileID->Get();

	if( treeViewMgr->IsNodeExpanded(ZMAdsTVNodeID::Create( eTVNodeAdFile, theAdFileIDStr )) )
		treeViewMgr->CollapseNode(ZMAdsTVNodeID::Create( eTVNodeAdFile, theAdFileIDStr ), kFalse);
	else
		treeViewMgr->ExpandNode(ZMAdsTVNodeID::Create( eTVNodeAdFile, theAdFileIDStr ), kFalse);
}

//----------------------------------------------------------------------------------------
// HandleAdRowDoubleClick
//----------------------------------------------------------------------------------------
void
CZMAdsTVObserver::HandleAdRowDoubleClick(
	ISubject*					inAdNodeSubject,	//Node
	void*						changedBy)			//Event
{
	IEvent * theEvent = reinterpret_cast<IEvent*>(changedBy);
	if (theEvent && theEvent->OptionAltKeyDown())
	{
		//Do something special
	}
	
	do
	{
		InterfacePtr<const IZMUITVNodeData> assetNodeData( inAdNodeSubject, UseDefaultIID() );
		ASSERT( assetNodeData );
		
		InterfacePtr<const IStringData> assetID( assetNodeData->GetDataObj(), IID_IZMDATAID );
		ASSERT( assetID );
		const PMString & theAdIDStr = assetID->Get();

		//TODO: what to do on Ad row double click?
	} while(kFalse);
}


//----------------------------------------------------------------------------------------
// GetAdIDFromNode
//----------------------------------------------------------------------------------------
const PMString &
CZMAdsTVObserver::GetAdIDFromNode(
	ISubject *					inAdNodeSubject ) const
{
	InterfacePtr<const IZMUITVNodeData> assetNodeData( inAdNodeSubject, UseDefaultIID() );
	ASSERT( assetNodeData );
	
	InterfacePtr<const IStringData> assetID( assetNodeData->GetDataObj(), IID_IZMDATAID );
	ASSERT( assetID );
	if( assetID )
		return assetID->Get();
	else
		return kNullString;
}


