//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/Utils/CZExpTextFrameUtils.cpp $
//	$Revision: 2518 $
//	$Date: 2010-11-15 13:25:17 +0100 (Mon, 15 Nov 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 15-11-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes

//OS / ThirdParty includes

#include "IZExpID.h"

//IZP Interfaces
#include "IZExpTextFrameUtils.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZExpTextFrameUtils>
class CZExpTextFrameUtils : public inheritClass
{
public:
						CZExpTextFrameUtils(IPMUnknown * boss);
	virtual				~CZExpTextFrameUtils();
protected:
private:
};


CREATE_PMINTERFACE(CZExpTextFrameUtils, kZExpTextFrameUtilsImpl)

CZExpTextFrameUtils::CZExpTextFrameUtils(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZExpTextFrameUtils::~CZExpTextFrameUtils()
{

}

#pragma mark -
