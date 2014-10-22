//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Controls/TitlesPopup/CZMTitlePopupController.cpp $
//	$Revision: 2715 $
//	$Date: 2011-04-06 15:51:32 +0200 (Wed, 06 Apr 2011) $
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
#include "IStringData.h"
#include "IStringListControlData.h"

//SDK General includes

//OS / ThirdParty includes

#include "iZMediaID.h"

//IZP Interfaces
#include "IZMApp.h"
#include "IZMTitleMgr.h"

//IZP General includes
#include "AZMDataObjPopupController.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_UI
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass AZMDataObjPopupController
class CZMTitlePopupController : public inheritClass
{
public:
						CZMTitlePopupController(IPMUnknown * boss);
	virtual				~CZMTitlePopupController();

	void				FillValues();

	PMString			GetNthObjID(
							int							inIndex ) const;
	int32				GetIndexOfObjID(
							const PMString &			inObjID ) const;
protected:private:
};


CREATE_PMINTERFACE(CZMTitlePopupController, kZMTitlePopupControllerImpl)

CZMTitlePopupController::CZMTitlePopupController(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZMTitlePopupController::~CZMTitlePopupController()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// FillValues
//----------------------------------------------------------------------------------------
void
CZMTitlePopupController::FillValues()
{
	LogFunctionEnterExit;

	InterfacePtr<IStringListControlData> iTitleStringListControlData( this, UseDefaultIID());
	ASSERT(iTitleStringListControlData);
	
	InterfacePtr<const IZMApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<const IZMTitleMgr> titleMgr( zpApp->GetTitleMgr(), UseDefaultIID() );
	ASSERT(titleMgr);

	ZMTitleIDsList titleList;
	titleMgr->GetAllTitleIDs(titleList);
	//Should we store the id list with IID_IZMUIOBJIDSSTRINGLISTDATA on this object.
	
	int newListLen = titleList.size();
	if( newListLen <= 0 )
		iTitleStringListControlData->Clear();	//Sends change message to self observer
	else
		iTitleStringListControlData->Clear(kFalse, kFalse);

	ZMTitleIDsList::iterator endIter = titleList.end();

	int i=1;
	for( ZMTitleIDsList::iterator currIter = titleList.begin(); currIter != endIter; ++currIter,++i)
	{
		PMString & titleID = *currIter;
		
		InterfacePtr<const IStringData> titleName( titleMgr->GetTitleForID(titleID), IID_IZMDATANAME );
		ASSERT( titleName );
		const PMString & theTitleName = titleName->Get();

		if ( i == newListLen )
			iTitleStringListControlData->AddString(theTitleName);
		else
			iTitleStringListControlData->AddString(theTitleName, IStringListControlData::kEnd, kFalse, kFalse );
	}

	this->SendMessage_ListLengthChanged();
}

//----------------------------------------------------------------------------------------
// GetNthObjID
//----------------------------------------------------------------------------------------
PMString
CZMTitlePopupController::GetNthObjID(
	int							inIndex) const
{
	PMString toReturn;

	InterfacePtr<const IZMApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<const IZMTitleMgr> titleMgr( zpApp->GetTitleMgr(), UseDefaultIID() );
	ASSERT(titleMgr);

	ZMTitleIDsList titleList;
	titleMgr->GetAllTitleIDs(titleList);

	if( inIndex >= 0 && inIndex < titleList.size() )
		toReturn = titleList.at(inIndex);

	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetIndexOfObjID
//----------------------------------------------------------------------------------------
int32
CZMTitlePopupController::GetIndexOfObjID(
	const PMString &			inObjID) const
{
	int32 toReturn = -1;
	InterfacePtr<const IZMApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<const IZMTitleMgr> titleMgr( zpApp->GetTitleMgr(), UseDefaultIID() );
	ASSERT(titleMgr);

	ZMTitleIDsList titleList;
	titleMgr->GetAllTitleIDs(titleList);

	toReturn = titleList.Location( inObjID );

	return toReturn;
}
