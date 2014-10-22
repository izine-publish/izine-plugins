//========================================================================================
//	
//	$HeadURL: https://my.redfive.biz/svn/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/SelectionSuite/CZPICExportSuiteASB.cpp $
//	$Revision: 2926 $
//	$Date: 2011-05-04 12:17:15 +0530 (Wed, 04 May 2011) $
//	$Author: mindfire $
//	
//	Creator: mindfire
//	Created: 7-5-2012
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#include "VCPlugInHeaders.h"

//Project includes:
#include "IZPID.h"
#include "IZPICExportSuite.h"

// API includes:
#include "SelectionASBTemplates.tpp" 

class CZPICExportSuiteASB : public CPMUnknown<IZPICExportSuite>
{
public:
						CZPICExportSuiteASB(IPMUnknown * boss);
	virtual				~CZPICExportSuiteASB();	

	/* get all selected page item uid list ASB  */
	virtual void		GetAllSelectedItems(UIDList& uidList);	
};

CREATE_PMINTERFACE(CZPICExportSuiteASB, kZPICExportSuiteASBImpl)

CZPICExportSuiteASB::CZPICExportSuiteASB(IPMUnknown * boss)
: CPMUnknown<IZPICExportSuite>(boss)
{

}

CZPICExportSuiteASB::~CZPICExportSuiteASB(){}

/* get all selected page item uid list ASB  */
void CZPICExportSuiteASB::GetAllSelectedItems(UIDList& uidList)
{
	return (CallEach(make_functor(&IZPICExportSuite::GetAllSelectedItems, uidList), this));
}
