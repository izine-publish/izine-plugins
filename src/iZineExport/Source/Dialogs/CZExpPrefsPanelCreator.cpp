//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/Dialogs/CZExpPrefsPanelCreator.cpp $
//	$Revision: 3322 $
//	$Date: 2011-06-21 10:38:39 +0200 (Tue, 21 Jun 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Aman Alam
//	Created: 6-6-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes
#include "CPanelCreator.h"

//OS / ThirdParty includes

#include "IZExpID.h"

//IZP Interfaces

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_Dialogs
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPanelCreator
class CZExpPrefsPanelCreator : public inheritClass
{
public:
						CZExpPrefsPanelCreator(IPMUnknown * boss);
	virtual				~CZExpPrefsPanelCreator();

	RsrcID				GetPanelRsrcID() const;
protected:
private:
};


CREATE_PMINTERFACE(CZExpPrefsPanelCreator, kZExpPrefsPanelCreatorImpl)

CZExpPrefsPanelCreator::CZExpPrefsPanelCreator(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZExpPrefsPanelCreator::~CZExpPrefsPanelCreator()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetPanelRsrcID
//----------------------------------------------------------------------------------------
RsrcID
CZExpPrefsPanelCreator::GetPanelRsrcID()const
{
	LogFunctionEnterExit;
#ifdef RVExport
	return kSDKDefIDListPairResourceID;
#else
	return 0;
#endif
}
