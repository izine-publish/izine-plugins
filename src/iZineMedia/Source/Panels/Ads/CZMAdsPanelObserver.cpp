//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Panels/Ads/CZMAdsPanelObserver.cpp $
//	$Revision: 2913 $
//	$Date: 2011-04-29 16:48:36 +0200 (Fri, 29 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 6-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IBoolData.h"
#include "IBooleanControlData.h"
#include "IDropDownListController.h"
#include "IPanelControlData.h"
#include "IStringData.h"
#include "IStringListControlData.h"
#include "ISubject.h"
#include "ISysFileData.h"
#include "ITreeViewController.h"

//SDK General includes
#include "CWidgetObserver.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "iZMediaID.h"

//IZP Interfaces
#include "IZMAdFileList.h"
#include "IZMAdsTVDataModel.h"
#include "IZMDataObjController.h"
#include "IZMUITVNodeData.h"
#include "IZMModelController.h"

//IZP General includes
#include "AZMDataObjPopupController.h"
#include "ZMDataHelper.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_PanelAssets
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CWidgetObserver
class CZMAdsPanelObserver : public inheritClass
{
public:
						CZMAdsPanelObserver(IPMUnknown * boss);
	virtual				~CZMAdsPanelObserver();

	void				AutoAttach();
	void				AutoDetach();
	void				Update(
							const ClassID&				theChange,
							ISubject*					theSubject,
							const PMIID &				protocol,
							void*						changedBy);
	
	void				DoAttachDetach(
							const WidgetID&				widgetID,
							const PMIID&				pmiid,
							bool16						attaching);
	
protected:
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
	
	void				RefreshEditionPopup();
	void				RefreshAdsTreeView();

	void				DoPanelRefresh();
	
	PMString			DataObjPopGetSelectedID(
							WidgetID					inWidgetID,
							bool						inEmptyIfNotCurrentlySelected = false ) const;
private:

	void				Initialize();
};


CREATE_PMINTERFACE(CZMAdsPanelObserver, kZMAdsPanelObserverImpl)

CZMAdsPanelObserver::CZMAdsPanelObserver(IPMUnknown * boss)
: inheritClass(boss)
{
	Initialize();
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZMAdsPanelObserver::~CZMAdsPanelObserver()
{

}

#pragma mark -

//--------------------------------------------------------------------------
// AutoAttach
//--------------------------------------------------------------------------
void
CZMAdsPanelObserver::AutoAttach()
{
	do
	{
		this->DoAttachDetach(kZMUIAdsPanelRefreshBtnWidgetID, IBooleanControlData::kDefaultIID, kTrue);
		this->DoAttachDetach( kZMUIAdsPanelTitleDropDownWidgetID, IStringListControlData::kDefaultIID, kTrue);
		this->DoAttachDetach( kZMUIAdsPanelEditionDropDownWidgetID, IStringListControlData::kDefaultIID, kTrue);
		this->DoAttachDetach( kZMUIAdsPanelLstAdsWidgetID, ITreeViewController::kDefaultIID, kTrue);

		this->HandleUserLoginDone();
		
	} while (false);
}

//----------------------------------------------------------------------------------------
// DoAttachDetach
//----------------------------------------------------------------------------------------
void 
CZMAdsPanelObserver::DoAttachDetach(
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

//--------------------------------------------------------------------------
// AutoDetach
//--------------------------------------------------------------------------
void
CZMAdsPanelObserver::AutoDetach()
{
	do
	{
		this->DoAttachDetach(kZMUIAdsPanelRefreshBtnWidgetID, IBooleanControlData::kDefaultIID, kFalse);
		//Don't Detach these widgets, as they broadcast messages even when not visible in UI.
		//kZMUIAdsPanelTitleDropDownWidgetID
		//kZMUIAdsPanelEditionDropDownWidgetID

		this->DoAttachDetach( kZMUIAdsPanelLstAdsWidgetID, ITreeViewController::kDefaultIID, kFalse);

	} while (false);
	
}

#pragma mark -
//--------------------------------------------------------------------------
// Update
//--------------------------------------------------------------------------
void
CZMAdsPanelObserver::Update(
	const ClassID& theChange,
	ISubject* theSubject,
	const PMIID &protocol,
	void* changedBy)
{
	LogFunctionEnterExit;
	
	do
	{
		///if ( theChange == kZMUIAdsTV_SelectionChangedMsg )
		///{
		///	this->HandleTreeSelectionChange( theChange, theSubject, protocol, changedBy );
		///}
		///else
			if ( theChange == kZMUIDataObjDropDown_ListLengthChangedMsg )
		{
			this->HandlePopupListChange( theChange, theSubject, protocol, changedBy );
		}
		else if ( theChange == kZMUIDataObjDropDown_SelectionChangedMsg )
		{
			this->HandlePopupSelectionChange( theChange, theSubject, protocol, changedBy );
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
				case kZMUIAdsPanelRefreshBtnWidgetID:
					this->DoPanelRefresh();
				break;
			}
		}
	}while(kFalse);
}

#pragma mark -
void
CZMAdsPanelObserver::Initialize()
{
	LogFunctionEnterExit;
	
	do
	{
#ifdef DEBUG
		char version[20] = {0};
		sprintf(version, "%d.%02d.%02dD", kIZMediaMajorVersion, kIZMediaMinorVersion, kIZMediaBuildNumber);
#else
		char version[20] = {0};
		sprintf(version, "%d.%02d.%02d", kIZMediaMajorVersion, kIZMediaMinorVersion, kIZMediaBuildNumber);
#endif
		PMString strVersion(version);
		strVersion.SetTranslatable(kFalse);

		this->SetTextControlData( kZMUIAdsPanelVersionWidgetID, strVersion );
		///this->EnableDisableControls( false );	//Controls comes as enabled after app crash.
	}while(kFalse);
}

//----------------------------------------------------------------------------------------
// HandleUserLoginDone
//----------------------------------------------------------------------------------------
void
CZMAdsPanelObserver::HandleUserLoginDone()
{
	this->EnableDisableControls( true );
}

//----------------------------------------------------------------------------------------
// HandlePopupListChange
//----------------------------------------------------------------------------------------
void
CZMAdsPanelObserver::HandlePopupListChange(
	const ClassID&				theChange,
	ISubject*					theSubject,
	const PMIID &				protocol,
	void*						changedBy)
{
	LogFunctionEnterExit;
	InterfacePtr<IControlView> controlView(theSubject, UseDefaultIID());
	ASSERT(controlView);

	WidgetID theChangedWidgetID = controlView->GetWidgetID();
	if( theChangedWidgetID.Get() == kZMUIAdsPanelTitleDropDownWidgetID )
	{	//Refresh the edition popup.
		//this->RefreshEditionPopup();	//Commented as It should be done on selection change only.
	}
	else if( theChangedWidgetID.Get() == kZMUIAdsPanelEditionDropDownWidgetID )
	{	//Refresh the Ads treeview
		//this->RefreshAdsTreeView();	//Commented as It should be done on selection change only.
	}
}


//----------------------------------------------------------------------------------------
// HandlePopupSelectionChange
//----------------------------------------------------------------------------------------
void
CZMAdsPanelObserver::HandlePopupSelectionChange(
	const ClassID&				theChange,
	ISubject*					theSubject,
	const PMIID &				protocol,
	void*						changedBy)
{
	LogFunctionEnterExit;

	InterfacePtr<IControlView> controlView(theSubject, UseDefaultIID());
	ASSERT(controlView);

	WidgetID theChangedWidgetID = controlView->GetWidgetID();
	if( theChangedWidgetID.Get() == kZMUIAdsPanelTitleDropDownWidgetID )
	{
		this->RefreshEditionPopup();
		//TODO: we should disable tree view till editions popup is not refreshed.
		// as the tree view might be showing data of some other title.
		this->RefreshAdsTreeView();
	}
	else if( theChangedWidgetID.Get() == kZMUIAdsPanelEditionDropDownWidgetID )
	{	//Refresh the Ads treeview
		this->RefreshAdsTreeView();
	}

}

//----------------------------------------------------------------------------------------
// HandleTreeSelectionChange
//----------------------------------------------------------------------------------------
void
CZMAdsPanelObserver::HandleTreeSelectionChange(
	const ClassID&				theChange,
	ISubject*					theSubject,
	const PMIID &				protocol,
	void*						changedBy)
{
	do
	{
		IControlView * nodeControlView = reinterpret_cast<IControlView*>(changedBy);

		if( !nodeControlView )
			break;

		ClassID theWidgetClass = ::GetClass( nodeControlView );
		if( theWidgetClass != kZMAdsTVAdNodeWidgetBoss )
			break;

		InterfacePtr<const IZMUITVNodeData> nodeData( nodeControlView, UseDefaultIID());
		ASSERT(nodeData);
		const IStringData * assetID = static_cast<const IStringData *>(nodeData->GetDataObj());
		
		//TODO: Do we need something to do on selection change.
		return;
	}while(kFalse);
}

#pragma mark -
//----------------------------------------------------------------------------------------
// DoPanelRefresh
//----------------------------------------------------------------------------------------
void
CZMAdsPanelObserver::DoPanelRefresh()
{
	//Mark Current Ad file objects as dirty, so that they are parsed again.
	IControlView * treeControlView = this->GetWidgetView( kZMUIAdsPanelLstAdsWidgetID );
	InterfacePtr<IZMAdsTVDataModel> treeViewModel( treeControlView, UseDefaultIID() );
	ASSERT( treeViewModel );
	treeViewModel->MarkAllAdFileDirty();


	//Refresh only the assets list for now.
	//Someday: refresh the edition list and title list as well.
	const PMString theEditionID = this->GetSelectedEditionID();

	if( theEditionID.IsEmpty() )
		return;	//Should we clear the tree here?
	const IStringData * editionID = ZMDataHelper::GetEditionByID( theEditionID );

	if( !editionID )
		return;	//Should we clear the tree here?
	
	InterfacePtr<IZMDataObjController> editionController( editionID, UseDefaultIID() );
	ASSERT( editionController );
	editionController->RequestUpdateFromServer( IZMAdFileList::kDefaultIID );

	//Also update the title & edition list.
	//Doing it after updating the selected edition.
	InterfacePtr<IZMModelController> modelController( gSession, UseDefaultIID() );
	ASSERT( modelController );

	modelController->FetchTitleList();
}

//----------------------------------------------------------------------------------------
// RefreshEditionPopup
//----------------------------------------------------------------------------------------
void
CZMAdsPanelObserver::RefreshEditionPopup()
{
	LogFunctionEnterExit;

	const PMString theTitleID = this->GetSelectedTitleID();
	//if( theTitleID.empty() == kFalse )
	{
		InterfacePtr<IStringData> editionTitleID( this->GetWidgetView(kZMUIAdsPanelEditionDropDownWidgetID), IID_IZMTITLEIDSTRINGDATA );
		ASSERT(editionTitleID);
		editionTitleID->Set( theTitleID );
		
		InterfacePtr<AZMDataObjPopupController> editionController( editionTitleID, IZMDataObjPopupController::kDefaultIID );
		ASSERT( editionController );

		editionController->Refresh();
	}
}

//----------------------------------------------------------------------------------------
// RefreshAdsTreeView
//----------------------------------------------------------------------------------------
void
CZMAdsPanelObserver::RefreshAdsTreeView()
{
	LogFunctionEnterExit;

	const PMString theTitleID = this->GetSelectedTitleID();
	const PMString theEditionID = this->GetSelectedEditionID();
	
	IControlView * treeControlView = this->GetWidgetView( kZMUIAdsPanelLstAdsWidgetID );
	
	InterfacePtr<IZMAdsTVDataModel> treeViewModel( treeControlView, UseDefaultIID() );
	ASSERT( treeViewModel );

	treeViewModel->UpdateData( theTitleID, theEditionID );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetSelectedTitleID
//----------------------------------------------------------------------------------------
PMString
CZMAdsPanelObserver::GetSelectedTitleID()const
{
	return this->DataObjPopGetSelectedID(kZMUIAdsPanelTitleDropDownWidgetID, true);
}

//----------------------------------------------------------------------------------------
// GetSelectedEditionID
//----------------------------------------------------------------------------------------
PMString
CZMAdsPanelObserver::GetSelectedEditionID()const
{
	return this->DataObjPopGetSelectedID(kZMUIAdsPanelEditionDropDownWidgetID, true);
}

//----------------------------------------------------------------------------------------
// DataObjPopGetSelectedID
//----------------------------------------------------------------------------------------
PMString
CZMAdsPanelObserver::DataObjPopGetSelectedID(
	WidgetID					inWidgetID,
	bool						inEmptyIfNotCurrentlySelected ) const
{
	InterfacePtr<const IStringData> selectedID( this->GetWidgetView(inWidgetID), IID_IZMUISELECTEDID );
	ASSERT(selectedID);

	InterfacePtr<IDropDownListController> popupListController( selectedID, UseDefaultIID());
	ASSERT(popupListController);
	
	int32 selectedIndex = popupListController->GetSelected();

	if( !inEmptyIfNotCurrentlySelected || selectedIndex >= 0 )
		return selectedID->Get();
	else
		return kNullString;

}

//---------------------------------------------------------------------------------------
// EnableDisableControls
//----------------------------------------------------------------------------------------
void
CZMAdsPanelObserver::EnableDisableControls(
	bool						inEnable )
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
CZMAdsPanelObserver::GetWidgetView(
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
