//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/TreeViewColumnWidgets/CZPTVColumnRollOverIconButtonObserver.cpp $
//	$Revision: 2442 $
//	$Date: 2010-11-02 13:26:30 +0100 (Tue, 02 Nov 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 2-11-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "ISubject.h"
#include "ITriStateControlData.h"

//SDK General includes
#include "CObserver.h"

//OS / ThirdParty includes

#include "IZPUIID.h"

//IZP Interfaces

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AssetsTV
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CObserver
class CZPTVColumnRollOverIconButtonObserver : public inheritClass
{
public:
						CZPTVColumnRollOverIconButtonObserver(IPMUnknown * boss);
	virtual				~CZPTVColumnRollOverIconButtonObserver();

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


CREATE_PMINTERFACE(CZPTVColumnRollOverIconButtonObserver, kZPTVColumnRollOverIconButtonObserverImpl)

CZPTVColumnRollOverIconButtonObserver::CZPTVColumnRollOverIconButtonObserver(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPTVColumnRollOverIconButtonObserver::~CZPTVColumnRollOverIconButtonObserver()
{

}

//----------------------------------------------------------------------------------------
// AutoAttach
//----------------------------------------------------------------------------------------
void
CZPTVColumnRollOverIconButtonObserver::AutoAttach()
{
	InterfacePtr<ISubject> selfSubject(this, UseDefaultIID());
	ASSERT(selfSubject);
	
	selfSubject->AttachObserver(this, ITriStateControlData::kDefaultIID);
}


//----------------------------------------------------------------------------------------
// AutoDetach
//----------------------------------------------------------------------------------------
void
CZPTVColumnRollOverIconButtonObserver::AutoDetach()
{
	InterfacePtr<ISubject> selfSubject(this, UseDefaultIID());
	ASSERT(selfSubject);

	selfSubject->DetachObserver(this, ITriStateControlData::kDefaultIID);
}


//----------------------------------------------------------------------------------------
// Update
//----------------------------------------------------------------------------------------
void
CZPTVColumnRollOverIconButtonObserver::Update(
	const ClassID&				theChange,
	ISubject*					theSubject,
	const PMIID &				protocol,
	void*						changedBy)
{
	if(theChange == kTrueStateMessage)
	{
		InterfacePtr<ISubject> selfSubject(this, UseDefaultIID());
		ASSERT(selfSubject);

		selfSubject->Change( kZPUITVNodeColumn_BtnPressedMsg, ITriStateControlData::kDefaultIID );
	}
}
