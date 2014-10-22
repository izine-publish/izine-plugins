//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/SystemProvider/CZPAMICDocumentSystemProvider.cpp $
//	$Revision: 3569 $
//	$Date: 2011-08-04 12:31:19 +0200 (Thu, 04 Aug 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 31-8-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IAMServiceProvider.h"
#include "IAMSystemProvider.h"

//SDK General includes
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPInDnUtils.h"

//IZP General includes
#include "AZPAMSystemProvider.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AMSysP
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass AZPAMSystemProvider
class CZPAMICDocumentSystemProvider : public inheritClass
{
public:
						CZPAMICDocumentSystemProvider(IPMUnknown * boss);
	virtual				~CZPAMICDocumentSystemProvider();

	ErrorCode			Init();
protected:
	const char *		GetProviderName() const;
private:
};


CREATE_PMINTERFACE(CZPAMICDocumentSystemProvider, kZPAMICDocumentSystemProviderImpl)

CZPAMICDocumentSystemProvider::CZPAMICDocumentSystemProvider(IPMUnknown * boss)
: inheritClass(boss)
{
	IZPLog_ObjC( thisFileLA );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMICDocumentSystemProvider::~CZPAMICDocumentSystemProvider()
{
	IZPLog_ObjD( thisFileLA );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetProviderName
//----------------------------------------------------------------------------------------
const char *
CZPAMICDocumentSystemProvider::GetProviderName()const
{
	return "InCopy Document system provider";
}

//----------------------------------------------------------------------------------------
// Init
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMICDocumentSystemProvider::Init()
{
	LogFunctionEnterExit;
	ErrorCode toReturn = kFailure;
	if( Utils<IZPInDnUtils>()->IsHostAppInCopy() == kTrue )
		toReturn = inheritClass::Init();
	
	return toReturn;
}


