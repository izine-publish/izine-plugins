//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Dialogs/TaskHistory/Windows/ZPWBTypedefs.h $
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
#ifndef _h_ZPWBTypedefs_
#define _h_ZPWBTypedefs_
#pragma once

typedef struct {
	IDocHostUIHandler		ui;			// My IDocHostUIHandler object. Must be first.

	///////////////////////////////////////////////////
	// Here you add any extra variables that you need
	// to access in your IDocHostUIHandler functions.
	///////////////////////////////////////////////////
} _IDocHostUIHandlerEx;

// We need to return an IOleInPlaceFrame struct to the browser object. And one of our IOleInPlaceFrame
// functions (Frame_GetWindow) is going to need to access our window handle. So let's create our own
// struct that starts off with an IOleInPlaceFrame struct (and that's important -- the IOleInPlaceFrame
// struct *must* be first), and then has an extra data field where we can store our own window's HWND.
//
// And because we may want to create multiple windows, each hosting its own browser object (to
// display its own web page), then we need to create a IOleInPlaceFrame struct for each window. So,
// we're not going to declare our IOleInPlaceFrame struct globally. We'll allocate it later using
// GlobalAlloc, and then stuff the appropriate HWND in it then, and also stuff a pointer to
// MyIOleInPlaceFrameTable in it. But let's just define it here.
typedef struct {
	IOleInPlaceFrame	frame;		// The IOleInPlaceFrame must be first!

	///////////////////////////////////////////////////
	// Here you add any extra variables that you need
	// to access in your IOleInPlaceFrame functions.
	// You don't want those functions to access global
	// variables, because then you couldn't use more
	// than one browser object. (ie, You couldn't have
	// multiple windows, each with its own embedded
	// browser object to display a different web page).
	//
	// So here is where I added my extra HWND that my
	// IOleInPlaceFrame function Frame_GetWindow() needs
	// to access.
	///////////////////////////////////////////////////
	HWND				window;
} _IOleInPlaceFrameEx;

typedef struct {
	IOleInPlaceSite			inplace;	// My IOleInPlaceSite object. Must be first with in _IOleInPlaceSiteEx.

	///////////////////////////////////////////////////
	// Here you add any extra variables that you need
	// to access in your IOleInPlaceSite functions.
	//
	// So here is where I added my IOleInPlaceFrame
	// struct. If you need extra variables, add them
	// at the end.
	///////////////////////////////////////////////////
	_IOleInPlaceFrameEx		frame;		// My IOleInPlaceFrame object. Must be first within my _IOleInPlaceFrameEx
} _IOleInPlaceSiteEx;

typedef struct {
	IOleClientSite			client;		// My IOleClientSite object. Must be first.
	_IOleInPlaceSiteEx		inplace;	// My IOleInPlaceSite object. A convenient place to put it.
	_IDocHostUIHandlerEx	ui;			// My IDocHostUIHandler object. Must be first within my _IDocHostUIHandlerEx.

	///////////////////////////////////////////////////
	// Here you add any extra variables that you need
	// to access in your IOleClientSite functions.
	///////////////////////////////////////////////////
} _IOleClientSiteEx;


// This is a simple C example. There are lots more things you can control about the browser object, but
// we don't do it in this example. _Many_ of the functions we provide below for the browser to call, will
// never actually be called by the browser in our example. Why? Because we don't do certain things
// with the browser that would require it to call those functions (even though we need to provide
// at least some stub for all of the functions).
//
// So, for these "dummy functions" that we don't expect the browser to call, we'll just stick in some
// assembly code that causes a debugger breakpoint and tells the browser object that we don't support
// the functionality. That way, if you try to do more things with the browser object, and it starts
// calling these "dummy functions", you'll know which ones you should add more meaningful code to.
#define NOTIMPLEMENTED _ASSERT(0); return(E_NOTIMPL)

#endif //_h_ZPWBTypedefs_
