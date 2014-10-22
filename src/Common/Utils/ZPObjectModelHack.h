//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/Common/Utils/ZPObjectModelHack.h $
//	$Revision: 1734 $
//	$Date: 2010-07-14 09:45:23 +0200 (Wed, 14 Jul 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 14-7-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_ZPObjectModelHack_
#define _h_ZPObjectModelHack_
#pragma once

/**
	Class used to help debugging where information is not available in adobe help.
	This can help debugging the object which are not instantiated before the startup is called.

	This hack is useful if we add our interface and impl in boss which are not instantiated by our plugins.
	It works at most place except IEventHandler.
*/

class ZPObjectModelHack
{
public:
						ZPObjectModelHack();
	virtual				~ZPObjectModelHack();

	static void			SwapImpls(
							const ClassID				inClassID,
							PMIID						inInterface1,
							PMIID						inInterface2,
							ImplementationID			inInterface2Impl = kInvalidImpl);
protected:
private:
};

#endif //_h_ZPObjectModelHack_
