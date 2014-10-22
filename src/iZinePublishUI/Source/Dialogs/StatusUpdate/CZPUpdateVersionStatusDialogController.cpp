//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Dialogs/StatusUpdate/CZPUpdateVersionStatusDialogController.cpp $
//	$Revision: 4089 $
//	$Date: 2012-10-30 06:59:29 +0100 (Tue, 30 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Aman Alam
//	Created: 16-6-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IBoolData.h"
#include "IDropDownListController.h"
#include "IPanelControlData.h"
#include "IStringData.h"

//SDK General includes
#include "CDialogController.h"
#include "CAlert.h"

//OS / ThirdParty includes

#include "IZPUIID.h"
#include "IZPID.h"

//IZP Interfaces
#include "IZPAssetData.h"
#include "IZPAssetCheckInData.h"
#include "IZPDBObjPopupController.h"
#include "IZPGlobalTempData.h"

//IZP General includes
#include "AZPDBObjPopupController.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_Dialogs
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CDialogController
class CZPUpdateVersionStatusDialogController : public inheritClass
{
public:
						CZPUpdateVersionStatusDialogController(IPMUnknown * boss);
	virtual				~CZPUpdateVersionStatusDialogController();

	/** Initialize each widget in the dialog with its default value.
	 Called when the dialog is opened.
	 */
	void				InitializeDialogFields(IActiveContext* dlgContext);
	
	/** Validate the values in the widgets.
	 By default, the widget with ID kOKButtonWidgetID causes
	 ValidateFields to be called. When all widgets are valid,
	 ApplyFields will be called.
	 @return kDefaultWidgetId if all widget values are valid, WidgetID of the widget to select otherwise.
	 
	 */
	WidgetID			ValidateDialogFields(IActiveContext* myContext);
	
	/** Retrieve the values from the widgets and act on them.
	 @param widgetId identifies the widget on which to act.
	 */
	void				ApplyDialogFields(IActiveContext* myContext, const WidgetID& widgetId);
protected:
	IControlView *		GetWidgetView(
							const WidgetID &			inWidgetID ) const;

	PMString			GetSelectedStatusID() const;
	PMString			DBObjPopGetSelectedID(
							WidgetID					inWidgetID,
							bool						inEmptyIfNotCurrentlySelected = false ) const;
private:
};


CREATE_PMINTERFACE(CZPUpdateVersionStatusDialogController, kZPUpdateVersionStatusDialogControllerImpl)

CZPUpdateVersionStatusDialogController::CZPUpdateVersionStatusDialogController(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPUpdateVersionStatusDialogController::~CZPUpdateVersionStatusDialogController()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// InitializeDialogFields
//----------------------------------------------------------------------------------------
void
CZPUpdateVersionStatusDialogController::InitializeDialogFields(
	IActiveContext* dlgContext)
{
	LogFunctionEnterExit;

	inheritClass::InitializeDialogFields(dlgContext);

	InterfacePtr<const IZPAssetCheckInData> selfAssetCheckInData( this, UseDefaultIID() );
	ASSERT( selfAssetCheckInData );

	this->SetTextControlData(kZPUISaveToRepNameEditBoxWidgetID, selfAssetCheckInData->GetDescription() );

	InterfacePtr<IStringData> selfTitleID(this, IID_IZPTITLEIDSTRINGDATA );
	ASSERT( selfTitleID );

	InterfacePtr<IStringData> statusTitleID( this->GetWidgetView(kZPUIStatusUpdateDlgPopStatusWidgetID), IID_IZPTITLEIDSTRINGDATA );
	ASSERT(statusTitleID);
	statusTitleID->Set( selfTitleID->Get() );

	InterfacePtr<IStringData> selectedStatusID( statusTitleID, IID_IZPUISELECTEDID );
	ASSERT(selectedStatusID);
	selectedStatusID->Set( selfAssetCheckInData->GetStatusID() );

	//Fill status dropdown
	InterfacePtr<AZPDBObjPopupController> statusPopupController( this->GetWidgetView(kZPUIStatusUpdateDlgPopStatusWidgetID), IZPDBObjPopupController::kDefaultIID );
	ASSERT( statusPopupController );
	statusPopupController->Refresh();
}

//----------------------------------------------------------------------------------------
// ValidateDialogFields
//----------------------------------------------------------------------------------------
WidgetID
CZPUpdateVersionStatusDialogController::ValidateDialogFields(
	IActiveContext* myContext)
{
	//WidgetID toReturn = inheritClass::ValidateDialogFields(myContext);
	WidgetID toReturn = kDefaultWidgetId;
	do
	{
		PMString	theDescription;
		theDescription			= this->GetTextControlData( kZPUIStatusUpdateDlgTxtDescriptionWidgetID );
		if( theDescription.IsEmpty() )
		{	
			//TODO: Is description neccessary
			//toReturn = kZPUIStatusUpdateDlgTxtDescriptionWidgetID;
			//break;
		}

		PMString	theStatusID;
		theStatusID			= this->GetSelectedStatusID();
		if( theStatusID.IsEmpty() )
		{
			CAlert::WarningAlert(kZPUIStatusEmptyErrorKey);
			toReturn = kZPUIStatusUpdateDlgPopStatusWidgetID;
			break;
		}
	}while(kFalse);

	return toReturn;
}

//----------------------------------------------------------------------------------------
// ApplyDialogFields
//----------------------------------------------------------------------------------------
void
CZPUpdateVersionStatusDialogController::ApplyDialogFields(
	IActiveContext* myContext, const WidgetID& widgetId)
{
	InterfacePtr<IZPAssetCheckInData> selfAssetCheckInData( this, UseDefaultIID() );
	ASSERT( selfAssetCheckInData );

	//Description
	PMString strDescription = this->GetTextControlData( kZPUISaveToRepDescriptionEditBoxWidgetID );
	selfAssetCheckInData->SetDescription( strDescription );

	// statusID
	PMString newStatusID = this->GetSelectedStatusID();
	selfAssetCheckInData->SetStatusID( newStatusID );

	//Set the dialog result to true
	InterfacePtr<IBoolData> selfResultBoolData( this, UseDefaultIID() );
	ASSERT( selfResultBoolData );
	selfResultBoolData->Set( kTrue );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetWidgetView
//----------------------------------------------------------------------------------------
IControlView *
CZPUpdateVersionStatusDialogController::GetWidgetView(
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
// GetSelectedStatusID
//----------------------------------------------------------------------------------------
PMString
CZPUpdateVersionStatusDialogController::GetSelectedStatusID() const
{
	return this->DBObjPopGetSelectedID(kZPUIStatusUpdateDlgPopStatusWidgetID, true);
}

//----------------------------------------------------------------------------------------
// DBObjPopGetSelectedID
//----------------------------------------------------------------------------------------
PMString
CZPUpdateVersionStatusDialogController::DBObjPopGetSelectedID(
	WidgetID					inWidgetID,
	bool						inEmptyIfNotCurrentlySelected ) const
{
	InterfacePtr<const IStringData> selectedID( this->GetWidgetView(inWidgetID), IID_IZPUISELECTEDID );
	ASSERT(selectedID);

	InterfacePtr<IDropDownListController> popupListController( selectedID, UseDefaultIID());
	ASSERT(popupListController);
	
	int32 selectedIndex = popupListController->GetSelected();

	if( !inEmptyIfNotCurrentlySelected || selectedIndex >= 0 )
		return selectedID->Get();
	
	return kNullString;

}