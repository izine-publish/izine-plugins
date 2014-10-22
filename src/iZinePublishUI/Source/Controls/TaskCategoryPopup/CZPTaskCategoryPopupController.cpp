//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/TaskCategoryPopup/CZPTaskCategoryPopupController.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 14-10-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IStringData.h"
#include "IStringListControlData.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes

//OS / ThirdParty includes

#include "IZPUIID.h"
#include "IZPID.h"

//IZP Interfaces
#include "IZPApp.h"
#include "IZPTaskCategoryMgr.h"
#include "IZPTitleMgr.h"

//IZP General includes
#include "AZPDBObjPopupWithFixedValController.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_UI
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass AZPDBObjPopupWithFixedValController
class CZPTaskCategoryPopupController : public inheritClass
{
public:
						CZPTaskCategoryPopupController(IPMUnknown * boss);
	virtual				~CZPTaskCategoryPopupController();

	void				AppendDBObjectValues();

	PMString			GetNthDBObjID(
							int							inIndex) const;
	int32				GetIndexOfDBObjID(
							const PMString &			inObjID ) const;
protected:
private:
	ZPTaskCategoryIDsList	mTaskCategoryIDsList;
};


CREATE_PMINTERFACE(CZPTaskCategoryPopupController, kZPTaskCategoryPopupControllerImpl)

CZPTaskCategoryPopupController::CZPTaskCategoryPopupController(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPTaskCategoryPopupController::~CZPTaskCategoryPopupController()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// AppendDBObjectValues
//----------------------------------------------------------------------------------------
void
CZPTaskCategoryPopupController::AppendDBObjectValues()
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IStringListControlData> iCategoryStringListControlData( this, UseDefaultIID());
	ASSERT(iCategoryStringListControlData);
	
	InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<const IZPTaskCategoryMgr> taskCategoryMgr( zpApp->GetTitleMgr(), UseDefaultIID() );
	ASSERT(taskCategoryMgr);

	ZPTaskCategoryIDsList & taskCategoryIDList = mTaskCategoryIDsList;
	taskCategoryIDList.clear();
	taskCategoryMgr->GetAllTaskCategoryIDs( taskCategoryIDList );
	//Should we store the id list with IID_IZPUIOBJIDSSTRINGLISTDATA on this object.
	
	int newListLen = taskCategoryIDList.size();
	if( newListLen <= 0 )
		iCategoryStringListControlData->Clear();	//Sends change message to self observer
	else
		iCategoryStringListControlData->Clear(kFalse, kFalse);

	ZPTaskCategoryIDsList::iterator endIter = taskCategoryIDList.end();

	int i=1;
	for( ZPTaskCategoryIDsList::iterator currIter = taskCategoryIDList.begin(); currIter != endIter; ++currIter,++i)
	{
		PMString & statusID = *currIter;
		
		InterfacePtr<const IStringData> statusName( taskCategoryMgr->GetTaskCategoryForID( statusID ), IID_IZPDATANAME );
		ASSERT( statusName );
		const PMString & theCategoryName = statusName->Get();

		if ( i == newListLen )
			iCategoryStringListControlData->AddString(theCategoryName);
		else
			iCategoryStringListControlData->AddString(theCategoryName, IStringListControlData::kEnd, kFalse, kFalse );
	}

	//this->SendMessage_ListLengthChanged();	//Commented as it is now send by its base class.
}

//----------------------------------------------------------------------------------------
// GetNthDBObjID
//----------------------------------------------------------------------------------------
PMString
CZPTaskCategoryPopupController::GetNthDBObjID(
	int							inIndex) const
{
	PMString toReturn;
	ASSERT( inIndex >= 0 );

	if( inIndex < 0 )
		return toReturn;

	if( inIndex >= 0 && inIndex < mTaskCategoryIDsList.size() )
			toReturn = mTaskCategoryIDsList.at( inIndex );
	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetIndexOfDBObjID
//----------------------------------------------------------------------------------------
int32
CZPTaskCategoryPopupController::GetIndexOfDBObjID(
	const PMString &			inObjID) const
{
	int32 toReturn = -1;

	toReturn = mTaskCategoryIDsList.Location( inObjID );

	return toReturn;
}
