//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Panels/Assets/CZPAssetsPanelObserver.cpp $
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
#include "IBoolData.h"
#include "IBooleanControlData.h"
#include "IPanelControlData.h"
#include "IStringData.h"
#include "IStringListControlData.h"
#include "ISubject.h"
#include "ISysFileData.h"
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
#include "IZPAMHelper.h"
#include "IZPAppController.h"
#include "IZPAssetList.h"
#include "IZPAssetsTVDataModel.h"
#include "IZPDBObjController.h"
#include "IZPDefaultSession.h"
#include "IZPLoginController.h"
#include "IZPPreviewMgr.h"
#include "IZPSessionData.h"
#include "IZPUserCredentials.h"
#include "IZPUITVNodeData.h"

//IZP General includes
#include "AZPDBObjPopupController.h"
#include "ZPDataHelper.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_PanelAssets
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CWidgetObserver
class CZPAssetsPanelObserver : public inheritClass
{
public:
						CZPAssetsPanelObserver(IPMUnknown * boss);
	virtual				~CZPAssetsPanelObserver();

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
	void				RefreshAssetsTreeView();

	void				DoPanelRefresh();

	void				ClearPreview() const;
	void				UpdatePreview(
							const IStringData *			inDBAssetID ) const;
	void				RefreshPreview() const;

	void				UpdateUserName();
private:

	void				Initialize();
	bool				mCheckLoginState;
};


CREATE_PMINTERFACE(CZPAssetsPanelObserver, kZPAssetsPanelObserverImpl)

CZPAssetsPanelObserver::CZPAssetsPanelObserver(IPMUnknown * boss)
: inheritClass(boss)
, mCheckLoginState( true )
{
	Initialize();
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAssetsPanelObserver::~CZPAssetsPanelObserver()
{

}

#pragma mark -

//--------------------------------------------------------------------------
// AutoAttach
//--------------------------------------------------------------------------
void
CZPAssetsPanelObserver::AutoAttach()
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
			loginControllerSubject->AttachObserver(this, IZPLoginController::kDefaultIID);

#if 0	//TODO:
		InterfacePtr<IApplication> app(gSession->QueryApplication());
		InterfacePtr<IDocumentList> documentList(app->QueryDocumentList());
		InterfacePtr<ISubject> documentListSubject( documentList, UseDefaultIID() );
		ASSERT(documentListSubject);
		
		if(!documentListSubject->IsAttached(this,IID_IDOCUMENTLIST, IID_IDOCLISTOBSERVER))
		{
			documentListSubject->AttachObserver(this, IID_IDOCUMENTLIST, IID_IDOCLISTOBSERVER);
		}

		this->DoAttachDetach(kIZPAssetsPanelWidgetTypeDropDownWidgetID, IStringListControlData::kDefaultIID, kTrue);
		this->DoAttachDetach(kIZPAssetsPanelShowDocumentsRadioButtonWidgetID, ITriStateControlData::kDefaultIID, kTrue);
		this->DoAttachDetach(kIZPAssetsPanelShowStoriesRadioButtonWidgetID, ITriStateControlData::kDefaultIID, kTrue);
#endif
		this->DoAttachDetach(kZPUIAssetsPanelRefreshButtonWidgetID, IBooleanControlData::kDefaultIID, kTrue);
		this->DoAttachDetach( kZPUIAssetsPanelTitleDropDownWidgetID, IStringListControlData::kDefaultIID, kTrue);
		this->DoAttachDetach( kZPUIAssetsPanelEditionDropDownWidgetID, IStringListControlData::kDefaultIID, kTrue);
		this->DoAttachDetach( kZPUIAssetsPanelLstAssetsWidgetID, ITreeViewController::kDefaultIID, kTrue);

		InterfacePtr<ISubject> sessionSubject( gSession, UseDefaultIID());
		ASSERT(sessionSubject);
		if( !sessionSubject->IsAttached( this, IZPPreviewMgr::kDefaultIID ) )
			sessionSubject->AttachObserver( this, IZPPreviewMgr::kDefaultIID );

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
	} while (false);
}

//----------------------------------------------------------------------------------------
// DoAttachDetach
//----------------------------------------------------------------------------------------
void 
CZPAssetsPanelObserver::DoAttachDetach(
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
CZPAssetsPanelObserver::AutoDetach()
{
	do
	{
#if 0	//TODO:
		InterfacePtr<IApplication> app(gSession->QueryApplication());
		InterfacePtr<IDocumentList> documentList(app->QueryDocumentList());
		InterfacePtr<ISubject> documentListSubject( documentList, UseDefaultIID() );
		ASSERT(documentListSubject);
		
		if(!documentListSubject->IsAttached(this,IID_IDOCUMENTLIST, IID_IDOCLISTOBSERVER))
		{
			documentListSubject->DetachObserver(this, IID_IDOCUMENTLIST, IID_IDOCLISTOBSERVER);
		}
		
		
		this->DoAttachDetach(kIZPAssetsPanelWidgetTypeDropDownWidgetID, IStringListControlData::kDefaultIID, kFalse);
		this->DoAttachDetach(kIZPAssetsPanelShowDocumentsRadioButtonWidgetID, ITriStateControlData::kDefaultIID, kFalse);
		this->DoAttachDetach(kIZPAssetsPanelShowStoriesRadioButtonWidgetID, ITriStateControlData::kDefaultIID, kFalse);
#endif
		this->DoAttachDetach(kZPUIAssetsPanelRefreshButtonWidgetID, IBooleanControlData::kDefaultIID, kFalse);
		//Don't Detach these widgets, as they broadcast messages even when not visible in UI.
		//kZPUIAssetsPanelTitleDropDownWidgetID
		//kZPUIAssetsPanelEditionDropDownWidgetID

		this->DoAttachDetach( kZPUIAssetsPanelLstAssetsWidgetID, ITreeViewController::kDefaultIID, kFalse);

	} while (false);
	
}

#pragma mark -
//--------------------------------------------------------------------------
// Update
//--------------------------------------------------------------------------
void
CZPAssetsPanelObserver::Update(
	const ClassID& theChange,
	ISubject* theSubject,
	const PMIID &protocol,
	void* changedBy)
{
	LogFunctionEnterExit;
	
	do
	{
		if ( theChange == kZPLogin_LoggedInMsg && protocol == IZPLoginController::kDefaultIID )
		{
			this->HandleUserLoginDone();
		}
		else if ( theChange == kZPLogin_AuthenticationDoneMsg )
		{
			this->DoPanelRefresh();			
		}
		else if( protocol == IZPLoginController::kDefaultIID && theChange != kZPLogin_CanLogoutMsg )
		{	//disable controls if state is not logged in, 
			this->UpdateUserName();
			this->EnableDisableControls( false );
		}
		else if ( theChange == kZPUIAssetTV_SelectionChangedMsg )
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
		else if ( theChange == kZPAssetThumbnailAvailableMsg )
		{
			this->RefreshPreview( );	//TODO: check the asset ID, if waiting for same ID then refresh.
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
				case kZPUIAssetsPanelRefreshButtonWidgetID:
					this->DoPanelRefresh();
				break;
			}
		}
	}while(kFalse);
}

#pragma mark -
void
CZPAssetsPanelObserver::Initialize()
{
	LogFunctionEnterExit;
	
	do
	{
		// Clear User Name
		this->SetTextControlData( kZPUIAssetsPanelUsernameWidgetID, kNullString );
		
#ifdef DEBUG
		char version[30] = {0};
		char svnRev [10];
		strcpy(svnRev, kIZPSVNNumberStr+11 );
		sprintf(version, "%d.%02d.%02d.%sD", kIZPMajorVersion, kIZPMinorVersion, kIZPBuildNumber, endPtrAtSpace(svnRev));
#else
		char version[20] = {0};
		sprintf(version, "%d.%02d.%02d", kIZPMajorVersion, kIZPMinorVersion, kIZPBuildNumber);
#endif
		PMString strVersion(version);
		strVersion.SetTranslatable(kFalse);

		this->SetTextControlData( kZPUIAssetsPanelVersionWidgetID, strVersion );
		this->EnableDisableControls( false );	//Controls comes as enabled after app crash.
	}while(kFalse);
}

//----------------------------------------------------------------------------------------
// HandleUserLoginDone
//----------------------------------------------------------------------------------------
void
CZPAssetsPanelObserver::HandleUserLoginDone()
{
	this->UpdateUserName();
	this->EnableDisableControls( true );
}

//----------------------------------------------------------------------------------------
// UpdateUserName
//----------------------------------------------------------------------------------------
void
CZPAssetsPanelObserver::UpdateUserName()
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IZPDefaultSession> defaultSession( gSession, UseDefaultIID() );
	ASSERT( defaultSession );

	const IZPUserCredentials * defaultCredentials = defaultSession->GetUserCredentials();

	InterfacePtr<const IZPSessionData> sessionData( defaultCredentials, UseDefaultIID() );
	InterfacePtr<const IBoolData> isValidData( defaultCredentials, IID_IZPISVALID );
	
	//Set User Name
	if( sessionData && isValidData && isValidData->Get())
		this->SetTextControlData( kZPUIAssetsPanelUsernameWidgetID, sessionData->GetUserDisplayName() );
	else
		this->SetTextControlData( kZPUIAssetsPanelUsernameWidgetID, kNullString );
}



//----------------------------------------------------------------------------------------
// HandlePopupListChange
//----------------------------------------------------------------------------------------
void
CZPAssetsPanelObserver::HandlePopupListChange(
	const ClassID&				theChange,
	ISubject*					theSubject,
	const PMIID &				protocol,
	void*						changedBy)
{
	LogFunctionEnterExit;
	InterfacePtr<IControlView> controlView(theSubject, UseDefaultIID());
	ASSERT(controlView);

	WidgetID theChangedWidgetID = controlView->GetWidgetID();
	if( theChangedWidgetID.Get() == kZPUIAssetsPanelTitleDropDownWidgetID )
	{	//Refresh the edition popup.
		//this->RefreshEditionPopup();	//Commented as It should be done on selection change only.
	}
	else if( theChangedWidgetID.Get() == kZPUIAssetsPanelEditionDropDownWidgetID )
	{	//Refresh the Assets treeview
		//this->RefreshAssetsTreeView();	//Commented as It should be done on selection change only.
	}
}


//----------------------------------------------------------------------------------------
// HandlePopupSelectionChange
//----------------------------------------------------------------------------------------
void
CZPAssetsPanelObserver::HandlePopupSelectionChange(
	const ClassID&				theChange,
	ISubject*					theSubject,
	const PMIID &				protocol,
	void*						changedBy)
{
	LogFunctionEnterExit;

	InterfacePtr<IControlView> controlView(theSubject, UseDefaultIID());
	ASSERT(controlView);

	WidgetID theChangedWidgetID = controlView->GetWidgetID();
	if( theChangedWidgetID.Get() == kZPUIAssetsPanelTitleDropDownWidgetID )
	{
		this->RefreshEditionPopup();
		//TODO: we should disable tree view till editions popup is not refreshed.
		// as the tree view might be showing data of some other title.
	}
	else if( theChangedWidgetID.Get() == kZPUIAssetsPanelEditionDropDownWidgetID )
	{	//Refresh the Assets treeview
		this->RefreshAssetsTreeView();
	}

}

//----------------------------------------------------------------------------------------
// HandleTreeSelectionChange
//----------------------------------------------------------------------------------------
void
CZPAssetsPanelObserver::HandleTreeSelectionChange(
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
		if( theWidgetClass != kZPAssetsTVAssetNodeWidgetBoss )
			break;

		InterfacePtr<const IZPUITVNodeData> nodeData( nodeControlView, UseDefaultIID());
		ASSERT(nodeData);
		const IStringData * assetID = static_cast<const IStringData *>(nodeData->GetDataObj());
		
		this->UpdatePreview( assetID );
		return;
	}while(kFalse);

	//Clear the preview
	this->ClearPreview();
}

#pragma mark -
//----------------------------------------------------------------------------------------
// DoPanelRefresh
//----------------------------------------------------------------------------------------
void
CZPAssetsPanelObserver::DoPanelRefresh()
{
	//Refresh only the assets list for now.
	//Someday: refresh the edition list and title list as well.
	const PMString theEditionID = this->GetSelectedEditionID();

	if( theEditionID.IsEmpty() )
		return;	//Should we clear the tree here?
	const IStringData * editionID = ZPDataHelper::GetEditionByID( theEditionID );

	if( !editionID )
		return;	//Should we clear the tree here?
	InterfacePtr<IZPDBObjController> editionController( editionID, UseDefaultIID() );
	ASSERT( editionController );
	editionController->RequestUpdateFromServer( IZPAssetList::kDefaultIID );
}

//----------------------------------------------------------------------------------------
// RefreshEditionPopup
//----------------------------------------------------------------------------------------
void
CZPAssetsPanelObserver::RefreshEditionPopup()
{
	LogFunctionEnterExit;

	const PMString theTitleID = this->GetSelectedTitleID();
	//if( theTitleID.empty() == kFalse )
	{
		InterfacePtr<IStringData> editionTitleID( this->GetWidgetView(kZPUIAssetsPanelEditionDropDownWidgetID), IID_IZPTITLEIDSTRINGDATA );
		ASSERT(editionTitleID);
		editionTitleID->Set( theTitleID );
		
		InterfacePtr<AZPDBObjPopupController> editionController( editionTitleID, IZPDBObjPopupController::kDefaultIID );
		ASSERT( editionController );

		editionController->Refresh();
	}
}

//----------------------------------------------------------------------------------------
// RefreshAssetsTreeView
//----------------------------------------------------------------------------------------
void
CZPAssetsPanelObserver::RefreshAssetsTreeView()
{
	LogFunctionEnterExit;

	const PMString theTitleID = this->GetSelectedTitleID();
	const PMString theEditionID = this->GetSelectedEditionID();
	
	IControlView * treeControlView = this->GetWidgetView( kZPUIAssetsPanelLstAssetsWidgetID );
	InterfacePtr<IZPAssetsTVDataModel> treeViewModel( treeControlView, UseDefaultIID() );
	ASSERT( treeViewModel );

	treeViewModel->UpdateData( theTitleID, theEditionID );
}

//----------------------------------------------------------------------------------------
// ClearPreview
//----------------------------------------------------------------------------------------
void
CZPAssetsPanelObserver::ClearPreview() const
{
	InterfacePtr<const IPanelControlData> selfPanelControlData( this, UseDefaultIID());
	ASSERT( selfPanelControlData );
		 
	IControlView* iPreviewWidgetView = selfPanelControlData->FindWidget( kZPUIDocumentsPanelPreviewPanelWidgetID );
	ASSERT( iPreviewWidgetView );

	InterfacePtr<ISysFileData> iSysFileData( iPreviewWidgetView, IID_ISYSFILEDATA );
	ASSERT(iSysFileData);
	
	iSysFileData->Set(IDFile());
	
	iPreviewWidgetView->Invalidate();
}

//----------------------------------------------------------------------------------------
// UpdatePreview
//----------------------------------------------------------------------------------------
void
CZPAssetsPanelObserver::UpdatePreview(
	const IStringData *			inDBAssetID) const
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	do
	{
		IDFile previewFilePath;
		if( !Utils<IZPAMHelper>()->GetPreviewFilePath( inDBAssetID, previewFilePath ) )
		{
			this->ClearPreview();
			break;
		}

		InterfacePtr<const IPanelControlData> selfPanelControlData( this, UseDefaultIID());
		ASSERT( selfPanelControlData );
			 
		IControlView* iPreviewWidgetView = selfPanelControlData->FindWidget( kZPUIDocumentsPanelPreviewPanelWidgetID );
		ASSERT( iPreviewWidgetView );

		InterfacePtr<ISysFileData> iSysFileData( iPreviewWidgetView, IID_ISYSFILEDATA );
		ASSERT(iSysFileData);
		
		iSysFileData->Set( previewFilePath );

		InterfacePtr<IZPPreviewMgr> thePreviewMgr( gSession, UseDefaultIID() );
		ASSERT(thePreviewMgr);

		const PMString & theAssetIDStr = inDBAssetID->Get();
		thePreviewMgr->FetchPreviewForAssetID( theAssetIDStr, previewFilePath );

		iPreviewWidgetView->Invalidate();
	}while(kFalse);
}

//----------------------------------------------------------------------------------------
// RefreshPreview
//----------------------------------------------------------------------------------------
void
CZPAssetsPanelObserver::RefreshPreview()const
{
	InterfacePtr<const IPanelControlData> selfPanelControlData( this, UseDefaultIID());
	ASSERT( selfPanelControlData );
			 
	IControlView* iPreviewWidgetView = selfPanelControlData->FindWidget( kZPUIDocumentsPanelPreviewPanelWidgetID );
	ASSERT( iPreviewWidgetView );

	iPreviewWidgetView->Invalidate();
}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetSelectedTitleID
//----------------------------------------------------------------------------------------
PMString
CZPAssetsPanelObserver::GetSelectedTitleID()const
{
	InterfacePtr<const IStringData> selectedTitleID( this->GetWidgetView(kZPUIAssetsPanelTitleDropDownWidgetID), IID_IZPUISELECTEDID );
	ASSERT(selectedTitleID);

	return selectedTitleID->Get();
}

//----------------------------------------------------------------------------------------
// GetSelectedEditionID
//----------------------------------------------------------------------------------------
PMString
CZPAssetsPanelObserver::GetSelectedEditionID()const
{
	InterfacePtr<const IStringData> selectedEditionID( this->GetWidgetView(kZPUIAssetsPanelEditionDropDownWidgetID), IID_IZPUISELECTEDID );
	ASSERT(selectedEditionID);

	return selectedEditionID->Get();
}

//----------------------------------------------------------------------------------------
// EnableDisableControls
//----------------------------------------------------------------------------------------
void
CZPAssetsPanelObserver::EnableDisableControls(
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
CZPAssetsPanelObserver::GetWidgetView(
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



