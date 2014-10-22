//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/EventWatcher/CZPUIStoryCheckoutEventWatcher.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Aman Alam
//	Created: 8-7-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IActionManager.h"
#include "IActiveContext.h"
#include "IApplication.h"
#include "IEventDispatcher.h"
#include "IEventHandler.h"
#include "IEventWatcher.h"
#if defined(InDnCS5) || defined(InDnCS5_5)	//TODO: CS5 Porting
#include "ILiveEditSelectionSuite.h"
#else
#include "IInCopyBridgeSelectionSuite.h"
#endif
#include "IInCopyBridgeUIUtils.h"
#include "IKeyBoard.h"
#include "ISelectionManager.h"
#include "IStoryRefData.h"
#include "ITextEditor.h"
#include "ITextFocus.h"
#include "ITextModel.h"
#include "IWorkgroupStoryFacade.h"
#include "IItemLockData.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes
#include "KeyboardDefs.h"
#include "TextEditorID.h"
#include "WritingModeID.h"
#include "CAlert.h"

//OS / ThirdParty includes

#include "IZPUIID.h"

//IZP Interfaces

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_ALL
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IEventWatcher>
class CZPUIStoryCheckoutEventWatcher : public inheritClass
{
public:
						CZPUIStoryCheckoutEventWatcher(IPMUnknown * boss);
	virtual				~CZPUIStoryCheckoutEventWatcher();

	IEventDispatcher::EventTypeList  WatchEvent(IEvent *e);
	void StartWatching();
	void StopWatching();
protected:
private:
	bool16 fWatching;
};


CREATE_PMINTERFACE(CZPUIStoryCheckoutEventWatcher, kZPUIStoryCheckoutEventWatcherImpl)

CZPUIStoryCheckoutEventWatcher::CZPUIStoryCheckoutEventWatcher(IPMUnknown * boss)
:inheritClass(boss), 
 fWatching(kFalse)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPUIStoryCheckoutEventWatcher::~CZPUIStoryCheckoutEventWatcher()
{

}

//----------------------------------------------------------------------------------------
// EventTypeList
//----------------------------------------------------------------------------------------
#pragma mark -
IEventDispatcher::EventTypeList
CZPUIStoryCheckoutEventWatcher::WatchEvent(
	IEvent *e)
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	IEventDispatcher::EventTypeList myInterest(IEvent::kKeyDown/* , IEvent::kKey, IEvent::kKeyUp */);
	
	IEvent::EventType etype = e->GetType();

	switch (e->GetType())
	{
		case IEvent::kKeyDown:
			if( !fWatching )
				return ~ IEventDispatcher::EventTypeList();	//This will stop calling event watcher
			break;
		/* case IEvent::kKey:
			break;
		case IEvent::kKeyUp:
			break; */
		default:
			return myInterest;
	}

	do
	{
		if (e->OptionAltKeyDown() || e->CmdKeyDown())
			break;

		SysChar c = e->GetChar();

		switch (c)
		{
			case kReturnKey:
			//case kTabKey:
			case kSpaceKey:
//#ifdef WINDOWS
//			case VK_OEM_PERIOD:
//			case VK_OEM_COMMA:
//			case VK_SEPARATOR:
//			//case VK_SLASH:
//			case 0xbf:	// '/' and '?'
//#endif
//#ifdef MACINTOSH
//			case 0x2e: //period
//			case 0x2c: //comma
//			//case 0x2d: //hyphen
//			case 0x2f: //slash
//			case 0x3f: //?
//#endif
				break;

			//Don't process further
			/*case kPageUpKey:
			case kPageDownKey:
			case kHomeKey:
			case kEndKey:
			case kUpArrowKey:
			case kDownArrowKey:
			case kRightArrowKey:
			case kLeftArrowKey:*/
			default:
				if ((c >= 'a' && c<= 'z') || (c >= 'A' && c<= 'Z') || (c >= '0' && c<= '9'))
					break;
				return myInterest;
		}

		InterfacePtr<const IApplication> theApp( gSession->QueryApplication() );
		InterfacePtr<const IKeyBoard> keyboard( theApp, UseDefaultIID() );
		if (!keyboard)
			break;

		IEventHandler * target = keyboard->GetKeyFocus();
		if (!target)
			break;

		ClassID bossId = ::GetClass(target);
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Event target Boss id : %08X", bossId.Get());
		if (! (bossId == kTextEditorBoss || bossId == kWritingModeWidgetBoss))
		{
			IZPLog_Str_( thisFileLA, enLT_DebugInfo, "NOT our target event handler.");
			break;
		}

		fWatching = false;
		myInterest = ~ IEventDispatcher::EventTypeList();	//This will stop calling event watcher.

		UIDRef storyUIDRef = UIDRef::gNull;
		if (bossId == kTextEditorBoss)
		{
			InterfacePtr<ITextEditor> textEditor( target, UseDefaultIID() );
			if (!textEditor)
				break;

			InterfacePtr<const ITextFocus> textFocus( textEditor->QueryTextFocus());
			if (!textFocus)
				break;

			InterfacePtr<const ITextModel> textModel( textFocus->QueryModel());
			if (!textModel)
				break;

			storyUIDRef = ::GetUIDRef(textModel);
		}
		else if (bossId == kWritingModeWidgetBoss)
		{
			InterfacePtr<IStoryRefData> storyRefData( target, UseDefaultIID() );
			if (!storyRefData)
				break;

			storyUIDRef = storyRefData->GetStoryRef();
		}

		if (storyUIDRef == UIDRef::gNull)
			break;

		UIDRef storyAssetUIDRef = Utils<Facade::IWorkgroupStoryFacade>()->GetAssetUIDRef( storyUIDRef );
		if (storyAssetUIDRef == UIDRef::gNull)
			break;

		InterfacePtr<IActiveContext> ac(gSession->GetActiveContext(), UseDefaultIID());
		ASSERT(ac);
		
#if defined(InDnCS5) || defined(InDnCS5_5)	//TODO: CS5 Porting
		InterfacePtr<ILiveEditSelectionSuite> selection (ac->GetContextSelection(), UseDefaultIID());
		ASSERT(selection);		
#else
		InterfacePtr<IInCopyBridgeSelectionSuite> selection (ac->GetContextSelection(), UseDefaultIID());
		ASSERT(selection);
#endif
		if (!selection)
			break;

#if defined(InDnCS5) || defined(InDnCS5_5)
		if (!selection->CanCheckOut())
#else
		if (!selection->CanEditInPlace())
#endif
		{
			fWatching = true;
			myInterest = IEventDispatcher::EventTypeList(IEvent::kKeyDown);
			break;
		}

		PMString msg("ICShared_WantCheckOut", PMString::kTranslateDuringCall);
		if( CAlert::ModalAlert(msg, kYesString, kNoString, kNullString, 1, CAlert::eWarningIcon) != 1 )
		{
			fWatching = true;
			myInterest = IEventDispatcher::EventTypeList(IEvent::kKeyDown);
			break;
		}

		//Checkout
		InterfacePtr<IItemLockData> storyLockData( storyUIDRef, UseDefaultIID() );
#if defined(InDnCS5) || defined(InDnCS5_5)
		if( false == selection->CheckOut() || (storyLockData && storyLockData->GetInsertLock()))
#else
		if( false == selection->EditInPlace() || (storyLockData && storyLockData->GetInsertLock()))
#endif
		{	//Failed to get lock
			fWatching = true;
			myInterest = IEventDispatcher::EventTypeList(IEvent::kKeyDown);
		}
	}while(kFalse);

	return myInterest;
}

//----------------------------------------------------------------------------------------
// StartWatching
//----------------------------------------------------------------------------------------
void
CZPUIStoryCheckoutEventWatcher::StartWatching()
{
	LogFunctionEnterExit;
	if (fWatching) 
		return;
	
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IApplication> app(gSession->QueryApplication());
	InterfacePtr<IEventDispatcher> dispatcher(app, UseDefaultIID());
	if (dispatcher)
	{
		IEventDispatcher::EventTypeList myInterest(IEvent::kKeyDown/* , IEvent::kKey, IEvent::kKeyUp */);
		dispatcher->AddEventWatcher(this, myInterest);
		fWatching = kTrue;
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Event watcher added.");
	}
}

//----------------------------------------------------------------------------------------
// StopWatching
//----------------------------------------------------------------------------------------
void
CZPUIStoryCheckoutEventWatcher::StopWatching()
{
	LogFunctionEnterExit;
	if (!fWatching) 
		return;

#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IApplication> app(gSession->QueryApplication());
	InterfacePtr<IEventDispatcher> dispatcher(app, UseDefaultIID());
	if (dispatcher)
	{
		fWatching = kFalse;
		dispatcher->RemoveEventWatcher(this, IEventDispatcher::EventTypeList());
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Event watcher removed.");
	}
}
