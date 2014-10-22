//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Panels/TaskHistory/CZPUITaskHistoryPanelObserver.cpp $
//	$Revision: 2408 $
//	$Date: 2010-10-27 11:10:50 +0200 (Wed, 27 Oct 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 27-10-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "ISubject.h"

//SDK General includes
#include "CObserver.h"

//OS / ThirdParty includes

#include "IZPUIID.h"
#include "IZPID.h"

//IZP Interfaces
#include "IZPAppController.h"
#include "IZPUIHistoryPanelController.h"
#include "IZPWSResponseHandler.h"

//IZP General includes
#include "AZPSoapResponse.h"
#include "CZPGetTaskHistoryResponse.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_PanelTasks
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CObserver
class CZPUITaskHistoryPanelObserver : public inheritClass
{
public:
						CZPUITaskHistoryPanelObserver(IPMUnknown * boss);
	virtual				~CZPUITaskHistoryPanelObserver();

	void				AutoAttach();
	void				AutoDetach();
	void				Update(
							const ClassID&				theChange,
							ISubject*					theSubject,
							const PMIID &				protocol,
							void*						changedBy);
protected:
	void				HandleWSResponse(
							const AZPSoapResponse *		inWSResponse );
	void				HandleTaskHistoryResponse(
							const AZPSoapResponse *		inWSResponse );

private:
};


CREATE_PMINTERFACE(CZPUITaskHistoryPanelObserver, kZPUITaskHistoryPanelObserverImpl)

CZPUITaskHistoryPanelObserver::CZPUITaskHistoryPanelObserver(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPUITaskHistoryPanelObserver::~CZPUITaskHistoryPanelObserver()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// AutoAttach
//----------------------------------------------------------------------------------------
void
CZPUITaskHistoryPanelObserver::AutoAttach()
{
	ASSERT( gZPApp );
	InterfacePtr<ISubject> zpAppSubject( gZPApp, UseDefaultIID() );
	ASSERT(zpAppSubject);
	if(!zpAppSubject->IsAttached(this, IZPWSResponseHandler::kDefaultIID))
	{
		zpAppSubject->AttachObserver(this, IZPWSResponseHandler::kDefaultIID);	//For task history
	}
}


//----------------------------------------------------------------------------------------
// AutoDetach
//----------------------------------------------------------------------------------------
void
CZPUITaskHistoryPanelObserver::AutoDetach()
{

}


//----------------------------------------------------------------------------------------
// Update
//----------------------------------------------------------------------------------------
void
CZPUITaskHistoryPanelObserver::Update(
	const ClassID&				theChange,
	ISubject*					theSubject,
	const PMIID &				protocol,
	void*						changedBy)
{
	LogFunctionEnterExit;
	
	do
	{
		if( theChange == kZPTaskInfoChangedMsg )
		{
			InterfacePtr<IZPUIHistoryPanelController> selfController( this, UseDefaultIID() );
			ASSERT(selfController);
			selfController->DoHistoryFetch( true);
		}
		else if( theChange == kZPWSResponseAvailable )
		{
			this->HandleWSResponse( (AZPSoapResponse *)changedBy );
		}
	}while(false);
}

//----------------------------------------------------------------------------------------
// HandleWSResponse
//----------------------------------------------------------------------------------------
void
CZPUITaskHistoryPanelObserver::HandleWSResponse(
	const AZPSoapResponse *		inWSResponse)
{
	if( inWSResponse->GetRespType() == eResp_GetTaskHistory )
	{
		this->HandleTaskHistoryResponse( inWSResponse );

	}
}

//----------------------------------------------------------------------------------------
// HandleTaskHistoryResponse
//----------------------------------------------------------------------------------------
void
CZPUITaskHistoryPanelObserver::HandleTaskHistoryResponse(
	const AZPSoapResponse *		inWSResponse)
{
	if( inWSResponse->GetRespType() == eResp_GetTaskHistory )
	{
		const CZPGetTaskHistoryResponse * theResponse = dynamic_cast<const CZPGetTaskHistoryResponse*>(inWSResponse);
		ASSERT(theResponse);

		InterfacePtr<IZPUIHistoryPanelController> selfController( this, UseDefaultIID() );
		ASSERT(selfController);
		selfController->UpdatePanel( theResponse->GetDataPtr() );
	}
}