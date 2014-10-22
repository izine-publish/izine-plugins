//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/SDK/Public/ZMDataMgr/IZMEditionMgr.h $
//	$Revision: 2749 $
//	$Date: 2011-04-08 11:32:30 +0200 (Fri, 08 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 29-3-2010
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#ifndef _h_IZPEditionMgr_
#define _h_IZPEditionMgr_
#pragma once

#include <map>
#include <vector>

#include "ZMTypes.h"

class IStringData;

typedef K2Vector<PMString>					ZMEditionIDsList;
typedef ZMEditionIDsList::iterator			ZMEditionIDsListIter;
typedef ZMEditionIDsList::const_iterator	ZMEditionIDsListCIter;

typedef std::map<PMString, IStringData *>	ZMEditionIDMap;
typedef ZMEditionIDMap::iterator			ZMEditionIDMapIter;
typedef ZMEditionIDMap::const_iterator		ZMEditionIDMapCIter;

class IZMEditionMgr : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZMEDITIONMGR };

	static	PMString	MakeEditionID(
							const PMString &				inTitleID,
							const PMString &				inEditionName );

	//for internal use only
	virtual void		Shutdown() = 0;

	virtual void		UpdateEditionList(
							const PMString & 				inTitleID,
							const ZMstEditionSList &		inEditionList ) = 0;

	virtual void		UpdateEditionList(
							const IStringData *				inTitleID,
							const ZMstEditionSList &		inEditionList ) = 0;

	virtual const IStringData *
						GetEditionForID(
							const PMString &				inEditionID ) const = 0;

	virtual const IStringData *
						GetEditionForAdFileID(
							const PMString &				inAdFileID ) const = 0;

	
	virtual void		DecrementReferenceForEditions (
							const ZMEditionIDsList &		inEditionIDs ) = 0;

	virtual void		FetchAdFileListForEdition(
							const PMString &				inEditinoID ) = 0;

};

#endif //_h_IZPEditionMgr_
