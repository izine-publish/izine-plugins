//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/AZPUIDBObjDropDownObserver.h $
//	$Revision: 1186 $
//	$Date: 2010-05-20 08:46:38 +0200 (Thu, 20 May 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 6-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_AZPUIDBObjDropDownObserver_
#define _h_AZPUIDBObjDropDownObserver_
#pragma once

#include "CObserver.h"

#define inheritClass_AZPUIDBObjDropDownObserver CObserver
class AZPUIDBObjDropDownObserver : public inheritClass_AZPUIDBObjDropDownObserver
{
public:
						AZPUIDBObjDropDownObserver(IPMUnknown * boss);
	virtual				~AZPUIDBObjDropDownObserver();

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


#endif //_h_AZPUIDBObjDropDownObserver_
