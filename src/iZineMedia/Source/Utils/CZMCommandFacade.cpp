//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Utils/CZMCommandFacade.cpp $
//	$Revision: 2739 $
//	$Date: 2011-04-07 13:10:59 +0200 (Thu, 07 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 22-4-2010
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "ICommand.h"
#include "IStringData.h"
#include "IWorkspace.h"

//SDK General includes
#include "UIDList.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "IZMediaID.h"

//IZP Interfaces
#include "IZMCommandFacade.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_Commands
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZMCommandFacade>
class CZMCommandFacade : public inheritClass
{
public:
						CZMCommandFacade(IPMUnknown * boss);
	virtual				~CZMCommandFacade();
	
#pragma mark -
//Command creation functions, sdk bosses.

#pragma mark -
//Command creation functions, partially setting the data with command.
	ICommand *			CreateCommand_SaveZMPrefs() const;
protected:
private:
};


CREATE_PMINTERFACE(CZMCommandFacade, kZMCommandFacadeImpl)

CZMCommandFacade::CZMCommandFacade(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZMCommandFacade::~CZMCommandFacade()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// CreateCommand_SaveZMPrefs
//----------------------------------------------------------------------------------------
ICommand *
CZMCommandFacade::CreateCommand_SaveZMPrefs()const
{
	InterfacePtr<ICommand> saveZMPrefsCmd(CmdUtils::CreateCommand(kZMSavePrefsCmdBoss));
	if(!saveZMPrefsCmd)
		return nil;

	InterfacePtr<const IWorkspace> sessionWS( gSession->QueryWorkspace() );
	saveZMPrefsCmd->SetItemList( UIDList( sessionWS ) );

	return saveZMPrefsCmd.forget();
}
