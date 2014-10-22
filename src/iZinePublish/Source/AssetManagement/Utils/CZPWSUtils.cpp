//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/Utils/CZPWSUtils.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 29-9-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IStringData.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPApp.h"
#include "IZPSoapRequest.h"
#include "IZPSoapResponse.h"
#include "IZPTaskMgr.h"
#include "IZPWSController.h"
#include "IZPWSResponseHandler.h"
#include "IZPWSUtils.h"

//IZP General includes
#include "CZPCreateTaskRequest.h"
#include "CZPCreateTaskResponse.h"
#include "CZPDeleteTaskRequest.h"
#include "CZPGetTaskListRequest.h"
#include "CZPGetTaskCategoryListRequest.h"
#include "CZPGetTaskHistoryRequest.h"
#include "CZPGetTaskStatusListRequest.h"
#include "CZPGetUsersForTitleRequest.h"
#include "CZPUpdateTaskRequest.h"
#include "StUtils.h"
#include "ZPDataHelper.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_WS
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define CreateAutoDelRequestObj(reqClass)				\
	reqClass * theRequest = new reqClass;				\
	StPtrDeleter<AZPSoapRequest>	autoDel( theRequest, true)
#define ReturnTheRequest								\
	autoDel.Forget();									\
	return theRequest

#define inheritClass CPMUnknown<IZPWSUtils>
class CZPWSUtils : public inheritClass
{
public:
						CZPWSUtils(IPMUnknown * boss);
	virtual				~CZPWSUtils();

#pragma mark -
// sync updater functions.
	ErrorCode			CreateNewTask(
							PMString &					oTaskID,
							const PMString &			inTitleID,
							const PMString &			inEditionID,
							const PMString &			inSubject,
							const PMString &			inDescription,
							const PMString &			inDocumentID,
							const PMString &			inAssignedToUserID,
							const PMString &			inStatusID,
							const PMString &			inCategory,
							const PMString &			inSpread,
							const PMString &			inPage ) const;
#pragma mark -
// async updater functions.
	void				UpdateTask(
							const PMString &			inTaskID,
							const PMString &			inTitleID,
							const PMString &			inEditionID,
							const PMString &			inSubject,
							const PMString &			inDescription,
							const PMString &			inDocumentID,
							const PMString &			inAssignedToUserID,
							const PMString &			inStatusID,
							const PMString &			inCategory,
							const PMString &			inSpread,
							const PMString &			inPage,
							int							inTaskOldVersion,
							const PMString &			inComments ) const;

	void				DeleteTask(
							const PMString &			inTaskID ) const;
#pragma mark -
// sync Getter functions, no changes made on WS state.

#pragma mark -
// async Getter functions, no changes made on WS state.
	void				FetchTaskCategoryList_async() const;
	void				FetchTaskStatusList_async() const;
	void				FetchTaskHistory_async(
							const PMString &			inTaskID ) const;
	void				FetchTitleUsersList_async(
							const PMString &			inTitleID ) const;
	void				FetchEditionTaskList_async(
							const PMString &			inEditionID ) const;
#pragma mark -
//Create requests - WS state mofiying
	AZPSoapRequest *	CreateCreateTaskRequest(
							const PMString &			inTitleID,
							const PMString &			inEditionID,
							const PMString &			inSubject,
							const PMString &			inDescription,
							const PMString &			inDocumentID,
							const PMString &			inAssignedToUserID,
							const PMString &			inStatusID,
							const PMString &			inCategory,
							const PMString &			inSpread,
							const PMString &			inPage ) const;
	AZPSoapRequest *	CreateUpdateTaskRequest(
							const PMString &			inTaskID,
							const PMString &			inTitleID,
							const PMString &			inEditionID,
							const PMString &			inSubject,
							const PMString &			inDescription,
							const PMString &			inDocumentID,
							const PMString &			inAssignedToUserID,
							const PMString &			inStatusID,
							const PMString &			inCategory,
							const PMString &			inSpread,
							const PMString &			inPage,
							int							inTaskOldVersion,
							const PMString &			inComments ) const;
	AZPSoapRequest *	CreateDeleteTaskRequest(
							const PMString &			inTaskID ) const;
#pragma mark -
//Create requests - WS state not mofiying
	AZPSoapRequest *	CreateFetchTaskCategoryListRequest( ) const;
	AZPSoapRequest *	CreateFetchTaskStatusListRequest( ) const;
	AZPSoapRequest *	CreateFetchTaskHistoryRequest(
							const PMString &			inTaskID ) const;
	AZPSoapRequest *	CreateFetchTitleUsersRequest(
							const PMString &			inTitleID ) const;
	AZPSoapRequest *	CreateFetchTasksListRequest(
							const PMString &			inEditionID ) const;
};


CREATE_PMINTERFACE(CZPWSUtils, kZPWSUtilsImpl)

CZPWSUtils::CZPWSUtils(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPWSUtils::~CZPWSUtils()
{

}

#pragma mark -
// sync updater functions.
//----------------------------------------------------------------------------------------
// CreateNewTask
//----------------------------------------------------------------------------------------
ErrorCode
CZPWSUtils::CreateNewTask(
	PMString &					oTaskID,
	const PMString &			inTitleID,
	const PMString &			inEditionID,
	const PMString &			inSubject,
	const PMString &			inDescription,
	const PMString &			inDocumentID,
	const PMString &			inAssignedToUserID,
	const PMString &			inStatusID,
	const PMString &			inCategory,
	const PMString &			inSpread,
	const PMString &			inPage) const
{
	LogFunctionEnterExit;
	oTaskID = kNullString;

#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IZPWSController> wsController( gSession , UseDefaultIID() );
	ASSERT(wsController);

	AZPSoapRequest * theRequest = this->CreateCreateTaskRequest( 
											inTitleID, inEditionID, inSubject, inDescription, inDocumentID,
											inAssignedToUserID, inStatusID, inCategory, inSpread, inPage);
	
	NewWSSyncRequestWithRequestData(theRequest)

#ifdef DEBUG
	InterfacePtr<const IControllingUnknown> controlling( soapRequest, UseDefaultIID() );
	int theRefCount = controlling->GetRefCount();
	IZPLog_Str( thisFileLA, enLT_ObjectCreations, "Sync Request boss at 0x%08X has refcount = %d", controlling.get(), theRefCount );
#endif
	wsController->AddRequest( soapRequest, false);	//Sync call.

	ErrorCode toReturn = kFailure;
	ZPWSError soapError = soapResponse->GetWSError();
	if( soapError == 0 )
	{
		const CZPCreateTaskResponse * theResponse = static_cast<const CZPCreateTaskResponse*>( soapResponse->GetResponseData() );
		if( theResponse )
		{
			toReturn = kSuccess;
			oTaskID = theResponse->GetTaskID();

			//Update the cache
			InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
			ASSERT( zpApp );

			InterfacePtr<IZPTaskMgr> theTaskMgr( zpApp->GetTaskMgr(), UseDefaultIID() );
			ASSERT(theTaskMgr);
			
			const IStringData * dbTaskID = nil;
			const IStringData * dbEditionID = ZPDataHelper::GetEditionByID( inEditionID );
			const ZPTaskInfo & taskInfo = theResponse->GetTaskInfo();
			theTaskMgr->AddTask( dbEditionID, oTaskID, &inSubject, &taskInfo );
		}
	}
#ifdef DEBUG
	theRefCount = controlling->GetRefCount();
	IZPLog_Str( thisFileLA, enLT_ObjectCreations, "Sync Request boss at 0x%08X has refcount = %d", controlling.get(), theRefCount );
#endif
	return toReturn;
}

#pragma mark -
// async updater functions.
//----------------------------------------------------------------------------------------
// UpdateTask
//----------------------------------------------------------------------------------------
void
CZPWSUtils::UpdateTask(
	const PMString &			inTaskID,
	const PMString &			inTitleID,
	const PMString &			inEditionID,
	const PMString &			inSubject,
	const PMString &			inDescription,
	const PMString &			inDocumentID,
	const PMString &			inAssignedToUserID,
	const PMString &			inStatusID,
	const PMString &			inCategory,
	const PMString &			inSpread,
	const PMString &			inPage,
	int							inTaskOldVersion,
	const PMString &			inComments) const
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IZPWSController> wsController( gSession , UseDefaultIID() );
	ASSERT(wsController);

	AZPSoapRequest * theRequest = this->CreateUpdateTaskRequest( inTaskID,
											inTitleID, inEditionID, inSubject, inDescription, inDocumentID,
											inAssignedToUserID, inStatusID, inCategory, inSpread, inPage,
											inTaskOldVersion, inComments );
	NewWSRequestWithData( theRequest )

	wsController->AddRequest( soapRequest );
}

//----------------------------------------------------------------------------------------
// DeleteTask
//----------------------------------------------------------------------------------------
void
CZPWSUtils::DeleteTask(
	const PMString &			inTaskID) const
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IZPWSController> wsController( gSession , UseDefaultIID() );
	ASSERT(wsController);

	NewWSRequest( CZPDeleteTaskRequest )

	//Someday: save the task to delete with this request, so that anyone that needs info from old can be fetched till its handling is over.
	theRequest->SetTaskID( inTaskID );

	wsController->AddRequest( soapRequest );
}

#pragma mark -
// sync Getter functions, no changes made on WS state.

#pragma mark -
// async Getter functions, no changes made on WS state.
//----------------------------------------------------------------------------------------
// FetchTaskCategoryList_async
//----------------------------------------------------------------------------------------
void
CZPWSUtils::FetchTaskCategoryList_async()const
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IZPWSController> wsController( gSession , UseDefaultIID() );
	ASSERT(wsController);

	AZPSoapRequest * theRequest = this->CreateFetchTaskCategoryListRequest();
	NewWSRequestWithData( theRequest )

	wsController->AddRequest( soapRequest );
}

//----------------------------------------------------------------------------------------
// FetchTaskStatusList_async
//----------------------------------------------------------------------------------------
void
CZPWSUtils::FetchTaskStatusList_async()const
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IZPWSController> wsController( gSession , UseDefaultIID() );
	ASSERT(wsController);

	AZPSoapRequest * theRequest = this->CreateFetchTaskStatusListRequest();
	NewWSRequestWithData( theRequest )

	wsController->AddRequest( soapRequest );
}

//----------------------------------------------------------------------------------------
// FetchTitleUsersList_async
//----------------------------------------------------------------------------------------
void
CZPWSUtils::FetchTitleUsersList_async(
	const PMString &			inTitleID ) const
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IZPWSController> wsController( gSession , UseDefaultIID() );
	ASSERT(wsController);

	AZPSoapRequest * theRequest = this->CreateFetchTitleUsersRequest( inTitleID );
	NewWSRequestWithData( theRequest )

	
	wsController->AddRequest( soapRequest );
}

//----------------------------------------------------------------------------------------
// FetchEditionTaskList_async
//----------------------------------------------------------------------------------------
void
CZPWSUtils::FetchEditionTaskList_async(
	const PMString &			inEditionID ) const
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IZPWSController> wsController( gSession , UseDefaultIID() );
	ASSERT(wsController);

	AZPSoapRequest * theRequest = this->CreateFetchTasksListRequest( inEditionID );
	NewWSRequestWithData( theRequest )

	
	wsController->AddRequest( soapRequest );
}

//----------------------------------------------------------------------------------------
// FetchTaskHistory_async
//----------------------------------------------------------------------------------------
void
CZPWSUtils::FetchTaskHistory_async(
	const PMString &			inTaskID) const
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IZPWSController> wsController( gSession , UseDefaultIID() );
	ASSERT(wsController);

	AZPSoapRequest * theRequest = this->CreateFetchTaskHistoryRequest( inTaskID );
	NewWSRequestWithData( theRequest )

	
	wsController->AddRequest( soapRequest );
}
#pragma mark -
//Create requests - WS state mofiying
//----------------------------------------------------------------------------------------
// CreateCreateTaskRequest
//----------------------------------------------------------------------------------------
AZPSoapRequest *
CZPWSUtils::CreateCreateTaskRequest(
	const PMString &			inTitleID,
	const PMString &			inEditionID,
	const PMString &			inSubject,
	const PMString &			inDescription,
	const PMString &			inDocumentID,
	const PMString &			inAssignedToUserID,
	const PMString &			inStatusID,
	const PMString &			inCategory,
	const PMString &			inSpread,
	const PMString &			inPage) const
{
	LogFunctionEnterExit;
	CreateAutoDelRequestObj(CZPCreateTaskRequest);

	theRequest->SetTitleID( inTitleID );
	theRequest->SetEditionID( inEditionID );
	theRequest->SetSubject( inSubject );
	theRequest->SetDescription( inDescription );
	theRequest->SetDocumentID( inDocumentID );
	theRequest->SetAssignedToUserID( inAssignedToUserID );
	theRequest->SetStatusID( inStatusID );
	theRequest->SetCategory( inCategory );
	theRequest->SetSpread( inSpread );
	theRequest->SetPage( inPage );

	ReturnTheRequest;
}

//----------------------------------------------------------------------------------------
// CreateUpdateTaskRequest
//----------------------------------------------------------------------------------------
AZPSoapRequest *
CZPWSUtils::CreateUpdateTaskRequest(
	const PMString &			inTaskID,
	const PMString &			inTitleID,
	const PMString &			inEditionID,
	const PMString &			inSubject,
	const PMString &			inDescription,
	const PMString &			inDocumentID,
	const PMString &			inAssignedToUserID,
	const PMString &			inStatusID,
	const PMString &			inCategory,
	const PMString &			inSpread,
	const PMString &			inPage,
	int							inTaskOldVersion,
	const PMString &			inComments) const
{
	LogFunctionEnterExit;
	CreateAutoDelRequestObj(CZPUpdateTaskRequest);

	theRequest->SetTaskID( inTaskID );
	theRequest->SetTitleID( inTitleID );
	theRequest->SetEditionID( inEditionID );
	theRequest->SetSubject( inSubject );
	theRequest->SetDescription( inDescription );
	theRequest->SetDocumentID( inDocumentID );
	theRequest->SetAssignedToUserID( inAssignedToUserID );
	theRequest->SetStatusID( inStatusID );
	theRequest->SetCategory( inCategory );
	theRequest->SetSpread( inSpread );
	theRequest->SetPage( inPage );
	theRequest->SetTaskOldVersion( inTaskOldVersion );
	theRequest->SetComments( inComments );

	ReturnTheRequest;
}

//----------------------------------------------------------------------------------------
// CreateDeleteTaskRequest
//----------------------------------------------------------------------------------------
AZPSoapRequest *
CZPWSUtils::CreateDeleteTaskRequest(
	const PMString &			inTaskID) const
{
	LogFunctionEnterExit;
	//CreateAutoDelRequestObj(CZPDeleteTaskRequest);

	//theRequest->SetTaskID( inTaskID );

	//ReturnTheRequest;
	//TODO:
	return nil;
}

#pragma mark -
//Create requests - WS state not mofiying
//----------------------------------------------------------------------------------------
// CreateFetchTaskCategoryListRequest
//----------------------------------------------------------------------------------------
AZPSoapRequest *
CZPWSUtils::CreateFetchTaskCategoryListRequest() const
{
	LogFunctionEnterExit;
	CreateAutoDelRequestObj(CZPGetTaskCategoryListRequest);

	ReturnTheRequest;
}

//----------------------------------------------------------------------------------------
// CreateFetchTaskStatusListRequest
//----------------------------------------------------------------------------------------
AZPSoapRequest *
CZPWSUtils::CreateFetchTaskStatusListRequest() const
{
	LogFunctionEnterExit;
	CreateAutoDelRequestObj(CZPGetTaskStatusListRequest);

	ReturnTheRequest;
}

//----------------------------------------------------------------------------------------
// CreateFetchTaskHistoryRequest
//----------------------------------------------------------------------------------------
AZPSoapRequest *
CZPWSUtils::CreateFetchTaskHistoryRequest(
	const PMString &			inTaskID) const
{
	LogFunctionEnterExit;
	CreateAutoDelRequestObj(CZPGetTaskHistoryRequest);

	theRequest->SetTaskID( inTaskID );

	ReturnTheRequest;
}

//----------------------------------------------------------------------------------------
// CreateFetchTitleUsersRequest
//----------------------------------------------------------------------------------------
AZPSoapRequest *
CZPWSUtils::CreateFetchTitleUsersRequest(
	const PMString &			inTitleID) const
{
	LogFunctionEnterExit;
	CreateAutoDelRequestObj(CZPGetUsersForTitleRequest);

	theRequest->SetTitleID( inTitleID );

	ReturnTheRequest;
}

//----------------------------------------------------------------------------------------
// CreateFetchTasksListRequest
//----------------------------------------------------------------------------------------
AZPSoapRequest *
CZPWSUtils::CreateFetchTasksListRequest(
	const PMString &			inEditionID) const
{
	LogFunctionEnterExit;
	CreateAutoDelRequestObj(CZPGetTaskListRequest);

	theRequest->SetEditionID( inEditionID );

	ReturnTheRequest;
}
