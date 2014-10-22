//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Data/CZMEditionObjController.cpp $
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
#include "IZMAdFileList.h"
#include "IZMEditionMgr.h"

//IZP General includes
#include "CZMDataObjController.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CZMDataObjController
class CZMEditionObjController : public inheritClass
{
public:
						CZMEditionObjController(IPMUnknown * boss);
	virtual				~CZMEditionObjController();

	void				RequestUpdateFromServer(
							PMIID						inWhichProtocol );
protected:
private:
};


CREATE_PMINTERFACE(CZMEditionObjController, kZMEditionObjControllerImpl)

CZMEditionObjController::CZMEditionObjController(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZMEditionObjController::~CZMEditionObjController()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// RequestUpdateFromServer
//----------------------------------------------------------------------------------------
void
CZMEditionObjController::RequestUpdateFromServer(
	PMIID						inWhichProtocol )
{
	if( inWhichProtocol == kInvalidInterfaceID || inWhichProtocol == IZMAdFileList::kDefaultIID )
	{
		InterfacePtr<const IZMApp> zpApp( gSession, UseDefaultIID() );
		ASSERT( zpApp );

		InterfacePtr<IZMEditionMgr> editionMgr( zpApp->GetEditionMgr(), UseDefaultIID() );
		ASSERT(editionMgr);

		InterfacePtr<const IStringData> editionID( this, IID_IZMDATAID );
		ASSERT( editionID );
		editionMgr->FetchAdFileListForEdition( editionID->Get() );
	}
}
