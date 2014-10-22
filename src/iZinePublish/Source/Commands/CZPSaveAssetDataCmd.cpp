//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Commands/CZPSaveAssetDataCmd.cpp $
//	$Revision: 3571 $
//	$Date: 2011-08-04 12:51:26 +0200 (Thu, 04 Aug 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 3-6-2010
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
#include "IZPAssetData.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_Commands
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass Command
class CZPSaveAssetDataCmd : public inheritClass
{
public:
						CZPSaveAssetDataCmd(IPMUnknown * boss);
	virtual				~CZPSaveAssetDataCmd();
protected:
	void				Do();

	PMString*			CreateName();
private:
};


CREATE_PMINTERFACE(CZPSaveAssetDataCmd, kZPSaveAssetDataCmdImpl)

CZPSaveAssetDataCmd::CZPSaveAssetDataCmd(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPSaveAssetDataCmd::~CZPSaveAssetDataCmd()
{

}

//----------------------------------------------------------------------------------------
// Do
//----------------------------------------------------------------------------------------
void
CZPSaveAssetDataCmd::Do()
{
	LogFunctionEnterExit;
	InterfacePtr<const IZPAssetData> selfAssetData( this, UseDefaultIID() );
	ASSERT( selfAssetData );

	const UIDList * itemList = this->GetItemList();
	ASSERT( itemList );
	if( itemList == nil || itemList->IsEmpty())
		return;
	int listLen = itemList->Length();
	for(int i = 0; i < listLen; ++i)
	{
		InterfacePtr<IZPAssetData> theTargetData(itemList->GetRef(i), UseDefaultIID());
		ASSERT(theTargetData);
		theTargetData->Copy( selfAssetData );
	}
}

//----------------------------------------------------------------------------------------
// CreateName
//----------------------------------------------------------------------------------------
PMString*
CZPSaveAssetDataCmd::CreateName()
{
	//LogFunctionEnterExit;
	PMString* string_name = new PMString("ZPSaveAssetDataCmdKey");
	string_name->SetTranslatable(kFalse);
	return string_name;
}

