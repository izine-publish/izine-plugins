//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Controls/AZMUIDataObjDropDownObserver.h $
//	$Revision: 2707 $
//	$Date: 2011-04-06 14:38:20 +0200 (Wed, 06 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 6-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_AZMUIDataObjDropDownObserver_
#define _h_AZMUIDataObjDropDownObserver_
#pragma once

#include "CObserver.h"

#define inheritClass_AZMUIDataObjDropDownObserver CObserver
class AZMUIDataObjDropDownObserver : public inheritClass_AZMUIDataObjDropDownObserver
{
public:
						AZMUIDataObjDropDownObserver(IPMUnknown * boss);
	virtual				~AZMUIDataObjDropDownObserver();

	virtual void		AutoAttach();
	virtual void		AutoDetach();
	virtual void		Update(
							const ClassID&				theChange,
							ISubject*					theSubject,
							const PMIID &				protocol,
							void*						changedBy);

	virtual void		RemoveListeningObject(
							const IPMUnknown *			inObjectToListen,
							const PMIID &				inProtocol);
	virtual void		AddListeningToObject(
							const IPMUnknown *			inObjectToListen,
							const PMIID &				inProtocol);
protected:
	
};

#endif //_h_AZMUIDataObjDropDownObserver_
