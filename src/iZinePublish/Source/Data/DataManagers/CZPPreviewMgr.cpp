//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Data/DataManagers/CZPPreviewMgr.cpp $
//	$Revision: 1849 $
//	$Date: 2010-07-26 15:24:30 +0200 (Mon, 26 Jul 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 26-7-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "ISubject.h"

//SDK General includes
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPPreviewMgr.h"
#include "IZPAMWSUtils.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZPPreviewMgr>
class CZPPreviewMgr : public inheritClass
{
public:
						CZPPreviewMgr(IPMUnknown * boss);
	virtual				~CZPPreviewMgr();

	void 				FetchPreviewForAssetID(
							const PMString &				inAssetID,
							const IDFile &					inPreviewFilePath,
							bool							inForceUpdation ) const;

	void				HandleWSResponse(
							const IZPSoapRequest *			inSoapRequest,
							const AZPSoapResponse *			inResponse,
							ZPWSError						inStatus);

protected:
private:
};


CREATE_PMINTERFACE(CZPPreviewMgr, kZPPreviewMgrImpl)

CZPPreviewMgr::CZPPreviewMgr(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPPreviewMgr::~CZPPreviewMgr()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// FetchPreviewForAssetID
//----------------------------------------------------------------------------------------
void
CZPPreviewMgr::FetchPreviewForAssetID(
	const PMString &				inAssetID,
	const IDFile &					inPreviewFilePath,
	bool							inForceUpdation) const
{
	//TODO: maintain and check cache for preview file path and version for which preview was fetched.
	Utils<IZPAMWSUtils>()->DownloadThumbnail_async( inAssetID, inPreviewFilePath );
}

//----------------------------------------------------------------------------------------
// HandleWSResponse
//----------------------------------------------------------------------------------------
void
CZPPreviewMgr::HandleWSResponse(
	const IZPSoapRequest *			inSoapRequest,
	const AZPSoapResponse *			inResponse,
	ZPWSError						inStatus)
{
	//TODO: update cache for preview file path and version for which preview was fetched.

	InterfacePtr<ISubject> selfSubject(this, UseDefaultIID());
	ASSERT(selfSubject);

	//TODO: pass the asset ID which was fetched.
	selfSubject->Change( kZPAssetThumbnailAvailableMsg, IZPPreviewMgr::kDefaultIID, nil );
}
