//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/Commands/CZExpSavePrefsCmd.cpp $
//	$Revision: 4012 $
//	$Date: 2012-05-22 15:05:16 +0200 (Tue, 22 May 2012) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Aman Alam
//	Created: 6-6-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes
#include "Command.h"

//OS / ThirdParty includes

#include "IZExpID.h"

//IZP Interfaces
#include "IZExpPrefs.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass Command
class CZExpSavePrefsCmd : public inheritClass
{
public:
						CZExpSavePrefsCmd(IPMUnknown * boss);
	virtual				~CZExpSavePrefsCmd();

	void				Do();
protected:
	PMString*			CreateName();
private:
};


CREATE_PMINTERFACE(CZExpSavePrefsCmd, kZExpSavePrefsCmdImpl)

CZExpSavePrefsCmd::CZExpSavePrefsCmd(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZExpSavePrefsCmd::~CZExpSavePrefsCmd()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// Do
//----------------------------------------------------------------------------------------
void
CZExpSavePrefsCmd::Do()
{
	InterfacePtr<const IZExpPrefs> selfExpPrefs( this, UseDefaultIID() );
	ASSERT(selfExpPrefs);

	const UIDList * itemList = this->GetItemList();
	ASSERT( itemList );
	if( itemList == nil || itemList->IsEmpty())
		return;

	InterfacePtr<IZExpPrefs> theTargetPrefs(itemList->GetRef(0), UseDefaultIID());
	ASSERT(theTargetPrefs);
	if( !theTargetPrefs )
		return;

	theTargetPrefs->SetUserName( selfExpPrefs->GetUserName() );

	theTargetPrefs->SetPassword( selfExpPrefs->GetPassword() );

	theTargetPrefs->SetPluginId( selfExpPrefs->GetPluginId() );

	theTargetPrefs->SetNeedsProxyServer( selfExpPrefs->NeedsProxyServer() );
	theTargetPrefs->SetProxyServer( selfExpPrefs->GetProxyServer() );
	theTargetPrefs->SetProxyPort( selfExpPrefs->GetProxyPort() );
}

//----------------------------------------------------------------------------------------
// CreateName
//----------------------------------------------------------------------------------------
PMString*
CZExpSavePrefsCmd::CreateName()
{
	PMString* string_name = new PMString("Save iZine Export Prefs");
	string_name->SetTranslatable(kFalse);
	return string_name;
}
