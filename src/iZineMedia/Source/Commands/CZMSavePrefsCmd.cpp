//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Commands/CZMSavePrefsCmd.cpp $
//	$Revision: 2909 $
//	$Date: 2011-04-29 16:02:23 +0200 (Fri, 29 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 15-9-2010
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "ISubject.h"

//SDK General includes
#include "Command.h"

//OS / ThirdParty includes

#include "iZMediaID.h"

//IZP Interfaces
#include "IZMPrefs.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass Command
class CZMSavePrefsCmd : public inheritClass
{
public:
						CZMSavePrefsCmd(IPMUnknown * boss);
	virtual				~CZMSavePrefsCmd();

	void				Do();
	void				DoNotify();
protected:
	PMString*			CreateName();
private:
};


CREATE_PMINTERFACE(CZMSavePrefsCmd, kZMSavePrefsCmdImpl)

CZMSavePrefsCmd::CZMSavePrefsCmd(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZMSavePrefsCmd::~CZMSavePrefsCmd()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// Do
//----------------------------------------------------------------------------------------
void
CZMSavePrefsCmd::Do()
{
	InterfacePtr<const IZMPrefs> selfZMPrefs( this, UseDefaultIID() );
	ASSERT(selfZMPrefs);

	const UIDList * itemList = this->GetItemList();
	ASSERT( itemList );
	if( itemList == nil || itemList->IsEmpty())
		return;
	InterfacePtr<IZMPrefs> theTargetPrefs(itemList->GetRef(0), UseDefaultIID());
	ASSERT(theTargetPrefs);
	if( !theTargetPrefs )
		return;

	theTargetPrefs->SetRepositoryPath( selfZMPrefs->GetRepositoryPath() );
	theTargetPrefs->SetAdFileNamePrefix( selfZMPrefs->GetAdFileNamePrefix() );
}

//----------------------------------------------------------------------------------------
// CreateName
//----------------------------------------------------------------------------------------
PMString*
CZMSavePrefsCmd::CreateName()
{
	PMString* string_name = new PMString("Save iZine Prefs");
	string_name->SetTranslatable(kFalse);
	return string_name;
}


//----------------------------------------------------------------------------------------
// DoNotify
//----------------------------------------------------------------------------------------
void
CZMSavePrefsCmd::DoNotify()
{
	const UIDList * itemList = this->GetItemList();
	ASSERT( itemList );
	if( itemList == nil || itemList->IsEmpty())
		return;
	InterfacePtr<const IZMPrefs> theTargetPrefs(itemList->GetRef(0), UseDefaultIID());
	if( !theTargetPrefs )
		return;

	// Notify change to default IBPIData through the workspace's subject.
	InterfacePtr<ISubject> subject(theTargetPrefs, UseDefaultIID());
	ASSERT(subject);
	if (subject)
		subject->ModelChange(kZMSavePrefsCmdMsg, IZMPrefs::kDefaultIID, this);
}
