//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/TitlesPopup/CZPTitlePopupController.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 7-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IStringData.h"
#include "IStringListData.h"
#include "IStringListControlData.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPApp.h"
#include "IZPTitleMgr.h"

//IZP General includes
#include "AZPDBObjPopupController.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_UI
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass AZPDBObjPopupController
class CZPTitlePopupController : public inheritClass
{
public:
						CZPTitlePopupController(IPMUnknown * boss);
	virtual				~CZPTitlePopupController();

	void				FillValues();

	PMString			GetNthObjID(
							int							inIndex ) const;
	int32				GetIndexOfObjID(
							const PMString &			inObjID ) const;
protected:private:
	ZPTitleIDsList mTitleIDList;
};


CREATE_PMINTERFACE(CZPTitlePopupController, kZPTitlePopupControllerImpl)

CZPTitlePopupController::CZPTitlePopupController(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPTitlePopupController::~CZPTitlePopupController()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// FillValues
//----------------------------------------------------------------------------------------
void
CZPTitlePopupController::FillValues()
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IStringListControlData> iTitleStringListControlData( this, UseDefaultIID());
	ASSERT(iTitleStringListControlData);
	
	InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<const IZPTitleMgr> titleMgr( zpApp->GetTitleMgr(), UseDefaultIID() );
	ASSERT(titleMgr);

	mTitleIDList.clear();
	//ZPTitleIDsList titleList;
	ZPTitleIDsList & titleList = mTitleIDList;
	titleMgr->GetAllTitleIDs(titleList);

	//Sort title id on name
	std::sort( titleList.begin(), titleList.end(), CompareTitleIDByName() );

	//Should we store the id list with IID_IZPUIOBJIDSSTRINGLISTDATA on this object.
	//InterfacePtr<IStringListData> selfTitleIDList( this, IID_IZPUIOBJIDSSTRINGLISTDATA );
	//ASSERT( selfTitleIDList );

	int newListLen = titleList.size();
	if( newListLen <= 0 )
		iTitleStringListControlData->Clear();	//Sends change message to self observer
	else
		iTitleStringListControlData->Clear(kFalse, kFalse);

	ZPTitleIDsList::iterator endIter = titleList.end();

	int i=1;
	for( ZPTitleIDsList::iterator currIter = titleList.begin(); currIter != endIter; ++currIter,++i)
	{
		PMString & titleID = *currIter;
		
		InterfacePtr<const IStringData> titleName( titleMgr->GetTitleForID(titleID), IID_IZPDATANAME );
		ASSERT( titleName );
		const PMString & theTitleName = titleName->Get();

		//selfTitleIDList->AddString( titleID );	//Store it with popup as sorting will change the indexes.

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
CZPTitlePopupController::GetNthObjID(
	int							inIndex) const
{
	PMString toReturn;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<const IZPTitleMgr> titleMgr( zpApp->GetTitleMgr(), UseDefaultIID() );
	ASSERT(titleMgr);

	//ZPTitleIDsList titleList;
	//titleMgr->GetAllTitleIDs(titleList);
	const ZPTitleIDsList & titleList = mTitleIDList;

	if( inIndex >= 0 && inIndex < titleList.size() )
		toReturn = titleList.at(inIndex);

	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetIndexOfObjID
//----------------------------------------------------------------------------------------
int32
CZPTitlePopupController::GetIndexOfObjID(
	const PMString &			inObjID) const
{
	int32 toReturn = -1;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<const IZPTitleMgr> titleMgr( zpApp->GetTitleMgr(), UseDefaultIID() );
	ASSERT(titleMgr);

	//ZPTitleIDsList titleList;
	//titleMgr->GetAllTitleIDs(titleList);
	const ZPTitleIDsList & titleList = mTitleIDList;

	toReturn = titleList.Location( inObjID );

	return toReturn;
}
