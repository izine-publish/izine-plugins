//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Commands/CZPModifyAssetLocalInfoCmd.cpp $
//	$Revision: 3571 $
//	$Date: 2011-08-04 12:51:26 +0200 (Thu, 04 Aug 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 7-7-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IBoolData.h"
#include "IDateTimeData.h"
#include "IIntData.h"
#include "IStringData.h"
#include "ISubject.h"

//SDK General includes
#include "Command.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPAssetLocalInfo.h"

//IZP General includes
#include "ZPTypes.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_Commands
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass Command
class CZPModifyAssetLocalInfoCmd : public inheritClass
{
public:
						CZPModifyAssetLocalInfoCmd(IPMUnknown * boss);
	virtual				~CZPModifyAssetLocalInfoCmd();

	void				Do();

	PMString*			CreateName();
protected:
	// Executes the command, causing changes to the model.
		//
		// This must be overridden by each command.  This method
		// is only called by ExecuteImmediate(), which also sets
		// the command state and calls DoNotify().

	/**	Initiates notification, commands that want to perform notification
		define this method. It is called after the Do method has been
		called.
		@see ISubject
	 */
	void				DoNotify();
private:
};


CREATE_PMINTERFACE(CZPModifyAssetLocalInfoCmd, kZPModifyAssetLocalInfoCmdImpl)

CZPModifyAssetLocalInfoCmd::CZPModifyAssetLocalInfoCmd(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPModifyAssetLocalInfoCmd::~CZPModifyAssetLocalInfoCmd()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// Do
//----------------------------------------------------------------------------------------
void
CZPModifyAssetLocalInfoCmd::Do()
{
	LogFunctionEnterExit;

	int32	itemsLen = fItemList.Length();
	if( itemsLen != 1 )
		return;

	InterfacePtr<IZPAssetLocalInfo> targetAssetLocalInfo( fItemList.GetRef(0), UseDefaultIID() );
	ASSERT( targetAssetLocalInfo );

	InterfacePtr<const IIntData> cmdFlagData( this, IID_IZPMODIFYASSETLOCALINFOFLAGS );
	ASSERT( cmdFlagData && cmdFlagData->Get() != 0 );

	int32 cmdFlags = cmdFlagData->Get();
	if( eUpdateAssetLocalInfo_IsModified & cmdFlags )
	{
		InterfacePtr<const IBoolData> cmdBoolData( this, UseDefaultIID() );
		ASSERT( cmdBoolData );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, " Locally mofified flag : %hd", cmdBoolData->Get() );
		targetAssetLocalInfo->SetFileIsLocallyModified( cmdBoolData->Get() );
	}

	if( eUpdateAssetLocalInfo_VersionNumber & cmdFlags )
	{
		InterfacePtr<const IIntData> cmdIntData( this, UseDefaultIID() );
		ASSERT( cmdIntData );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, " Version number being set to : %d", cmdIntData->Get() );
		targetAssetLocalInfo->SetFileVersionNumber( cmdIntData->Get() );
	}

	if( eUpdateAssetLocalInfo_AssetID & cmdFlags )
	{
		InterfacePtr<const IStringData> cmdIDData( this, IID_IZPDATAID );
		ASSERT( cmdIDData );
		InterfacePtr<IStringData> targetIDData( targetAssetLocalInfo, IID_IZPDATAID );
		ASSERT( targetIDData );
		ASSERT( targetIDData->Get().IsEmpty() );	//AssetID cannot be changed.
		targetIDData->Set( cmdIDData->Get() );
	}

	if (cmdFlags)
	{
		InterfacePtr<IDateTimeData> modifiedDateTime(targetAssetLocalInfo, UseDefaultIID());
		ASSERT( modifiedDateTime );
		if (modifiedDateTime)
			modifiedDateTime->Set(time(nil));
	}
}

//----------------------------------------------------------------------------------------
// DoNotify
//----------------------------------------------------------------------------------------
void
CZPModifyAssetLocalInfoCmd::DoNotify()
{
	LogFunctionEnterExit;
	int32	itemsLen = fItemList.Length();
	if( itemsLen != 1 )
		return;

	InterfacePtr<const IZPAssetLocalInfo> targetAssetLocalInfo( fItemList.GetRef(0), UseDefaultIID() );
	ASSERT( targetAssetLocalInfo );

	// Notify change for the local info
	InterfacePtr<ISubject> subject(targetAssetLocalInfo, UseDefaultIID());
	ASSERT(subject);
	if (subject)
		subject->ModelChange(kZPAssetLocalInfoChangedMsg, IZPAssetLocalInfo::kDefaultIID, this);
}

//----------------------------------------------------------------------------------------
// CreateName
//----------------------------------------------------------------------------------------
PMString*
CZPModifyAssetLocalInfoCmd::CreateName()
{
	//LogFunctionEnterExit;
	PMString* string_name = new PMString("Modify Asset Local info");
	string_name->SetTranslatable(kFalse);
	return string_name;
}
