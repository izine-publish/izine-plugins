//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/Session/IZPDefaultSession.h $
//	$Revision: 1012 $
//	$Date: 2010-04-16 13:05:30 +0200 (Fri, 16 Apr 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 29-3-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#ifndef _h_IZPDefaultSession_
#define _h_IZPDefaultSession_
#pragma once

class IZPUserCredentials;

class IZPDefaultSession : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPDEFAULTSESSION };

	virtual const IZPUserCredentials *	GetUserCredentials() const = 0;
	virtual void						SetUserCredentials(
											const IZPUserCredentials *			inUserCredentials ) = 0;
};

#endif //_h_IZPDefaultSession_
