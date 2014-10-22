//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Controls/AdsTreeView/TVNodeAd/CZMAdsTVAdNodeEH.cpp $
//	$Revision: 2740 $
//	$Date: 2011-04-07 14:23:49 +0200 (Thu, 07 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 28-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPluginHeaders.h"

//SDK Interfaces
#include "IEvent.h"
#include "ISubject.h"
#include "ITreeNodeIDData.h"


//SDK General includes
#include "CEventHandler.h"

//OS / ThirdParty includes

#include "iZMediaID.h"

//IZP Interfaces
#include "IZPLog.h"

//IZP General includes

#pragma mark -
#include "IZPLog.h"

#define thisFileLA	enLMA_None //AssetsTV
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CEventHandler
class ZMAdsTVAdNodeEH : public inheritClass
{
public:
						ZMAdsTVAdNodeEH(IPMUnknown * boss);
	virtual				~ZMAdsTVAdNodeEH();
	
	/**  Window has been activated. Traditional response is to
	 activate the controls in the window.
	 @param e [IN] event of interest
	 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
	 */
	virtual bool16 Activate(IEvent* e);
	
	/** Window has been deactivated. Traditional response is to
	 deactivate the controls in the window.
	 @param e [IN] event of interest
	 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
	 */
	virtual bool16 Deactivate(IEvent* e);
	
	/** Application has been suspended. Control is passed to
	 another application.
	 @param e [IN] event of interest
	 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
	 */
	virtual bool16 Suspend(IEvent* e);
	
	/** Application has been resumed. Control is passed back to the
	 application from another application.
	 @param e [IN] event of interest
	 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
	 */
	virtual bool16 Resume(IEvent* e);
	
	/** Mouse has moved outside the sensitive region.
	 @param e [IN] event of interest
	 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
	 */
	virtual bool16 MouseMove(IEvent* e);
	
	/** User is holding down the mouse button and dragging the mouse.
	 @param e [IN] event of interest
	 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
	 */
	virtual bool16 MouseDrag(IEvent* e);
	
	/** Left mouse button (or only mouse button) has been pressed.
	 @param e [IN] event of interest
	 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
	 */ 
	virtual bool16 LButtonDn(IEvent* e);
	
	/** Right mouse button (or second button) has been pressed.
	 @param e [IN] event of interest
	 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
	 */
	virtual bool16 RButtonDn(IEvent* e);
	
	/** Middle mouse button of a 3 button mouse has been pressed.
	 @param e [IN] event of interest
	 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
	 */
	virtual bool16 MButtonDn(IEvent* e);
	
	/** Left mouse button released.
	 @param e [IN] event of interest
	 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
	 */
	virtual bool16 LButtonUp(IEvent* e);
	
	/** Right mouse button released.
	 @param e [IN] event of interest
	 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
	 */
	virtual bool16 RButtonUp(IEvent* e);
	
	/** Middle mouse button released.
	 @param e [IN] event of interest
	 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
	 */
	virtual bool16 MButtonUp(IEvent* e);
	
	/** Double click with any button; this is the only event that we're interested in here-
	 on this event we load the placegun with an asset if it can be imported.
	 @param e [IN] event of interest
	 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
	 */
	virtual bool16 ButtonDblClk(IEvent* e);
	/** Triple click with any button.
	 @param e [IN] event of interest
	 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
	 */
	virtual bool16 ButtonTrplClk(IEvent* e);
	
	/** Quadruple click with any button.
	 @param e [IN] event of interest
	 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
	 */
	virtual bool16 ButtonQuadClk(IEvent* e);
	
	/** Quintuple click with any button.
	 @param e [IN] event of interest
	 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
	 */
	virtual bool16 ButtonQuintClk(IEvent* e);
	
	/** mouse scroll wheel
	 @param e [IN] event of interest
	 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
	 */
	virtual bool16 MouseWheel(IEvent* e);
	
	/** Event for a particular control. Used only on Windows.
	 @param e [IN] event of interest
	 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
	 */
	virtual bool16 ControlCmd(IEvent* e);
	
	
	// Keyboard Related Events
	
	/** Keyboard key down for every key.  Normally you want to override KeyCmd, rather than KeyDown.
	 @param e [IN] event of interest
	 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
	 */
	virtual bool16 KeyDown(IEvent* e);
	
	/** Keyboard key down that generates a character.
	 @param e [IN] event of interest
	 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
	 */
	virtual bool16 KeyCmd(IEvent* e);
	
	/** Keyboard key released.
	 @param e [IN] event of interest
	 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
	 */
	virtual bool16 KeyUp(IEvent* e);
	
	
	// Keyboard Focus Related Functions
	
	/** Key focus is now passed to the window.
	 @param e [IN] event of interest
	 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
	 */
	virtual bool16 GetKeyFocus(IEvent* e);
	
	/** Window has lost key focus.
	 @param e [IN] event of interest
	 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
	 */
	virtual bool16 GiveUpKeyFocus(IEvent* e);
	
	/** Typically called before GiveUpKeyFocus() is called. Return kFalse
	 to hold onto the keyboard focus.
	 @return kFalse to hold onto the keyboard focus
	 */
	virtual bool16 WillingToGiveUpKeyFocus();
	
	/** The keyboard is temporarily being taken away. Remember enough state
	 to resume where you left off.
	 @return kTrue if you really suspended
	 yourself. If you simply gave up the keyboard, return kFalse.
	 */
	virtual bool16 SuspendKeyFocus();
	
	/** The keyboard has been handed back.
	 @return kTrue if you resumed yourself. Otherwise, return kFalse.
	 */
	virtual bool16 ResumeKeyFocus();
	
	/** Determine if this eventhandler can be focus of keyboard event
	 @return kTrue if this eventhandler supports being the focus
	 of keyboard event
	 */
	virtual bool16 CanHaveKeyFocus() const;
	
	/** Return kTrue if this event handler wants to get keyboard focus
	 while tabbing through widgets. Note: For almost all event handlers
	 CanHaveKeyFocus and WantsTabKeyFocus will return the same value.
	 If WantsTabKeyFocus returns kTrue then CanHaveKeyFocus should also return kTrue
	 for the event handler to actually get keyboard focus. If WantsTabKeyFocus returns
	 kFalse then the event handler is skipped.
	 @return kTrue if event handler wants to get focus during tabbing, kFalse otherwise
	 */
	virtual bool16 WantsTabKeyFocus() const;
	
	/** Window needs to repaint.
	 @param e [IN] event of interest
	 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
	 */
	virtual bool16 Update(IEvent* e);
	
	/** Method to handle platform specific events
	 @param e [IN] event of interest
	 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
	 */
	virtual bool16 PlatformEvent(IEvent* e);
	
	/** Call the base system event handler.
	 @param e [IN] event of interest
	 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
	 */
	virtual bool16 CallSysEventHandler(IEvent* e);
	
	
	/** Temporary.
	 */
	virtual void SetView(IControlView* view);
protected:
private:
	PMIID	mDelegateIID;
};


CREATE_PMINTERFACE(ZMAdsTVAdNodeEH, kZMAdsTVAdNodeEHImpl)

ZMAdsTVAdNodeEH::ZMAdsTVAdNodeEH(IPMUnknown * boss)
: inheritClass(boss)
, mDelegateIID( IID_IZMOLDEVENTHANDLER )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
ZMAdsTVAdNodeEH::~ZMAdsTVAdNodeEH()
{

}

#pragma mark -

/**  Window has been activated. Traditional response is to
 activate the controls in the window.
 @param e [IN] event of interest
 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
 */
bool16
ZMAdsTVAdNodeEH::Activate(
	IEvent* e)
{
	LogFunctionEnterExit;
	InterfacePtr<IEventHandler> orgImpl( this, mDelegateIID );
	bool16 toReturn = orgImpl->Activate( e );
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
	return toReturn;

}

/** Window has been deactivated. Traditional response is to
 deactivate the controls in the window.
 @param e [IN] event of interest
 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
 */
bool16
ZMAdsTVAdNodeEH::Deactivate(
	IEvent* e)
{
	LogFunctionEnterExit;
	InterfacePtr<IEventHandler> orgImpl( this, mDelegateIID );
	bool16 toReturn = orgImpl->Deactivate( e );
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
	return toReturn;
}

/** Application has been suspended. Control is passed to
 another application.
 @param e [IN] event of interest
 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
 */
bool16
ZMAdsTVAdNodeEH::Suspend(
	IEvent* e)
{
	LogFunctionEnterExit;
	InterfacePtr<IEventHandler> orgImpl( this, mDelegateIID );
	bool16 toReturn = orgImpl->Suspend( e );
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
	return toReturn;
}

/** Application has been resumed. Control is passed back to the
 application from another application.
 @param e [IN] event of interest
 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
 */
bool16
ZMAdsTVAdNodeEH::Resume(
	IEvent* e)
{
	LogFunctionEnterExit;
	InterfacePtr<IEventHandler> orgImpl( this, mDelegateIID );
	bool16 toReturn = orgImpl->Resume( e );
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
	return toReturn;
}

/** Mouse has moved outside the sensitive region.
 @param e [IN] event of interest
 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
 */
bool16
ZMAdsTVAdNodeEH::MouseMove(
	IEvent* e)
{
	LogFunctionEnterExit;
	InterfacePtr<IEventHandler> orgImpl( this, mDelegateIID );
	bool16 toReturn = orgImpl->MouseMove( e );
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
	return toReturn;
}

/** User is holding down the mouse button and dragging the mouse.
 @param e [IN] event of interest
 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
 */
bool16
ZMAdsTVAdNodeEH::MouseDrag(
	IEvent* e)
{
	LogFunctionEnterExit;
	InterfacePtr<IEventHandler> orgImpl( this, mDelegateIID );
	bool16 toReturn = orgImpl->MouseDrag( e );
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
	return toReturn;
}

/** Left mouse button (or only mouse button) has been pressed.
 @param e [IN] event of interest
 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
 */ 
bool16
ZMAdsTVAdNodeEH::LButtonDn(
	IEvent* e)
{
	LogFunctionEnterExit;
	InterfacePtr<IEventHandler> orgImpl( this, mDelegateIID );
	bool16 toReturn = orgImpl->LButtonDn( e );
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
	return toReturn;
}

/** Right mouse button (or second button) has been pressed.
 @param e [IN] event of interest
 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
 */
bool16
ZMAdsTVAdNodeEH::RButtonDn(
	IEvent* e)
{
	LogFunctionEnterExit;
	InterfacePtr<IEventHandler> orgImpl( this, mDelegateIID );
	bool16 toReturn = orgImpl->RButtonDn( e );
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
	return toReturn;
}

/** Middle mouse button of a 3 button mouse has been pressed.
 @param e [IN] event of interest
 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
 */
bool16
ZMAdsTVAdNodeEH::MButtonDn(
	IEvent* e)
{
	LogFunctionEnterExit;
	InterfacePtr<IEventHandler> orgImpl( this, mDelegateIID );
	bool16 toReturn = orgImpl->MButtonDn( e );
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
	return toReturn;
}

/** Left mouse button released.
 @param e [IN] event of interest
 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
 */
bool16
ZMAdsTVAdNodeEH::LButtonUp(
	IEvent* e)
{
	LogFunctionEnterExit;
	InterfacePtr<IEventHandler> orgImpl( this, mDelegateIID );
	bool16 toReturn = orgImpl->LButtonUp( e );
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
	return toReturn;
}

/** Right mouse button released.
 @param e [IN] event of interest
 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
 */
bool16
ZMAdsTVAdNodeEH::RButtonUp(
	IEvent* e)
{
	LogFunctionEnterExit;
	InterfacePtr<IEventHandler> orgImpl( this, mDelegateIID );
	bool16 toReturn = orgImpl->RButtonUp( e );
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
	return toReturn;
}

/** Middle mouse button released.
 @param e [IN] event of interest
 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
 */
bool16
ZMAdsTVAdNodeEH::MButtonUp(
	IEvent* e)
{
	LogFunctionEnterExit;
	InterfacePtr<IEventHandler> orgImpl( this, mDelegateIID );
	bool16 toReturn = orgImpl->MButtonUp( e );
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
	return toReturn;
}

/** Double click with any button; this is the only event that we're interested in here-
 on this event we load the placegun with an asset if it can be imported.
 @param e [IN] event of interest
 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
 */
bool16 
ZMAdsTVAdNodeEH::ButtonDblClk(
	IEvent* e)
{
	LogFunctionEnterExit;
	
	InterfacePtr<ISubject> selfSubject(this, UseDefaultIID());
	ASSERT(selfSubject);

	selfSubject->Change( kZMUIAdsTV_AdNodeDoubleClickPressedMsg, IEventHandler::kDefaultIID, e );

	InterfacePtr<IEventHandler> orgImpl( this, mDelegateIID );
	bool16 toReturn = orgImpl->ButtonDblClk( e );
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
	return toReturn;

}

/** Triple click with any button.
 @param e [IN] event of interest
 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
 */
bool16
ZMAdsTVAdNodeEH::ButtonTrplClk(
	IEvent* e)
{
	LogFunctionEnterExit;
	InterfacePtr<IEventHandler> orgImpl( this, mDelegateIID );
	bool16 toReturn = orgImpl->ButtonTrplClk( e );
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
	return toReturn;

}

/** Quadruple click with any button.
 @param e [IN] event of interest
 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
 */
bool16
ZMAdsTVAdNodeEH::ButtonQuadClk(
	IEvent* e)
{
	LogFunctionEnterExit;
	InterfacePtr<IEventHandler> orgImpl( this, mDelegateIID );
	bool16 toReturn = orgImpl->ButtonQuadClk( e );
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
	return toReturn;
}

/** Quintuple click with any button.
 @param e [IN] event of interest
 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
 */
bool16
ZMAdsTVAdNodeEH::ButtonQuintClk(
	IEvent* e)
{
	LogFunctionEnterExit;
	InterfacePtr<IEventHandler> orgImpl( this, mDelegateIID );
	bool16 toReturn = orgImpl->ButtonQuintClk( e );
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
	return toReturn;
}

/** mouse scroll wheel
 @param e [IN] event of interest
 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
 */
bool16
ZMAdsTVAdNodeEH::MouseWheel(
	IEvent* e)
{
	LogFunctionEnterExit;
	InterfacePtr<IEventHandler> orgImpl( this, mDelegateIID );
	bool16 toReturn = orgImpl->MouseWheel( e );
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
	return toReturn;
}

/** Event for a particular control. Used only on Windows.
 @param e [IN] event of interest
 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
 */
bool16
ZMAdsTVAdNodeEH::ControlCmd(
	IEvent* e)
{
	LogFunctionEnterExit;
	InterfacePtr<IEventHandler> orgImpl( this, mDelegateIID );
	bool16 toReturn = orgImpl->ControlCmd( e );
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
	return toReturn;
}


#pragma mark -
// Keyboard Related Events

/** Keyboard key down for every key.  Normally you want to override KeyCmd, rather than KeyDown.
 @param e [IN] event of interest
 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
 */
bool16
ZMAdsTVAdNodeEH::KeyDown(
	IEvent* e)
{
	LogFunctionEnterExit;
	InterfacePtr<IEventHandler> orgImpl( this, mDelegateIID );
	bool16 toReturn = orgImpl->KeyDown( e );
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
	return toReturn;
}

/** Keyboard key down that generates a character.
 @param e [IN] event of interest
 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
 */
bool16
ZMAdsTVAdNodeEH::KeyCmd(
	IEvent* e)
{
	LogFunctionEnterExit;
	InterfacePtr<IEventHandler> orgImpl( this, mDelegateIID );
	bool16 toReturn = orgImpl->KeyCmd( e );
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
	return toReturn;
}

/** Keyboard key released.
 @param e [IN] event of interest
 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
 */
bool16
ZMAdsTVAdNodeEH::KeyUp(
	IEvent* e)
{
	LogFunctionEnterExit;
	InterfacePtr<IEventHandler> orgImpl( this, mDelegateIID );
	bool16 toReturn = orgImpl->KeyUp( e );
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
	return toReturn;
}

// Keyboard Focus Related Functions

/** Key focus is now passed to the window.
 @param e [IN] event of interest
 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
 */
bool16
ZMAdsTVAdNodeEH::GetKeyFocus(
	IEvent* e)
{
	LogFunctionEnterExit;
	InterfacePtr<IEventHandler> orgImpl( this, mDelegateIID );
	bool16 toReturn = orgImpl->GetKeyFocus( e );
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
	return toReturn;
}

/** Window has lost key focus.
 @param e [IN] event of interest
 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
 */
bool16
ZMAdsTVAdNodeEH::GiveUpKeyFocus(
	IEvent* e)
{
	LogFunctionEnterExit;
	InterfacePtr<IEventHandler> orgImpl( this, mDelegateIID );
	bool16 toReturn = orgImpl->GiveUpKeyFocus( e );
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
	return toReturn;
}

/** Typically called before GiveUpKeyFocus() is called. Return kFalse
 to hold onto the keyboard focus.
 @return kFalse to hold onto the keyboard focus
 */
bool16
ZMAdsTVAdNodeEH::WillingToGiveUpKeyFocus()
{
	LogFunctionEnterExit;
	InterfacePtr<IEventHandler> orgImpl( this, mDelegateIID );
	bool16 toReturn = orgImpl->WillingToGiveUpKeyFocus(  );
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
	return toReturn;
}

/** The keyboard is temporarily being taken away. Remember enough state
 to resume where you left off.
 @return kTrue if you really suspended
 yourself. If you simply gave up the keyboard, return kFalse.
 */
bool16
ZMAdsTVAdNodeEH::SuspendKeyFocus()
{
	LogFunctionEnterExit;
	InterfacePtr<IEventHandler> orgImpl( this, mDelegateIID );
	bool16 toReturn = orgImpl->SuspendKeyFocus(  );
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
	return toReturn;
}

/** The keyboard has been handed back.
 @return kTrue if you resumed yourself. Otherwise, return kFalse.
 */
bool16
ZMAdsTVAdNodeEH::ResumeKeyFocus()
{
	LogFunctionEnterExit;
	InterfacePtr<IEventHandler> orgImpl( this, mDelegateIID );
	bool16 toReturn = orgImpl->ResumeKeyFocus(  );
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
	return toReturn;

}

/** Determine if this eventhandler can be focus of keyboard event
 @return kTrue if this eventhandler supports being the focus
 of keyboard event
 */
bool16
ZMAdsTVAdNodeEH::CanHaveKeyFocus()const
{
	LogFunctionEnterExit;
	InterfacePtr<IEventHandler> orgImpl( this, mDelegateIID );
	bool16 toReturn = orgImpl->CanHaveKeyFocus(  );
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
	return toReturn;

}

/** Return kTrue if this event handler wants to get keyboard focus
 while tabbing through widgets. Note: For almost all event handlers
 CanHaveKeyFocus and WantsTabKeyFocus will return the same value.
 If WantsTabKeyFocus returns kTrue then CanHaveKeyFocus should also return kTrue
 for the event handler to actually get keyboard focus. If WantsTabKeyFocus returns
 kFalse then the event handler is skipped.
 @return kTrue if event handler wants to get focus during tabbing, kFalse otherwise
 */
bool16
ZMAdsTVAdNodeEH::WantsTabKeyFocus() const
{
	LogFunctionEnterExit;
	InterfacePtr<IEventHandler> orgImpl( this, mDelegateIID );
	bool16 toReturn = orgImpl->WantsTabKeyFocus(  );
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
	return toReturn;

}

/** Window needs to repaint.
 @param e [IN] event of interest
 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
 */
bool16
ZMAdsTVAdNodeEH::Update(
	IEvent* e)
{
	LogFunctionEnterExit;
	InterfacePtr<IEventHandler> orgImpl( this, mDelegateIID );
	bool16 toReturn = orgImpl->Update( e );
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
	return toReturn;

}

/** Method to handle platform specific events
 @param e [IN] event of interest
 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
 */
bool16
ZMAdsTVAdNodeEH::PlatformEvent(
	IEvent* e)
{
	LogFunctionEnterExit;
	InterfacePtr<IEventHandler> orgImpl( this, mDelegateIID );
	bool16 toReturn = orgImpl->PlatformEvent( e );
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
	return toReturn;

}

/** Call the base system event handler.
 @param e [IN] event of interest
 @return kTrue if event has been handled and should not be further dispatched, kFalse otherwise (pass event to next handler)
 */
bool16
ZMAdsTVAdNodeEH::CallSysEventHandler(
	IEvent* e)
{
	LogFunctionEnterExit;
	InterfacePtr<IEventHandler> orgImpl( this, mDelegateIID );
	bool16 toReturn = orgImpl->CallSysEventHandler( e );
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "Out :  ", toReturn );
	return toReturn;

}

//----------------------------------------------------------------------------------------
// SetView
//----------------------------------------------------------------------------------------
void
ZMAdsTVAdNodeEH::SetView(
	IControlView* view)
{
	LogFunctionEnterExit;
	InterfacePtr<IEventHandler> orgImpl( this, mDelegateIID );
	orgImpl->SetView(view);
}
