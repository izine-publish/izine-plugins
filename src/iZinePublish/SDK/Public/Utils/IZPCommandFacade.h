//----------------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------------
#ifndef _h_IZPCommandFacade_
#define _h_IZPCommandFacade_
#pragma once

#include "IDataLink.h"
class ICommand;
class IStringData;

class IZPAssetData;
class IZPAssetLocalInfo;

class IZPCommandFacade : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPCOMMANDFACADE };
	
	virtual void		SetInCopyUser(
							const PMString &			inUserName ) = 0;

	virtual void		SaveAssetData(
							const IZPAssetData *		inSourceData,
							const IPMUnknown *			inTarget ) = 0;

	virtual void		SaveAssetData(					//Not being used.
							const IStringData *			inAssetID,	//Asset to take data from
							const IPMUnknown *			inTarget )const = 0;

	virtual void		UpdateAssetLocalInfo(			//Any one pointer for data must be not nil.
							const IZPAssetLocalInfo *	inTarget,
							const PMString *			inAssetID,		//can be nil
							const bool *				inIsModified,	//can be nil
							const int32 *				inVersionNumber ) const = 0;	//can be nil
#pragma mark -
//Command creation functions, sdk bosses.
	virtual ICommand *	CreateCommand_DataLinkRefresh(
							const IDataLink *			inDL,
							IDataLink::StateType		inLinkNewState) const = 0;

#pragma mark -
//Command creation functions, partially setting the data with command.
	//Caller must release the command

	virtual ICommand *	CreateCommand_SaveAssetData(
							const IPMUnknown *			inTarget ) const = 0;

	virtual ICommand*	CreateCommand_AddAssetLocalInfo( ) const = 0;
	virtual ICommand*	CreateCommand_DeleteAssetLocalInfo( ) const = 0;

	virtual ICommand*	CreateCommand_UpdateAssetLocalInfo(
							const IZPAssetLocalInfo *	inTarget ) const = 0;

	virtual ICommand *	CreateCommand_SaveZPPrefs() const = 0;
	virtual ICommand *	CreateCommand_SaveZPUserCredential(
							const UIDRef &				inTarget ) const = 0;
};



#endif //_h_IZPCommandFacade_
