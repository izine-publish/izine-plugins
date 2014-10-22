//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/EditionAssetsPopup/CZPEditionAssetsPopupObserver.cpp $
//	$Revision: 2374 $
//	$Date: 2010-10-25 11:16:54 +0200 (Mon, 25 Oct 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 30-9-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes

//OS / ThirdParty includes

#include "IZPUIID.h"
#include "IZPID.h"

//IZP Interfaces
#include "IZPAssetList.h"

//IZP General includes
#include "AZPUIDBObjDropDownObserver.h"
#include "AZPDBObjPopupController.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_UI
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass AZPUIDBObjDropDownObserver
class CZPEditionAssetsPopupObserver : public inheritClass
{
public:
						CZPEditionAssetsPopupObserver(IPMUnknown * boss);
	virtual				~CZPEditionAssetsPopupObserver();

	void				AutoAttach();
	void				AutoDetach();
	void				Update(
							const ClassID&				theChange,
							ISubject*					theSubject,
							const PMIID &				protocol,
							void*						changedBy);
protected:
	void				RefreshPopup();
private:
};


CREATE_PMINTERFACE(CZPEditionAssetsPopupObserver, kZPEditionAssetsPopupObserverImpl)

CZPEditionAssetsPopupObserver::CZPEditionAssetsPopupObserver(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPEditionAssetsPopupObserver::~CZPEditionAssetsPopupObserver()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// AutoAttach
//----------------------------------------------------------------------------------------
void
CZPEditionAssetsPopupObserver::AutoAttach()
{
	inheritClass::AutoAttach();
}

//----------------------------------------------------------------------------------------
// AutoDetach
//----------------------------------------------------------------------------------------
void
CZPEditionAssetsPopupObserver::AutoDetach()
{
	inheritClass::AutoDetach();
}

//----------------------------------------------------------------------------------------
// Update
//----------------------------------------------------------------------------------------
void
CZPEditionAssetsPopupObserver::Update(
	const ClassID&				theChange,
	ISubject*					theSubject,
	const PMIID &				protocol,
	void*						changedBy)
{
	inheritClass::Update( theChange, theSubject, protocol, changedBy );
	
	switch(theChange.Get())
	{
		case kZPAssetListWillChangeMsg:
			//if( protocol == IZPAssetList::kDefaultIID )
				//mSkipAssetListUpdates = true;
			break;
		case kZPAssetListChangedMsg:	//From edition
		{
			if( protocol == IZPAssetList::kDefaultIID )
			{
				//mSkipAssetListUpdates = false;
				this->RefreshPopup();
			}
			break;
		}
	}
}

//----------------------------------------------------------------------------------------
// RefreshPopup
//----------------------------------------------------------------------------------------
void
CZPEditionAssetsPopupObserver::RefreshPopup()
{
	LogFunctionEnterExit;

	InterfacePtr<AZPDBObjPopupController> selfController( this, IZPDBObjPopupController::kDefaultIID );
	ASSERT( selfController );

	selfController->Refresh();
}