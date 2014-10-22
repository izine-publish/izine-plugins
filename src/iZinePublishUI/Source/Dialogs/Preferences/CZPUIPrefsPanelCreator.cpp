//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Dialogs/Preferences/CZPUIPrefsPanelCreator.cpp $
//	$Revision: 2087 $
//	$Date: 2010-09-13 13:59:56 +0200 (Mon, 13 Sep 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 13-9-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes
#include "CPanelCreator.h"

//OS / ThirdParty includes

#include "IZPUIID.h"

//IZP Interfaces

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_Dialogs
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPanelCreator
class CZPUIPrefsPanelCreator : public inheritClass
{
public:
						CZPUIPrefsPanelCreator(IPMUnknown * boss);
	virtual				~CZPUIPrefsPanelCreator();

	RsrcID				GetPanelRsrcID() const;
protected:
private:
};


CREATE_PMINTERFACE(CZPUIPrefsPanelCreator, kZPUIPrefsPanelCreatorImpl)

CZPUIPrefsPanelCreator::CZPUIPrefsPanelCreator(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPUIPrefsPanelCreator::~CZPUIPrefsPanelCreator()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetPanelRsrcID
//----------------------------------------------------------------------------------------
RsrcID
CZPUIPrefsPanelCreator::GetPanelRsrcID()const
{
	LogFunctionEnterExit;
	
	return kSDKDefIDListPairResourceID;
}
