//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/DnD/CZPUIAssetDnDObserver.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 28-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================


#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "ISubject.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes
#include "DragDropID.h"
#include "CObserver.h"

//OS / ThirdParty includes

#include "IZPUIID.h"
#include "IZPID.h"

//IZP Interfaces
#include "IZPGlobalTempData.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_DnD
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CObserver
class CZPUIAssetDnDObserver : public inheritClass
{
public:
						CZPUIAssetDnDObserver(IPMUnknown * boss);
	virtual				~CZPUIAssetDnDObserver();

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


CREATE_PMINTERFACE(CZPUIAssetDnDObserver, kZPUIAssetDnDObserverImpl)

CZPUIAssetDnDObserver::CZPUIAssetDnDObserver(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPUIAssetDnDObserver::~CZPUIAssetDnDObserver()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// AutoAttach
//----------------------------------------------------------------------------------------
void
CZPUIAssetDnDObserver::AutoAttach()
{
	InterfacePtr<ISubject> selfSubject(this, UseDefaultIID());
	
	if( selfSubject->IsAttached( this, IID_IDRAGDROPCONTROLLER, IID_IZPUIASSETDNDOBSERVER ) == false )
		selfSubject->AttachObserver( this, IID_IDRAGDROPCONTROLLER, IID_IZPUIASSETDNDOBSERVER );

}

//----------------------------------------------------------------------------------------
// AutoDetach
//----------------------------------------------------------------------------------------
void
CZPUIAssetDnDObserver::AutoDetach()
{
	InterfacePtr<ISubject> selfSubject(this, UseDefaultIID());
	
	if( selfSubject->IsAttached( this, IID_IDRAGDROPCONTROLLER, IID_IZPUIASSETDNDOBSERVER ) )
		selfSubject->DetachObserver(this, IID_IDRAGDROPCONTROLLER, IID_IZPUIASSETDNDOBSERVER );
}

//----------------------------------------------------------------------------------------
// Update
//----------------------------------------------------------------------------------------
void
CZPUIAssetDnDObserver::Update(
	const ClassID&				theChange,
	ISubject*					theSubject,
	const PMIID &				protocol,
	void*						changedBy)
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif	
	switch(theChange.Get())
	{
		case kFinishDragDrop:
		{
			InterfacePtr<IZPGlobalTempData> zpGlobalData( gSession, UseDefaultIID() );
			ASSERT( zpGlobalData );
			//zpGlobalData->SetLastDraggedAssetID( kNullString );
			this->AutoDetach();
			break;
		}
	}
}
