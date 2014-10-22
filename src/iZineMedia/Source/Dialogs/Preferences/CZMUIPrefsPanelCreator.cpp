//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Dialogs/Preferences/CZMUIPrefsPanelCreator.cpp $
//	$Revision: 2739 $
//	$Date: 2011-04-07 13:10:59 +0200 (Thu, 07 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 13-9-2010
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes
#include "CPanelCreator.h"

//OS / ThirdParty includes

#include "iZMediaID.h"

//IZP Interfaces

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_Dialogs
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPanelCreator
class CZMUIPrefsPanelCreator : public inheritClass
{
public:
						CZMUIPrefsPanelCreator(IPMUnknown * boss);
	virtual				~CZMUIPrefsPanelCreator();

	RsrcID				GetPanelRsrcID() const;
protected:
private:
};


CREATE_PMINTERFACE(CZMUIPrefsPanelCreator, kZMUIPrefsPanelCreatorImpl)

CZMUIPrefsPanelCreator::CZMUIPrefsPanelCreator(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZMUIPrefsPanelCreator::~CZMUIPrefsPanelCreator()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetPanelRsrcID
//----------------------------------------------------------------------------------------
RsrcID
CZMUIPrefsPanelCreator::GetPanelRsrcID()const
{
	LogFunctionEnterExit;
	
	return kSDKDefIDListPairResourceID;
}
