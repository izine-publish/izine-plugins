//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Data/CZMFileInfo.cpp $
//	$Revision: 2706 $
//	$Date: 2011-04-06 14:33:48 +0200 (Wed, 06 Apr 2011) $
//	$Author: aman.alam $
//	
//	Creator: Aman Alam
//	Created: 6-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes

//OS / ThirdParty includes

#include "iZMediaID.h"

//IZP Interfaces

//IZP General includes
#include "IZMFileInfo.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZMFileInfo>
class CZMFileInfo : public inheritClass
{
public:
						CZMFileInfo(IPMUnknown * boss);
	virtual				~CZMFileInfo();

	uint32				GetLastUpdate() const;
	void				SetLastUpdate(
							uint32									inLastUpdateDate);

protected:
private:
	uint32					mLastUpdateDate;
};


CREATE_PMINTERFACE(CZMFileInfo, kZMFileInfoImpl)

CZMFileInfo::CZMFileInfo(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZMFileInfo::~CZMFileInfo()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetLastUpdate
//----------------------------------------------------------------------------------------
uint32
CZMFileInfo::GetLastUpdate()const
{
	return mLastUpdateDate;
}

//----------------------------------------------------------------------------------------
// SetLastUpdate
//----------------------------------------------------------------------------------------
void
CZMFileInfo::SetLastUpdate(
	uint32									inLastUpdateDate)
{
	mLastUpdateDate = inLastUpdateDate;
}
