//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/ZPDataMgr/IZPPreviewMgr.h $
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
#ifndef _h_IZPPreviewMgr_
#define _h_IZPPreviewMgr_
#pragma once

#include "ZPWSTypeDefs.h"
class IZPSoapRequest;
class AZPSoapResponse;

class IZPPreviewMgr : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPPREVIEWMGR };

	virtual void 		FetchPreviewForAssetID(
							const PMString &				inAssetID,
							const IDFile &					inPreviewFilePath,
							bool							inForceUpdation = false) const = 0;

	virtual void		HandleWSResponse(
							const IZPSoapRequest *			inSoapRequest,
							const AZPSoapResponse *			inResponse,
							ZPWSError						inStatus) = 0;
};


#endif //_h_IZPPreviewMgr_
