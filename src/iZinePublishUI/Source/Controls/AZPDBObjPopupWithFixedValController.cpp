//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/AZPDBObjPopupWithFixedValController.cpp $
//	$Revision: 2359 $
//	$Date: 2010-10-19 15:15:22 +0200 (Tue, 19 Oct 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 7-10-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IStringListControlData.h"

//SDK General includes

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces

//IZP General includes
#include "AZPDBObjPopupWithFixedValController.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_UI
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
AZPDBObjPopupWithFixedValController::AZPDBObjPopupWithFixedValController(IPMUnknown * boss)
: inheritClass_AZPDBObjPopupWithFixedValController(boss)
, mFixedValuesCount( 0 )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
AZPDBObjPopupWithFixedValController::~AZPDBObjPopupWithFixedValController()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// ClearValues
//----------------------------------------------------------------------------------------
void
AZPDBObjPopupWithFixedValController::ClearValues()
{
	LogFunctionEnterExit;

	inheritClass_AZPDBObjPopupWithFixedValController::ClearValues();

	//Add Fixed values
	this->AppendFixedValues();
}

//----------------------------------------------------------------------------------------
// FillValues
//----------------------------------------------------------------------------------------
void
AZPDBObjPopupWithFixedValController::FillValues()
{
	LogFunctionEnterExit;
	this->AppendDBObjectValues();
	this->AppendFixedValues();	//Adds at the begining
	this->SendMessage_ListLengthChanged();
}

//----------------------------------------------------------------------------------------
// GetNthObjID
//----------------------------------------------------------------------------------------
PMString
AZPDBObjPopupWithFixedValController::GetNthObjID(
	int							inIndex) const
{
	PMString toReturn;
	ASSERT( inIndex >= 0 );
	if( inIndex < 0 )
		return toReturn;

	if( inIndex < mFixedValuesCount )
	{	//Fixed display string and its ID are same.
		toReturn = this->GetNthFixedObjID( inIndex );
	}
	else
	{	//Return index of dynamic object list
		toReturn = GetNthDBObjID( inIndex - mFixedValuesCount );
	}
	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetIndexOfObjID
//----------------------------------------------------------------------------------------
int32
AZPDBObjPopupWithFixedValController::GetIndexOfObjID(
	const PMString &			inObjID) const
{
	int32 toReturn = this->GetIndexOfFixedObjID( inObjID );
	if( toReturn == -1 )
	{
		toReturn = this->GetIndexOfDBObjID( inObjID );
		if( toReturn != -1 )
			toReturn += mFixedValuesCount;
	}
	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetNthFixedObjID
//----------------------------------------------------------------------------------------
PMString
AZPDBObjPopupWithFixedValController::GetNthFixedObjID(
	int							inIndex) const
{
	PMString toReturn;

	InterfacePtr<IStringListControlData> iPopupFixedStringListControlData( this, IID_IZPPOPFIXEDITEMSTRINGDATA );
	ASSERT(iPopupFixedStringListControlData);

	int theFixedValuesCount = iPopupFixedStringListControlData->Length();
	ASSERT( theFixedValuesCount == mFixedValuesCount );

	ASSERT( inIndex >= 0 && inIndex < theFixedValuesCount );
	if( inIndex >= 0 && inIndex < theFixedValuesCount )
		toReturn = iPopupFixedStringListControlData->GetString( inIndex );
		
	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetIndexOfFixedObjID
//----------------------------------------------------------------------------------------
int32
AZPDBObjPopupWithFixedValController::GetIndexOfFixedObjID(
	const PMString &			inObjID) const
{
	int32 toReturn = -1;

	InterfacePtr<IStringListControlData> iPopupFixedStringListControlData( this, IID_IZPPOPFIXEDITEMSTRINGDATA );
	ASSERT(iPopupFixedStringListControlData);

	int theFixedValuesCount = iPopupFixedStringListControlData->Length();
	ASSERT( theFixedValuesCount == mFixedValuesCount );

	int i = 0;
	for( ; i < theFixedValuesCount ; ++i)
	{
		const PMString currVal = iPopupFixedStringListControlData->GetString( i );
		
		if( inObjID.Compare( kFalse, currVal ) == 0 )
		{
			toReturn = i; break;
		}
	}

	return toReturn;
}

//----------------------------------------------------------------------------------------
// AppendFixedValues
//----------------------------------------------------------------------------------------
void
AZPDBObjPopupWithFixedValController::AppendFixedValues()
{
	LogFunctionEnterExit;
	InterfacePtr<IStringListControlData> iPopupStringListControlData( this, UseDefaultIID());
	ASSERT(iPopupStringListControlData);
	
	InterfacePtr<IStringListControlData> iPopupFixedStringListControlData( this, IID_IZPPOPFIXEDITEMSTRINGDATA );
	ASSERT(iPopupFixedStringListControlData);

	mFixedValuesCount = iPopupFixedStringListControlData->Length();
	int i = mFixedValuesCount -1;
	for( ; i >= 0 ; --i)
	{
		const PMString currVal = iPopupFixedStringListControlData->GetString( i );
		
		if ( i == 0 )
			iPopupStringListControlData->AddString(currVal, IStringListControlData::kBeginning );
		else
			iPopupStringListControlData->AddString(currVal, IStringListControlData::kBeginning, kFalse, kFalse );
	}
}
