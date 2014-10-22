//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Data/CZPAssetData.cpp $
//	$Revision: 3393 $
//	$Date: 2011-06-27 15:17:49 +0200 (Mon, 27 Jun 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 1-6-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPAssetData.h"
#include "IZPCommandFacade.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZPAssetData>
class CZPAssetData : public inheritClass
{
public:
						CZPAssetData(IPMUnknown * boss);
	virtual				~CZPAssetData();

	void				Copy(
							const IZPAssetData *		inValuesFrom );

	const PMString &	GetAssetID() const;
	void				SetAssetID(
							const PMString &			inAssetID );

	const PMString &	GetTitleID() const;
	void				SetTitleID(
							const PMString &			inTitleID );

	const PMString &	GetEditionID() const;
	void				SetEditionID(
							const PMString &			inEditionID );

	void				Clear();

	void				ReadWrite(
							IPMStream*					stream,
							ImplementationID			implementation);
	virtual void		PreDirty(ImplementationID prop = kInvalidImpl, bool16 allowModification = kTrue)
						{ CPMUnknown<IZPAssetData>::PreDirty( prop, allowModification ); }
protected:
private:
	PMString	mAssetID;
	PMString	mTitleID;
	PMString	mEditionID;
};

class CZPAssetDataNonPersist : public CZPAssetData
{
public:
						CZPAssetDataNonPersist(IPMUnknown * boss);
	virtual				~CZPAssetDataNonPersist();
	void				PreDirty(ImplementationID prop = kInvalidImpl, bool16 allowModification = kTrue){};
};

CREATE_PMINTERFACE(CZPAssetDataNonPersist, kZPAssetDataImpl)
CREATE_PERSIST_PMINTERFACE(CZPAssetData, kZPAssetDataPersistImpl)

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZPAssetDataNonPersist::CZPAssetDataNonPersist(IPMUnknown * boss)
: CZPAssetData( boss )
{
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAssetDataNonPersist::~CZPAssetDataNonPersist()
{
}

#pragma mark -



CZPAssetData::CZPAssetData(IPMUnknown * boss)
: inheritClass(boss)
{
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAssetData::~CZPAssetData()
{
}

#pragma mark -
//----------------------------------------------------------------------------------------
// Copy
//----------------------------------------------------------------------------------------
void
CZPAssetData::Copy(
	const IZPAssetData *		inValuesFrom)
{
	if( inValuesFrom )
	{
		PreDirty();
		this->mAssetID = inValuesFrom->GetAssetID();
		this->mTitleID = inValuesFrom->GetTitleID();
		this->mEditionID = inValuesFrom->GetEditionID();
	}
	else
	{
		this->Clear();
	}
}

//----------------------------------------------------------------------------------------
// Clear
//----------------------------------------------------------------------------------------
void
CZPAssetData::Clear()
{
	PreDirty();
	this->mAssetID = kNullString;
	this->mTitleID = kNullString;
	this->mEditionID = kNullString;
}

//----------------------------------------------------------------------------------------
// GetAssetID
//----------------------------------------------------------------------------------------
const PMString &
CZPAssetData::GetAssetID()const
{
	return mAssetID;
}

//----------------------------------------------------------------------------------------
// SetAssetID
//----------------------------------------------------------------------------------------
void
CZPAssetData::SetAssetID(
	const PMString &			inAssetID)
{
	PreDirty();
	mAssetID = inAssetID;
}


//----------------------------------------------------------------------------------------
// GetTitleID
//----------------------------------------------------------------------------------------
const PMString &
CZPAssetData::GetTitleID()const
{
	return mTitleID;
}

//----------------------------------------------------------------------------------------
// SetTitleID
//----------------------------------------------------------------------------------------
void
CZPAssetData::SetTitleID(
	const PMString &			inTitleID)
{
	PreDirty();
	mTitleID = inTitleID;
}

//----------------------------------------------------------------------------------------
// GetEditionID
//----------------------------------------------------------------------------------------
const PMString &
CZPAssetData::GetEditionID()const
{
	return mEditionID;
}

//----------------------------------------------------------------------------------------
// SetEditionID
//----------------------------------------------------------------------------------------
void
CZPAssetData::SetEditionID(
	const PMString &			inEditionID)
{
	PreDirty();
	mEditionID = inEditionID;
}

//----------------------------------------------------------------------------------------
// ReadWrite
//----------------------------------------------------------------------------------------
void
CZPAssetData::ReadWrite(
	IPMStream*					stream,
	ImplementationID			implementation)
{
	mAssetID.ReadWrite( stream );
	mTitleID.ReadWrite( stream );
	mEditionID.ReadWrite( stream );
}


#pragma mark -
//----------------------------------------------------------------------------------------
// SaveAssetData
//----------------------------------------------------------------------------------------
ErrorCode
ZPCommandHelper::SaveAssetData(
	const IPMUnknown *			inTarget,
	const PMString &			inAssetID,
	const PMString &			inTitleID,
	const PMString &			inEditionID)
{

	InterfacePtr<ICommand> assetDataSaveCmd( Utils<IZPCommandFacade>()->CreateCommand_SaveAssetData( inTarget ) );
	if(!assetDataSaveCmd)
		return kFailure;
	
	InterfacePtr<IZPAssetData> cmdAssetData(assetDataSaveCmd, UseDefaultIID());
	ASSERT(cmdAssetData);

	cmdAssetData->SetAssetID( inAssetID );
	cmdAssetData->SetTitleID( inTitleID );
	cmdAssetData->SetEditionID( inEditionID );

	return CmdUtils::ProcessCommand(assetDataSaveCmd);
}
