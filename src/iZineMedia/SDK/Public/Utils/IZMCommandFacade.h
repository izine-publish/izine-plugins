//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/SDK/Public/Utils/IZMCommandFacade.h $
//	$Revision: 2713 $
//	$Date: 2011-04-06 15:37:25 +0200 (Wed, 06 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 6-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZMCommandFacade_
#define _h_IZMCommandFacade_
#pragma once

#include "IDataLink.h"
class ICommand;
class IStringData;

class IZMCommandFacade : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZMCOMMANDFACADE };
	
#pragma mark -
//Command creation functions, sdk bosses.

#pragma mark -
//Command creation functions, partially setting the data with command.
	//Caller must release the command
	virtual ICommand *	CreateCommand_SaveZMPrefs() const = 0;
};

#endif //_h_IZMCommandFacade_
