//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Panels/TaskHistory/CZPUITaskHistoryPanelController.cpp $
//	$Revision: 2984 $
//	$Date: 2011-05-11 06:27:57 +0200 (Wed, 11 May 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 27-10-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IObserver.h"
#include "ISubject.h"
#include "IStringData.h"

//SDK General includes
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPUIID.h"
#include "IZPID.h"

//IZP Interfaces
#include "IZPTaskInfo.h"
#include "IZPUIHistoryPanelController.h"
#include "IZPUIDialogFacade.h"
#include "IZPWSUtils.h"

//IZP General includes
#include "ZPDataHelper.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

extern "C"
void	HandleHistoryPanelClose();

IZPUIHistoryPanelController * gHistoryController;

#define inheritClass CPMUnknown<IZPUIHistoryPanelController>
class CZPUITaskHistoryPanelController : public inheritClass
{
public:
						CZPUITaskHistoryPanelController(IPMUnknown * boss);
	virtual				~CZPUITaskHistoryPanelController();

	void				ShowHistoryForTask(
							const IStringData *			inDBTaskID );
	void				ShowHistoryForTask(
							const PMString &			inTaskID );

	void				DoHistoryFetch(
							bool						inFetchIfPanelIsVisible );
	void				UpdatePanel(
							const unsigned char *		inHistoryHTML );

	void				HandlePanelClosed();
protected:
	void				RemoveListeningTask();
	void				AddListeningToTask();

	void				RemoveListeningObject(
							const IPMUnknown *			inObjectToListen,
							const PMIID &				inProtocol);

	void				AddListeningToObject(
							const IPMUnknown *			inObjectToListen,
							const PMIID &				inProtocol);

private:
	InterfacePtr<const IStringData>	mTask;

	bool		mIsPanelVisisble;
};


CREATE_PMINTERFACE(CZPUITaskHistoryPanelController, kZPUITaskHistoryPanelControllerImpl)

CZPUITaskHistoryPanelController::CZPUITaskHistoryPanelController(IPMUnknown * boss)
: inheritClass(boss)
, mIsPanelVisisble( false )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPUITaskHistoryPanelController::~CZPUITaskHistoryPanelController()
{
	if (this == gHistoryController)
		gHistoryController = nil;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// HandleHistoryPanelClose
//----------------------------------------------------------------------------------------
void
HandleHistoryPanelClose()
{
	IZPUIHistoryPanelController::GetHistoryController()->HandlePanelClosed();
}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetHistoryController
//----------------------------------------------------------------------------------------
IZPUIHistoryPanelController *
IZPUIHistoryPanelController::GetHistoryController(bool inFlagCreateIfNil)
{
	if( !gHistoryController && inFlagCreateIfNil)
	{
		gHistoryController = (::CreateObject2<IZPUIHistoryPanelController>(kZPUITasksHistoryPanelBoss));	//Release will be made during shutdown
		InterfacePtr<IObserver> selfObserver( gHistoryController, UseDefaultIID() );
		ASSERT( selfObserver );
		selfObserver->AutoAttach();
	}
	return gHistoryController;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// ShowHistoryForTask
//----------------------------------------------------------------------------------------
void
CZPUITaskHistoryPanelController::ShowHistoryForTask(
	const PMString &			inTaskID)
{
	const IStringData * dbTaskID = ZPDataHelper::GetTaskByID( inTaskID );
	this->ShowHistoryForTask( dbTaskID );
}

//----------------------------------------------------------------------------------------
// ShowHistoryForTask
//----------------------------------------------------------------------------------------
void
CZPUITaskHistoryPanelController::ShowHistoryForTask(
	const IStringData *			inDBTaskID)
{
	this->RemoveListeningTask();
	if( inDBTaskID )
		inDBTaskID->AddRef();
	mTask.reset( inDBTaskID );
	this->AddListeningToTask();
	this->DoHistoryFetch( false );
}

//----------------------------------------------------------------------------------------
// DoHistoryFetch
//----------------------------------------------------------------------------------------
void
CZPUITaskHistoryPanelController::DoHistoryFetch(
	bool						inFetchIfPanelIsVisible )
{
	if( mTask && (!inFetchIfPanelIsVisible || mIsPanelVisisble ))
		Utils<IZPWSUtils>()->FetchTaskHistory_async( mTask->Get() );
}

//----------------------------------------------------------------------------------------
// UpdatePanel
//----------------------------------------------------------------------------------------
void
CZPUITaskHistoryPanelController::UpdatePanel(
	const unsigned char *		inHistoryHTML)
{
	mIsPanelVisisble = true;
	Utils<IZPUIDialogFacade>()->ShowTaskHistoryDialog( inHistoryHTML );
}

//----------------------------------------------------------------------------------------
// HandlePanelClosed
//----------------------------------------------------------------------------------------
void
CZPUITaskHistoryPanelController::HandlePanelClosed()
{
	mIsPanelVisisble = false;
	this->RemoveListeningTask();
	mTask.reset( nil );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// RemoveListeningTask
//----------------------------------------------------------------------------------------
void
CZPUITaskHistoryPanelController::RemoveListeningTask()
{
	if( !mTask )
		return;

	this->RemoveListeningObject( mTask.get(), IZPTaskInfo::kDefaultIID );
}

//----------------------------------------------------------------------------------------
// AddListeningToTask
//----------------------------------------------------------------------------------------
void
CZPUITaskHistoryPanelController::AddListeningToTask()
{
	if( !mTask )
		return;

	this->AddListeningToObject( mTask.get(), IZPTaskInfo::kDefaultIID );
}

//----------------------------------------------------------------------------------------
// RemoveListeningObject
//----------------------------------------------------------------------------------------
void
CZPUITaskHistoryPanelController::RemoveListeningObject(
	const IPMUnknown *			inObjectToListen,
	const PMIID &				inProtocol)
{
	InterfacePtr<ISubject> inSubject( inObjectToListen, UseDefaultIID() );
	ASSERT(inSubject);

	InterfacePtr<IObserver> selfObserver( this, UseDefaultIID() );
	ASSERT(selfObserver);

	if( inSubject->IsAttached( selfObserver, inProtocol ) )
		inSubject->DetachObserver( selfObserver, inProtocol );
}

//----------------------------------------------------------------------------------------
// AddListeningToObject
//----------------------------------------------------------------------------------------
void
CZPUITaskHistoryPanelController::AddListeningToObject(
	const IPMUnknown *			inObjectToListen,
	const PMIID &				inProtocol)
{
	InterfacePtr<ISubject> inSubject( inObjectToListen, UseDefaultIID() );
	ASSERT(inSubject);

	InterfacePtr<IObserver> selfObserver( this, UseDefaultIID() );
	ASSERT(selfObserver);

	if( false == inSubject->IsAttached( selfObserver, inProtocol ) )
		inSubject->AttachObserver( selfObserver, inProtocol );
}
