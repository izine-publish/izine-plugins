#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IObserver.h"
#include "ISubject.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes
#include "CObserver.h"
#include "PageItemScrapID.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPGlobalTempData.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CObserver
class CZPDocCmdObserver : public inheritClass
{
public:
						CZPDocCmdObserver(IPMUnknown * boss);
	virtual				~CZPDocCmdObserver();

	void		Update(const ClassID& theChange, ISubject* theSubject, const PMIID &protocol, void* changedBy);
	//void		LazyUpdate(ISubject* theSubject, const PMIID &protocol, const LazyNotificationData* data);
protected:
private:
};


CREATE_PMINTERFACE(CZPDocCmdObserver, kZPDocCmdObserverImpl)

CZPDocCmdObserver::CZPDocCmdObserver(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPDocCmdObserver::~CZPDocCmdObserver()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// Update
//----------------------------------------------------------------------------------------
void
CZPDocCmdObserver::Update(
	const ClassID& theChange, ISubject* theSubject, const PMIID &protocol, void* changedBy)
{
	if( protocol == IID_ICOMMANDMGR )
	{
		InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
		if( theSubject == selfSubject.get() )
		{
			if( theChange == kAfterDoMessageBoss )
			{
				ICommand * cmd = (ICommand*) changedBy;
				ClassID cmdClass = ::GetClass(cmd);
				if( cmdClass == kPasteCmdBoss )
				{
#if defined(InDnCS5) || defined(InDnCS5_5)
					const IExecutionContext* ec = GetExecutionContext();
					ISession* gSession = ec->GetSession();
#endif
					InterfacePtr<IZPGlobalTempData> gTempData( gSession, UseDefaultIID() );
					ASSERT( gTempData );
					gTempData->SetSkipAddLink( false );
					selfSubject->DetachObserver( this, IID_ICOMMANDMGR, IID_IZPDOCCMDOBSERVER );
				}
			}
		}
	}
}

#if 0
//----------------------------------------------------------------------------------------
// LazyUpdate
//----------------------------------------------------------------------------------------
void
CZPDocCmdObserver::LazyUpdate(
	ISubject* theSubject, const PMIID &protocol, const LazyNotificationData* data)
{
	if( protocol == IID_ICOMMANDMGR )
	{
		InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
		if( theSubject == selfSubject.get() )
		{
			//if( theChange == kAfterDoMessageBoss )
			{
				int i = 0;
				//selfSubject->DeatachObserver( selfCmdObserver, IID_ICOMMANDMGR );
			}
		}
	}
}
#endif
