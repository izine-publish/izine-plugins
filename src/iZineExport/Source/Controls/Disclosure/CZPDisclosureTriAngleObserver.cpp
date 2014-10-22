//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/Controls/Disclosure/CZPDisclosureTriAngleObserver.cpp $
//	$Revision: 3974 $
//	$Date: 2012-03-06 12:53:00 +0100 (Tue, 06 Mar 2012) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Aman Alam
//	Created: 10-5-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "ISubject.h"
#include "ITriStateControlData.h"
#include "IBoolData.h"
#include "IControlView.h"

//SDK General includes
#include "CObserver.h"

//OS / ThirdParty includes

#include "IZExpID.h"

//IZP Interfaces
#include "IZPDisclosureTriAngleOptions.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CObserver
class CZPDisclosureTriAngleObserver : public inheritClass
{
public:
						CZPDisclosureTriAngleObserver(IPMUnknown * boss);
	virtual				~CZPDisclosureTriAngleObserver();

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


CREATE_PMINTERFACE(CZPDisclosureTriAngleObserver, kZExpDisclosureTriAngleObserverImpl)

CZPDisclosureTriAngleObserver::CZPDisclosureTriAngleObserver(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPDisclosureTriAngleObserver::~CZPDisclosureTriAngleObserver()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// AutoAttach
//----------------------------------------------------------------------------------------
void
CZPDisclosureTriAngleObserver::AutoAttach()
{
	InterfacePtr<ISubject> selfSubject(this, UseDefaultIID());
	ASSERT(selfSubject);
	
	selfSubject->AttachObserver(this, ITriStateControlData::kDefaultIID);
}


//----------------------------------------------------------------------------------------
// AutoDetach
//----------------------------------------------------------------------------------------
void
CZPDisclosureTriAngleObserver::AutoDetach()
{
	InterfacePtr<ISubject> selfSubject(this, UseDefaultIID());
	ASSERT(selfSubject);

	selfSubject->DetachObserver(this, ITriStateControlData::kDefaultIID);
}


//----------------------------------------------------------------------------------------
// Update
//----------------------------------------------------------------------------------------
void
CZPDisclosureTriAngleObserver::Update(
	const ClassID&				theChange,
	ISubject*					theSubject,
	const PMIID &				protocol,
	void*						changedBy)
{
	if(theChange == kTrueStateMessage)
	{
		InterfacePtr<IBoolData> selfIsExpandedBoolData(this, IID_IZEXPISEXPANDED);
		if(!selfIsExpandedBoolData)
			return;

		selfIsExpandedBoolData->Set(!selfIsExpandedBoolData->Get());

		InterfacePtr<IControlView> selfDisclosureView(this, UseDefaultIID());
		if(!selfDisclosureView)
			return;

		InterfacePtr<IZPDisclosureTriAngleOptions> selfDisclosureOptions(this, IID_IZEXPDISCLOSURETRIANGLEOPTIONS);
		if(!selfDisclosureOptions)
			return;

		if ( selfIsExpandedBoolData->Get() )
		{
			selfDisclosureView->SetRsrcID(selfDisclosureOptions->GetExpandedRsrcID());
			selfDisclosureView->SetRsrcPluginID(selfDisclosureOptions->GetExpandedRsrcPluginID());
		}
		else
		{
			selfDisclosureView->SetRsrcID(selfDisclosureOptions->GetCollapseRsrcID());
			selfDisclosureView->SetRsrcPluginID(selfDisclosureOptions->GetCollapseRsrcPluginID());
		}

		InterfacePtr<ISubject> selfSubject(this, UseDefaultIID());
		ASSERT(selfSubject);

		selfSubject->Change( kZPUIDisclosureBtnStateChangedMsg, ITriStateControlData::kDefaultIID );
	}
}
