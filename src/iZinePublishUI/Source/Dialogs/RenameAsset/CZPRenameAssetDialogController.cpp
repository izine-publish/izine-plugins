//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Dialogs/RenameAsset/CZPRenameAssetDialogController.cpp $
//	$Revision: 3553 $
//	$Date: 2011-07-26 16:56:48 +0200 (Tue, 26 Jul 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Aman Alam
//	Created: 19-7-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IBoolData.h"
#include "IPanelControlData.h"
#include "IStringData.h"

//SDK General includes
#include "CDialogController.h"
#include "ErrorUtils.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPUIID.h"
#include "IZPID.h"

//IZP Interfaces
#include "IZPAMWSUtils.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_Dialogs
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CDialogController
class CZPRenameAssetDialogController : public inheritClass
{
public:
						CZPRenameAssetDialogController(IPMUnknown * boss);
	virtual				~CZPRenameAssetDialogController();

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
private:
};


CREATE_PMINTERFACE(CZPRenameAssetDialogController, kZPRenameAssetDialogControllerImpl)

CZPRenameAssetDialogController::CZPRenameAssetDialogController(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPRenameAssetDialogController::~CZPRenameAssetDialogController()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// InitializeDialogFields
//----------------------------------------------------------------------------------------
void
CZPRenameAssetDialogController::InitializeDialogFields(
	IActiveContext* dlgContext)
{
	LogFunctionEnterExit;

	inheritClass::InitializeDialogFields(dlgContext);

	InterfacePtr<const IStringData> assetName( this, IID_IZPDATANAME );
	ASSERT( assetName );
	this->SetTextControlData( kZPUIRenameAssetDlgTxtNewNameWidgetID, assetName->Get() );
}

//----------------------------------------------------------------------------------------
// ValidateDialogFields
//----------------------------------------------------------------------------------------
WidgetID
CZPRenameAssetDialogController::ValidateDialogFields(
	IActiveContext* myContext)
{
	WidgetID toReturn = inheritClass::ValidateDialogFields(myContext);

	do
	{
		PMString newName;
		newName = this->GetTextControlData( kZPUIRenameAssetDlgTxtNewNameWidgetID );
		newName.StripWhiteSpace( PMString::kLeadingAndTrailingWhiteSpace );
		if( newName.IsEmpty() )
		{	
			toReturn = kZPUIRenameAssetDlgTxtNewNameWidgetID;
			break;
		}

		InterfacePtr<const IStringData> dlgAssetOldName( this, IID_IZPDATANAME );
		ASSERT( dlgAssetOldName );
		const PMString & assetOldName = dlgAssetOldName->Get();
		if ( newName.Compare(kFalse, assetOldName) != 0)
		{
			InterfacePtr<IStringData> dlgAssetID( this, IID_IZPDATAID );
			ASSERT( dlgAssetID );
			const PMString & assetId = dlgAssetID->Get();
			if (Utils<IZPAMWSUtils>()->CanUpdateAssetName_sync(assetId, kNullString, kNullString, kNullString , newName ) != kSuccess)
			{
				ErrorUtils::PMSetGlobalErrorCode(kZPUIDuplicateAssetNameError);
				toReturn = kZPUIRenameAssetDlgTxtNewNameWidgetID;
				break;
			}
		}
	}while(kFalse);

	return toReturn;
}

//----------------------------------------------------------------------------------------
// ApplyDialogFields
//----------------------------------------------------------------------------------------
void
CZPRenameAssetDialogController::ApplyDialogFields(
	IActiveContext* myContext, const WidgetID& widgetId)
{
	InterfacePtr<IStringData> assetName( this, IID_IZPDATANAME );
	ASSERT( assetName );

	//Description
	PMString strName = this->GetTextControlData( kZPUIRenameAssetDlgTxtNewNameWidgetID );
	assetName->Set( strName );

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
CZPRenameAssetDialogController::GetWidgetView(
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
