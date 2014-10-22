//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Dialogs/SaveToRepository/CZPUISaveToRepDialogController.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 1-6-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IBoolData.h"
#include "IDropDownListController.h"
#include "IIntData.h"
#include "IPanelControlData.h"
#include "IStringData.h"

//SDK General includes
#include "CAlert.h"
#include "CDialogController.h"
#include "ErrorUtils.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPUIID.h"
#include "IZPID.h"

//IZP Interfaces
#include "IZPAMWSUtils.h"
#include "IZPAssetCheckInData.h"
#include "IZPAssetData.h"
#include "IZPAssetUtils.h"
#include "IZPInDnUtils.h"

//IZP General includes
#include "AZPDBObjPopupController.h"
#include "ZPTypes.h"
#include "ZPConstants.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_Dialogs
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CDialogController
class CZPUISaveToRepDialogController : public inheritClass
{
public:
						CZPUISaveToRepDialogController(IPMUnknown * boss);
	virtual				~CZPUISaveToRepDialogController();

	/** Initialize each widget in the dialog with its default value.
			Called when the dialog is opened.
		*/
	virtual void		InitializeDialogFields(
							IActiveContext* dlgContext);

	/** Validate the values in the widgets.
		By default, the widget with ID kOKButtonWidgetID causes
		ValidateFields to be called. When all widgets are valid,
		ApplyFields will be called.
		@return kDefaultWidgetId if all widget values are valid, WidgetID of the widget to select otherwise.

	*/
	virtual WidgetID	ValidateDialogFields(
							IActiveContext* myContext);


	/** Retrieve the values from the widgets and act on them.
		@param widgetId identifies the widget on which to act.
	*/
	virtual void		ApplyDialogFields(
							IActiveContext* myContext, const WidgetID& widgetId);
	
protected:
	IControlView *		GetWidgetView(
							const WidgetID &			inWidgetID ) const;

	PMString			GetSelectedTitleID()const;
	PMString			GetSelectedEditionID()const;
	PMString			GetSelectedStatusID()const;

	enAssetType			GetAssetNewType() const;
private:
	bool				mAllowStatusChange;
};


CREATE_PMINTERFACE(CZPUISaveToRepDialogController, kZPUISaveToRepDialogControllerImpl)

CZPUISaveToRepDialogController::CZPUISaveToRepDialogController(IPMUnknown * boss)
: inheritClass(boss)
, mAllowStatusChange( false )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPUISaveToRepDialogController::~CZPUISaveToRepDialogController()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// InitializeDialogFields
//----------------------------------------------------------------------------------------
void
CZPUISaveToRepDialogController::InitializeDialogFields(
	IActiveContext* dlgContext)
{
	LogFunctionEnterExit;
	
	CDialogController::InitializeDialogFields(dlgContext);
	do
	{
		InterfacePtr<const IBoolData> dlgIsNewVersion( this, IID_IZPISNEWVERSION );
		ASSERT( dlgIsNewVersion );

		InterfacePtr<const IZPAssetData> selfAssetData( this, UseDefaultIID());
		ASSERT( selfAssetData );
		
		const PMString & titleID = selfAssetData->GetTitleID();

		InterfacePtr<const IZPAssetCheckInData> selfAssetCheckInData( this, UseDefaultIID() );
		ASSERT( selfAssetCheckInData );
		//Fill Asset name
#if defined(InDnCS5) || defined(InDnCS5_5)
		PMString theName(selfAssetCheckInData->GetAssetName().GrabCString());
		theName.SetTranslatable(kFalse);
#else
		const PMString & theName = selfAssetCheckInData->GetAssetName();
#endif
		this->SetTextControlData(kZPUISaveToRepNameEditBoxWidgetID, theName);

		//Fill description
		const PMString & theDescription = selfAssetCheckInData->GetDescription();
		this->SetTextControlData(kZPUISaveToRepDescriptionEditBoxWidgetID, theDescription);
		
		//IsTemplate checkbox
		InterfacePtr<const IIntData> selfAssetType( this, IID_IZPASSETTYPE );
		ASSERT( selfAssetType );

		enAssetType	assetType = (enAssetType) selfAssetType->Get();

		//2011May30: #1838, don't allow status value to be set for story type of assets.
		mAllowStatusChange = true;
		if( assetType == eAssetType_ICDocument
		|| assetType == eAssetType_ICTemplate
		|| assetType == eAssetType_IDTemplate
		|| assetType == eAssetType_IDStory
		||( assetType == eAssetTypeNone && Utils<IZPInDnUtils>()->IsHostAppInCopy() == kTrue ))
		{
			mAllowStatusChange = false;
		}

		IControlView * isTemplateControlView = this->GetWidgetView( kZPUISaveToRepIsTemplateCheckBoxWidgetID );
		if( assetType == eAssetType_ICAssignment || assetType == eAssetType_IDStory )
		{	//Hide IsTemplate checkbox
			isTemplateControlView->Hide();
		}
		else
		{	//Show IsTemplate checkbox
			isTemplateControlView->Show();
			if( Utils<IZPAssetUtils>()->IsTemplateType( assetType ) )
				SetTriStateControlData(kZPUISaveToRepIsTemplateCheckBoxWidgetID, kTrue );
			else
				SetTriStateControlData(kZPUISaveToRepIsTemplateCheckBoxWidgetID, kFalse );

			isTemplateControlView->Enable( !dlgIsNewVersion->Get() );
		}
		
		//Set the selected IDs before filling
		InterfacePtr<IStringData> selectedTitleID( this->GetWidgetView(kZPUISaveToRepPublicationDropDownWidgetID), IID_IZPUISELECTEDID );
		ASSERT(selectedTitleID);
		if (! selfAssetData->GetTitleID().IsEmpty())
		{	//Only set the selected value if title ID is non-empty,
			//Doing so we can retain the values saved with UI, that user last selected
			selectedTitleID->Set( selfAssetData->GetTitleID() );

			InterfacePtr<IStringData> editionTitleID( this->GetWidgetView(kZPUISaveToRepEditionDropDownWidgetID ), IID_IZPTITLEIDSTRINGDATA );
			ASSERT(editionTitleID);
			editionTitleID->Set( selfAssetData->GetTitleID() );

			InterfacePtr<IStringData> selectedEditionID( this->GetWidgetView(kZPUISaveToRepEditionDropDownWidgetID), IID_IZPUISELECTEDID );
			ASSERT(selectedEditionID);
			selectedEditionID->Set( selfAssetData->GetEditionID() );
			
			InterfacePtr<IStringData> statusTitleID( this->GetWidgetView(kZPUISaveToRepStatusDropDownWidgetID), IID_IZPTITLEIDSTRINGDATA );
			ASSERT(statusTitleID);
			statusTitleID->Set( selfAssetData->GetTitleID() );

			InterfacePtr<IStringData> selectedStatusID( this->GetWidgetView(kZPUISaveToRepStatusDropDownWidgetID), IID_IZPUISELECTEDID );
			ASSERT(selectedStatusID);
			selectedStatusID->Set( selfAssetCheckInData->GetStatusID() );
		}

		IControlView * statusDropDown = this->GetWidgetView(kZPUISaveToRepStatusDropDownWidgetID);
		if( mAllowStatusChange == false )
		{
			InterfacePtr<IStringData> selectedStatusID( statusDropDown, IID_IZPUISELECTEDID );
			ASSERT(selectedStatusID);
			selectedStatusID->Set( kNullString );
			statusDropDown->Disable();
		}
		else
			statusDropDown->Enable();

		//Fill title dropdown
		InterfacePtr<AZPDBObjPopupController> titlePopupController( this->GetWidgetView(kZPUISaveToRepPublicationDropDownWidgetID), IZPDBObjPopupController::kDefaultIID );
		ASSERT( titlePopupController );
		titlePopupController->Refresh();

		//Fill edition dropdown
		InterfacePtr<AZPDBObjPopupController> editionPopupController( this->GetWidgetView(kZPUISaveToRepEditionDropDownWidgetID), IZPDBObjPopupController::kDefaultIID );
		ASSERT( editionPopupController );
		editionPopupController->Refresh();

		//Fill status dropdown
		InterfacePtr<AZPDBObjPopupController> statusPopupController( this->GetWidgetView(kZPUISaveToRepStatusDropDownWidgetID), IZPDBObjPopupController::kDefaultIID );
		ASSERT( statusPopupController );
		statusPopupController->Refresh();
	} while(kFalse);
}
 
//----------------------------------------------------------------------------------------
// ValidateDialogFields
//----------------------------------------------------------------------------------------
WidgetID
CZPUISaveToRepDialogController::ValidateDialogFields(
	IActiveContext* myContext)
{
	LogFunctionEnterExit;
	WidgetID result = kDefaultWidgetId;
	//WidgetID result = CDialogController::ValidateDialogFields(myContext);
	do {
		//asset name cannot be empty
		PMString strName = this->GetTextControlData(kZPUISaveToRepNameEditBoxWidgetID);
		strName.StripWhiteSpace(PMString::kLeadingAndTrailingWhiteSpace);
#if defined(InDnCS5) || defined(InDnCS5_5)
		strName.SetTranslatable(kFalse);
#endif
		
		if(strName.IsEmpty())
		{
			//ErrorUtils::PMSetGlobalErrorCode(kZPUINameEmptyError);
			CAlert::WarningAlert(kZPUINameEmptyErrorKey);			
			result = kZPUISaveToRepNameEditBoxWidgetID;
			break;
		}
		
		//Check Title, it cannot be empty
		PMString titleID = this->GetSelectedTitleID();
		if ( titleID.IsEmpty() )
		{
			//ErrorUtils::PMSetGlobalErrorCode(kZPUITitleEmptyError);
			CAlert::WarningAlert(kZPUITitleEmptyErrorKey);
			result = kZPUISaveToRepPublicationDropDownWidgetID;
			break;
		}
		
		//Check edition, it cannot be empty
		PMString editionID = this->GetSelectedEditionID();
		if ( editionID.IsEmpty() )
		{
			//ErrorUtils::PMSetGlobalErrorCode(kZPUIEditionEmptyError);
			CAlert::WarningAlert(kZPUIEditionEmptyErrorKey);
			result = kZPUISaveToRepEditionDropDownWidgetID;
			break;
		}
		
		bool isTemplate = false;

		InterfacePtr<const IBoolData> dlgIsNewVersion( this, IID_IZPISNEWVERSION );
		ASSERT( dlgIsNewVersion );
		if( dlgIsNewVersion && dlgIsNewVersion->Get() == kFalse && Utils<IZPInDnUtils>()->IsHostAppInDesign() )
		{
			//Don't check status for new indesign template check-in
			isTemplate = this->GetTriStateControlData(kZPUISaveToRepIsTemplateCheckBoxWidgetID );
		}

		//Check Title status, it cannot be empty
		PMString statusID = this->GetSelectedStatusID();
		if ( mAllowStatusChange && statusID.IsEmpty() && isTemplate == false)
		{
			//ErrorUtils::PMSetGlobalErrorCode(kZPUIStatusEmptyError);
			CAlert::WarningAlert(kZPUIStatusEmptyErrorKey);
			result = kZPUISaveToRepStatusDropDownWidgetID;
			break;
		}
		
		InterfacePtr<const IZPAssetData> selfAssetData( this, UseDefaultIID());
		ASSERT( selfAssetData );
			
		//Title/Edition/Type cannot change for old assets.
		if( dlgIsNewVersion && dlgIsNewVersion->Get() == kTrue )
		{
			if( titleID.Compare( kFalse, selfAssetData->GetTitleID() ) != 0 )
			{
				//ErrorUtils::PMSetGlobalErrorCode(kZPUITitleCannotChangeError);
				CAlert::WarningAlert(kZPUITitleCannotChangeErrorKey);
				result = kZPUISaveToRepPublicationDropDownWidgetID;
				break;
			}
		
			if( editionID.Compare( kFalse, selfAssetData->GetEditionID() ) != 0 )
			{
				//ErrorUtils::PMSetGlobalErrorCode(kZPUIEditionCannotChangeError);
				CAlert::WarningAlert(kZPUIEditionCannotChangeErrorKey);
				result = kZPUISaveToRepEditionDropDownWidgetID;
				break;
			}
		}

		//duplicate asset name check
		InterfacePtr<IStringData> dlgParentAssetID(this, IID_IZPPARENTASSETID);
		ASSERT( dlgParentAssetID );
		const PMString & parentID = dlgParentAssetID->Get();

		const PMString & assetID = selfAssetData->GetAssetID();
		enAssetType	assetType = this->GetAssetNewType();
		const PMString & assetTypeId = Utils<IZPAssetUtils>()->GetAssetTypeID(assetType);

		InterfacePtr<const IZPAssetCheckInData> selfAssetCheckInData( this, UseDefaultIID() );
		ASSERT( selfAssetCheckInData );
		const PMString & theOldName = selfAssetCheckInData->GetAssetName();
		if ( strName.Compare(kFalse, theOldName) != 0)
		{
			if (Utils<IZPAMWSUtils>()->CanUpdateAssetName_sync(assetID, editionID, assetTypeId, parentID , strName ) != kSuccess)
			{
				//ErrorUtils::PMSetGlobalErrorCode(kZPUIDuplicateAssetNameError);
				CAlert::WarningAlert(kZPUIDuplicateAssetNameErrorKey);
				result = kZPUISaveToRepNameEditBoxWidgetID;
				break;
			}
		}
	}while( false );

	return result;
}


//----------------------------------------------------------------------------------------
// ApplyDialogFields
//----------------------------------------------------------------------------------------
void
CZPUISaveToRepDialogController::ApplyDialogFields(
	IActiveContext* myContext, const WidgetID& widgetId)
{
	LogFunctionEnterExit;
	
	do
	{
		InterfacePtr<IZPAssetCheckInData> selfAssetCheckInData( this, UseDefaultIID() );
		ASSERT( selfAssetCheckInData );
		//Name
		PMString strName = this->GetTextControlData(kZPUISaveToRepNameEditBoxWidgetID);
		selfAssetCheckInData->SetAssetName(strName);
		
		//Description
		PMString strDescription = this->GetTextControlData( kZPUISaveToRepDescriptionEditBoxWidgetID );
		selfAssetCheckInData->SetDescription( strDescription );
		
		// IsTemplate
		InterfacePtr<IIntData> selfAssetType( this, IID_IZPASSETTYPE );
		ASSERT( selfAssetType );
		enAssetType	assetType = this->GetAssetNewType();
		selfAssetType->Set( assetType );

		InterfacePtr<IZPAssetData> selfAssetData( this, UseDefaultIID());
		ASSERT( selfAssetData );

		//titleID
		selfAssetData->SetTitleID( this->GetSelectedTitleID() );

		// editionID
		selfAssetData->SetEditionID( this->GetSelectedEditionID() );

		if (mAllowStatusChange && assetType == eAssetType_IDTemplate)
			mAllowStatusChange = false;
		
		// statusID
		PMString newStatusID = this->GetSelectedStatusID();
		if( mAllowStatusChange == false )
		{
			if( assetType == eAssetType_ICDocument || assetType == eAssetType_IDStory )
				newStatusID = kTitleStatus_Unassigned;
			else if(assetType == eAssetType_ICTemplate || assetType == eAssetType_IDTemplate)
				newStatusID = kTitleStatus_Template;
		}
		selfAssetCheckInData->SetStatusID( newStatusID );
		
		//Set the dialog result to true
		InterfacePtr<IBoolData> selfResultBoolData( this, UseDefaultIID() );
		ASSERT( selfResultBoolData );
		selfResultBoolData->Set( kTrue );
	}while(kFalse);
}

//----------------------------------------------------------------------------------------
// GetSelectedTitleID
//----------------------------------------------------------------------------------------
PMString
CZPUISaveToRepDialogController::GetSelectedTitleID()const
{
	InterfacePtr<const IStringData> selectedTitleID( this->GetWidgetView(kZPUISaveToRepPublicationDropDownWidgetID), IID_IZPUISELECTEDID );
	ASSERT(selectedTitleID);

	return selectedTitleID->Get();
}

//----------------------------------------------------------------------------------------
// GetSelectedEditionID
//----------------------------------------------------------------------------------------
PMString
CZPUISaveToRepDialogController::GetSelectedEditionID()const
{
	InterfacePtr<const IStringData> selectedEditionID( this->GetWidgetView(kZPUISaveToRepEditionDropDownWidgetID), IID_IZPUISELECTEDID );
	ASSERT(selectedEditionID);

	return selectedEditionID->Get();
}

//----------------------------------------------------------------------------------------
// GetSelectedStatusID
//----------------------------------------------------------------------------------------
PMString
CZPUISaveToRepDialogController::GetSelectedStatusID()const
{
	InterfacePtr<const IStringData> selectedStatusID( this->GetWidgetView(kZPUISaveToRepStatusDropDownWidgetID), IID_IZPUISELECTEDID );
	ASSERT(selectedStatusID);

	return selectedStatusID->Get();
}

//----------------------------------------------------------------------------------------
// GetWidgetView
//----------------------------------------------------------------------------------------
IControlView *
CZPUISaveToRepDialogController::GetWidgetView(
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
// GetAssetNewType
//----------------------------------------------------------------------------------------
enAssetType
CZPUISaveToRepDialogController::GetAssetNewType()const
{
	InterfacePtr<IIntData> selfAssetType( this, IID_IZPASSETTYPE );
	ASSERT( selfAssetType );
	enAssetType	assetType = (enAssetType) selfAssetType->Get();
	if( assetType != eAssetType_ICAssignment && assetType != eAssetType_IDStory )
	{	//Asset type is not changed for Assignment / Linked Story.
		//Which document/template is selected based on Host application.
		if (this->GetTriStateControlData(kZPUISaveToRepIsTemplateCheckBoxWidgetID ))
			assetType = Utils<IZPAssetUtils>()->GetHostAppDocAssetType( true );
		else
			assetType = Utils<IZPAssetUtils>()->GetHostAppDocAssetType( false );
	}

	return assetType;
}
