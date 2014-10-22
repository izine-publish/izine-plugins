//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Commands/CZPSaveWSConfigCmd.cpp $
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
#include "UIDList.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPWSConfig.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	(eIZPMajorLogArea)(enLMA_Commands | enLMA_WS)
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass Command
class CZPSaveWSConfigCmd : public inheritClass
{
public:
						CZPSaveWSConfigCmd(IPMUnknown * boss);
	virtual				~CZPSaveWSConfigCmd();
protected:
	void				Do();

	PMString*			CreateName();
private:
};


CREATE_PMINTERFACE(CZPSaveWSConfigCmd, kZPSaveWSConfigCmdImpl)

CZPSaveWSConfigCmd::CZPSaveWSConfigCmd(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPSaveWSConfigCmd::~CZPSaveWSConfigCmd()
{

}

//----------------------------------------------------------------------------------------
// Do
//----------------------------------------------------------------------------------------
void
CZPSaveWSConfigCmd::Do()
{
	LogFunctionEnterExit;
	InterfacePtr<const IZPWSConfig> selfWSConfig( this, UseDefaultIID() );
	ASSERT(selfWSConfig);

	const UIDList * itemList = this->GetItemList();
	ASSERT( itemList );
	if( itemList == nil || itemList->IsEmpty())
		return;
	InterfacePtr<IZPWSConfig> theWorkspace(itemList->GetRef(0), UseDefaultIID());
	ASSERT(theWorkspace);
	theWorkspace->Copy( selfWSConfig );
}

//----------------------------------------------------------------------------------------
// CreateName
//----------------------------------------------------------------------------------------
PMString*
CZPSaveWSConfigCmd::CreateName()
{
	//LogFunctionEnterExit;
	PMString* string_name = new PMString("IZSaveWSConfigCmdKey");
	string_name->SetTranslatable(kFalse);
	return string_name;
}

