//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Controls/TreeViewHeader/CZMTVHeaderObserver.cpp $
//	$Revision: 2724 $
//	$Date: 2011-04-07 09:30:18 +0200 (Thu, 07 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 7-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes
#include "CWidgetObserver.h"

//OS / ThirdParty includes

#include "iZMediaID.h"

//IZP Interfaces
#include "IZMTVHeaderController.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_TVHeader
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CWidgetObserver
class CZMTVHeaderObserver : public inheritClass
{
public:
						CZMTVHeaderObserver(IPMUnknown * boss);
	virtual				~CZMTVHeaderObserver();

	void				AutoAttach();
	void				AutoDetach();
	void				Update(
							const ClassID&				theChange,
							ISubject*					theSubject,
							const PMIID &				protocol,
							void*						changedBy);
protected:
private:
};


CREATE_PMINTERFACE(CZMTVHeaderObserver, kZMTVHeaderObserverImpl)

CZMTVHeaderObserver::CZMTVHeaderObserver(IPMUnknown * boss)
: inheritClass(boss)
{
	InterfacePtr<IZMTVHeaderController> selfController( this, UseDefaultIID() );
	ASSERT( selfController );
	selfController->InitHeaderControlsInfo();
	selfController->CreateHeaderWidgets();
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZMTVHeaderObserver::~CZMTVHeaderObserver()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// AutoAttach
//----------------------------------------------------------------------------------------
void
CZMTVHeaderObserver::AutoAttach()
{

}

//----------------------------------------------------------------------------------------
// AutoDetach
//----------------------------------------------------------------------------------------
void
CZMTVHeaderObserver::AutoDetach()
{

}

//----------------------------------------------------------------------------------------
// Update
//----------------------------------------------------------------------------------------
void
CZMTVHeaderObserver::Update(
	const ClassID&				theChange,
	ISubject*					theSubject,
	const PMIID &				protocol,
	void*						changedBy)
{

}
