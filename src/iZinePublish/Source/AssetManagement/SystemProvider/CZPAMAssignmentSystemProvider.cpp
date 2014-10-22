//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/SystemProvider/CZPAMAssignmentSystemProvider.cpp $
//	$Revision: 1571 $
//	$Date: 2010-06-29 07:41:04 +0200 (Tue, 29 Jun 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 28-6-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IAMServiceProvider.h"
#include "IAMSystemProvider.h"
#include "IAMUIServiceProvider.h"

//SDK General includes

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces

//IZP General includes
#include "AZPAMSystemProvider.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AMSysP
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass AZPAMSystemProvider
class CZPAMAssignmentSystemProvider : public inheritClass
{
public:
						CZPAMAssignmentSystemProvider(IPMUnknown * boss);
	virtual				~CZPAMAssignmentSystemProvider();

protected:
	const char *		GetProviderName() const;
private:
	
};


CREATE_PMINTERFACE(CZPAMAssignmentSystemProvider, kZPAMAssignmentSystemProviderImpl)

CZPAMAssignmentSystemProvider::CZPAMAssignmentSystemProvider(IPMUnknown * boss)
: inheritClass(boss)
{
	IZPLog_ObjC( thisFileLA );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMAssignmentSystemProvider::~CZPAMAssignmentSystemProvider()
{
	IZPLog_ObjD( thisFileLA );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetProviderName
//----------------------------------------------------------------------------------------
const char *
CZPAMAssignmentSystemProvider::GetProviderName()const
{
	return "Assignment system provider";
}
