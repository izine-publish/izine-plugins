//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Dialogs/TaskHistory/Windows/IOleInPlaceSiteFunctions.c $
//	$Revision: 2368 $
//	$Date: 2010-10-22 11:58:01 +0200 (Fri, 22 Oct 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 22-10-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#include <windows.h>
#include <exdisp.h>		// Defines of stuff like IWebBrowser2. This is an include file with Visual C 6 and above
#include <mshtml.h>		// Defines of stuff like IHTMLDocument2. This is an include file with Visual C 6 and above
#include <mshtmhst.h>	// Defines of stuff like IDocHostUIHandler. This is an include file with Visual C 6 and above
#include <crtdbg.h>		// for _ASSERT()

//SDK Interfaces

//SDK General includes

//OS / ThirdParty includes


//IZP Interfaces

//IZP General includes
#include "IOleInPlaceSiteFunctions.h"
#include "IOleClientSiteFunctions.h"
#include "ZPWBTypedefs.h"

IOleInPlaceSiteVtbl gIOleInPlaceSiteTable =  {InPlace_QueryInterface,
						InPlace_AddRef,
						InPlace_Release,
						InPlace_GetWindow,
						InPlace_ContextSensitiveHelp,
						InPlace_CanInPlaceActivate,
						InPlace_OnInPlaceActivate,
						InPlace_OnUIActivate,
						InPlace_GetWindowContext,
						InPlace_Scroll,
						InPlace_OnUIDeactivate,
						InPlace_OnInPlaceDeactivate,
						InPlace_DiscardUndoState,
						InPlace_DeactivateAndUndo,
						InPlace_OnPosRectChange};


////////////////////////////////////// My IOleInPlaceSite functions  /////////////////////////////////////
// The browser object asks us for the pointer to our IOleInPlaceSite object by calling our IOleClientSite's
// QueryInterface (ie, Site_QueryInterface) and specifying a REFIID of IID_IOleInPlaceSite.

HRESULT STDMETHODCALLTYPE InPlace_QueryInterface(IOleInPlaceSite FAR* This, REFIID riid, LPVOID FAR* ppvObj)
{
	// The browser assumes that our IOleInPlaceSite object is associated with our IOleClientSite
	// object. So it is possible that the browser may call our IOleInPlaceSite's QueryInterface()
	// to ask us to return a pointer to our IOleClientSite, in the same way that the browser calls
	// our IOleClientSite's QueryInterface() to ask for a pointer to our IOleInPlaceSite.
	//
	// Rather than duplicate much of the code in IOleClientSite's QueryInterface, let's just get
	// a pointer to our _IOleClientSiteEx object, substitute it as the 'This' arg, and call our
	// our IOleClientSite's QueryInterface. Note that since our IOleInPlaceSite is embedded right
	// inside our _IOleClientSiteEx, and comes immediately after the IOleClientSite, we can employ
	// the following trickery to get the pointer to our _IOleClientSiteEx.
	return(Site_QueryInterface((IOleClientSite *)((char *)This - sizeof(IOleClientSite)), riid, ppvObj));
}

HRESULT STDMETHODCALLTYPE InPlace_AddRef(IOleInPlaceSite FAR* This)
{
	return(1);
}

HRESULT STDMETHODCALLTYPE InPlace_Release(IOleInPlaceSite FAR* This)
{
	return(1);
}

HRESULT STDMETHODCALLTYPE InPlace_GetWindow(IOleInPlaceSite FAR* This, HWND FAR* lphwnd)
{
	// Return the HWND of the window that contains this browser object. We stored that
	// HWND in our _IOleInPlaceSiteEx struct. Nevermind that the function declaration for
	// Site_GetWindow says that 'This' is an IOleInPlaceSite *. Remember that in
	// EmbedBrowserObject(), we allocated our own _IOleInPlaceSiteEx struct which
	// contained an embedded IOleInPlaceSite struct within it. And when the browser
	// called Site_QueryInterface() to get a pointer to our IOleInPlaceSite object, we
	// returned a pointer to our _IOleClientSiteEx. The browser doesn't know this. But
	// we do. That's what we're really being passed, so we can recast it and use it as
	// so here.
	*lphwnd = ((_IOleInPlaceSiteEx FAR*)This)->frame.window;

	return(S_OK);
}

HRESULT STDMETHODCALLTYPE InPlace_ContextSensitiveHelp(IOleInPlaceSite FAR* This, BOOL fEnterMode)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE InPlace_CanInPlaceActivate(IOleInPlaceSite FAR* This)
{
	// Tell the browser we can in place activate
	return(S_OK);
}

HRESULT STDMETHODCALLTYPE InPlace_OnInPlaceActivate(IOleInPlaceSite FAR* This)
{
	// Tell the browser we did it ok
	return(S_OK);
}

HRESULT STDMETHODCALLTYPE InPlace_OnUIActivate(IOleInPlaceSite FAR* This)
{
	return(S_OK);
}

HRESULT STDMETHODCALLTYPE InPlace_GetWindowContext(IOleInPlaceSite FAR* This, LPOLEINPLACEFRAME FAR* lplpFrame, LPOLEINPLACEUIWINDOW FAR* lplpDoc, LPRECT lprcPosRect, LPRECT lprcClipRect, LPOLEINPLACEFRAMEINFO lpFrameInfo)
{
	// Give the browser the pointer to our IOleInPlaceFrame struct. We stored that pointer
	// in our _IOleInPlaceSiteEx struct. Nevermind that the function declaration for
	// Site_GetWindowContext says that 'This' is an IOleInPlaceSite *. Remember that in
	// EmbedBrowserObject(), we allocated our own _IOleInPlaceSiteEx struct which
	// contained an embedded IOleInPlaceSite struct within it. And when the browser
	// called Site_QueryInterface() to get a pointer to our IOleInPlaceSite object, we
	// returned a pointer to our _IOleClientSiteEx. The browser doesn't know this. But
	// we do. That's what we're really being passed, so we can recast it and use it as
	// so here.
	//
	// Actually, we're giving the browser a pointer to our own _IOleInPlaceSiteEx struct,
	// but telling the browser that it's a IOleInPlaceSite struct. No problem. Our
	// _IOleInPlaceSiteEx starts with an embedded IOleInPlaceSite, so the browser is
	// cool with it. And we want the browser to pass a pointer to this _IOleInPlaceSiteEx
	// wherever it would pass a IOleInPlaceSite struct to our IOleInPlaceSite functions.
	*lplpFrame = (LPOLEINPLACEFRAME)&((_IOleInPlaceSiteEx *)This)->frame;

	// We have no OLEINPLACEUIWINDOW
	*lplpDoc = 0;

	// Fill in some other info for the browser
	lpFrameInfo->fMDIApp = FALSE;
	lpFrameInfo->hwndFrame = ((_IOleInPlaceFrameEx *)*lplpFrame)->window;
	lpFrameInfo->haccel = 0;
	lpFrameInfo->cAccelEntries = 0;

	// Give the browser the dimensions of where it can draw. We give it our entire window to fill.
	// We do this in InPlace_OnPosRectChange() which is called right when a window is first
	// created anyway, so no need to duplicate it here.
//	GetClientRect(lpFrameInfo->hwndFrame, lprcPosRect);
//	GetClientRect(lpFrameInfo->hwndFrame, lprcClipRect);

	return(S_OK);
}

HRESULT STDMETHODCALLTYPE InPlace_Scroll(IOleInPlaceSite FAR* This, SIZE scrollExtent)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE InPlace_OnUIDeactivate(IOleInPlaceSite FAR* This, BOOL fUndoable)
{
	return(S_OK);
}

HRESULT STDMETHODCALLTYPE InPlace_OnInPlaceDeactivate(IOleInPlaceSite FAR* This)
{
	return(S_OK);
}

HRESULT STDMETHODCALLTYPE InPlace_DiscardUndoState(IOleInPlaceSite FAR* This)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE InPlace_DeactivateAndUndo(IOleInPlaceSite FAR* This)
{
	NOTIMPLEMENTED;
}

// Called when the position of the browser object is changed, such as when we call the IWebBrowser2's put_Width(),
// put_Height(), put_Left(), or put_Right().
HRESULT STDMETHODCALLTYPE InPlace_OnPosRectChange(IOleInPlaceSite FAR* This, LPCRECT lprcPosRect)
{
	IOleObject			*browserObject;
	IOleInPlaceObject	*inplace;

	// We need to get the browser's IOleInPlaceObject object so we can call its SetObjectRects
	// function.
	browserObject = *((IOleObject **)((char *)This - sizeof(IOleObject *) - sizeof(IOleClientSite)));
	if (!browserObject->lpVtbl->QueryInterface(browserObject, &IID_IOleInPlaceObject, (void**)&inplace))
	{
		// Give the browser the dimensions of where it can draw.
		inplace->lpVtbl->SetObjectRects(inplace, lprcPosRect, lprcPosRect);
		inplace->lpVtbl->Release(inplace);
	}

	return(S_OK);
}