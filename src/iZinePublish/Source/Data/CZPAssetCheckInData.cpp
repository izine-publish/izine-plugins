//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Data/CZPAssetCheckInData.cpp $
//	$Revision: 3117 $
//	$Date: 2011-06-01 09:10:07 +0200 (Wed, 01 Jun 2011) $
//	$Author: rajkumar.sehrawat $
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

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPAssetCheckInData.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZPAssetCheckInData>
class CZPAssetCheckInData : public inheritClass
{
public:
						CZPAssetCheckInData(IPMUnknown * boss);
	virtual				~CZPAssetCheckInData();

	const PMString &	GetAssetName() const;
	void				SetAssetName(
							const PMString &			inAssetName );
	const PMString &	GetDescription() const;
	void				SetDescription(
							const PMString &			inDescription );
	const PMString &	GetStatusID() const;
	void				SetStatusID(
							const PMString &			inStatusID );
	const PMString &	GetTypeID() const;
	void				SetTypeID(
							const PMString &			inTypeID );

protected:
private:
	PMString	mAssetName;
	PMString	mDescription;
	PMString	mStatusID;
	PMString	mTypeID;		//Not being used.
};


CREATE_PMINTERFACE(CZPAssetCheckInData, kZPAssetCheckInDataImpl)

CZPAssetCheckInData::CZPAssetCheckInData(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAssetCheckInData::~CZPAssetCheckInData()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetAssetName
//----------------------------------------------------------------------------------------
const PMString &
CZPAssetCheckInData::GetAssetName()const
{
	return mAssetName;
}

//----------------------------------------------------------------------------------------
// SetAssetName
//----------------------------------------------------------------------------------------
void
CZPAssetCheckInData::SetAssetName(
	const PMString &			inAssetName)
{
	mAssetName = inAssetName;
}

//----------------------------------------------------------------------------------------
// GetDescription
//----------------------------------------------------------------------------------------
const PMString &
CZPAssetCheckInData::GetDescription()const
{
	return mDescription;
}

//----------------------------------------------------------------------------------------
// SetDescription
//----------------------------------------------------------------------------------------
void
CZPAssetCheckInData::SetDescription(
	const PMString &			inDescription)
{
	mDescription = inDescription;
}

//----------------------------------------------------------------------------------------
// GetStatusID
//----------------------------------------------------------------------------------------
const PMString &
CZPAssetCheckInData::GetStatusID()const
{
	return mStatusID;
}

//----------------------------------------------------------------------------------------
// SetStatusID
//----------------------------------------------------------------------------------------
void
CZPAssetCheckInData::SetStatusID(
	const PMString &			inStatusID)
{
	mStatusID = inStatusID;
}

//----------------------------------------------------------------------------------------
// GetTypeID
//----------------------------------------------------------------------------------------
const PMString &
CZPAssetCheckInData::GetTypeID()const
{
	return mTypeID;
}

//----------------------------------------------------------------------------------------
// SetTypeID
//----------------------------------------------------------------------------------------
void
CZPAssetCheckInData::SetTypeID(
	const PMString &			inTypeID)
{
	mTypeID = inTypeID;
}
