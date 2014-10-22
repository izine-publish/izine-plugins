//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/AssetsTreeView/TVNodeAsset/CZPAssetsTVNodeDeleteBtnObserver.cpp $
//	$Revision: 1430 $
//	$Date: 2010-06-15 07:35:17 +0200 (Tue, 15 Jun 2010) $
//	$Author: rajkumar.sehrawat $
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
class ZPAssetsTVNodeDeleteBtnObserver : public inheritClass
{
public:
						ZPAssetsTVNodeDeleteBtnObserver(IPMUnknown * boss);
	virtual				~ZPAssetsTVNodeDeleteBtnObserver();

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


CREATE_PMINTERFACE(ZPAssetsTVNodeDeleteBtnObserver, kZPAssetsTVNodeDeleteBtnObserverImpl)

ZPAssetsTVNodeDeleteBtnObserver::ZPAssetsTVNodeDeleteBtnObserver(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
ZPAssetsTVNodeDeleteBtnObserver::~ZPAssetsTVNodeDeleteBtnObserver()
{

}

//----------------------------------------------------------------------------------------
// AutoAttach
//----------------------------------------------------------------------------------------
void
ZPAssetsTVNodeDeleteBtnObserver::AutoAttach()
{
	InterfacePtr<ISubject> selfSubject(this, UseDefaultIID());
	ASSERT(selfSubject);
	
	selfSubject->AttachObserver(this, ITriStateControlData::kDefaultIID);
}


//----------------------------------------------------------------------------------------
// AutoDetach
//----------------------------------------------------------------------------------------
void
ZPAssetsTVNodeDeleteBtnObserver::AutoDetach()
{
	InterfacePtr<ISubject> selfSubject(this, UseDefaultIID());
	ASSERT(selfSubject);

	selfSubject->DetachObserver(this, ITriStateControlData::kDefaultIID);
}


//----------------------------------------------------------------------------------------
// Update
//----------------------------------------------------------------------------------------
void
ZPAssetsTVNodeDeleteBtnObserver::Update(
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
