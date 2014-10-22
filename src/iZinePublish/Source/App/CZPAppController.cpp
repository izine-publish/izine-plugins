//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/App/CZPAppController.cpp $
//	$Revision: 3566 $
//	$Date: 2011-08-04 12:14:39 +0200 (Thu, 04 Aug 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 29-3-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "ISubject.h"

//SDK General includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPAppController.h"

//IZP General includes
#include "IZPLog.h"

#define thisFileLA	enLMA_App
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

IZPAppController *	gZPApp = NULL;	//Easy access to global object.

class CZPAppController : public CPMUnknown<IZPAppController>
{
public:
				CZPAppController(IPMUnknown * boss);
	virtual		~CZPAppController();

	virtual enAppState		GetAppState() const;
	virtual void			SetAppState(
								enAppState				inAppState );
protected:
private:

	enAppState		mState;
};

CREATE_PMINTERFACE(CZPAppController, kZPAppControllerImpl)

CZPAppController::CZPAppController(IPMUnknown * boss) :
CPMUnknown<IZPAppController>(boss)
, mState( eAppState_NotLoggedIn )
{
	gZPApp = this;
	this->AddRef();	//Release in shutdown.
}

CZPAppController::~CZPAppController()
{
	//TODO: boadcast message the global app will no longer be available.
	gZPApp = NULL;
}

//----------------------------------------------------------------------------------------
// GetAppState
//----------------------------------------------------------------------------------------
enAppState
CZPAppController::GetAppState()const
{
	//LogFunctionEnterExit;
	return mState;
}

//----------------------------------------------------------------------------------------
// SetAppState
//----------------------------------------------------------------------------------------
void
CZPAppController::SetAppState(
	enAppState				inAppState)
{
	LogFunctionEnterExit;

	mState = inAppState;
	InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
	ASSERT(selfSubject);

	selfSubject->Change( kZPAppStateChanged, IZPAppController::kDefaultIID );
}
