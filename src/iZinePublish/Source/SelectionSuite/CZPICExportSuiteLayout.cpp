//========================================================================================
//	
//	$HeadURL: https://my.redfive.biz/svn/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/SelectionSuite/CZPICExportSuiteLayout.cpp $
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

#include "VCPlugInHeaders.h"								// For MSVC

// Interface headers
#include "ILayoutTarget.h"
#include "IZPID.h"
#include "IZPICExportSuite.h"										// Superclass declaration


class CZPICExportSuiteLayout : public CPMUnknown<IZPICExportSuite>
{
public:

	CZPICExportSuiteLayout(IPMUnknown* iBoss);	

	virtual					~CZPICExportSuiteLayout(void);

public:	
	/* get all selected page item uid list 
	@OUT: uidlist of page items selected
	*/
	virtual void			GetAllSelectedItems(UIDList&);
};

/* CREATE_PMINTERFACE
 Binds the C++ implementation class onto its ImplementationID making the C++ code callable by the application.
*/
CREATE_PMINTERFACE(CZPICExportSuiteLayout, kZPICExportSuiteLayoutImpl)

CZPICExportSuiteLayout::CZPICExportSuiteLayout(IPMUnknown* iBoss) :
	CPMUnknown<IZPICExportSuite>(iBoss)
{
}

CZPICExportSuiteLayout::~CZPICExportSuiteLayout(void)
{
}

/* get all selected page item uid list */
void CZPICExportSuiteLayout::GetAllSelectedItems(UIDList& uidList)
{
	InterfacePtr<ILayoutTarget>		iLayoutTarget(this, UseDefaultIID());
	UIDList							selectedItems(iLayoutTarget->GetUIDList(kStripStandoffs));

	uidList = selectedItems;	
}

