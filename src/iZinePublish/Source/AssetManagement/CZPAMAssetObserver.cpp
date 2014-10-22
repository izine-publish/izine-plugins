//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/CZPAMAssetObserver.cpp $
//	$Revision: 3572 $
//	$Date: 2011-08-11 08:47:35 +0200 (Thu, 11 Aug 2011) $
//	$Author: aman.alam $
//	
//	Creator: Aman Alam
//	Created: 20-6-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IStringData.h"
#include "ISubject.h"

//SDK General includes
#include "CObserver.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces

//IZP General includes
#include "AZPAMAssetMonitor.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AssetMonitor
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CObserver
class CZPAMAssetObserver : public inheritClass
{
public:
						CZPAMAssetObserver(IPMUnknown * boss);
	virtual				~CZPAMAssetObserver();

	void AutoAttach();
	void AutoDetach();

	void				Update(
							const ClassID&				theChange,
							ISubject*					theSubject,
							const PMIID &				protocol,
							void*						changedBy);
protected:
private:
};


CREATE_PMINTERFACE(CZPAMAssetObserver, kZPAMAssetObserverImpl)

CZPAMAssetObserver::CZPAMAssetObserver(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMAssetObserver::~CZPAMAssetObserver()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// AutoAttach
//----------------------------------------------------------------------------------------
void
CZPAMAssetObserver::AutoAttach()
{

}

//----------------------------------------------------------------------------------------
// AutoDetach
//----------------------------------------------------------------------------------------
void
CZPAMAssetObserver::AutoDetach()
{

}

//----------------------------------------------------------------------------------------
// Update
//----------------------------------------------------------------------------------------
void
CZPAMAssetObserver::Update(
	const ClassID&				theChange,
	ISubject*					theSubject,
	const PMIID &				protocol,
	void*						changedBy)
{
	if ( theChange == kZPAssetInfoChangedMsg || theChange == kZPAssetLockInfoChangedMsg || theChange == kZPDataNameChangedMsg)
	{
		LogFunctionEnterExit;
		if (theSubject)
		{
			InterfacePtr<const IStringData> assetId(theSubject , IID_IZPDATAID );
			const PMString & assetIdStr = assetId->GetString();

			InterfacePtr<AZPAMAssetMonitor> assetMonitor(this , UseDefaultIID() );
			ASSERT(assetMonitor);
			if (assetMonitor)
				assetMonitor->AddUpdatedAssetId(assetIdStr);
		}
	}
}
