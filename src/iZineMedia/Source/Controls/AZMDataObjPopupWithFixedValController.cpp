//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Controls/AZMDataObjPopupWithFixedValController.cpp $
//	$Revision: 2707 $
//	$Date: 2011-04-06 14:38:20 +0200 (Wed, 06 Apr 2011) $
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
#include "IStringListControlData.h"

//SDK General includes

//OS / ThirdParty includes

#include "iZMediaID.h"

//IZP Interfaces

//IZP General includes
#include "AZMDataObjPopupWithFixedValController.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_UI
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
AZMDataObjPopupWithFixedValController::AZMDataObjPopupWithFixedValController(IPMUnknown * boss)
: inheritClass_AZMDataObjPopupWithFixedValController(boss)
, mFixedValuesCount( 0 )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
AZMDataObjPopupWithFixedValController::~AZMDataObjPopupWithFixedValController()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// ClearValues
//----------------------------------------------------------------------------------------
void
AZMDataObjPopupWithFixedValController::ClearValues()
{
	LogFunctionEnterExit;

	inheritClass_AZMDataObjPopupWithFixedValController::ClearValues();

	//Add Fixed values
	this->AppendFixedValues();
}

//----------------------------------------------------------------------------------------
// FillValues
//----------------------------------------------------------------------------------------
void
AZMDataObjPopupWithFixedValController::FillValues()
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
AZMDataObjPopupWithFixedValController::GetNthObjID(
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
AZMDataObjPopupWithFixedValController::GetIndexOfObjID(
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
AZMDataObjPopupWithFixedValController::GetNthFixedObjID(
	int							inIndex) const
{
	PMString toReturn;

	InterfacePtr<IStringListControlData> iPopupFixedStringListControlData( this, IID_IZMPOPFIXEDITEMSTRINGDATA );
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
AZMDataObjPopupWithFixedValController::GetIndexOfFixedObjID(
	const PMString &			inObjID) const
{
	int32 toReturn = -1;

	InterfacePtr<IStringListControlData> iPopupFixedStringListControlData( this, IID_IZMPOPFIXEDITEMSTRINGDATA );
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
AZMDataObjPopupWithFixedValController::AppendFixedValues()
{
	LogFunctionEnterExit;
	InterfacePtr<IStringListControlData> iPopupStringListControlData( this, UseDefaultIID());
	ASSERT(iPopupStringListControlData);
	
	InterfacePtr<IStringListControlData> iPopupFixedStringListControlData( this, IID_IZMPOPFIXEDITEMSTRINGDATA );
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
