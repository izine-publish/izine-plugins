//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Dialogs/Alert/CZPUIAlertDialogObserver.cpp $
//	$Revision: 2506 $
//	$Date: 2010-11-11 09:59:06 +0100 (Thu, 11 Nov 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 10-7-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IControlView.h"
#include "IDialogController.h"
#include "IPanelControlData.h"
#include "IBooleanControlData.h"
#include "ISubject.h"

//SDK General includes
#include "CDialogObserver.h"

//OS / ThirdParty includes

#include "IZPUIID.h"

//IZP Interfaces

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_Dialogs
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CDialogObserver
class CZPUIAlertDialogObserver : public inheritClass
{
public:
						CZPUIAlertDialogObserver(IPMUnknown * boss);
	virtual				~CZPUIAlertDialogObserver();

	void				AutoAttach();
	void				AutoDetach();

	/**
	 Called by the host when the observed object changes, in this case when
	 the dialog's info button is clicked.
	 @param theChange specifies the class ID of the change to the subject. Frequently this is a command ID.
	 @param theSubject points to the ISubject interface for the subject that has changed.
	 @param protocol specifies the ID of the changed interface on the subject boss.
	 @param changedBy points to additional data about the change. Often this pointer indicates the class ID of the command that has caused the change.
	 */
	void				Update(
							const ClassID&			theChange,
							ISubject*				theSubject,
							const PMIID&			protocol,
							void*					changedBy);
protected:
	void				AttachToBtn( 
							WidgetID				inWidgetID );
	void				DetachFromBtn( 
							WidgetID				inWidgetID );
private:
};


CREATE_PMINTERFACE(CZPUIAlertDialogObserver, kZPUIAlertDialogObserverImpl)

CZPUIAlertDialogObserver::CZPUIAlertDialogObserver(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPUIAlertDialogObserver::~CZPUIAlertDialogObserver()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// AutoAttach
//----------------------------------------------------------------------------------------
void
CZPUIAlertDialogObserver::AutoAttach()
{
	inheritClass::AutoAttach();
	this->AttachToBtn( kZPUIAlertDlgBtn1WidgetID );
	this->AttachToBtn( kZPUIAlertDlgBtn2WidgetID );
	this->AttachToBtn( kZPUIAlertDlgBtn3WidgetID );
	this->AttachToBtn( kZPUIAlertDlgBtn4WidgetID );
}

//----------------------------------------------------------------------------------------
// AutoDetach
//----------------------------------------------------------------------------------------
void
CZPUIAlertDialogObserver::AutoDetach()
{
	inheritClass::AutoDetach();
	this->DetachFromBtn( kZPUIAlertDlgBtn1WidgetID );
	this->DetachFromBtn( kZPUIAlertDlgBtn2WidgetID );
	this->DetachFromBtn( kZPUIAlertDlgBtn3WidgetID );
	this->DetachFromBtn( kZPUIAlertDlgBtn4WidgetID );
}

//----------------------------------------------------------------------------------------
// Update
//----------------------------------------------------------------------------------------
void
CZPUIAlertDialogObserver::Update(
	const ClassID&			theChange,
	ISubject*				theSubject,
	const PMIID&			protocol,
	void*					changedBy)
{
	LogFunctionEnterExit;
	
	// Call the base class Update function so that default behavior will still occur (OK and Cancel buttons, etc.).
	inheritClass::Update(theChange, theSubject, protocol, changedBy);
	
	do
	{
		if( theChange == kTrueStateMessage )
		{
			InterfacePtr<IControlView> controlView(theSubject, UseDefaultIID());
			ASSERT(controlView);
			if(!controlView)
				break;
			
			// Get the button ID from the view.
			WidgetID theSelectedWidget = controlView->GetWidgetID();

			if( theSelectedWidget == kZPUIAlertDlgBtn1WidgetID
			 || theSelectedWidget == kZPUIAlertDlgBtn2WidgetID
			 || theSelectedWidget == kZPUIAlertDlgBtn3WidgetID
			 || theSelectedWidget == kZPUIAlertDlgBtn4WidgetID )
			{
				InterfacePtr<IDialogController> dialogController(this, UseDefaultIID());

				dialogController->ApplyDialog( theSelectedWidget );
				this->CloseDialog( kFalse );
			}
		}
	}while( false );
}

//----------------------------------------------------------------------------------------
// AttachToBtn
//----------------------------------------------------------------------------------------
void
CZPUIAlertDialogObserver::AttachToBtn(
	WidgetID				inWidgetID)
{
	InterfacePtr<const IPanelControlData> selfPanelData(this, UseDefaultIID());
	ASSERT( selfPanelData != nil );

	IControlView * btnControlView = selfPanelData->FindWidget( inWidgetID );
	if( btnControlView )
	{
		InterfacePtr<ISubject> btnSubject( btnControlView, UseDefaultIID() );
		ASSERT( btnSubject );
		if( !btnSubject->IsAttached( this, IBooleanControlData::kDefaultIID ) )
			btnSubject->AttachObserver( this, IBooleanControlData::kDefaultIID);
	}
}

//----------------------------------------------------------------------------------------
// DetachFromBtn
//----------------------------------------------------------------------------------------
void
CZPUIAlertDialogObserver::DetachFromBtn(
	WidgetID				inWidgetID)
{
	InterfacePtr<const IPanelControlData> selfPanelData(this, UseDefaultIID());
	ASSERT( selfPanelData != nil );

	IControlView * btnControlView = selfPanelData->FindWidget( inWidgetID );
	if( btnControlView )
	{
		InterfacePtr<ISubject> btnSubject( btnControlView, UseDefaultIID() );
		ASSERT( btnSubject );
		if( btnSubject->IsAttached( this, IBooleanControlData::kDefaultIID ) )
			btnSubject->DetachObserver( this, IBooleanControlData::kDefaultIID);

	}
}
