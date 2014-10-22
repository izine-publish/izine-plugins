//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/TaskStatusPopup/CZPTaskStatusPopupController.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 30-9-2010
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
#include "IZPTaskStatusMgr.h"
#include "IZPTitleMgr.h"

//IZP General includes
#include "AZPDBObjPopupWithFixedValController.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_UI
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass AZPDBObjPopupWithFixedValController
class CZPTaskStatusPopupController : public inheritClass
{
public:
						CZPTaskStatusPopupController(IPMUnknown * boss);
	virtual				~CZPTaskStatusPopupController();

	void				AppendDBObjectValues();

	PMString			GetNthDBObjID(
							int							inIndex) const;
	int32				GetIndexOfDBObjID(
							const PMString &			inObjID ) const;
protected:
private:
	ZPTaskStatusIDsList	mTaskStatusIDsList;
};


CREATE_PMINTERFACE(CZPTaskStatusPopupController, kZPTaskStatusPopupControllerImpl)

CZPTaskStatusPopupController::CZPTaskStatusPopupController(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPTaskStatusPopupController::~CZPTaskStatusPopupController()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// AppendDBObjectValues
//----------------------------------------------------------------------------------------
void
CZPTaskStatusPopupController::AppendDBObjectValues()
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IStringListControlData> iStatusStringListControlData( this, UseDefaultIID());
	ASSERT(iStatusStringListControlData);
	
	InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<const IZPTaskStatusMgr> taskStatusMgr( zpApp->GetTitleMgr(), UseDefaultIID() );
	ASSERT(taskStatusMgr);

	ZPTaskStatusIDsList & taskStatusIDList = mTaskStatusIDsList;
	taskStatusIDList.clear();
	taskStatusMgr->GetAllTaskStatusIDs( taskStatusIDList );
	//Should we store the id list with IID_IZPUIOBJIDSSTRINGLISTDATA on this object.
	
	int newListLen = taskStatusIDList.size();
	if( newListLen <= 0 )
		iStatusStringListControlData->Clear();	//Sends change message to self observer
	else
		iStatusStringListControlData->Clear(kFalse, kFalse);

	ZPTaskStatusIDsList::iterator endIter = taskStatusIDList.end();

	int i=1;
	for( ZPTaskStatusIDsList::iterator currIter = taskStatusIDList.begin(); currIter != endIter; ++currIter,++i)
	{
		PMString & statusID = *currIter;
		
		InterfacePtr<const IStringData> statusName( taskStatusMgr->GetTaskStatusForID( statusID ), IID_IZPDATANAME );
		ASSERT( statusName );
		const PMString & theStatusName = statusName->Get();

		if ( i == newListLen )
			iStatusStringListControlData->AddString(theStatusName);
		else
			iStatusStringListControlData->AddString(theStatusName, IStringListControlData::kEnd, kFalse, kFalse );
	}

	//this->SendMessage_ListLengthChanged();	//Commented as it is now send by its base class.
}

//----------------------------------------------------------------------------------------
// GetNthDBObjID
//----------------------------------------------------------------------------------------
PMString
CZPTaskStatusPopupController::GetNthDBObjID(
	int							inIndex) const
{
	PMString toReturn;
	ASSERT( inIndex >= 0 );

	if( inIndex < 0 )
		return toReturn;

	if( inIndex >= 0 && inIndex < mTaskStatusIDsList.size() )
			toReturn = mTaskStatusIDsList.at( inIndex );
	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetIndexOfDBObjID
//----------------------------------------------------------------------------------------
int32
CZPTaskStatusPopupController::GetIndexOfDBObjID(
	const PMString &			inObjID) const
{
	int32 toReturn = -1;

	toReturn = mTaskStatusIDsList.Location( inObjID );

	return toReturn;
}
