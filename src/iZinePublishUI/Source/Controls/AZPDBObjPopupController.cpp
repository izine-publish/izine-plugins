//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/AZPDBObjPopupController.cpp $
//	$Revision: 2926 $
//	$Date: 2011-05-04 08:47:15 +0200 (Wed, 04 May 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 7-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "ISubject.h"
#include "IStringData.h"
#include "IStringListControlData.h"
#include "IDropDownListController.h"

//SDK General includes

//OS / ThirdParty includes

#include "IZPUIID.h"
#include "IZPID.h"

//IZP Interfaces
#include "IZPAppController.h"
#include "IZPLoginController.h"

//IZP General includes
#include "AZPDBObjPopupController.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_None
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

AZPDBObjPopupController::AZPDBObjPopupController(IPMUnknown * boss)
: inheritClass_AZPDBObjPopupController(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
AZPDBObjPopupController::~AZPDBObjPopupController()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// ClearValues
//----------------------------------------------------------------------------------------
void
AZPDBObjPopupController::ClearValues()
{
	InterfacePtr<IStringListControlData> iStatusStringListControlData( this, UseDefaultIID());
	ASSERT(iStatusStringListControlData);
	
	iStatusStringListControlData->Clear();	//Sends change message to self observer
}

//----------------------------------------------------------------------------------------
// Refresh
//----------------------------------------------------------------------------------------
void
AZPDBObjPopupController::Refresh()
{
	LogFunctionEnterExit;

	//Save selection
	//this->SaveSelection();	//Don't save selection here. If it was empty then selection will go.

	//Refill values
	this->FillValues();

	//Restore selection
	this->RestoreSelection();

	//If same selection is not present then send message - selection chagned.
}

//----------------------------------------------------------------------------------------
// BroadcastMessage
//----------------------------------------------------------------------------------------
void
AZPDBObjPopupController::BroadcastMessage(
	const ClassID&						inTheChange,
	void *								inChangedBy) const
{
	InterfacePtr<ISubject> selfSubject(this, UseDefaultIID());
	ASSERT(selfSubject);
	
	selfSubject->Change( inTheChange, IStringListControlData::kDefaultIID, inChangedBy );
}



//----------------------------------------------------------------------------------------
// SendMessage_ListLengthChanged
//----------------------------------------------------------------------------------------
void
AZPDBObjPopupController::SendMessage_ListLengthChanged()const
{
	this->BroadcastMessage( kZPUIDBObjDropDown_ListLengthChangedMsg );
}



	
//----------------------------------------------------------------------------------------
// SendMessage_SelectionChanged
//----------------------------------------------------------------------------------------
void
AZPDBObjPopupController::SendMessage_SelectionChanged() const
{
	LogFunctionEnterExit;

	this->BroadcastMessage( kZPUIDBObjDropDown_SelectionChangedMsg );
}

//----------------------------------------------------------------------------------------
// SaveSelection
//----------------------------------------------------------------------------------------
void
AZPDBObjPopupController::SaveSelection()
{
	LogFunctionEnterExit;

	InterfacePtr<IDropDownListController> selfListController( this, UseDefaultIID());
	ASSERT(selfListController);
	
	InterfacePtr<IStringData> selectedObjID( this, IID_IZPUISELECTEDID );
	ASSERT(selectedObjID);

	int32 selectedIndex = selfListController->GetSelected();
	if( selectedIndex >= 0 )
	{
		const PMString theSelectedObjID = this->GetNthObjID( selectedIndex );
		selectedObjID->Set( theSelectedObjID);
	}
}

//----------------------------------------------------------------------------------------
// RestoreSelection
//----------------------------------------------------------------------------------------
void
AZPDBObjPopupController::RestoreSelection()
{
	LogFunctionEnterExit;

	InterfacePtr<IDropDownListController> selfListController( this, UseDefaultIID());
	ASSERT(selfListController);
	
	InterfacePtr<IStringData> selectedObjID( this, IID_IZPUISELECTEDID );
	ASSERT(selectedObjID);

	const PMString & theSelectedObjID = selectedObjID->Get();
	int32 selectedIndex = this->GetIndexOfObjID( theSelectedObjID );

	InterfacePtr<IStringListControlData> iStringListControlData( this, UseDefaultIID());
	ASSERT(iStringListControlData);

	if( iStringListControlData->Length() > 0 && selectedIndex >= 0 )
		selfListController->Select( selectedIndex );
#ifdef MACINTOSH	
	//Patch for bug: 1713
	else 
		selfListController->Select( IDropDownListController::kNoSelection, kTrue, kFalse );
#endif	

}
