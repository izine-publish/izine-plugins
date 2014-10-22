//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Commands/CZPSaveUserCredentialsCmd.cpp $
//	$Revision: 3571 $
//	$Date: 2011-08-04 12:51:26 +0200 (Thu, 04 Aug 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 22-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes
#include "Command.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPUserCredentials.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_Commands
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass Command
class CZPSaveUserCredentialsCmd : public inheritClass
{
public:
						CZPSaveUserCredentialsCmd(IPMUnknown * boss);
	virtual				~CZPSaveUserCredentialsCmd();
protected:
	void				Do();

	PMString*			CreateName();
private:
};


CREATE_PMINTERFACE(CZPSaveUserCredentialsCmd, kZPSaveUserCredentialsCmdImpl)

CZPSaveUserCredentialsCmd::CZPSaveUserCredentialsCmd(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPSaveUserCredentialsCmd::~CZPSaveUserCredentialsCmd()
{

}

//----------------------------------------------------------------------------------------
// Do
//----------------------------------------------------------------------------------------
void
CZPSaveUserCredentialsCmd::Do()
{
	LogFunctionEnterExit;
	InterfacePtr<const IZPUserCredentials> selfUserCredentials(this, UseDefaultIID());
	ASSERT(selfUserCredentials);

	const UIDList * itemList = this->GetItemList();
	ASSERT( itemList );
	if( itemList == nil || itemList->IsEmpty())
		return;
	InterfacePtr<IZPUserCredentials> theWorkspace(itemList->GetRef(0), UseDefaultIID());
	ASSERT(theWorkspace);
	theWorkspace->Copy( selfUserCredentials );
}

//----------------------------------------------------------------------------------------
// CreateName
//----------------------------------------------------------------------------------------
PMString*
CZPSaveUserCredentialsCmd::CreateName()
{
	//LogFunctionEnterExit;
	PMString* string_name = new PMString("IZSaveUserCredentialCmdKey");
	string_name->SetTranslatable(kFalse);
	return string_name;
}

















