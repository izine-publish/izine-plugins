//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Service/CZMAdFileData.cpp $
//	$Revision: 2681 $
//	$Date: 2011-04-05 11:35:14 +0200 (Tue, 05 Apr 2011) $
//	$Author: aman.alam $
//	
//	Creator: Aman Alam
//	Created: 5-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes

//OS / ThirdParty includes

#include "IZMediaID.h"

//IZP Interfaces

//IZP General includes
#include "IZMAdFileData.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_ALL
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZMAdFileData>
class CZMAdFileData : public inheritClass
{
public:
						CZMAdFileData(IPMUnknown * boss);
	virtual				~CZMAdFileData();

	const ZMstAdSList &		GetAdsList() const;
		ZMstAdSList &		GetAdsList();
		
protected:
private:
		ZMstAdSList			mAdsList;
};


CREATE_PMINTERFACE(CZMAdFileData, kZMAdFileDataImpl)

CZMAdFileData::CZMAdFileData(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZMAdFileData::~CZMAdFileData()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetAdsList
//----------------------------------------------------------------------------------------
const ZMstAdSList &
CZMAdFileData::GetAdsList()const
{
	return mAdsList;
}

//----------------------------------------------------------------------------------------
// GetAdsList
//----------------------------------------------------------------------------------------
ZMstAdSList &
CZMAdFileData::GetAdsList()
{
	return mAdsList;
}
