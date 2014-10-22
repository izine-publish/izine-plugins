//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Responders/CZPDocResponderServiceProvider.cpp $
//	$Revision: 3536 $
//	$Date: 2011-07-25 09:32:03 +0200 (Mon, 25 Jul 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 7-9-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "CServiceProvider.h"

//SDK General includes
#include "DocumentID.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_Responder
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CServiceProvider
class CZPDocResponderServiceProvider : public inheritClass
{
public:
						CZPDocResponderServiceProvider(IPMUnknown * boss);
	virtual				~CZPDocResponderServiceProvider();
	// See IK2ServiceProvider::GetName
	virtual void GetName(PMString* pName);

	// See IK2ServiceProvider::GetServiceID
	virtual ServiceID GetServiceID();

	// See IK2ServiceProvider::IsDefaultServiceProvider
	virtual bool16 IsDefaultServiceProvider();
	
	// See IK2ServiceProvider::GetInstantiationPolicy
	virtual InstancePerX GetInstantiationPolicy();

	// See IK2ServiceProvider::HasMultipleIDs
	virtual bool16 HasMultipleIDs() const;

	// See IK2ServiceProvider::GetServiceIDs
	virtual void GetServiceIDs(K2Vector<ServiceID>& serviceIDs);
protected:
private:
	K2Vector<ServiceID> fServices;
};


CREATE_PMINTERFACE(CZPDocResponderServiceProvider, kZPDocResponderServiceProviderImpl)

CZPDocResponderServiceProvider::CZPDocResponderServiceProvider(IPMUnknown * boss)
: inheritClass(boss)
{
// Add the serviceIDs we want the associated responder to handle.
	// (See DocumentID.h)
	fServices.clear();
	
	//	NewDoc
	//fServices.push_back(kBeforeNewDocSignalResponderService);
	//fServices.push_back(kDuringNewDocSignalResponderService);
	fServices.push_back(kAfterNewDocSignalResponderService);

	//	OpenDoc
	fServices.push_back(kBeforeOpenDocSignalResponderService);
	fServices.push_back(kDuringOpenDocSignalResponderService);
	fServices.push_back(kAfterOpenDocSignalResponderService);

	//	SaveDoc
	fServices.push_back(kBeforeSaveDocSignalResponderService);
	fServices.push_back(kAfterSaveDocSignalResponderService);

	//	SaveAsDoc
	//fServices.push_back(kBeforeSaveAsDocSignalResponderService);
	//fServices.push_back(kAfterSaveAsDocSignalResponderService);

	//	SaveACopyDoc
	//fServices.push_back(kBeforeSaveACopyDocSignalResponderService);
	fServices.push_back(kDuringSaveACopyDocSignalResponderService);
	//fServices.push_back(kAfterSaveACopyDocSignalResponderService);

	//	RevertDoc
	//fServices.push_back(kBeforeRevertDocSignalResponderService);
	fServices.push_back(kAfterRevertDocSignalResponderService);

	//	CloseDoc
	fServices.push_back(kBeforeCloseDocSignalResponderService);
	//fServices.push_back(kAfterCloseDocSignalResponderService);

	if (fServices.size()<=0)
	{
		ASSERT_FAIL("We must support at least 1 service ID");
		fServices.push_back(kInvalidService);
	}
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPDocResponderServiceProvider::~CZPDocResponderServiceProvider()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetName
//----------------------------------------------------------------------------------------
void
CZPDocResponderServiceProvider::GetName(
	PMString* pName)
{
	ASSERT_UNIMPLEMENTED();
}

//----------------------------------------------------------------------------------------
// GetServiceID
//----------------------------------------------------------------------------------------
ServiceID
CZPDocResponderServiceProvider::GetServiceID()
{
	return fServices.size() > 0 ? fServices[0] : kInvalidService;
}

//----------------------------------------------------------------------------------------
// IsDefaultServiceProvider
//----------------------------------------------------------------------------------------
bool16
CZPDocResponderServiceProvider::IsDefaultServiceProvider()
{
	return kFalse;
}

//----------------------------------------------------------------------------------------
// GetInstantiationPolicy
//----------------------------------------------------------------------------------------
IK2ServiceProvider::InstancePerX
CZPDocResponderServiceProvider::GetInstantiationPolicy()
{
	return IK2ServiceProvider::kInstancePerSession;
}

//----------------------------------------------------------------------------------------
// HasMultipleIDs
//----------------------------------------------------------------------------------------
bool16
CZPDocResponderServiceProvider::HasMultipleIDs()const
{
	return kTrue;
}

//----------------------------------------------------------------------------------------
// GetServiceIDs
//----------------------------------------------------------------------------------------
void
CZPDocResponderServiceProvider::GetServiceIDs(
	K2Vector<ServiceID>& serviceIDs)
{
	std::copy(this->fServices.begin(), this->fServices.end(), std::back_inserter(serviceIDs));
}