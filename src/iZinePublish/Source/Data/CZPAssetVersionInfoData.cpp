//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Data/CZPAssetVersionInfoData.cpp $
//	$Revision: 1376 $
//	$Date: 2010-06-09 13:47:43 +0200 (Wed, 09 Jun 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 9-6-2010
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
#include "IZPAssetVersionInfoData.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZPAssetVersionInfoData>
class CZPAssetVersionInfoData : public inheritClass
{
public:
						CZPAssetVersionInfoData(IPMUnknown * boss);
	virtual				~CZPAssetVersionInfoData();

	const PMString &	GetVersionID() const;
	void				SetVersionID(
							const PMString &			inVersionID );
	const int			GetVersionNumber() const;
	void				SetVersionNumber(
							const int					inVersionNumber );
	const PMString &	GetVersionUserID() const;
	void				SetVersionUserID(
							const PMString &			inVersionUserID );
	const PMString &	GetVersionUserName() const;
	void				SetVersionUserName(
							const PMString &			inVersionUserName );
protected:
private:
	int				mVersionNumber;
	PMString		mVersionID;
	PMString		mVersionUserID;
	PMString		mVersionUserName;
};


CREATE_PMINTERFACE(CZPAssetVersionInfoData, kZPAssetVersionInfoDataImpl)

CZPAssetVersionInfoData::CZPAssetVersionInfoData(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAssetVersionInfoData::~CZPAssetVersionInfoData()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetVersionID
//----------------------------------------------------------------------------------------
const PMString &
CZPAssetVersionInfoData::GetVersionID()const
{
	return mVersionID;
}

//----------------------------------------------------------------------------------------
// SetVersionID
//----------------------------------------------------------------------------------------
void
CZPAssetVersionInfoData::SetVersionID(
	const PMString &			inVersionID)
{
	mVersionID = inVersionID;
}

//----------------------------------------------------------------------------------------
// GetVersionNumber
//----------------------------------------------------------------------------------------
const int
CZPAssetVersionInfoData::GetVersionNumber()const
{
	return mVersionNumber;
}

//----------------------------------------------------------------------------------------
// SetVersionNumber
//----------------------------------------------------------------------------------------
void
CZPAssetVersionInfoData::SetVersionNumber(
	const int					inVersionNumber)
{
	mVersionNumber = inVersionNumber;
}

//----------------------------------------------------------------------------------------
// GetVersionUserID
//----------------------------------------------------------------------------------------
const PMString &
CZPAssetVersionInfoData::GetVersionUserID()const
{
	return mVersionUserID;
}

//----------------------------------------------------------------------------------------
// SetVersionUserID
//----------------------------------------------------------------------------------------
void
CZPAssetVersionInfoData::SetVersionUserID(
	const PMString &			inVersionUserID)
{
	mVersionUserID = inVersionUserID;
}

//----------------------------------------------------------------------------------------
// GetVersionUserName
//----------------------------------------------------------------------------------------
const PMString &
CZPAssetVersionInfoData::GetVersionUserName()const
{
	return mVersionUserName;
}

//----------------------------------------------------------------------------------------
// SetVersionUserName
//----------------------------------------------------------------------------------------
void
CZPAssetVersionInfoData::SetVersionUserName(
	const PMString &			inVersionUserName)
{
	mVersionUserName = inVersionUserName;
}
