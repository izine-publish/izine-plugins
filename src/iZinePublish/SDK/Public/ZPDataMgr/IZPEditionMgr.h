//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/ZPDataMgr/IZPEditionMgr.h $
//	$Revision: 2390 $
//	$Date: 2010-10-26 11:08:55 +0200 (Tue, 26 Oct 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 29-3-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#ifndef _h_IZPEditionMgr_
#define _h_IZPEditionMgr_
#pragma once

#include <map>
#include <vector>

#include "ZPWSTypeDefs.h"

class IStringData;

typedef std::map<PMString, IStringData *>	ZPEditionIDMap;
typedef K2Vector<PMString>					ZPEditionIDsList;
typedef ZPEditionIDMap::iterator			ZPEditionIDMapIter;
typedef ZPEditionIDMap::const_iterator		ZPEditionIDMapConstIter;

class IZPEditionMgr : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPEDITIONMGR };

	//for internal use only
	virtual void		Shutdown() = 0;

	virtual void		UpdateEditionList(
							const ZPTitleEditionInfoSList &	inEditionInfoList ) = 0;

	virtual const IStringData *
						GetEditionForID(
							const PMString &				inEditionID ) const = 0;

	virtual const IStringData *
						GetEditionForAssetID(
							const PMString &				inAssetID ) const = 0;

	
	virtual const IStringData *
						GetEditionForTaskID(
							const PMString &				inTaskID ) const = 0;


	virtual void		DecrementReferenceForEditions (
							const ZPEditionIDsList &		inEditionIDs ) = 0;
};

#endif //_h_IZPEditionMgr_
