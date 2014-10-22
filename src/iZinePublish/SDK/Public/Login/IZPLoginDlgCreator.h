//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/Login/IZPLoginDlgCreator.h $
//	$Revision: 992 $
//	$Date: 2010-04-14 22:25:41 +0200 (Wed, 14 Apr 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 14-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#ifndef _h_IZPLoginDlgCreator_
#define _h_IZPLoginDlgCreator_
#pragma once

class IZPWSConfig;
class IZPUserCredentials;

class IZPLoginDlgCreator : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPLOGINDLGCREATOR };

	virtual void			ShowLoginDialog(
								const IZPWSConfig *					inWSConfig,
								const IZPUserCredentials *			inUserCeredentials ) = 0;
};

#endif //_h_IZPLoginDlgCreator_
