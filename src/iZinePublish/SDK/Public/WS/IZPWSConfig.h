//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/WS/IZPWSConfig.h $
//	$Revision: 1005 $
//	$Date: 2010-04-16 03:29:59 +0200 (Fri, 16 Apr 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 29-3-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#ifndef _h_IZPWSConfig_
#define _h_IZPWSConfig_
#pragma once

class IZPWSConfig : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPWSCONFIG };

	virtual void				Copy(
									const IZPWSConfig *			inValuesFrom ) = 0;

	virtual const PMString &	GetURL() const = 0;
	virtual void				SetURL(
									const PMString &			inURL) = 0;

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

#endif //_h_IZPWSConfig_
