//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Data/ZPData/CZPEditionObjController.cpp $
//	$Revision: 2249 $
//	$Date: 2010-10-06 16:44:33 +0200 (Wed, 06 Oct 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 21-6-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPAssetList.h"
#include "IZPTaskList.h"

//IZP General includes
#include "CZPDBObjController.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CZPDBObjController
class CZPEditionObjController : public inheritClass
{
public:
						CZPEditionObjController(IPMUnknown * boss);
	virtual				~CZPEditionObjController();

	void				RequestUpdateFromServer(
							PMIID						inWhichProtocol );
protected:
private:
};


CREATE_PMINTERFACE(CZPEditionObjController, kZPEditionObjControllerImpl)

CZPEditionObjController::CZPEditionObjController(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPEditionObjController::~CZPEditionObjController()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// RequestUpdateFromServer
//----------------------------------------------------------------------------------------
void
CZPEditionObjController::RequestUpdateFromServer(
	PMIID						inWhichProtocol )
{
	if( inWhichProtocol == kInvalidInterfaceID || inWhichProtocol == IZPAssetList::kDefaultIID )
	{
		InterfacePtr<IZPAssetList> editionAssetList( this, UseDefaultIID() );
		ASSERT( editionAssetList );
		editionAssetList->RequestUpdateFromServer();
	}

	if( inWhichProtocol == kInvalidInterfaceID || inWhichProtocol == IZPTaskList::kDefaultIID )
	{
		InterfacePtr<IZPTaskList> editionTaskList( this, UseDefaultIID() );
		ASSERT( editionTaskList );
		editionTaskList->RequestUpdateFromServer();
	}
}
