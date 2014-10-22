//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/ExportProviders/CZPICExportSelectionStories.cpp $
//	$Revision: 2027 $
//	$Date: 2010-09-03 16:32:41 +0200 (Fri, 03 Sep 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 3-9-2010
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
class CZPICExportSelectionStories : public inheritClass
{
public:
						CZPICExportSelectionStories(IPMUnknown * boss);
	virtual				~CZPICExportSelectionStories();

	PMIID				GetDelegateInterfaceID() const;
protected:
private:
};


CREATE_PMINTERFACE(CZPICExportSelectionStories, kZPICExportSelectionStoriesImpl)

CZPICExportSelectionStories::CZPICExportSelectionStories(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPICExportSelectionStories::~CZPICExportSelectionStories()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetDelegateInterfaceID
//----------------------------------------------------------------------------------------
PMIID
CZPICExportSelectionStories::GetDelegateInterfaceID()const
{
	return IID_IINCOPYEXPORTSUITE;
}
