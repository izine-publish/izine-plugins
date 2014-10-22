//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Commands/CZPModifyPersistUIDListCmd.cpp $
//	$Revision: 3571 $
//	$Date: 2011-08-04 12:51:26 +0200 (Thu, 04 Aug 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 4-7-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IBoolData.h"
#include "IInterfaceIDData.h"
#include "IClassIDData.h"

//SDK General includes
#include "Command.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPUIDListData.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_Commands
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass Command
class CZPModifyPersistUIDListCmd : public inheritClass
{
public:
						CZPModifyPersistUIDListCmd(IPMUnknown * boss);
	virtual				~CZPModifyPersistUIDListCmd();
protected:
	void				Do();

	void				DoCreateUID();
	void				DoDeleteUID();

	IZPUIDListData *	QueryListToModify();

	PMString*			CreateName();
private:
};


CREATE_PMINTERFACE(CZPModifyPersistUIDListCmd, kZPModifyPersistUIDListCmdImpl)

CZPModifyPersistUIDListCmd::CZPModifyPersistUIDListCmd(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPModifyPersistUIDListCmd::~CZPModifyPersistUIDListCmd()
{

}

#pragma mark -

//----------------------------------------------------------------------------------------
// Do
//----------------------------------------------------------------------------------------
void
CZPModifyPersistUIDListCmd::Do()
{
	LogFunctionEnterExit;
	InterfacePtr<const IBoolData> selfBoolData( this, UseDefaultIID() );
	ASSERT( selfBoolData );

	if( selfBoolData->Get() )
		this->DoDeleteUID();
	else
		this->DoCreateUID();
}

//----------------------------------------------------------------------------------------
// DoCreateUID
//----------------------------------------------------------------------------------------
void
CZPModifyPersistUIDListCmd::DoCreateUID()
{
	LogFunctionEnterExit;

	int32	itemsLen = fItemList.Length();
	if( itemsLen != 1 )
		return;

	InterfacePtr<IZPUIDListData> listToModify( this->QueryListToModify() );
	ASSERT( listToModify );
	if( !listToModify )
		return;

	InterfacePtr<IClassIDData> classIDData( this, UseDefaultIID() );
	ASSERT( classIDData && classIDData->GetClassID() != kInvalidClass );
	if( !classIDData || classIDData->GetClassID() == kInvalidClass )
		return;

	IDataBase *db = fItemList.GetDataBase();
	UID newUID = db->NewUID( classIDData->GetClassID() );
	listToModify->Append( newUID );

	fItemList.Append( newUID );
}


//----------------------------------------------------------------------------------------
// DoDeleteUID
//----------------------------------------------------------------------------------------
void
CZPModifyPersistUIDListCmd::DoDeleteUID()
{
	LogFunctionEnterExit;

	int32	itemsLen = fItemList.Length();
	if( itemsLen < 2 )
		return;

	InterfacePtr<IZPUIDListData> listToModify( this->QueryListToModify() );
	if( !listToModify )
		return;

	IDataBase *db = fItemList.GetDataBase();

	for (int32 i = 1; i < itemsLen; i++)
	{
		UID uid = fItemList.At(i);
		if (uid != kInvalidUID)
		{
			//We don't care about the object, which we are deleting, just 
			listToModify->Remove(uid);
			db->DeleteUID(uid);
		}
		else
		{
			//err = kFailure;
		}
	}
}

//----------------------------------------------------------------------------------------
// QueryListToModify
//----------------------------------------------------------------------------------------
IZPUIDListData *
CZPModifyPersistUIDListCmd::QueryListToModify()
{
	//LogFunctionEnterExit;
	PMIID listIntefaceID = IZPUIDListData::kDefaultIID;
	InterfacePtr<const IInterfaceIDData> interfaceID( this, UseDefaultIID() );
	if( interfaceID && interfaceID->GetInterfaceID() != kInvalidInterfaceID )
		listIntefaceID = interfaceID->GetInterfaceID();

	IDataBase *db = fItemList.GetDataBase();
	InterfacePtr<IZPUIDListData> listToModify( db, fItemList[0], listIntefaceID );
	return listToModify.forget();
}


//----------------------------------------------------------------------------------------
// CreateName
//----------------------------------------------------------------------------------------
PMString*
CZPModifyPersistUIDListCmd::CreateName()
{
	//LogFunctionEnterExit;
	PMString* string_name = new PMString("Modify Persist UID list cmd");
	string_name->SetTranslatable(kFalse);
	return string_name;
}