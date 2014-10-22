//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/SDK/Public/ZMDataMgr/IZMAdMgr.h $
//	$Revision: 2781 $
//	$Date: 2011-04-11 16:12:21 +0200 (Mon, 11 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 11-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZMAdMgr_
#define _h_IZMAdMgr_
#pragma once


#include "ZMTypes.h"
//#include "ZPWSTypeDefs.h"

class IStringData;

class IZMAdMgr : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZMADMGR };

	static	PMString	MakeAdID(
							const PMString &				inAdFileID,
							const PMString &				inAdID );	//As in xml file

	//for internal use only
	virtual void		Shutdown() = 0;

	virtual void		UpdateAdList(
							const PMString &			inAdFileID,
							const ZMstAdSList &			inAdList ) = 0;
	virtual void		UpdateAdList(
							const IStringData *			inAdFileID,
							const ZMstAdSList &			inAdFileList ) = 0;

	//virtual void		AddAd(
	//						const IStringData *			inAdFileID,
	//						const PMString &			inAdID,			//New AdID
	//						const PMString &			inAdName ) = 0;

	virtual const IStringData *
						GetAdByID(
							const PMString &			inAdID ) const = 0;

	//virtual void		DeleteAdFile(					//Sends WS request async
	//						const PMString &			inAdFileID ) const = 0;

	virtual void		DecrementReferenceForAds(
							const ZMAdIDsList &			inAdIDs ) = 0;

};

#endif //_h_IZMAdMgr_
