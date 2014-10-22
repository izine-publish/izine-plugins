//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/App/CZPDefaultSession.cpp $
//	$Revision: 3566 $
//	$Date: 2011-08-04 12:14:39 +0200 (Thu, 04 Aug 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 16-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPDefaultSession.h"
#include "IZPUserCredentials.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_None
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZPDefaultSession>
class CZPDefaultSession : public inheritClass
{
public:
				CZPDefaultSession(IPMUnknown * boss);
	virtual		~CZPDefaultSession();

	const IZPUserCredentials *	GetUserCredentials() const;
	void						SetUserCredentials(
									const IZPUserCredentials *			inUserCredentials );
protected:
private:
	InterfacePtr<const IZPUserCredentials>	mDefaultCredentials;
};


CREATE_PMINTERFACE(CZPDefaultSession, kZPDefaultSessionImpl)

CZPDefaultSession::CZPDefaultSession(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPDefaultSession::~CZPDefaultSession()
{

}

//----------------------------------------------------------------------------------------
// GetUserCredentials
//----------------------------------------------------------------------------------------
const IZPUserCredentials *
CZPDefaultSession::GetUserCredentials()const
{
	//LogFunctionEnterExit;
	return mDefaultCredentials;
}

//----------------------------------------------------------------------------------------
// SetUserCredentials
//----------------------------------------------------------------------------------------
void
CZPDefaultSession::SetUserCredentials(
	const IZPUserCredentials *			inUserCredentials)
{
	//LogFunctionEnterExit;
	inUserCredentials->AddRef();
	mDefaultCredentials.reset( inUserCredentials );
}

