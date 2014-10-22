//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Data/CZMTitleObjController.cpp $
//	$Revision: 2750 $
//	$Date: 2011-04-08 11:33:52 +0200 (Fri, 08 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 8-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IStringData.h"

//SDK General includes

//OS / ThirdParty includes

#include "iZMediaID.h"

//IZP Interfaces
#include "IZMApp.h"
#include "IZMEditionList.h"
#include "IZMTitleMgr.h"

//IZP General includes
#include "CZMDataObjController.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CZMDataObjController
class CZMTitleObjController : public inheritClass
{
public:
						CZMTitleObjController(IPMUnknown * boss);
	virtual				~CZMTitleObjController();

	void				RequestUpdateFromServer(
							PMIID						inWhichProtocol );
protected:
private:
};


CREATE_PMINTERFACE(CZMTitleObjController, kZMTitleObjControllerImpl)

CZMTitleObjController::CZMTitleObjController(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZMTitleObjController::~CZMTitleObjController()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// RequestUpdateFromServer
//----------------------------------------------------------------------------------------
void
CZMTitleObjController::RequestUpdateFromServer(
	PMIID						inWhichProtocol )
{
	if( inWhichProtocol == kInvalidInterfaceID || inWhichProtocol == IZMEditionList::kDefaultIID )
	{
		InterfacePtr<const IZMApp> zpApp( gSession, UseDefaultIID() );
		ASSERT( zpApp );

		InterfacePtr<IZMTitleMgr> titleMgr( zpApp->GetTitleMgr(), UseDefaultIID() );
		ASSERT(titleMgr);

		InterfacePtr<const IStringData> titleID( this, IID_IZMDATAID );
		ASSERT( titleID );
		titleMgr->FetchEditionsForTitle( titleID->Get() );
	}
}
