//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/Data/IZExpPrefs.h $
//	$Revision: 4010 $
//	$Date: 2012-05-22 13:56:23 +0200 (Tue, 22 May 2012) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Aman Alam
//	Created: 6-6-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZExpPrefsPersist_
#define _h_IZExpPrefsPersist_
#pragma once


class IZExpPrefs : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZEXPPREFS };

	virtual const PMString &	GetUserName() const = 0;
	virtual void				SetUserName(
									const PMString &			inUserName) = 0;

	virtual const PMString &	GetPassword() const = 0;
	virtual void				SetPassword(
									const PMString &			inPassword) = 0;

	virtual const PMString &	GetPluginId() const = 0;
	virtual void				SetPluginId(
									const PMString &			inPluginId) = 0;

	virtual bool16				NeedsProxyServer() const = 0;
	virtual void				SetNeedsProxyServer(
									bool16						inNeedsProxy) = 0;

	virtual int32				GetProxyPort() const = 0;
	virtual void				SetProxyPort(
									int32						inPort) = 0;

	virtual const PMString &	GetProxyServer() const = 0;
	virtual void				SetProxyServer(
									const PMString &			inProxyServer) = 0;

};


#endif //_h_IZExpPrefsPersist_
