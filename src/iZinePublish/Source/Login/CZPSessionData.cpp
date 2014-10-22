//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Login/CZPSessionData.cpp $
//	$Revision: 1364 $
//	$Date: 2010-06-08 09:50:35 +0200 (Tue, 08 Jun 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 29-3-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPSessionData.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZPSessionData>
class CZPSessionData : public inheritClass
{
public:
						CZPSessionData(IPMUnknown * boss);
	virtual				~CZPSessionData();

	const PMString &	GetUserID() const;
	void				SetUserID(
							const PMString &			inUserID );
	
	const PMString &	GetUserDisplayName() const;
	void				SetUserDisplayName(
							const PMString &			inUserName );

protected:
private:
	PMString		mUserID;
	PMString		mUserDisplayName;

};


CREATE_PMINTERFACE(CZPSessionData, kZPSessionDataImpl)

CZPSessionData::CZPSessionData(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPSessionData::~CZPSessionData()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetUserID
//----------------------------------------------------------------------------------------
const PMString &
CZPSessionData::GetUserID()const
{
	return mUserID;
}

//----------------------------------------------------------------------------------------
// SetUserID
//----------------------------------------------------------------------------------------
void
CZPSessionData::SetUserID(
	const PMString &			inUserID)
{
	mUserID = inUserID;
}

//----------------------------------------------------------------------------------------
// GetUserName
//----------------------------------------------------------------------------------------
const PMString &
CZPSessionData::GetUserDisplayName()const
{
	return mUserDisplayName;
}

//----------------------------------------------------------------------------------------
// SetUserDisplayName
//----------------------------------------------------------------------------------------
void
CZPSessionData::SetUserDisplayName(
	const PMString &			inUserName)
{
	mUserDisplayName = inUserName;
}



