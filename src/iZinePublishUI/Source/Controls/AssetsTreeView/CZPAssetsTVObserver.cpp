//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/AssetsTreeView/CZPAssetsTVObserver.cpp $
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
#include "IApplication.h"
#include "IControlView.h"
#include "IDocFileType.h"
#include "IDocumentCommands.h"
#include "IEvent.h"
#include "IPanelControlData.h"
#include "IPanelMgr.h"
#include "ISubject.h"
#include "IStringData.h"
#include "ISysFileData.h"
#include "ITreeNodeIDData.h"
#include "ITreeViewController.h"
#include "ITreeViewMgr.h"
#include "IUIFlagData.h"
#include "IWidgetParent.h"
#include "IWorkspace.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes
#include "CAlert.h"
#include "CObserver.h"
#include "K2Vector.tpp" // For NodeIDList to compile
#include "SDKFileHelper.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPUIID.h"
#include "IZPID.h"

//IZP Interfaces
#include "IZPAMHelper.h"
#include "IZPAMWSUtils.h"
#include "IZPApp.h"
#include "IZPAssetDocUtils.h"
#include "IZPAssetInfo.h"
#include "IZPAssetList.h"
#include "IZPAssetMgr.h"
#include "IZPAssetsTVDataModel.h"
#include "IZPAssetUtils.h"
#include "IZPDocUtils.h"
#include "IZPUITVNodeData.h"

//IZP General includes
#include "ZPAssetsTVNodeID.h"
#include "ZPDataHelper.h"

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
class CZPAssetsTVObserver : public inheritClass
{
public:
						CZPAssetsTVObserver(IPMUnknown * boss);
	virtual				~CZPAssetsTVObserver();

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
	
	void				HandleEditionAssetListWillChange();
	void				HandleEditionAssetListChange();

	void				HandleAssetInfoChange(
							ISubject*					inAssetSubject );
	
	void				HandleStatusRowDoubleClick(
							ISubject*					inAssetNodeSubject ,	//Node
							void*						changedBy );			//Event

	void				HandleAssetRowDoubleClick(		//Someday: this handling should be with panel observer
							ISubject*					inAssetNodeSubject ,	//Node
							void*						changedBy );			//Event
	
	void				HandleAssetRowSaveBtnPress(			//Someday: this handling should be with panel observer
							ISubject*					inAssetNodeSubject  );
	void				HandleAssetRowDeleteBtnPress(			//Someday: this handling should be with panel observer
							ISubject*					inAssetNodeSubject  );

	const PMString &	GetAssetIDFromNode(
							ISubject *					inAssetNodeSubject  ) const;

	/**	Populates the tree model.
	 */
	void				initTree();

	void				RefreshTree();
private:
};


CREATE_PMINTERFACE(CZPAssetsTVObserver, kZPAssetsTVObserverImpl)

CZPAssetsTVObserver::CZPAssetsTVObserver(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAssetsTVObserver::~CZPAssetsTVObserver()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// AutoAttach
//----------------------------------------------------------------------------------------
void
CZPAssetsTVObserver::AutoAttach()
{
	// Show/refresh tree when widget made visible
	initTree();
	
	InterfacePtr<ISubject> selfSubject(this, UseDefaultIID());
	
	selfSubject->AttachObserver(this, ITreeViewController::kDefaultIID);

	if( selfSubject->IsAttached( this, IZPAssetsTVDataModel::kDefaultIID ) == false )	//Don't Detach it.
		selfSubject->AttachObserver( this, IZPAssetsTVDataModel::kDefaultIID );

}

//----------------------------------------------------------------------------------------
// AutoDetach
//----------------------------------------------------------------------------------------
void
CZPAssetsTVObserver::AutoDetach()
{
	InterfacePtr<ISubject> subj(this, UseDefaultIID());
	
	subj->DetachObserver(this, ITreeViewController::kDefaultIID);
}

//----------------------------------------------------------------------------------------
// Update
//----------------------------------------------------------------------------------------
void
CZPAssetsTVObserver::Update(
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
		case kZPUIAssetTV_ModelChangedMsg:
			this->RefreshTree();
			break;
		case kZPAssetListWillChangeMsg:
			if( protocol == IZPAssetList::kDefaultIID )
				this->HandleEditionAssetListWillChange();
			break;
		case kZPAssetListChangedMsg:	//From edition
		{
			if( protocol == IZPAssetList::kDefaultIID )
				this->HandleEditionAssetListChange();
			break;
		}
		case kZPAssetInfoChangedMsg:
		case kZPDataNameChangedMsg:
			this->HandleAssetInfoChange( theSubject );
			break;
		case kZPUIAssetTV_AssetNodeDoubleClickPressedMsg:
			this->HandleAssetRowDoubleClick( theSubject, changedBy );
			break;
		case kZPUIAssetTV_AssetNodeSaveBtnPressedMsg:
			this->HandleAssetRowSaveBtnPress( theSubject );
			break;
		case kZPUIAssetTV_AssetNodeDeleteBtnPressedMsg:
			this->HandleAssetRowDeleteBtnPress( theSubject );
			break;
		case kZPUIAssetTV_StatusNodeDoubleClickPressedMsg:
			this->HandleStatusRowDoubleClick( theSubject, changedBy );
			break;
	}
}

//----------------------------------------------------------------------------------------
// initTree
//----------------------------------------------------------------------------------------
void
CZPAssetsTVObserver::initTree()
{
	LogFunctionEnterExit;
	
	do
	{
		this->RefreshTree();
	} while(kFalse);
}

//----------------------------------------------------------------------------------------
// RefreshTree
//----------------------------------------------------------------------------------------
void
CZPAssetsTVObserver::RefreshTree()
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
CZPAssetsTVObserver::handleSelectionChanged()
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

			ZPTreeNodePtr<ZPAssetsTVNodeID> nodeID( firstSelectedNodeID );
			ASSERT(nodeID);
			
			InterfacePtr<ITreeViewMgr> selfTreeMgr( this, UseDefaultIID() );
			ASSERT( selfTreeMgr );
			selectedNodeView.reset( selfTreeMgr->QueryWidgetFromNode( firstSelectedNodeID ) );
		}

		InterfacePtr<ISubject> selfSubject(this, UseDefaultIID());
		ASSERT(selfSubject);

		selfSubject->Change( kZPUIAssetTV_SelectionChangedMsg, ITreeViewController::kDefaultIID, selectedNodeView.get() );
	} while(kFalse);
}

//----------------------------------------------------------------------------------------
// HandleEditionAssetListWillChange
// Stop updating the Tree till list is changed.
//----------------------------------------------------------------------------------------
void
CZPAssetsTVObserver::HandleEditionAssetListWillChange()
{
	InterfacePtr<IZPAssetsTVDataModel> selfModel( this, UseDefaultIID() );
	ASSERT(selfModel);

	selfModel->SetSkipAssetListUpdates( true );
}

//----------------------------------------------------------------------------------------
// HandleEditionAssetListChange
//----------------------------------------------------------------------------------------
void
CZPAssetsTVObserver::HandleEditionAssetListChange()
{
	InterfacePtr<IZPAssetsTVDataModel> selfModel( this, UseDefaultIID() );
	ASSERT(selfModel);

	selfModel->SetSkipAssetListUpdates( false );
	selfModel->UpdateAssetList();

}

//----------------------------------------------------------------------------------------
// HandleAssetInfoChange
//----------------------------------------------------------------------------------------
void
CZPAssetsTVObserver::HandleAssetInfoChange(
	ISubject*					inAssetSubject )
{
	InterfacePtr<IZPAssetsTVDataModel> selfModel( this, UseDefaultIID() );
	ASSERT(selfModel);

	selfModel->HandleAssetInfoChange( inAssetSubject );
}

//----------------------------------------------------------------------------------------
// HandleStatusRowDoubleClick
//----------------------------------------------------------------------------------------
void
CZPAssetsTVObserver::HandleStatusRowDoubleClick(
	ISubject*					inAssetNodeSubject,	//Node
	void*						changedBy)			//Event
{
	//IEvent * theEvent = reinterpret_cast<IEvent*>(changedBy);
	InterfacePtr<ITreeViewMgr> treeViewMgr( this, UseDefaultIID() );
	ASSERT(treeViewMgr);

	InterfacePtr<const IZPUITVNodeData> statusNodeData( inAssetNodeSubject, UseDefaultIID() );
	ASSERT( statusNodeData );
	
	InterfacePtr<const IStringData> statusID( statusNodeData->GetDataObj(), IID_IZPDATAID );
	ASSERT(statusID);
	const PMString & theStatusIDStr = statusID->Get();

	if( treeViewMgr->IsNodeExpanded(ZPAssetsTVNodeID::Create( eTVNodeStatus, theStatusIDStr )) )
		treeViewMgr->CollapseNode(ZPAssetsTVNodeID::Create( eTVNodeStatus, theStatusIDStr ), kFalse);
	else
		treeViewMgr->ExpandNode(ZPAssetsTVNodeID::Create( eTVNodeStatus, theStatusIDStr ), kFalse);
}

//----------------------------------------------------------------------------------------
// HandleAssetRowDoubleClick
//----------------------------------------------------------------------------------------
void
CZPAssetsTVObserver::HandleAssetRowDoubleClick(
	ISubject*					inAssetNodeSubject,	//Node
	void*						changedBy)			//Event
{
	bool bOpenTemplateOriginal = false;

	IEvent * theEvent = reinterpret_cast<IEvent*>(changedBy);
	if (theEvent && theEvent->OptionAltKeyDown())
	{
		// open template original
		bOpenTemplateOriginal = true;
	}
	
	do
	{
		InterfacePtr<const IZPUITVNodeData> assetNodeData( inAssetNodeSubject, UseDefaultIID() );
		ASSERT( assetNodeData );
		
		InterfacePtr<const IStringData> assetID( assetNodeData->GetDataObj(), IID_IZPDATAID );
		ASSERT( assetID );
		const PMString & theAssetIDStr = assetID->Get();

		//Someday: Some (designing) status were not allowed to be modified/open in InCopy.
		//Do we need that still? As we don't have fixed status values.
		
		if( false == Utils<IZPAssetUtils>()->CanOpenAsset( theAssetIDStr ) )
			break;

		Utils<IZPAMHelper>()->ProcessAssetForOpening( assetID , bOpenTemplateOriginal);
	} while(kFalse);
}


//----------------------------------------------------------------------------------------
// HandleAssetRowSaveBtnPress
//----------------------------------------------------------------------------------------
void
CZPAssetsTVObserver::HandleAssetRowSaveBtnPress(
	ISubject*					inAssetNodeSubject  )		//Node
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	do
	{
		const PMString & theAssetIDStr = this->GetAssetIDFromNode( inAssetNodeSubject );
		InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
		ASSERT( zpApp );

		InterfacePtr<const IZPAssetMgr> assetMgr( zpApp->GetAssetMgr(), UseDefaultIID() );
		ASSERT(assetMgr);

		const IStringData * dbAssetID = ZPDataHelper::GetAssetByID( theAssetIDStr );
		if( !dbAssetID )
			break;

		InterfacePtr<const IStringData> dbAssetName( dbAssetID, IID_IZPDATANAME );
		ASSERT( dbAssetName );
		PMString name = dbAssetName->Get();

		SDKFileSaveChooser fileChooser;

#if defined(InDnCS5) || defined(InDnCS5_5)
		PMString inddFilter("InDesign document (indd)", PMString::kUnknownEncoding);
		fileChooser.AddFilter(kInDesignFileTypeInfoID, inddFilter);
		
		PMString title("Export asset", PMString::kUnknownEncoding);		
		fileChooser.SetTitle(title);
#else
		fileChooser.AddFilter(kInDesignFileTypeInfoID, PMString("InDesign document (indd)", -1, PMString::kNoTranslate));
		fileChooser.SetTitle(PMString("Export asset", -1, PMString::kNoTranslate));
#endif
		fileChooser.SetFilename(name);
		fileChooser.ShowDialog();
		if (!fileChooser.IsChosen())
			break;
		
		IDFile file = fileChooser.GetIDFile();

		IDFile filePath;
		Utils<IZPAMHelper>()->GetAssetFilePath( dbAssetID, filePath, true );
		Utils<IZPAMHelper>()->CheckForHeadAndDownloadIfNeeded( dbAssetID, filePath, false );

		bool flagCurrentlyDocOpened = false;
		IDocument* currentDoc = Utils<IZPAssetDocUtils>()->GetDocumentForDocAssetID( theAssetIDStr );

		UIDRef theOpenedDocRef;
		if(currentDoc == nil)
		{
			IOpenFileCmdData::OpenFlags openFlags = IOpenFileCmdData::kOpenOriginal;
			ErrorCode errorCode = Utils<IZPDocUtils>()->OpenDocumentFile( filePath, openFlags, theOpenedDocRef );
			if(errorCode == kSuccess)
			{
				InterfacePtr<IDocument> document(theOpenedDocRef, UseDefaultIID());
				currentDoc = document;
			}
		}
		else
		{
			flagCurrentlyDocOpened = true;
			theOpenedDocRef = ::GetUIDRef(currentDoc);
		}

		if(currentDoc)
		{
			Utils<IZPDocUtils>()->SaveACopy( currentDoc, file, kSuppressUI, kInDesignFileTypeInfoID);

			if(flagCurrentlyDocOpened == false)
			{
				InterfacePtr<ICommand> closeDoc(Utils<IDocumentCommands>()->CreateCloseCommand(theOpenedDocRef, kSuppressUI));
				CmdUtils::ScheduleCommand(closeDoc);
			}
		}
	}while(false);
}

//----------------------------------------------------------------------------------------
// HandleAssetRowDeleteBtnPress
//----------------------------------------------------------------------------------------
void
CZPAssetsTVObserver::HandleAssetRowDeleteBtnPress(
	ISubject*					inAssetNodeSubject )		//Node
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	int16 selectedResult = CAlert::ModalAlert(
		kIZPManualDeleteAssetAlertKey,	// Alert string
		kYesString,					// Yes button
		kNoString,					// No button
		kNullString,				// No third button
		2,							// Set No button to default
		CAlert::eQuestionIcon		// Question icon.
		);

	if (selectedResult != 1)	//Yes not selected
		return;

	//Delete the asset from server.
	const PMString & theAssetIDStr = this->GetAssetIDFromNode( inAssetNodeSubject );
	InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<const IZPAssetMgr> assetMgr( zpApp->GetAssetMgr(), UseDefaultIID() );
	ASSERT(assetMgr);

	assetMgr->DeleteAsset( theAssetIDStr );
}

//----------------------------------------------------------------------------------------
// GetAssetIDFromNode
//----------------------------------------------------------------------------------------
const PMString &
CZPAssetsTVObserver::GetAssetIDFromNode(
	ISubject *					inAssetNodeSubject ) const
{
	InterfacePtr<const IZPUITVNodeData> assetNodeData( inAssetNodeSubject, UseDefaultIID() );
	ASSERT( assetNodeData );
	
	InterfacePtr<const IStringData> assetID( assetNodeData->GetDataObj(), IID_IZPDATAID );
	ASSERT( assetID );
	if( assetID )
		return assetID->Get();
	else
		return kNullString;
}


