//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/App/CZPUICurrDocSelChangeObserver.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Aman Alam
//	Created: 12-7-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IActiveContext.h"
#include "IAssignment.h"
#include "IDocument.h"
#include "IEventWatcher.h"
#include "IInCopyWorkFlowUtils.h"
#include "IStandAloneDoc.h"
#include "IStringData.h"
#include "ISubject.h"
#include "IWorkgroupStoryFacade.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes
#include "CObserver.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPUIID.h"
#include "IZPID.h"

//IZP Interfaces
#include "IZPAssetData.h"
#include "IZPAssetInfo.h"
#include "IZPAssetLockInfo.h"
#include "IZPDocUtils.h"
#include "IZPInDnUtils.h"

//IZP General includes
#include "ZPDataHelper.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_App
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CObserver
class CZPUICurrDocSelChangeObserver : public inheritClass
{
public:
						CZPUICurrDocSelChangeObserver(IPMUnknown * boss);
	virtual				~CZPUICurrDocSelChangeObserver();

	void				AutoAttach();
	void				AutoDetach();
	void				Update(
							const ClassID&				theChange,
							ISubject*					theSubject,
							const PMIID &				protocol,
							void*						changedBy);
protected:
	void				ActiveDocChange(
							IActiveContext *			context);
	void				TemplateStoryCheckoutHookStartStop();
	void				StartWatchingEvent();
	void				StopWatchingEvent();
private:
	IDocument *			mCurrentDocument;
};


CREATE_PMINTERFACE(CZPUICurrDocSelChangeObserver, kZPUICurrDocSelChangeObserverImpl)

CZPUICurrDocSelChangeObserver::CZPUICurrDocSelChangeObserver(IPMUnknown * boss)
: inheritClass(boss)
, mCurrentDocument(nil)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPUICurrDocSelChangeObserver::~CZPUICurrDocSelChangeObserver()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// AutoAttach
//----------------------------------------------------------------------------------------
void
CZPUICurrDocSelChangeObserver::AutoAttach()
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	IActiveContext* context = gSession->GetActiveContext();
	if (context)
	{
		InterfacePtr<ISubject> contextSubject(context, UseDefaultIID());
		if (contextSubject->IsAttached(this, IID_IACTIVECONTEXT, IID_IZPUICURRDOCSELCHANGEOBSERVER) == false)
			contextSubject->AttachObserver(this, IID_IACTIVECONTEXT, IID_IZPUICURRDOCSELCHANGEOBSERVER);
	}
	
	this->ActiveDocChange(context);
}

//----------------------------------------------------------------------------------------
// AutoDetach
//----------------------------------------------------------------------------------------
void
CZPUICurrDocSelChangeObserver::AutoDetach()
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	IActiveContext* context = gSession->GetActiveContext();
	if (context)
	{
		InterfacePtr<ISubject> contextSubject(context, UseDefaultIID());
		if (contextSubject->IsAttached(this, IID_IACTIVECONTEXT, IID_IZPUICURRDOCSELCHANGEOBSERVER))
			contextSubject->DetachObserver(this, IID_IACTIVECONTEXT, IID_IZPUICURRDOCSELCHANGEOBSERVER);
	}

	if (mCurrentDocument)
		mCurrentDocument = nil;
}

//----------------------------------------------------------------------------------------
// Update
//----------------------------------------------------------------------------------------
void
CZPUICurrDocSelChangeObserver::Update(
	const ClassID&				theChange,
	ISubject*					theSubject,
	const PMIID &				protocol,
	void*						changedBy)
{
	switch (protocol.Get())
	{
	case IID_IACTIVECONTEXT:
		{
			IActiveContext::ContextInfo* contextInfo = reinterpret_cast<IActiveContext::ContextInfo*>(changedBy);
			if (contextInfo->Key() == IID_IDOCUMENT)
			{
				InterfacePtr<IActiveContext> context( theSubject, UseDefaultIID() );
				this->ActiveDocChange(context);
			}
		}

		break;
	}
}

#pragma mark -
//----------------------------------------------------------------------------------------
// ActiveDocChange
//----------------------------------------------------------------------------------------
void
CZPUICurrDocSelChangeObserver::ActiveDocChange(
	IActiveContext *			context)
{
	IDocument* theDocument = context->GetContextDocument();
	if (theDocument != mCurrentDocument)
	{
		mCurrentDocument = theDocument;

		if (mCurrentDocument)
		{
			PMString name;
			mCurrentDocument->GetName(name);
			IZPLog_Str_( thisFileLA, enLT_DebugInfo, "****** Document selection change %s", name.GrabCString());
		}

		this->TemplateStoryCheckoutHookStartStop();
	}
}

//----------------------------------------------------------------------------------------
// TemplateStoryCheckoutHookStartStop
//----------------------------------------------------------------------------------------
void
CZPUICurrDocSelChangeObserver::TemplateStoryCheckoutHookStartStop()
{
	if(Utils<IZPInDnUtils>()->IsHostAppInCopy() == false)
		return;	//This patch is required only in InCopy for IC template

	this->StopWatchingEvent();
	do
	{
		if (mCurrentDocument == nil)
			break;

		IDataBase* db = ::GetDataBase(mCurrentDocument);
		if(!db)
			break;

		InterfacePtr<IStandAloneDoc> standAlone(mCurrentDocument, IID_ISTANDALONEDOC);
		if (!standAlone || standAlone->IsStandAloneDoc() == false)	//IC template document are standalone documents
			break;

		UIDRef docRef = ::GetUIDRef( mCurrentDocument );
		UIDRef storyRef = Utils<IInCopyWorkFlowUtils>()->GetStandaloneStory( docRef );
		if (storyRef == UIDRef::gNull)
			break;

		UIDRef storyAssetUIDRef = Utils<Facade::IWorkgroupStoryFacade>()->GetAssetUIDRef( storyRef );
		if (storyAssetUIDRef == UIDRef::gNull)
			break;

		InterfacePtr<const IZPAssetData> asset(storyAssetUIDRef, UseDefaultIID());
		if(!asset)
			break;

		const PMString & theAssetIDStr = asset->GetAssetID();
		if( theAssetIDStr.IsEmpty() )		//Handle only if our Asset Data.
			break;

		const IStringData * dbAssetID = ZPDataHelper::GetAssetByID( theAssetIDStr );
		if( !dbAssetID )
			break;

		InterfacePtr<const IZPAssetInfo> dbAssetInfo( dbAssetID, UseDefaultIID() );
		ASSERT( dbAssetInfo );
		if( dbAssetInfo && dbAssetInfo->GetType() == eAssetType_ICTemplate )
		{
			InterfacePtr<const IZPAssetLockInfo> assetLockInfo( dbAssetInfo, UseDefaultIID());
			if (assetLockInfo && assetLockInfo->GetLockState() == IAMLockable::enAvailable)
				this->StartWatchingEvent();
			//else already stopped above.
		}
	}while(kFalse);
}

//----------------------------------------------------------------------------------------
// StartWatchingEvent
//----------------------------------------------------------------------------------------
void
CZPUICurrDocSelChangeObserver::StartWatchingEvent()
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IEventWatcher> eventWatcher( gSession , IID_IZPUISTORYCHECKOUTEVENTWATCHER );
	if (eventWatcher)
		eventWatcher->StartWatching();
}

//----------------------------------------------------------------------------------------
// StopWatchingEvent
//----------------------------------------------------------------------------------------
void
CZPUICurrDocSelChangeObserver::StopWatchingEvent()
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IEventWatcher> eventWatcher( gSession , IID_IZPUISTORYCHECKOUTEVENTWATCHER );
	if (eventWatcher)
		eventWatcher->StopWatching();
}
