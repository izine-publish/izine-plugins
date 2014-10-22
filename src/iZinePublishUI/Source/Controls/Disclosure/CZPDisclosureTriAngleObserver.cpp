//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/Disclosure/CZPDisclosureTriAngleObserver.cpp $
//	$Revision: 2976 $
//	$Date: 2011-05-10 13:03:39 +0200 (Tue, 10 May 2011) $
//	$Author: aman.alam $
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

#include "IZPUIID.h"

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


CREATE_PMINTERFACE(CZPDisclosureTriAngleObserver, kZPDisclosureTriAngleObserverImpl)

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
		InterfacePtr<IBoolData> selfIsExpandedBoolData(this, IID_IZPISEXPANDED);
		if(!selfIsExpandedBoolData)
			return;

		selfIsExpandedBoolData->Set(!selfIsExpandedBoolData->Get());

		InterfacePtr<IControlView> selfDisclosureView(this, UseDefaultIID());
		if(!selfDisclosureView)
			return;

		InterfacePtr<IZPDisclosureTriAngleOptions> selfDisclosureOptions(this, IID_IZPDISCLOSURETRIANGLEOPTIONS);
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
