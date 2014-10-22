//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Data/ZPData/CZPAssetLockInfo.cpp $
//	$Revision: 2134 $
//	$Date: 2010-09-20 16:44:34 +0200 (Mon, 20 Sep 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 15-6-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "ISubject.h"

//SDK General includes
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPAssetLockInfo.h"
#include "IZPAMHelper.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZPAssetLockInfo>
class CZPAssetLockInfo : public inheritClass
{
public:
						CZPAssetLockInfo(IPMUnknown * boss);
	virtual				~CZPAssetLockInfo();

	const PMString &	GetLockID() const;
	void				SetLockID(
							const PMString &			inLockID );
	const PMString &	GetDocumentID() const;
	void				SetDocumentID(
							const PMString &			inDocumentID );
	const PMString &	GetDocumentName() const;
	void				SetDocumentName(
							const PMString &			inDocumentName );
	const PMString &	GetUserID() const;
	void				SetUserID(
							const PMString &			inUserID );
	const PMString &	GetUserName() const;
	void				SetUserName(
							const PMString &			inUserName );
	const PMString &	GetAppName() const;
	void				SetAppName(
							const PMString &			inAppName );

	void				BroadcastMessage();

	//Cached values
	IAMLockable::LockState GetLockState() const;

protected:
private:
	PMString	mLockID;
	PMString	mDocumentID;
	PMString	mDocumentName;	//Should we store it here? When AssetID will ready be in cache with AssetMgr.
	PMString	mUserID;
	PMString	mUserName;	//Someday: create users mgr class, to save memory used by duplicate objects having same value.
	PMString	mAppName;	//Someday: use shared string

	mutable IAMLockable::LockState	mLockState;
};


CREATE_PMINTERFACE(CZPAssetLockInfo, kZPAssetLockInfoImpl)

CZPAssetLockInfo::CZPAssetLockInfo(IPMUnknown * boss)
: inheritClass(boss)
, mLockState( IAMLockable::enUndefined )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAssetLockInfo::~CZPAssetLockInfo()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetLockID
//----------------------------------------------------------------------------------------
const PMString &
CZPAssetLockInfo::GetLockID()const
{
	return mLockID;
}

//----------------------------------------------------------------------------------------
// SetLockID
//----------------------------------------------------------------------------------------
void
CZPAssetLockInfo::SetLockID(
	const PMString &			inLockID)
{
	mLockID = inLockID;
	mLockState = IAMLockable::enUndefined;
}

//----------------------------------------------------------------------------------------
// GetDocumentID
//----------------------------------------------------------------------------------------
const PMString &
CZPAssetLockInfo::GetDocumentID()const
{
	return mDocumentID;
}

//----------------------------------------------------------------------------------------
// SetDocumentID
//----------------------------------------------------------------------------------------
void
CZPAssetLockInfo::SetDocumentID(
	const PMString &			inDocumentID)
{
	mDocumentID = inDocumentID;
}

//----------------------------------------------------------------------------------------
// GetDocumentName
//----------------------------------------------------------------------------------------
const PMString &
CZPAssetLockInfo::GetDocumentName()const
{
	return mDocumentName;
}

//----------------------------------------------------------------------------------------
// SetDocumentName
//----------------------------------------------------------------------------------------
void
CZPAssetLockInfo::SetDocumentName(
	const PMString &			inDocumentName)
{
	mDocumentName = inDocumentName;
}

//----------------------------------------------------------------------------------------
// GetUserID
//----------------------------------------------------------------------------------------
const PMString &
CZPAssetLockInfo::GetUserID()const
{
	return mUserID;
}

//----------------------------------------------------------------------------------------
// SetUserID
//----------------------------------------------------------------------------------------
void
CZPAssetLockInfo::SetUserID(
	const PMString &			inUserID)
{
	mUserID = inUserID;
}

//----------------------------------------------------------------------------------------
// GetUserName
//----------------------------------------------------------------------------------------
const PMString &
CZPAssetLockInfo::GetUserName()const
{
	return mUserName;
}

//----------------------------------------------------------------------------------------
// SetUserName
//----------------------------------------------------------------------------------------
void
CZPAssetLockInfo::SetUserName(
	const PMString &			inUserName)
{
	mUserName = inUserName;
}

//----------------------------------------------------------------------------------------
// GetAppName
//----------------------------------------------------------------------------------------
const PMString &
CZPAssetLockInfo::GetAppName()const
{
	return mAppName;
}

//----------------------------------------------------------------------------------------
// SetAppName
//----------------------------------------------------------------------------------------
void
CZPAssetLockInfo::SetAppName(
	const PMString &			inAppName)
{
	mAppName = inAppName;
}

//----------------------------------------------------------------------------------------
// LockState
//----------------------------------------------------------------------------------------

IAMLockable::LockState
CZPAssetLockInfo::GetLockState()const
{
	do
	{
		if( mLockState != IAMLockable::enUndefined )
			break;

		mLockState = Utils<IZPAMHelper>()->CalculateAssetLockState( this );

	}while(kFalse);

	return mLockState;
}

//----------------------------------------------------------------------------------------
// BroadcastMessage
//----------------------------------------------------------------------------------------
void
CZPAssetLockInfo::BroadcastMessage()
{
	InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
	ASSERT(selfSubject);

	selfSubject->Change( kZPAssetLockInfoChangedMsg, IZPAssetLockInfo::kDefaultIID, nil );
}
