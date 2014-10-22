//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/App/CZMAppObserver.cpp $
//	$Revision: 2910 $
//	$Date: 2011-04-29 16:02:51 +0200 (Fri, 29 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 29-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "ISubject.h"
#include "IWorkspace.h"

//SDK General includes
#include "CObserver.h"

//OS / ThirdParty includes

#include "IZMediaID.h"

//IZP Interfaces
#include "IZMPrefs.h"
#include "IZMModelController.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CObserver
class CZMAppObserver : public inheritClass
{
public:
						CZMAppObserver(IPMUnknown * boss);
	virtual				~CZMAppObserver();

	void				AutoAttach();
	void				AutoDetach();
	void				Update(
							const ClassID&				theChange,
							ISubject*					theSubject,
							const PMIID &				protocol,
							void*						changedBy);

	void				LazyUpdate(
							ISubject *					theSubject,
							const PMIID &				protocol,
							const LazyNotificationData *data);
protected:
private:
};


CREATE_PMINTERFACE(CZMAppObserver, kZMAppObserverImpl)

CZMAppObserver::CZMAppObserver(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZMAppObserver::~CZMAppObserver()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// AutoAttach
//----------------------------------------------------------------------------------------
void
CZMAppObserver::AutoAttach()
{
	InterfacePtr<const IWorkspace> iWorkspace(gSession->QueryWorkspace());
	ASSERT( iWorkspace );

	InterfacePtr<ISubject> workspaceSubject( iWorkspace, UseDefaultIID() );
	ASSERT(workspaceSubject);

	if(!workspaceSubject->IsAttached(ISubject::kLazyAttachment, this, IZMPrefs::kDefaultIID, this->GetAttachIID()))
		workspaceSubject->AttachObserver(ISubject::kLazyAttachment, this, IZMPrefs::kDefaultIID, this->GetAttachIID());
}

//----------------------------------------------------------------------------------------
// AutoDetach
//----------------------------------------------------------------------------------------
void
CZMAppObserver::AutoDetach()
{
	InterfacePtr<const IWorkspace> iWorkspace(gSession->QueryWorkspace());
	ASSERT( iWorkspace );

	InterfacePtr<ISubject> workspaceSubject( iWorkspace, UseDefaultIID() );
	ASSERT(workspaceSubject);

	if(workspaceSubject->IsAttached(ISubject::kLazyAttachment, this, IZMPrefs::kDefaultIID, this->GetAttachIID()))
		workspaceSubject->DetachObserver(ISubject::kLazyAttachment, this, IZMPrefs::kDefaultIID, this->GetAttachIID());
}

//----------------------------------------------------------------------------------------
// Update
//----------------------------------------------------------------------------------------
void
CZMAppObserver::Update(
	const ClassID&				theChange,
	ISubject*					theSubject,
	const PMIID &				protocol,
	void*						changedBy)
{
	if( protocol.Get() == IZMPrefs::kDefaultIID )
	{
		InterfacePtr<IZMModelController> modelController( gSession, UseDefaultIID() );
		ASSERT( modelController );

		modelController->FetchTitleList();
	}
}

//----------------------------------------------------------------------------------------
// LazyUpdate
//----------------------------------------------------------------------------------------
void
CZMAppObserver::LazyUpdate(
	ISubject *					theSubject,
	const PMIID &				protocol,
	const LazyNotificationData *data)
{
	if( protocol.Get() == IZMPrefs::kDefaultIID )
	{
		InterfacePtr<IZMModelController> modelController( gSession, UseDefaultIID() );
		ASSERT( modelController );

		modelController->FetchTitleList();
	}
}
