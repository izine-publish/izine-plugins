//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Commands/CZPSavePrefsCmd.cpp $
//	$Revision: 3571 $
//	$Date: 2011-08-04 12:51:26 +0200 (Thu, 04 Aug 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 15-9-2010
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
#include "IZPPrefs.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_Commands
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass Command
class CZPSavePrefsCmd : public inheritClass
{
public:
						CZPSavePrefsCmd(IPMUnknown * boss);
	virtual				~CZPSavePrefsCmd();

	void				Do();
protected:
	PMString*			CreateName();
private:
};


CREATE_PMINTERFACE(CZPSavePrefsCmd, kZPSavePrefsCmdImpl)

CZPSavePrefsCmd::CZPSavePrefsCmd(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPSavePrefsCmd::~CZPSavePrefsCmd()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// Do
//----------------------------------------------------------------------------------------
void
CZPSavePrefsCmd::Do()
{
	LogFunctionEnterExit;
	InterfacePtr<const IZPPrefs> selfZPPrefs( this, UseDefaultIID() );
	ASSERT(selfZPPrefs);

	const UIDList * itemList = this->GetItemList();
	ASSERT( itemList );
	if( itemList == nil || itemList->IsEmpty())
		return;
	InterfacePtr<IZPPrefs> theTargetPrefs(itemList->GetRef(0), UseDefaultIID());
	ASSERT(theTargetPrefs);
	if( !theTargetPrefs )
		return;

	theTargetPrefs->SetAssetMonitorAutoUpdateDuration( selfZPPrefs->GetAssetMonitorAutoUpdateDuration() );

	theTargetPrefs->SetAutoUpdateLinkedStoriesContent( selfZPPrefs->GetAutoUpdateLinkedStoriesContent() );

	theTargetPrefs->SetAutoUnlockStories( selfZPPrefs->GetAutoUnlockStories() );

	theTargetPrefs->SetCreateAssignmentOnStoryImport( selfZPPrefs->GetCreateAssignmentOnStoryImport() );

	theTargetPrefs->SetIncludeAllAssignmentsOnDocumentCheckin( selfZPPrefs->GetIncludeAllAssignmentsOnDocumentCheckin() );

	theTargetPrefs->SetRemoveStyles( selfZPPrefs->GetRemoveStyles() );
}

//----------------------------------------------------------------------------------------
// CreateName
//----------------------------------------------------------------------------------------
PMString*
CZPSavePrefsCmd::CreateName()
{
	//LogFunctionEnterExit;
	PMString* string_name = new PMString("Save iZine Prefs");
	string_name->SetTranslatable(kFalse);
	return string_name;
}
