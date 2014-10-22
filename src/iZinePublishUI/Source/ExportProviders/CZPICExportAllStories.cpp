//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/ExportProviders/CZPICExportAllStories.cpp $
//	$Revision: 2014 $
//	$Date: 2010-09-01 16:56:46 +0200 (Wed, 01 Sep 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 1-9-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes
#include "InCopyWorkflowID.h"

//OS / ThirdParty includes

#include "IZPUIID.h"
#include "IZPID.h"

//IZP Interfaces

//IZP General includes
#include "CZPInCopyStoryExportSuite.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CZPInCopyStoryExportSuite
class CZPICExportAllStories : public inheritClass
{
public:
						CZPICExportAllStories(IPMUnknown * boss);
	virtual				~CZPICExportAllStories();

	PMIID				GetDelegateInterfaceID() const;
protected:
private:
};


CREATE_PMINTERFACE(CZPICExportAllStories, kZPICExportAllStoriesImpl)

CZPICExportAllStories::CZPICExportAllStories(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPICExportAllStories::~CZPICExportAllStories()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetDelegateInterfaceID
//----------------------------------------------------------------------------------------
PMIID
CZPICExportAllStories::GetDelegateInterfaceID()const
{
	return IID_IINCOPYEXPORTALLSTORIESSUITE;
}
