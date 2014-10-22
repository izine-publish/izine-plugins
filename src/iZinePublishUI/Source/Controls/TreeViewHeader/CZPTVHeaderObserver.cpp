//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/TreeViewHeader/CZPTVHeaderObserver.cpp $
//	$Revision: 2284 $
//	$Date: 2010-10-13 13:21:18 +0200 (Wed, 13 Oct 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 11-10-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes
#include "CWidgetObserver.h"

//OS / ThirdParty includes

#include "IZPUIID.h"

//IZP Interfaces
#include "IZPTVHeaderController.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_TVHeader
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CWidgetObserver
class CZPTVHeaderObserver : public inheritClass
{
public:
						CZPTVHeaderObserver(IPMUnknown * boss);
	virtual				~CZPTVHeaderObserver();

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


CREATE_PMINTERFACE(CZPTVHeaderObserver, kZPTVHeaderObserverImpl)

CZPTVHeaderObserver::CZPTVHeaderObserver(IPMUnknown * boss)
: inheritClass(boss)
{
	InterfacePtr<IZPTVHeaderController> selfController( this, UseDefaultIID() );
	ASSERT( selfController );
	selfController->InitHeaderControlsInfo();
	selfController->CreateHeaderWidgets();
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPTVHeaderObserver::~CZPTVHeaderObserver()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// AutoAttach
//----------------------------------------------------------------------------------------
void
CZPTVHeaderObserver::AutoAttach()
{

}

//----------------------------------------------------------------------------------------
// AutoDetach
//----------------------------------------------------------------------------------------
void
CZPTVHeaderObserver::AutoDetach()
{

}

//----------------------------------------------------------------------------------------
// Update
//----------------------------------------------------------------------------------------
void
CZPTVHeaderObserver::Update(
	const ClassID&				theChange,
	ISubject*					theSubject,
	const PMIID &				protocol,
	void*						changedBy)
{

}
