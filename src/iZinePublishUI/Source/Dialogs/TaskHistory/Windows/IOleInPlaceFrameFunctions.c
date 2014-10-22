//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Dialogs/TaskHistory/Windows/IOleInPlaceFrameFunctions.c $
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
#include "IOleInPlaceFrameFunctions.h"
#include "ZPWBTypedefs.h"

IOleInPlaceFrameVtbl gIOleInPlaceFrameTable = {Frame_QueryInterface,
						Frame_AddRef,
						Frame_Release,
						Frame_GetWindow,
						Frame_ContextSensitiveHelp,
						Frame_GetBorder,
						Frame_RequestBorderSpace,
						Frame_SetBorderSpace,
						Frame_SetActiveObject,
						Frame_InsertMenus,
						Frame_SetMenu,
						Frame_RemoveMenus,
						Frame_SetStatusText,
						Frame_EnableModeless,
						Frame_TranslateAccelerator};


////////////////////////////////////// My IOleInPlaceFrame functions  /////////////////////////////////////////

HRESULT STDMETHODCALLTYPE Frame_QueryInterface(IOleInPlaceFrame FAR* This, REFIID riid, LPVOID FAR* ppvObj)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE Frame_AddRef(IOleInPlaceFrame FAR* This)
{
	return(1);
}

HRESULT STDMETHODCALLTYPE Frame_Release(IOleInPlaceFrame FAR* This)
{
	return(1);
}

HRESULT STDMETHODCALLTYPE Frame_GetWindow(IOleInPlaceFrame FAR* This, HWND FAR* lphwnd)
{
	// Give the browser the HWND to our window that contains the browser object. We
	// stored that HWND in our IOleInPlaceFrame struct. Nevermind that the function
	// declaration for Frame_GetWindow says that 'This' is an IOleInPlaceFrame *. Remember
	// that in EmbedBrowserObject(), we allocated our own IOleInPlaceFrameEx struct which
	// contained an embedded IOleInPlaceFrame struct within it. And then we lied when
	// Site_GetWindowContext() returned that IOleInPlaceFrameEx. So that's what the
	// browser is passing us. It doesn't know that. But we do. So we can recast it and
	// use it as so here.
	*lphwnd = ((_IOleInPlaceFrameEx *)This)->window;
	return(S_OK);
}

HRESULT STDMETHODCALLTYPE Frame_ContextSensitiveHelp(IOleInPlaceFrame FAR* This, BOOL fEnterMode)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE Frame_GetBorder(IOleInPlaceFrame FAR* This, LPRECT lprectBorder)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE Frame_RequestBorderSpace(IOleInPlaceFrame FAR* This, LPCBORDERWIDTHS pborderwidths)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE Frame_SetBorderSpace(IOleInPlaceFrame FAR* This, LPCBORDERWIDTHS pborderwidths)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE Frame_SetActiveObject(IOleInPlaceFrame FAR* This, IOleInPlaceActiveObject *pActiveObject, LPCOLESTR pszObjName)
{
	return(S_OK);
}

HRESULT STDMETHODCALLTYPE Frame_InsertMenus(IOleInPlaceFrame FAR* This, HMENU hmenuShared, LPOLEMENUGROUPWIDTHS lpMenuWidths)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE Frame_SetMenu(IOleInPlaceFrame FAR* This, HMENU hmenuShared, HOLEMENU holemenu, HWND hwndActiveObject)
{
	return(S_OK);
}

HRESULT STDMETHODCALLTYPE Frame_RemoveMenus(IOleInPlaceFrame FAR* This, HMENU hmenuShared)
{
	NOTIMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE Frame_SetStatusText(IOleInPlaceFrame FAR* This, LPCOLESTR pszStatusText)
{
	return(S_OK);
}

HRESULT STDMETHODCALLTYPE Frame_EnableModeless(IOleInPlaceFrame FAR* This, BOOL fEnable)
{
	return(S_OK);
}

HRESULT STDMETHODCALLTYPE Frame_TranslateAccelerator(IOleInPlaceFrame FAR* This, LPMSG lpmsg, WORD wID)
{
	NOTIMPLEMENTED;
}
