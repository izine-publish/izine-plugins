//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/AssetsTreeView/TVNodeAsset/CZPAssetsTVNodeSaveBtnObserver.cpp $
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
class ZPAssetsTVNodeSaveBtnObserver : public inheritClass
{
public:
						ZPAssetsTVNodeSaveBtnObserver(IPMUnknown * boss);
	virtual				~ZPAssetsTVNodeSaveBtnObserver();

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


CREATE_PMINTERFACE(ZPAssetsTVNodeSaveBtnObserver, kZPAssetsTVNodeSaveBtnObserverImpl)

ZPAssetsTVNodeSaveBtnObserver::ZPAssetsTVNodeSaveBtnObserver(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
ZPAssetsTVNodeSaveBtnObserver::~ZPAssetsTVNodeSaveBtnObserver()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// AutoAttach
//----------------------------------------------------------------------------------------
void
ZPAssetsTVNodeSaveBtnObserver::AutoAttach()
{
	InterfacePtr<ISubject> selfSubject(this, UseDefaultIID());
	ASSERT(selfSubject);
	
	selfSubject->AttachObserver(this, ITriStateControlData::kDefaultIID);
}


//----------------------------------------------------------------------------------------
// AutoDetach
//----------------------------------------------------------------------------------------
void
ZPAssetsTVNodeSaveBtnObserver::AutoDetach()
{
	InterfacePtr<ISubject> selfSubject(this, UseDefaultIID());
	ASSERT(selfSubject);

	selfSubject->DetachObserver(this, ITriStateControlData::kDefaultIID);
}


//----------------------------------------------------------------------------------------
// Update
//----------------------------------------------------------------------------------------
void
ZPAssetsTVNodeSaveBtnObserver::Update(
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
