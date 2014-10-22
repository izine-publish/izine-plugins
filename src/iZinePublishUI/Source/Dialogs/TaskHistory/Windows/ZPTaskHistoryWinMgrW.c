//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Dialogs/TaskHistory/Windows/ZPTaskHistoryWinMgrW.c $
//	$Revision: 2409 $
//	$Date: 2010-10-27 11:43:45 +0200 (Wed, 27 Oct 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 22-10-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description: Sample code copied from http://www.codeproject.com/KB/COM/cwebpage/cwebpage_src.zip
//========================================================================================
/* Simple.c
 
This is a Win32 C application (ie, no MFC, WTL, nor even any C++ -- just plain C) that demonstrates
how to embed a browser "control" (actually, an OLE object) in your own window (in order to display a
web page, or an HTML file on disk).

This example opens two windows, each containing a browser object. The first window displays an HTML
string in memory. The second window displays Microsoft's web site. We also disable the pop-up context
menu, and resize the browser to fill our window.

This is very loosely based upon a C++ example written by Chris Becke. I used that to learn the minimum
of what I needed to know about hosting the browser object. Then I wrote this example from the ground up
in C.

This requires IE 5.0 (or better) -- due to the IDocHostUIHandler interface, or a browser that supports
the same level of OLE in-place activation.
*/

#include <windows.h>
#include <exdisp.h>		// Defines of stuff like IWebBrowser2. This is an include file with Visual C 6 and above
#include <mshtml.h>		// Defines of stuff like IHTMLDocument2. This is an include file with Visual C 6 and above
#include <mshtmhst.h>	// Defines of stuff like IDocHostUIHandler. This is an include file with Visual C 6 and above
#include <crtdbg.h>		// for _ASSERT()

// A running count of how many windows we have open that contain a browser object
//unsigned char gZPWebWindowCount = 0;
HWND	gZPWebWindowHWND = NULL;

// The class name of our Window to host the browser. It can be anything of your choosing.
static const TCHAR	gWinClassName[] = "ZP WebBrowser Window";

// This is used by DisplayHTMLStr(). It can be global because we never change it.
static const SAFEARRAYBOUND ArrayBound = {1, 0};

int gWindowX = 100, gWindowY = 100, gWindowWidth = 400, gWindowHeight = 400;

extern void	HandleHistoryPanelClose();

#include "IOleInPlaceFrameFunctions.h"
#include "IOleClientSiteFunctions.h"
#include "IDocHostUIHandlerFunctions.h"
#include "IOleInPlaceSiteFunctions.h"
#include "ZPWBTypedefs.h"
#include "ZPTaskHistoryWinMgrW.h"


/*************************** UnEmbedBrowserObject() ************************
 * Called to detach the browser object from our host window, and free its
 * resources, right before we destroy our window.
 *
 * hwnd =		Handle to the window hosting the browser object.
 *
 * NOTE: The pointer to the browser object must have been stored in the
 * window's USERDATA field. In other words, don't call UnEmbedBrowserObject().
 * with a HWND that wasn't successfully passed to EmbedBrowserObject().
 */

void UnEmbedBrowserObject(HWND hwnd)
{
	IOleObject	**browserHandle;
	IOleObject	*browserObject;

	// Retrieve the browser object's pointer we stored in our window's GWL_USERDATA when
	// we initially attached the browser object to this window.
	if ((browserHandle = (IOleObject **)GetWindowLong(hwnd, GWL_USERDATA)))
	{
		// Unembed the browser object, and release its resources.
		browserObject = *browserHandle;
		browserObject->lpVtbl->Close(browserObject, OLECLOSE_NOSAVE);
		browserObject->lpVtbl->Release(browserObject);

		GlobalFree(browserHandle);

		return;
	}

	// You must have called this for a window that wasn't successfully passed to EmbedBrowserObject().
	// Bad boy!
	_ASSERT(0);
}


/******************************* DisplayHTMLStr() ****************************
 * Takes a string containing some HTML BODY, and displays it in the specified
 * window. For example, perhaps you want to display the HTML text of...
 *
 * <P>This is a picture.<P><IMG src="mypic.jpg">
 *
 * hwnd =		Handle to the window hosting the browser object.
 * string =		Pointer to nul-terminated string containing the HTML BODY.
 *				(NOTE: No <BODY></BODY> tags are required in the string).
 *
 * RETURNS: 0 if success, or non-zero if an error.
 *
 * NOTE: EmbedBrowserObject() must have been successfully called once with the
 * specified window, prior to calling this function. You need call
 * EmbedBrowserObject() once only, and then you can make multiple calls to
 * this function to display numerous pages in the specified window.
 */

long DisplayHTMLStr(HWND hwnd, LPCTSTR string)
{	
	IWebBrowser2	*webBrowser2;
	LPDISPATCH		lpDispatch;
	IHTMLDocument2	*htmlDoc2;
	IOleObject		*browserObject;
	SAFEARRAY		*sfArray;
	VARIANT			myURL;
	VARIANT			*pVar;
	BSTR			bstr;

	// Retrieve the browser object's pointer we stored in our window's GWL_USERDATA when
	// we initially attached the browser object to this window.
	browserObject = *((IOleObject **)GetWindowLong(hwnd, GWL_USERDATA));

	// Assume an error.
	bstr = 0;

	// We want to get the base address (ie, a pointer) to the IWebBrowser2 object embedded within the browser
	// object, so we can call some of the functions in the former's table.
	if (!browserObject->lpVtbl->QueryInterface(browserObject, &IID_IWebBrowser2, (void**)&webBrowser2))
	{
		// Ok, now the pointer to our IWebBrowser2 object is in 'webBrowser2', and so its VTable is
		// webBrowser2->lpVtbl.

		// Before we can get_Document(), we actually need to have some HTML page loaded in the browser. So,
		// let's load an empty HTML page. Then, once we have that empty page, we can get_Document() and
		// write() to stuff our HTML string into it.
		VariantInit(&myURL);
		myURL.vt = VT_BSTR;
		myURL.bstrVal = SysAllocString(L"about:blank");

		// Call the Navigate2() function to actually display the page.
		webBrowser2->lpVtbl->Navigate2(webBrowser2, &myURL, 0, 0, 0, 0);

		// Free any resources (including the BSTR).
		VariantClear(&myURL);

		// Call the IWebBrowser2 object's get_Document so we can get its DISPATCH object. I don't know why you
		// don't get the DISPATCH object via the browser object's QueryInterface(), but you don't.
		if (!webBrowser2->lpVtbl->get_Document(webBrowser2, &lpDispatch))
		{
			// We want to get a pointer to the IHTMLDocument2 object embedded within the DISPATCH
			// object, so we can call some of the functions in the former's table.
			if (!lpDispatch->lpVtbl->QueryInterface(lpDispatch, &IID_IHTMLDocument2, (void**)&htmlDoc2))
			{
				// Ok, now the pointer to our IHTMLDocument2 object is in 'htmlDoc2', and so its VTable is
				// htmlDoc2->lpVtbl.

				// Our HTML must be in the form of a BSTR. And it must be passed to write() in an
				// array of "VARIENT" structs. So let's create all that.
				if ((sfArray = SafeArrayCreate(VT_VARIANT, 1, (SAFEARRAYBOUND *)&ArrayBound)))
				{
					if (!SafeArrayAccessData(sfArray, (void**)&pVar))
					{
						pVar->vt = VT_BSTR;
#ifndef UNICODE
						{
						wchar_t		*buffer;
						DWORD		size;

						size = MultiByteToWideChar(CP_ACP, 0, string, -1, 0, 0);
						if (!(buffer = (wchar_t *)GlobalAlloc(GMEM_FIXED, sizeof(wchar_t) * size))) goto bad;
						MultiByteToWideChar(CP_ACP, 0, string, -1, buffer, size);
						bstr = SysAllocString(buffer);
						GlobalFree(buffer);
						}
#else
						bstr = SysAllocString(string);
#endif
						// Store our BSTR pointer in the VARIENT.
						if ((pVar->bstrVal = bstr))
						{
							// Pass the VARIENT with its BSTR to write() in order to shove our desired HTML string
							// into the body of that empty page we created above.
							htmlDoc2->lpVtbl->write(htmlDoc2, sfArray);

							// Close the document. If we don't do this, subsequent calls to DisplayHTMLStr
							// would append to the current contents of the page
							htmlDoc2->lpVtbl->close(htmlDoc2);
							
							// Normally, we'd need to free our BSTR, but SafeArrayDestroy() does it for us
//							SysFreeString(bstr);
						}
					}

					// Free the array. This also frees the VARIENT that SafeArrayAccessData created for us,
					// and even frees the BSTR we allocated with SysAllocString
					SafeArrayDestroy(sfArray);
				}

				// Release the IHTMLDocument2 object.
bad:			htmlDoc2->lpVtbl->Release(htmlDoc2);
			}

			// Release the DISPATCH object.
			lpDispatch->lpVtbl->Release(lpDispatch);
		}

		// Release the IWebBrowser2 object.
		webBrowser2->lpVtbl->Release(webBrowser2);
	}

	// No error?
	if (bstr) return(0);

	// An error
	return(-1);
}


/******************************* ResizeBrowser() ****************************
 * Resizes the browser object for the specified window to the specified
 * width and height.
 *
 * hwnd =			Handle to the window hosting the browser object.
 * width =			Width.
 * height =			Height.
 *
 * NOTE: EmbedBrowserObject() must have been successfully called once with the
 * specified window, prior to calling this function. You need call
 * EmbedBrowserObject() once only, and then you can make multiple calls to
 * this function to resize the browser object.
 */

void ResizeBrowser(HWND hwnd, DWORD width, DWORD height)
{
	IWebBrowser2	*webBrowser2;
	IOleObject		*browserObject;

	// Retrieve the browser object's pointer we stored in our window's GWL_USERDATA when
	// we initially attached the browser object to this window.
	browserObject = *((IOleObject **)GetWindowLong(hwnd, GWL_USERDATA));

	// We want to get the base address (ie, a pointer) to the IWebBrowser2 object embedded within the browser
	// object, so we can call some of the functions in the former's table.
	if (!browserObject->lpVtbl->QueryInterface(browserObject, &IID_IWebBrowser2, (void**)&webBrowser2))
	{
		// Ok, now the pointer to our IWebBrowser2 object is in 'webBrowser2', and so its VTable is
		// webBrowser2->lpVtbl.

		// Call are put_Width() and put_Height() to set the new width/height.
		webBrowser2->lpVtbl->put_Width(webBrowser2, width);
		webBrowser2->lpVtbl->put_Height(webBrowser2, height);

		// We no longer need the IWebBrowser2 object (ie, we don't plan to call any more functions in it,
		// so we can release our hold on it). Note that we'll still maintain our hold on the browser
		// object.
		webBrowser2->lpVtbl->Release(webBrowser2);
	}
}

/***************************** EmbedBrowserObject() **************************
 * Puts the browser object inside our host window, and save a pointer to this
 * window's browser object in the window's GWL_USERDATA field.
 *
 * hwnd =		Handle of our window into which we embed the browser object.
 *
 * RETURNS: 0 if success, or non-zero if an error.
 *
 * NOTE: We tell the browser object to occupy the entire client area of the
 * window.
 *
 * NOTE: No HTML page will be displayed here. We can do that with a subsequent
 * call to either DisplayHTMLPage() or DisplayHTMLStr(). This is merely once-only
 * initialization for using the browser object. In a nutshell, what we do
 * here is get a pointer to the browser object in our window's GWL_USERDATA
 * so we can access that object's functions whenever we want, and we also pass
 * the browser a pointer to our IOleClientSite struct so that the browser can
 * call our functions in our struct's VTable.
 */

long EmbedBrowserObject(HWND hwnd)
{
	LPCLASSFACTORY		pClassFactory;
	IOleObject			*browserObject;
	IWebBrowser2		*webBrowser2;
	RECT				rect;
	char				*ptr;
	_IOleClientSiteEx	*_iOleClientSiteEx;

	// Our IOleClientSite, IOleInPlaceSite, and IOleInPlaceFrame functions need to get our window handle. We
	// could store that in some global. But then, that would mean that our functions would work with only that
	// one window. If we want to create multiple windows, each hosting its own browser object (to display its
	// own web page), then we need to create unique IOleClientSite, IOleInPlaceSite, and IOleInPlaceFrame
	// structs for each window. And we'll put an extra field at the end of those structs to store our extra
	// data such as a window handle. So, our functions won't have to touch global data, and can therefore be
	// re-entrant and work with multiple objects/windows.
	//
	// Remember that a pointer to our IOleClientSite we create here will be passed as the first arg to every
	// one of our IOleClientSite functions. Ditto with the IOleInPlaceFrame object we create here, and the
	// IOleInPlaceFrame functions. So, our functions are able to retrieve the window handle we'll store here,
	// and then, they'll work with all such windows containing a browser control.
	//
	// Furthermore, since the browser will be calling our IOleClientSite's QueryInterface to get a pointer to
	// our IOleInPlaceSite and IDocHostUIHandler objects, that means that our IOleClientSite QueryInterface
	// must have an easy way to grab those pointers. Probably the easiest thing to do is just embed our
	// IOleInPlaceSite and IDocHostUIHandler objects inside of an extended IOleClientSite which we'll call
	// a _IOleClientSiteEx. As long as they come after the pointer to the IOleClientSite VTable, then we're
	// ok.
	//
	// Of course, we need to GlobalAlloc the above structs now. We'll just get all 4 with a single call to
	// GlobalAlloc, especially since 3 of them are all contained inside of our _IOleClientSiteEx anyway.
	//
	// So, we're not actually allocating separate IOleClientSite, IOleInPlaceSite, IOleInPlaceFrame and
	// IDocHostUIHandler structs.
	//
	// One final thing. We're going to allocate extra room to store the pointer to the browser object.
	if (!(ptr = (char *)GlobalAlloc(GMEM_FIXED, sizeof(_IOleClientSiteEx) + sizeof(IOleObject *))))
		return(-1);

	// Initialize our IOleClientSite object with a pointer to our IOleClientSite VTable.
	_iOleClientSiteEx = (_IOleClientSiteEx *)(ptr + sizeof(IOleObject *));
	_iOleClientSiteEx->client.lpVtbl = &gIOleClientSiteTable;

	// Initialize our IOleInPlaceSite object with a pointer to our IOleInPlaceSite VTable.
	_iOleClientSiteEx->inplace.inplace.lpVtbl = &gIOleInPlaceSiteTable;

	// Initialize our IOleInPlaceFrame object with a pointer to our IOleInPlaceFrame VTable.
	_iOleClientSiteEx->inplace.frame.frame.lpVtbl = &gIOleInPlaceFrameTable;

	// Save our HWND (in the IOleInPlaceFrame object) so our IOleInPlaceFrame functions can retrieve it.
	_iOleClientSiteEx->inplace.frame.window = hwnd;

	// Initialize our IDocHostUIHandler object with a pointer to our IDocHostUIHandler VTable.
	_iOleClientSiteEx->ui.ui.lpVtbl = &gIDocHostUIHandlerTable;

	// Get a pointer to the browser object and lock it down (so it doesn't "disappear" while we're using
	// it in this program). We do this by calling the OS function CoGetClassObject().
	//	
	// NOTE: We need this pointer to interact with and control the browser. With normal WIN32 controls such as a
	// Static, Edit, Combobox, etc, you obtain an HWND and send messages to it with SendMessage(). Not so with
	// the browser object. You need to get a pointer to its "base structure" (as returned by CoGetClassObject()). This
	// structure contains an array of pointers to functions you can call within the browser object. Actually, the
	// base structure contains a 'lpVtbl' field that is a pointer to that array. We'll call the array a 'VTable'.
	//
	// For example, the browser object happens to have a SetHostNames() function we want to call. So, after we
	// retrieve the pointer to the browser object (in a local we'll name 'browserObject'), then we can call that
	// function, and pass it args, as so:
	//
	// browserObject->lpVtbl->SetHostNames(browserObject, SomeString, SomeString);
	//
	// There's our pointer to the browser object in 'browserObject'. And there's the pointer to the browser object's
	// VTable in 'browserObject->lpVtbl'. And the pointer to the SetHostNames function happens to be stored in an
	// field named 'SetHostNames' within the VTable. So we are actually indirectly calling SetHostNames by using
	// a pointer to it. That's how you use a VTable.
	//
	// NOTE: We pass our _IOleClientSiteEx struct and lie -- saying that it's a IOleClientSite. It's ok. A
	// _IOleClientSiteEx struct starts with an embedded IOleClientSite. So the browser won't care, and we want
	// this extended struct passed to our IOleClientSite functions.

	// Get a pointer to the browser object's IClassFactory object via CoGetClassObject()
	pClassFactory = 0;
	if (!CoGetClassObject(&CLSID_WebBrowser, CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER, NULL, &IID_IClassFactory, (void **)&pClassFactory) && pClassFactory)
	{
		// Call the IClassFactory's CreateInstance() to create a browser object
		if (!pClassFactory->lpVtbl->CreateInstance(pClassFactory, 0, &IID_IOleObject, &browserObject))
		{
			// Free the IClassFactory. We need it only to create a browser object instance
			pClassFactory->lpVtbl->Release(pClassFactory);

			// Ok, we now have the pointer to the browser object in 'browserObject'. Let's save this in the
			// memory block we allocated above, and then save the pointer to that whole thing in our window's
			// USERDATA field. That way, if we need multiple windows each hosting its own browser object, we can
			// call EmbedBrowserObject() for each one, and easily associate the appropriate browser object with
			// its matching window and its own objects containing per-window data.
			*((IOleObject **)ptr) = browserObject;
			SetWindowLong(hwnd, GWL_USERDATA, (LONG)ptr);

			// Give the browser a pointer to my IOleClientSite object
			if (!browserObject->lpVtbl->SetClientSite(browserObject, (IOleClientSite *)_iOleClientSiteEx))
			{
				// We can now call the browser object's SetHostNames function. SetHostNames lets the browser object know our
				// application's name and the name of the document in which we're embedding the browser. (Since we have no
				// document name, we'll pass a 0 for the latter). When the browser object is opened for editing, it displays
				// these names in its titlebar.
				//	
				// We are passing 3 args to SetHostNames. You'll note that the first arg to SetHostNames is the base
				// address of our browser control. This is something that you always have to remember when working in C
				// (as opposed to C++). When calling a VTable function, the first arg to that function must always be the
				// structure which contains the VTable. (In this case, that's the browser control itself). Why? That's
				// because that function is always assumed to be written in C++. And the first argument to any C++ function
				// must be its 'this' pointer (ie, the base address of its class, which in this case is our browser object
				// pointer). In C++, you don't have to pass this first arg, because the C++ compiler is smart enough to
				// produce an executable that always adds this first arg. In fact, the C++ compiler is smart enough to
				// know to fetch the function pointer from the VTable, so you don't even need to reference that. In other
				// words, the C++ equivalent code would be:
				//
				// browserObject->SetHostNames(L"My Host Name", 0);
				//
				// So, when you're trying to convert C++ code to C, always remember to add this first arg whenever you're
				// dealing with a VTable (ie, the field is usually named 'lpVtbl') in the standard objects, and also add
				// the reference to the VTable itself.
				//
				// Oh yeah, the L is because we need UNICODE strings. And BTW, the host and document names can be anything
				// you want.
				browserObject->lpVtbl->SetHostNames(browserObject, L"My Host Name", 0);

				GetClientRect(hwnd, &rect);

				// Let browser object know that it is embedded in an OLE container.
				if (!OleSetContainedObject((struct IUnknown *)browserObject, TRUE) &&

					// Set the display area of our browser control the same as our window's size
					// and actually put the browser object into our window.
					!browserObject->lpVtbl->DoVerb(browserObject, OLEIVERB_SHOW, NULL, (IOleClientSite *)_iOleClientSiteEx, -1, hwnd, &rect) &&

					// Ok, now things may seem to get even trickier, One of those function pointers in the browser's VTable is
					// to the QueryInterface() function. What does this function do? It lets us grab the base address of any
					// other object that may be embedded within the browser object. And this other object has its own VTable
					// containing pointers to more functions we can call for that object.
					//
					// We want to get the base address (ie, a pointer) to the IWebBrowser2 object embedded within the browser
					// object, so we can call some of the functions in the former's table. For example, one IWebBrowser2 function
					// we intend to call below will be Navigate2(). So we call the browser object's QueryInterface to get our
					// pointer to the IWebBrowser2 object.
					!browserObject->lpVtbl->QueryInterface(browserObject, &IID_IWebBrowser2, (void**)&webBrowser2))
				{
					// Ok, now the pointer to our IWebBrowser2 object is in 'webBrowser2', and so its VTable is
					// webBrowser2->lpVtbl.

					// Let's call several functions in the IWebBrowser2 object to position the browser display area
					// in our window. The functions we call are put_Left(), put_Top(), put_Width(), and put_Height().
					// Note that we reference the IWebBrowser2 object's VTable to get pointers to those functions. And
					// also note that the first arg we pass to each is the pointer to the IWebBrowser2 object.
					webBrowser2->lpVtbl->put_Left(webBrowser2, 0);
					webBrowser2->lpVtbl->put_Top(webBrowser2, 0);
					webBrowser2->lpVtbl->put_Width(webBrowser2, rect.right);
					webBrowser2->lpVtbl->put_Height(webBrowser2, rect.bottom);

					// We no longer need the IWebBrowser2 object (ie, we don't plan to call any more functions in it
					// right now, so we can release our hold on it). Note that we'll still maintain our hold on the
					// browser object until we're done with that object.
					webBrowser2->lpVtbl->Release(webBrowser2);

					// Success
					return(0);
				}
			}

			// Something went wrong setting up the browser!
			UnEmbedBrowserObject(hwnd);
			return(-4);
		}

		pClassFactory->lpVtbl->Release(pClassFactory);
		GlobalFree(ptr);

		// Can't create an instance of the browser!
		return(-3);
	}

	GlobalFree(ptr);

	// Can't get the web browser's IClassFactory!
	return(-2);
}

/****************************** ZPWebBrowserWindowProc() ***************************
 * Our message handler for our window to host the browser.
 */

LRESULT CALLBACK ZPWebBrowserWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_SIZE:
		{
			// Resize the browser object to fit the window
			ResizeBrowser(hwnd, LOWORD(lParam), HIWORD(lParam));
			return(0);
		}

		case WM_CREATE:
		{
			// Embed the browser object into our host window. We need do this only
			// once. Note that the browser object will start calling some of our
			// IOleInPlaceFrame and IOleClientSite functions as soon as we start
			// calling browser object functions in EmbedBrowserObject().
			if (EmbedBrowserObject(hwnd)) return(-1);

			// Another window created with an embedded browser object
			//++gZPWebWindowCount;
			gZPWebWindowHWND = hwnd;

			// Success
			return(0);
		}

		case WM_DESTROY:
		{
			// Detach the browser object from this window, and free resources.
			UnEmbedBrowserObject(hwnd);

			{	//Save the window size.
				RECT windowSize;
				GetWindowRect( hwnd, &windowSize );
				gWindowX = windowSize.left;
				gWindowY = windowSize.top;
				gWindowWidth = windowSize.right - windowSize.left;
				gWindowHeight = windowSize.bottom - windowSize.top;
			}

			// One less window
			//--gZPWebWindowCount;
			gZPWebWindowHWND = NULL;
			HandleHistoryPanelClose();
	
			// Free the OLE library.
			//OleUninitialize();	//Commented as OleInitialize is also commented

			// If all the windows are now closed, quit this app
			//if (!gZPWebWindowCount) PostQuitMessage(0);

			return(TRUE);
		}
	}

	return(DefWindowProc(hwnd, uMsg, wParam, lParam));
}

HWND CreateNewWebWindow( HINSTANCE hInstance, HWND inParentWindow )
{
	HWND toReturn = NULL;
	// Initialize the OLE interface. We do this once-only.
	//if (OleInitialize(NULL) == S_OK)	//Commented as it fails when called from InDesign/InCopy
	{
		WNDCLASSEX		wc;
		DWORD windowStyle = WS_CAPTION;
		DWORD widnowExStyle = 0;

		// Register the class of our window to host the browser. 'ZPWebBrowserWindowProc' is our message handler
		// and 'gWinClassName' is the class name. You can choose any class name you want.
		ZeroMemory(&wc, sizeof(WNDCLASSEX));
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.hInstance = hInstance;
		wc.lpfnWndProc = ZPWebBrowserWindowProc;
		wc.lpszClassName = &gWinClassName[0];
		RegisterClassEx(&wc);

		// Create a window. NOTE: We embed the browser object duing our WM_CREATE handling for
		// this window.
		//inParentWindow = HWND_DESKTOP;
		
		//widnowExStyle |= WS_EX_PALETTEWINDOW;
		widnowExStyle |= WS_EX_TOOLWINDOW;

		windowStyle |= WS_SYSMENU;
		//windowStyle |= WS_THICKFRAME;
		windowStyle |= WS_MINIMIZEBOX;
		windowStyle |= WS_MAXIMIZEBOX;
		windowStyle |= WS_SIZEBOX;
		windowStyle |= WS_VISIBLE;
		toReturn = CreateWindowEx(widnowExStyle, &gWinClassName[0], "Task History", windowStyle ,// | WS_CHILDWINDOW,
							//CW_USEDEFAULT, 0, CW_USEDEFAULT, 0,
							gWindowX, gWindowY, gWindowWidth, gWindowHeight,
							inParentWindow, NULL, hInstance, 0);
	}
	return toReturn;
}

//----------------------------------------------------------------------------------------
// ShowTaskHistoryWindow
//----------------------------------------------------------------------------------------
void ShowTaskHistoryWindow(	const unsigned char * inHTMLString, HINSTANCE hInstance, HWND inParentWindow )
{
	int nCmdShow = SW_SHOWNORMAL;

	if( !gZPWebWindowHWND )
	{
		gZPWebWindowHWND = CreateNewWebWindow( hInstance, inParentWindow );
	}
	else
	{	//Patch: setting the html again is not showing.
		UnEmbedBrowserObject(gZPWebWindowHWND);
		EmbedBrowserObject(gZPWebWindowHWND);
	}

	if( !gZPWebWindowHWND )
		return;

	// For this window, display a string in the BODY of a web page.
	//DisplayHTMLStr(msg.hwnd, "<H2><CENTER>HTML string test</CENTER></H2><P><FONT COLOR=RED>This is a <U>HTML string</U> in memory.</FONT>");
	//DisplayHTMLStr(msg.hwnd, "<html><body><p><b>#5 Updated by  on 10/19/2010 5:16:53 PM</b></p><ul><li><b>Status</b> change from <i>done</i> to <i>assigned</i></li></ul><p>comments 3</p><hr/><p><b>#4 Updated by  on 10/19/2010 5:12:27 PM</b></p><ul><li><b>Status</b> change from <i>assigned</i> to <i>done</i></li></ul><p>test comment"
	//"</p><hr/><p><b>#3 Updated by  on 10/19/2010 4:59:13 PM</b></p><ul></ul><hr/><p><b>#2 Updated by  on 10/15/2010 5:02:33 PM</b></p><ul><li><b>Spread</b> set to <i></i></li><li><b>Page</b> set to <i></i></li><li><b>Status</b> change from <i>new</i> to <i>assigned</i></li></ul><hr/><p><b>#1 Updated by  on 10/15/2010 4:09:41 PM</b></p><ul><li><b>Spread</b> set to <i></i></li><li><b>Page</b> set to <i></i></li><li><b>Category</b> set to <i>Formatting</i></li></body></html>" );
	DisplayHTMLStr( gZPWebWindowHWND, inHTMLString );

	// Show the window.
	ShowWindow(gZPWebWindowHWND, nCmdShow);
	UpdateWindow(gZPWebWindowHWND);
}

#if 0
/****************************** WinMain() ***************************
 * C program entry point.
 *
 * This creates a window to host the web browser, and displays a web
 * page.
 */

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hInstNULL, LPSTR lpszCmdLine, int nCmdShow)
{
	MSG			msg;

	// Initialize the OLE interface. We do this once-only.
	if (OleInitialize(NULL) == S_OK)
	{
		WNDCLASSEX		wc;

		// Register the class of our window to host the browser. 'ZPWebBrowserWindowProc' is our message handler
		// and 'gWinClassName' is the class name. You can choose any class name you want.
		ZeroMemory(&wc, sizeof(WNDCLASSEX));
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.hInstance = hInstance;
		wc.lpfnWndProc = ZPWebBrowserWindowProc;
		wc.lpszClassName = &gWinClassName[0];
		RegisterClassEx(&wc);

		// Create a window. NOTE: We embed the browser object duing our WM_CREATE handling for
		// this window.
		if ((msg.hwnd = CreateWindowEx(0, &gWinClassName[0], "Task History", WS_OVERLAPPEDWINDOW,
							CW_USEDEFAULT, 0, CW_USEDEFAULT, 0,
							HWND_DESKTOP, NULL, hInstance, 0)))
		{
			// For this window, display a string in the BODY of a web page.
			//DisplayHTMLStr(msg.hwnd, "<H2><CENTER>HTML string test</CENTER></H2><P><FONT COLOR=RED>This is a <U>HTML string</U> in memory.</FONT>");
			DisplayHTMLStr(msg.hwnd, "<html><body><p><b>#5 Updated by  on 10/19/2010 5:16:53 PM</b></p><ul><li><b>Status</b> change from <i>done</i> to <i>assigned</i></li></ul><p>comments 3</p><hr/><p><b>#4 Updated by  on 10/19/2010 5:12:27 PM</b></p><ul><li><b>Status</b> change from <i>assigned</i> to <i>done</i></li></ul><p>test comment"
"</p><hr/><p><b>#3 Updated by  on 10/19/2010 4:59:13 PM</b></p><ul></ul><hr/><p><b>#2 Updated by  on 10/15/2010 5:02:33 PM</b></p><ul><li><b>Spread</b> set to <i></i></li><li><b>Page</b> set to <i></i></li><li><b>Status</b> change from <i>new</i> to <i>assigned</i></li></ul><hr/><p><b>#1 Updated by  on 10/15/2010 4:09:41 PM</b></p><ul><li><b>Spread</b> set to <i></i></li><li><b>Page</b> set to <i></i></li><li><b>Category</b> set to <i>Formatting</i></li></body></html>" );

			// Show the window.
			ShowWindow(msg.hwnd, nCmdShow);
			UpdateWindow(msg.hwnd);
		}

		// Do a message loop until WM_QUIT.
		while (GetMessage(&msg, 0, 0, 0) == 1)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// Free the OLE library.
		OleUninitialize();

		return(0);
	}

	MessageBox(0, "Can't open OLE!", "ERROR", MB_OK);
	return(-1);
}

#endif
