//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Dialogs/Alert/CZPUIAlertDialogController.cpp $
//	$Revision: 2506 $
//	$Date: 2010-11-11 09:59:06 +0100 (Thu, 11 Nov 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 9-7-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IIntData.h"
#include "IPanelControlData.h"
#include "IStringData.h"
#include "IStringListData.h"

//SDK General includes
#include "CDialogController.h"

//OS / ThirdParty includes

#include "IZPUIID.h"

//IZP Interfaces

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_Dialogs
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CDialogController
class CZPUIAlertDialogController : public inheritClass
{
public:
						CZPUIAlertDialogController(IPMUnknown * boss);
	virtual				~CZPUIAlertDialogController();

	void				InitializeDialogFields(
							IActiveContext*				dlgContext);
	
	WidgetID			ValidateDialogFields(
							IActiveContext*				myContext);
	
	void				ApplyDialogFields(
							IActiveContext*				myContext,
							const WidgetID&				widgetId);
	void				UserCancelled();
protected:
	void				SetControlVisible(
							WidgetID					inWidgetid,
							bool						inVisible) const;
	void				SetControlWidgetID(
							WidgetID					inWidgetOldID,
							WidgetID					inWidgetNewID ) const;
private:
	WidgetID	mButtonIDs[4];
};


CREATE_PMINTERFACE(CZPUIAlertDialogController, kZPUIAlertDialogControllerImpl)

CZPUIAlertDialogController::CZPUIAlertDialogController(IPMUnknown * boss)
: inheritClass(boss)
{
	mButtonIDs[0] = kZPUIAlertDlgBtn1WidgetID;
	mButtonIDs[1] = kZPUIAlertDlgBtn2WidgetID;
	mButtonIDs[2] = kZPUIAlertDlgBtn3WidgetID;
	mButtonIDs[3] = kZPUIAlertDlgBtn4WidgetID;
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPUIAlertDialogController::~CZPUIAlertDialogController()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// InitializeDialogFields
//----------------------------------------------------------------------------------------
void
CZPUIAlertDialogController::InitializeDialogFields(
	IActiveContext*				dlgContext)
{

	InterfacePtr<const IIntData> dlgDefaultButtonIndex( this, UseDefaultIID());
	ASSERT( dlgDefaultButtonIndex );
	int defaultBtnIndex = dlgDefaultButtonIndex->Get();
	ASSERT( defaultBtnIndex >= 1 && defaultBtnIndex <= 4 );

	if( defaultBtnIndex >= 1 && defaultBtnIndex <= 4 )
	{
		this->SetControlWidgetID( mButtonIDs[defaultBtnIndex -1], kOKButtonWidgetID );
		mButtonIDs[defaultBtnIndex -1] = kOKButtonWidgetID;
	}
	
	InterfacePtr<const IIntData> dlgCancelButtonIndex( this, IID_IZPUIINTDATA);
	ASSERT( dlgCancelButtonIndex );
	int cancelBtnIndex = dlgCancelButtonIndex->Get();
	if( cancelBtnIndex >= 1 && cancelBtnIndex <= 4 )
	{
		this->SetControlWidgetID( mButtonIDs[cancelBtnIndex -1], kCancelButton_WidgetID );
		mButtonIDs[cancelBtnIndex -1] = kCancelButton_WidgetID;
	}
	
	InterfacePtr<const IStringData> dlgMessage( this, UseDefaultIID() );
	ASSERT( dlgMessage );
	const PMString & messageStr = dlgMessage->Get();
	this->SetTextControlData( kZPUIAlertDlgMessageWidgetID, messageStr );

	InterfacePtr<IStringListData> dlgButtonLabelsData( this, UseDefaultIID() );
	ASSERT( dlgButtonLabelsData );

	const K2Vector<PMString> & buttonLabels = dlgButtonLabelsData->GetStringList();
	
	int buttonLabelsLen = buttonLabels.size();

	ASSERT( buttonLabelsLen > 1 );

	bool hideButton = true;

	if( buttonLabelsLen > 0 )
	{
		const PMString & btnLabel1 = buttonLabels.at(0);
		if( !(hideButton = btnLabel1.IsEmpty() == kTrue) )
			this->SetTextControlData( mButtonIDs[0], btnLabel1 );
	}
	this->SetControlVisible( mButtonIDs[0], !hideButton );

	hideButton = true;
	if( buttonLabelsLen > 1 )
	{
		const PMString & btnLabel2 = buttonLabels.at(1);
		if( !(hideButton = btnLabel2.IsEmpty() == kTrue) )
			this->SetTextControlData( mButtonIDs[1], btnLabel2 );
	}
	this->SetControlVisible( mButtonIDs[1], !hideButton );


	hideButton = true;
	if( buttonLabelsLen > 2 )
	{
		const PMString & btnLabel3 = buttonLabels.at(2);
		if( !(hideButton = btnLabel3.IsEmpty() == kTrue) )
			this->SetTextControlData( mButtonIDs[2], btnLabel3 );
	}
	this->SetControlVisible( mButtonIDs[2], !hideButton );


	hideButton = true;
	if( buttonLabelsLen > 3 )
	{
		const PMString & btnLabel4 = buttonLabels.at(3);
		if( !(hideButton = btnLabel4.IsEmpty() == kTrue) )
			this->SetTextControlData( mButtonIDs[3], btnLabel4 );
	}
	this->SetControlVisible( mButtonIDs[3], !hideButton );


	inheritClass::InitializeDialogFields( dlgContext );
}

//----------------------------------------------------------------------------------------
// ValidateDialogFields
//----------------------------------------------------------------------------------------
WidgetID
CZPUIAlertDialogController::ValidateDialogFields(
	IActiveContext*				myContext)
{
	return inheritClass::ValidateDialogFields( myContext );
}

//----------------------------------------------------------------------------------------
// ApplyDialogFields
//----------------------------------------------------------------------------------------
void
CZPUIAlertDialogController::ApplyDialogFields(
	IActiveContext*				myContext,
	const WidgetID&				widgetId)
{
	int btnCount = 4;
	for( int i = 0; i < btnCount; ++i )
	{
		if( mButtonIDs[i] == widgetId )
		{
			InterfacePtr<IIntData> dlgDefaultButtonIndex( this, UseDefaultIID());
			ASSERT( dlgDefaultButtonIndex );
			dlgDefaultButtonIndex->Set( i + 1 );
			break;
		}
	}
}

//----------------------------------------------------------------------------------------
// UserCancelled
//----------------------------------------------------------------------------------------
void
CZPUIAlertDialogController::UserCancelled()
{
	(static_cast<IDialogController *>( this ) )->ApplyDialog( kCancelButton_WidgetID );	//Store the cancel button index as result.
	inheritClass::UserCancelled();
}

//----------------------------------------------------------------------------------------
// SetControlVisible
//----------------------------------------------------------------------------------------
void
CZPUIAlertDialogController::SetControlVisible(
	WidgetID					inWidgetid,
	bool						inVisible) const
{
	InterfacePtr<const IPanelControlData> selfPanelControlData(this, UseDefaultIID());
	ASSERT(selfPanelControlData);

	IControlView* controlView = selfPanelControlData->FindWidget( inWidgetid );
	ASSERT(controlView);
	
	if( inVisible )
		controlView->Show();
	else
		controlView->Hide();
}

//----------------------------------------------------------------------------------------
// SetControlWidgetID
//----------------------------------------------------------------------------------------
void
CZPUIAlertDialogController::SetControlWidgetID(
	WidgetID					inWidgetOldID,
	WidgetID					inWidgetNewID) const
{
	InterfacePtr<const IPanelControlData> selfPanelControlData(this, UseDefaultIID());
	ASSERT(selfPanelControlData);

	IControlView* controlView = selfPanelControlData->FindWidget( inWidgetOldID );
	ASSERT(controlView);

	if( controlView )
		controlView->SetWidgetID( inWidgetNewID );
}
