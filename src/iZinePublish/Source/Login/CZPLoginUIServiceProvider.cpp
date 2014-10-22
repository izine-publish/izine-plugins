//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Login/CZPLoginUIServiceProvider.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 14-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes
#include "CServiceProvider.h"

#include "IZPID.h"

//IZP Interfaces

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CServiceProvider
class CZPLoginUIServiceProvider : public inheritClass
{
public:
				CZPLoginUIServiceProvider(IPMUnknown * boss);
	virtual		~CZPLoginUIServiceProvider();

	/**
		GetName initializes the name of the service.
		@param pName Ptr to PMString to receive the name.
	*/
	virtual void GetName(PMString* pName);

	/**
		GetServiceID returns a single service ID.  This is required, even though
		GetServiceIDs() will return the complete list initialized in the constructor.
		This method just returns the first service ID in the list.
	*/
	virtual ServiceID GetServiceID();

	/**
		IsDefaultServiceProvider tells application this service is not the default service.
	*/
	virtual bool16 IsDefaultServiceProvider();
	
	/**
		GetInstantiationPolicy returns a InstancePerX value to indicate that only
		one instance per session is needed.
	*/
	virtual InstancePerX GetInstantiationPolicy();

	/**
		HasMultipleIDs returns kTrue in order to force a call to GetServiceIDs().
	*/
	virtual bool16 HasMultipleIDs() const;

	/**
		GetServiceIDs returns a list of services provided.
		@param serviceIDs List of IDs describing the services that
		IZPDocServiceProvider registers to handle.
	*/
	virtual void GetServiceIDs(K2Vector<ServiceID>& serviceIDs);
protected:
private:
};


CREATE_PMINTERFACE(CZPLoginUIServiceProvider, kZPLoginUIServiceProviderImpl)

CZPLoginUIServiceProvider::CZPLoginUIServiceProvider(IPMUnknown * boss)
:inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPLoginUIServiceProvider::~CZPLoginUIServiceProvider()
{

}

//----------------------------------------------------------------------------------------
// GetName
//----------------------------------------------------------------------------------------
void
CZPLoginUIServiceProvider::GetName(
	PMString* pName)
{
	if( pName )
#if defined(InDnCS5) || defined(InDnCS5_5)	//TODO: CS5 Porting
		(*pName).SetCString( "ZP Login UI Service Provider" );
#else
		(*pName).SetCString( "ZP Login UI Service Provider", PMString::kNoTranslate );
#endif
}


/**
	GetServiceID returns a single service ID.  This is required, even though
	GetServiceIDs() will return the complete list initialized in the constructor.
	This method just returns the first service ID in the list.
*/
//----------------------------------------------------------------------------------------
// GetServiceID
//----------------------------------------------------------------------------------------
ServiceID
CZPLoginUIServiceProvider::GetServiceID()
{
	return kZPLoginUIService;
}


/**
	IsDefaultServiceProvider tells application this service is not the default service.
*/
//----------------------------------------------------------------------------------------
// IsDefaultServiceProvider
//----------------------------------------------------------------------------------------
bool16
CZPLoginUIServiceProvider::IsDefaultServiceProvider()
{
	return kTrue;
}


/**
	GetInstantiationPolicy returns a InstancePerX value to indicate that only
	one instance per session is needed.
*/
//----------------------------------------------------------------------------------------
// GetInstantiationPolicy
//----------------------------------------------------------------------------------------
IK2ServiceProvider::InstancePerX
CZPLoginUIServiceProvider::GetInstantiationPolicy()
{
	return IK2ServiceProvider::kInstancePerUse;
}


/**
	HasMultipleIDs returns kTrue in order to force a call to GetServiceIDs().
*/
//----------------------------------------------------------------------------------------
// HasMultipleIDs
//----------------------------------------------------------------------------------------
bool16
CZPLoginUIServiceProvider::HasMultipleIDs()const
{
	return kFalse;
}


/**
	GetServiceIDs returns a list of services provided.
	@param serviceIDs List of IDs describing the services that
	IZPDocServiceProvider registers to handle.
*/
//----------------------------------------------------------------------------------------
// GetServiceIDs
//----------------------------------------------------------------------------------------
void
CZPLoginUIServiceProvider::GetServiceIDs(
	K2Vector<ServiceID>& serviceIDs)
{
	serviceIDs.push_back( kZPLoginUIService );
}

