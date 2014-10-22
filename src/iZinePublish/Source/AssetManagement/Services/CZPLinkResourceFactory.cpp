//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/Services/CZPLinkResourceFactory.cpp $
//	$Revision: 3923 $
//	$Date: 2012-02-16 05:25:15 +0100 (Thu, 16 Feb 2012) $
//	$Author: aman.alam $
//	
//	Creator: Aman Alam
//	Created: 13-2-2012
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IDataBase.h"
#include "ILinkResourceFactory.h"
#include "ILinkResourceHandler.h"
#include "ILinkResourceStateUpdater.h"

//SDK General includes

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces

//IZP General includes
#include "ZPConstants.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<ILinkResourceFactory>
class CZPLinkResourceFactory : public inheritClass
{
public:
						CZPLinkResourceFactory(IPMUnknown * boss);
	virtual				~CZPLinkResourceFactory();

	/**
	 Returns the list of URI schemes supported by the handler, which in our case, CSVLink.
	 
	 @see ILinkResourceFactory::GetSchemes
	 @param schemes [OUT] List of supported schemes.
	 */ 
	virtual void GetSchemes(K2Vector<WideString>& schemes) const;
	/**
	 Returns the link resource handler, kCusDtLnkLinkResourceHandlerBoss, that handles our 'CSVLink' type resource.
	 
	 @see ILinkResourceFactory::GetSchemes
	 */ 
	virtual ILinkResourceHandler* QueryHandler(const URI& uri, IDataBase* db) const;

	/**
	 Returns the link resource state updater who provides status update for link resource whose URI scheme is 'CSVLink'	 
	 @see ILinkResourceFactory::QueryStateUpdater
	 */ 
	virtual ILinkResourceStateUpdater* QueryStateUpdater(const URI& uri, IDataBase* db) const;

protected:
private:
	// Prevent copy construction and assignment.
	CZPLinkResourceFactory(const CZPLinkResourceFactory&);
	CZPLinkResourceFactory& operator=(const CZPLinkResourceFactory&);
};


CREATE_PMINTERFACE(CZPLinkResourceFactory, kZPLinkResourceFactoryImpl)

CZPLinkResourceFactory::CZPLinkResourceFactory(IPMUnknown * boss)
: inheritClass(boss)
{
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPLinkResourceFactory::~CZPLinkResourceFactory()
{
}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetSchemes
//----------------------------------------------------------------------------------------
static const WideString k_fileScheme(kURIScheme);
void
CZPLinkResourceFactory::GetSchemes(
	K2Vector<WideString>& schemes) const
{
	schemes.clear();
	schemes.push_back(k_fileScheme);
}

//----------------------------------------------------------------------------------------
// QueryHandler
//----------------------------------------------------------------------------------------
ILinkResourceHandler*
CZPLinkResourceFactory::QueryHandler(
	const URI& uri, IDataBase* db) const
{
	return ::CreateObject2<ILinkResourceHandler>(kZPLinkResourceHandlerBoss);
}

//----------------------------------------------------------------------------------------
// QueryStateUpdater
//----------------------------------------------------------------------------------------
ILinkResourceStateUpdater*
CZPLinkResourceFactory::QueryStateUpdater(
	const URI& uri, IDataBase* db) const
{
	return ::CreateObject2<ILinkResourceStateUpdater>(kZPLinkResourceHandlerBoss);
}