//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Dialogs/TaskHistory/Windows/IOleClientSiteFunctions.h $
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
#ifndef _h_IOleClientSiteFunctions_
#define _h_IOleClientSiteFunctions_
#pragma once

// Our IOleClientSite functions that the browser may call
HRESULT STDMETHODCALLTYPE Site_QueryInterface(IOleClientSite FAR* This, REFIID riid, void ** ppvObject);
HRESULT STDMETHODCALLTYPE Site_AddRef(IOleClientSite FAR* This);
HRESULT STDMETHODCALLTYPE Site_Release(IOleClientSite FAR* This);
HRESULT STDMETHODCALLTYPE Site_SaveObject(IOleClientSite FAR* This);
HRESULT STDMETHODCALLTYPE Site_GetMoniker(IOleClientSite FAR* This, DWORD dwAssign, DWORD dwWhichMoniker, IMoniker ** ppmk);
HRESULT STDMETHODCALLTYPE Site_GetContainer(IOleClientSite FAR* This, LPOLECONTAINER FAR* ppContainer);
HRESULT STDMETHODCALLTYPE Site_ShowObject(IOleClientSite FAR* This);
HRESULT STDMETHODCALLTYPE Site_OnShowWindow(IOleClientSite FAR* This, BOOL fShow);
HRESULT STDMETHODCALLTYPE Site_RequestNewObjectLayout(IOleClientSite FAR* This);

// Our IOleClientSite VTable. This is the array of pointers to the above functions in our C
// program that the browser may call in order to interact with our frame window that contains
// the browser object. We must define a particular set of functions that comprise the
// IOleClientSite set of functions (see above), and then stuff pointers to those functions
// in their respective 'slots' in this table. We want the browser to use this VTable with our
// IOleClientSite structure.
extern IOleClientSiteVtbl gIOleClientSiteTable;

#endif //_h_IOleClientSiteFunctions_
