//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/SDK/Public/ZMDataMgr/IZMAdFileMgr.h $
//	$Revision: 2738 $
//	$Date: 2011-04-07 13:08:10 +0200 (Thu, 07 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 29-3-2010
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#ifndef _h_IZPAdFileMgr_
#define _h_IZPAdFileMgr_
#pragma once

#include "ZMTypes.h"
//#include "ZPWSTypeDefs.h"

class IStringData;

class IZMAdFileMgr : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZMADFILEMGR };

	static	PMString	MakeAdFileID(
							const PMString &				inEditionID,
							const PMString &				inAdFileName );

	//for internal use only
	virtual void		Shutdown() = 0;

	virtual void		UpdateAdFileList(
							const PMString &			inEditionID,
							const ZMPMStringSList &		inAdFileList ) = 0;
	virtual void		UpdateAdFileList(
							const IStringData *			inEditionID,
							const ZMPMStringSList &		inAdFileList ) = 0;

	virtual void		AddAdFile(
							const IStringData *			inEditionID,
							const PMString &			inAdFileID,			//New AdFileID
							const PMString &			inAdFileName ) = 0;

	virtual const IStringData *
						GetAdFileByID(
							const PMString &			inAdFileID ) const = 0;

	//virtual void		DeleteAdFile(					//Sends WS request async
	//						const PMString &			inAdFileID ) const = 0;

	virtual void		DecrementReferenceForAdFiles(
							const ZMAdFileIDsList &		inAdFileIDs ) = 0;

};

#endif //_h_IZPAdFileMgr_
