//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Data/DataManagers/CZPDataObserver.cpp $
//	$Revision: 933 $
//	$Date: 2010-03-31 05:00:29 +0200 (Wed, 31 Mar 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 29-3-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IObserver.h"

//SDK General includes
#include "CObserver.h"

#include "IZPID.h"

//IZP Interfaces

//IZP General includes

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

class CZPDataObserver : public CObserver
{
public:
				CZPDataObserver(IPMUnknown * boss);
	virtual		~CZPDataObserver();

	void		Update(const ClassID& theChange, ISubject* theSubject, const PMIID &protocol, void* changedBy);
protected:
private:
};


CREATE_PMINTERFACE(CZPDataObserver, kZPDataObserverImpl)

CZPDataObserver::CZPDataObserver(IPMUnknown * boss) :
CObserver(boss)
{

}

CZPDataObserver::~CZPDataObserver()
{

}

//----------------------------------------------------------------------------------------
// Update
//----------------------------------------------------------------------------------------
void
CZPDataObserver::Update(
	const ClassID& theChange, ISubject* theSubject, const PMIID &protocol, void* changedBy)
{
}

