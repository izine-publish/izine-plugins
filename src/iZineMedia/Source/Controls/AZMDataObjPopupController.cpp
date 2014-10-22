//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Controls/AZMDataObjPopupController.cpp $
//	$Revision: 2756 $
//	$Date: 2011-04-08 13:50:19 +0200 (Fri, 08 Apr 2011) $
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
#include "ISubject.h"
#include "IStringData.h"
#include "IStringListControlData.h"
#include "IDropDownListController.h"

//SDK General includes

//OS / ThirdParty includes

#include "iZMediaID.h"

//IZP Interfaces
//#include "IZMAppController.h"

//IZP General includes
#include "AZMDataObjPopupController.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_None
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

AZMDataObjPopupController::AZMDataObjPopupController(IPMUnknown * boss)
: inheritClass_AZMDataObjPopupController(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
AZMDataObjPopupController::~AZMDataObjPopupController()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// ClearValues
//----------------------------------------------------------------------------------------
void
AZMDataObjPopupController::ClearValues()
{
	InterfacePtr<IStringListControlData> iStringListControlData( this, UseDefaultIID());
	ASSERT(iStringListControlData);
	
	iStringListControlData->Clear();	//Sends change message to self observer
}

//----------------------------------------------------------------------------------------
// Refresh
//----------------------------------------------------------------------------------------
void
AZMDataObjPopupController::Refresh()
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
AZMDataObjPopupController::BroadcastMessage(
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
AZMDataObjPopupController::SendMessage_ListLengthChanged()const
{
	this->BroadcastMessage( kZMUIDataObjDropDown_ListLengthChangedMsg );
}



	
//----------------------------------------------------------------------------------------
// SendMessage_SelectionChanged
//----------------------------------------------------------------------------------------
void
AZMDataObjPopupController::SendMessage_SelectionChanged() const
{
	LogFunctionEnterExit;

	this->BroadcastMessage( kZMUIDataObjDropDown_SelectionChangedMsg );
}

//----------------------------------------------------------------------------------------
// SaveSelection
//----------------------------------------------------------------------------------------
void
AZMDataObjPopupController::SaveSelection()
{
	LogFunctionEnterExit;

	InterfacePtr<IDropDownListController> selfListController( this, UseDefaultIID());
	ASSERT(selfListController);
	
	InterfacePtr<IStringData> selectedObjID( this, IID_IZMUISELECTEDID );
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
AZMDataObjPopupController::RestoreSelection()
{
	LogFunctionEnterExit;

	InterfacePtr<IDropDownListController> selfListController( this, UseDefaultIID());
	ASSERT(selfListController);
	
	InterfacePtr<IStringData> selectedObjID( this, IID_IZMUISELECTEDID );
	ASSERT(selectedObjID);

	const PMString & theSelectedObjID = selectedObjID->Get();
	int32 selectedIndex = this->GetIndexOfObjID( theSelectedObjID );

	InterfacePtr<IStringListControlData> iStringListControlData( this, UseDefaultIID());
	ASSERT(iStringListControlData);

	if( iStringListControlData->Length() > 0 && selectedIndex >= 0 )
		selfListController->Select( selectedIndex );
}
