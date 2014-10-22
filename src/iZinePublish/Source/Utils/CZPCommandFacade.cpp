//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Utils/CZPCommandFacade.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 22-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IBoolData.h"
#include "IClassIDData.h"
#include "ICommand.h"
#include "IIntData.h"
#include "IInterfaceIDData.h"
#include "InCopySharedID.h"
#ifdef InDnCS3	//TODO: CS5 Porting
#include "IRefreshCmdData.h"
#endif
#include "IStringData.h"
#include "IWorkspace.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes
#include "UIDList.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPAssetData.h"
#include "IZPAssetUtils.h"
#include "IZPAssetLocalInfo.h"
#include "IZPCommandFacade.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_Commands
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZPCommandFacade>
class CZPCommandFacade : public inheritClass
{
public:
						CZPCommandFacade(IPMUnknown * boss);
	virtual				~CZPCommandFacade();

	void				SetInCopyUser(
							const PMString &			inUserName );

	void				SaveAssetData(
							const IZPAssetData *		inSourceData,
							const IPMUnknown *			inTarget );

	void				SaveAssetData(
							const IStringData *			inAssetID,	//Asset to take data from
							const IPMUnknown *			inTarget ) const;

	void				UpdateAssetLocalInfo(			//Any one pointer for data must be not nil.
							const IZPAssetLocalInfo *	inTarget,
							const PMString *			inAssetID,		//can be nil
							const bool *				inIsModified,	//can be nil
							const int32 *				inVersionNumber ) const;	//can be nil

#pragma mark -
//Command creation functions, sdk bosses.
	ICommand *			CreateCommand_DataLinkRefresh(
							const IDataLink *			inDL,
							IDataLink::StateType		inLinkNewState ) const;
#pragma mark -
//Command creation functions, partially setting the data with command.

	ICommand *			CreateCommand_SaveAssetData(
							const IPMUnknown *			inTarget ) const;

	ICommand*			CreateCommand_AddAssetLocalInfo( ) const;
	ICommand*			CreateCommand_DeleteAssetLocalInfo( ) const;

	ICommand*			CreateCommand_UpdateAssetLocalInfo(
							const IZPAssetLocalInfo *	inTarget ) const;

	ICommand *			CreateCommand_SaveZPPrefs() const;
	ICommand *			CreateCommand_SaveZPUserCredential(
							const UIDRef &				inTarget ) const;
protected:
private:
};


CREATE_PMINTERFACE(CZPCommandFacade, kZPCommandFacadeImpl)

CZPCommandFacade::CZPCommandFacade(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPCommandFacade::~CZPCommandFacade()
{

}

//----------------------------------------------------------------------------------------
// SetInCopyUser
//----------------------------------------------------------------------------------------
void
CZPCommandFacade::SetInCopyUser(
	const PMString &			inUserName)
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	do
	{
		InterfacePtr<ICommand> setUsernameCmd( CmdUtils::CreateCommand( kSetUserNameCmdBoss ) );
		ASSERT( setUsernameCmd );

		InterfacePtr<const IWorkspace> sessionWS( gSession->QueryWorkspace() );
		setUsernameCmd->SetItemList( UIDList( sessionWS ) );
		
		InterfacePtr<IStringData> stringDataPrefs( setUsernameCmd, UseDefaultIID());
		ASSERT( stringDataPrefs );
		
		stringDataPrefs->Set( inUserName );
		
		CmdUtils::ProcessCommand( setUsernameCmd );
	}while(kFalse);
}

//----------------------------------------------------------------------------------------
// SaveAssetData
//----------------------------------------------------------------------------------------
void
CZPCommandFacade::SaveAssetData(
	const IZPAssetData *		inSourceData,
	const IPMUnknown *			inTarget)
{
	InterfacePtr<ICommand> assetDataSaveCmd( this->CreateCommand_SaveAssetData( inTarget ) );
	if(!assetDataSaveCmd)
		return;
	
	InterfacePtr<IZPAssetData> cmdAssetData(assetDataSaveCmd, UseDefaultIID());
	ASSERT(cmdAssetData);

	cmdAssetData->Copy( inSourceData );

	CmdUtils::ProcessCommand(assetDataSaveCmd);
}

//----------------------------------------------------------------------------------------
// SaveAssetData
//----------------------------------------------------------------------------------------
void
CZPCommandFacade::SaveAssetData(
	const IStringData *			inAssetID,	//Asset to take data from
	const IPMUnknown *			inTarget) const
{
	InterfacePtr<ICommand> assetDataSaveCmd( this->CreateCommand_SaveAssetData( inTarget ) );
	if(!assetDataSaveCmd)
		return;
	
	InterfacePtr<IZPAssetData> cmdAssetData(assetDataSaveCmd, UseDefaultIID());
	ASSERT(cmdAssetData);

	Utils<IZPAssetUtils>()->CopyAssetDataFromAsset( inAssetID, cmdAssetData );

	CmdUtils::ProcessCommand(assetDataSaveCmd);
}

//----------------------------------------------------------------------------------------
// UpdateAssetLocalInfo
//	Any one pointer for data must be not nil.
//----------------------------------------------------------------------------------------
void
CZPCommandFacade::UpdateAssetLocalInfo(
	const IZPAssetLocalInfo *	inTarget,
	const PMString *			inAssetID,		//can be nil
	const bool *				inIsModified,	//can be nil
	const int32 *				inVersionNumber) const
{
	LogFunctionEnterExit;

	InterfacePtr<ICommand> updateAssetLocalInfoCmd( this->CreateCommand_UpdateAssetLocalInfo( inTarget ) );
	if(!updateAssetLocalInfoCmd)
		return;

	int cmdFlags = 0;
	if( inIsModified )
	{
		InterfacePtr<IBoolData> cmdBoolData( updateAssetLocalInfoCmd, UseDefaultIID() );
		ASSERT( cmdBoolData );
		if( *inIsModified )
			cmdBoolData->Set( kTrue );
		else
			cmdBoolData->Set( kFalse );
		cmdFlags = cmdFlags | eUpdateAssetLocalInfo_IsModified;
	}

	if( inVersionNumber )
	{
		InterfacePtr<IIntData> cmdIntData( updateAssetLocalInfoCmd, UseDefaultIID() );
		ASSERT( cmdIntData );
		cmdIntData->Set( *inVersionNumber );
		cmdFlags = cmdFlags | eUpdateAssetLocalInfo_VersionNumber;
	}

	if( inAssetID )
	{
		InterfacePtr<IStringData> cmdIDData( updateAssetLocalInfoCmd, IID_IZPDATAID );
		ASSERT( cmdIDData );
		cmdIDData->Set( *inAssetID );
		cmdFlags = cmdFlags | eUpdateAssetLocalInfo_AssetID;
	}

	InterfacePtr<IIntData> cmdFlagData( updateAssetLocalInfoCmd, IID_IZPMODIFYASSETLOCALINFOFLAGS );
	ASSERT( cmdFlagData );
	cmdFlagData->Set( cmdFlags );

	CmdUtils::ProcessCommand( updateAssetLocalInfoCmd );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// CreateCommand_DataLinkRefresh
//----------------------------------------------------------------------------------------
ICommand *
CZPCommandFacade::CreateCommand_DataLinkRefresh(
	const IDataLink *			inDL,
	IDataLink::StateType		inLinkNewState) const
{
#if defined(InDnCS5) || defined(InDnCS5_5)	//TODO: CS5 Porting
	return nil;
#else
	ASSERT( inDL );
	if( !inDL )
		return nil;
	InterfacePtr<ICommand> refreshLinkCmd(CmdUtils::CreateCommand(kRefreshCmdBoss));
	InterfacePtr<IRefreshCmdData> refreshLinkCmdData( refreshLinkCmd, UseDefaultIID());
	refreshLinkCmdData->Set(GetDataBase(inDL), ::GetUID(inDL), inLinkNewState);
	return refreshLinkCmd.forget();
#endif
}

#pragma mark -

//----------------------------------------------------------------------------------------
// CreateCommand_SaveAssetData
//----------------------------------------------------------------------------------------
ICommand *
CZPCommandFacade::CreateCommand_SaveAssetData(
	const IPMUnknown *			inTarget) const
{
	InterfacePtr<ICommand> assetDataSaveCmd(CmdUtils::CreateCommand(kZPSaveAssetDataCmdBoss));
	if(!assetDataSaveCmd)
		return assetDataSaveCmd;
	
	UIDList itemList(::GetUIDRef(inTarget));
	assetDataSaveCmd->SetItemList( itemList );

	return assetDataSaveCmd.forget();
}

//----------------------------------------------------------------------------------------
// CreateCommand_AddAssetLocalInfo
//----------------------------------------------------------------------------------------
ICommand*
CZPCommandFacade::CreateCommand_AddAssetLocalInfo() const
{
	
	InterfacePtr<ICommand> modifyPersistUIDListCmd(CmdUtils::CreateCommand(kZPModifyPersistUIDListCmdBoss));
	if( !modifyPersistUIDListCmd )
		return modifyPersistUIDListCmd;

	InterfacePtr<IBoolData> cmdBoolData( modifyPersistUIDListCmd, UseDefaultIID() );
	ASSERT( cmdBoolData );
	cmdBoolData->Set( kFalse );

	InterfacePtr<IInterfaceIDData> cmdInterfaceIDData( modifyPersistUIDListCmd, UseDefaultIID() );
	ASSERT( cmdInterfaceIDData );
	cmdInterfaceIDData->Set( IID_IZPASSETLOCALINFOUIDLIST );

	InterfacePtr<IClassIDData> cmdClassIDData( modifyPersistUIDListCmd, UseDefaultIID() );
	ASSERT( cmdClassIDData );
	cmdClassIDData->Set( kZPAssetLocalInfoBoss );

	return modifyPersistUIDListCmd.forget();
}

//----------------------------------------------------------------------------------------
// CreateCommand_DeleteAssetLocalInfo
//----------------------------------------------------------------------------------------
ICommand*
CZPCommandFacade::CreateCommand_DeleteAssetLocalInfo() const
{
	InterfacePtr<ICommand> modifyPersistUIDListCmd(CmdUtils::CreateCommand(kZPModifyPersistUIDListCmdBoss));
	if( !modifyPersistUIDListCmd )
		return modifyPersistUIDListCmd;

	InterfacePtr<IBoolData> cmdBoolData( modifyPersistUIDListCmd, UseDefaultIID() );
	ASSERT( cmdBoolData );
	cmdBoolData->Set( kTrue );

	InterfacePtr<IInterfaceIDData> cmdInterfaceIDData( modifyPersistUIDListCmd, UseDefaultIID() );
	ASSERT( cmdInterfaceIDData );
	cmdInterfaceIDData->Set( IID_IZPASSETLOCALINFOUIDLIST );

	return modifyPersistUIDListCmd.forget();
}

//----------------------------------------------------------------------------------------
// CreateCommand_UpdateAssetLocalInfo
//----------------------------------------------------------------------------------------
ICommand*
CZPCommandFacade::CreateCommand_UpdateAssetLocalInfo(
	const IZPAssetLocalInfo *	inTarget) const
{
	InterfacePtr<ICommand> updateAssetLocalInfoCmd(CmdUtils::CreateCommand(kZPModifyAssetLocalInfoCmdBoss));
	if(!updateAssetLocalInfoCmd)
		return updateAssetLocalInfoCmd;
	
	UIDList itemList(::GetUIDRef(inTarget));
	updateAssetLocalInfoCmd->SetItemList( itemList );

	return updateAssetLocalInfoCmd.forget();
}

//----------------------------------------------------------------------------------------
// CreateCommand_SaveZPPrefs
//----------------------------------------------------------------------------------------
ICommand *
CZPCommandFacade::CreateCommand_SaveZPPrefs()const
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<ICommand> saveZPPrefsCmd(CmdUtils::CreateCommand(kZPSavePrefsCmdBoss));
	if(!saveZPPrefsCmd)
		return nil;

	InterfacePtr<const IWorkspace> sessionWS( gSession->QueryWorkspace() );
	saveZPPrefsCmd->SetItemList( UIDList( sessionWS ) );

	return saveZPPrefsCmd.forget();
}

//----------------------------------------------------------------------------------------
// CreateCommand_SaveZPUserCredential
//----------------------------------------------------------------------------------------
ICommand *
CZPCommandFacade::CreateCommand_SaveZPUserCredential(
	const UIDRef &				inTarget )const
{
	InterfacePtr<ICommand> userCredentialSaveCmd(CmdUtils::CreateCommand(kZPSaveUserCredentialCmdBoss));
	if(!userCredentialSaveCmd)
		return nil;
	
	userCredentialSaveCmd->SetItemList( UIDList( inTarget ) );

	return userCredentialSaveCmd.forget();
}


