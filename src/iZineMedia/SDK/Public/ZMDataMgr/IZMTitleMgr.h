//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/SDK/Public/ZMDataMgr/IZMTitleMgr.h $
//	$Revision: 2750 $
//	$Date: 2011-04-08 11:33:52 +0200 (Fri, 08 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 29-3-2010
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#ifndef _h_IZMTitleMgr_
#define _h_IZMTitleMgr_
#pragma once

#include <map>

class IStringData;

#include "ZMTypes.h"
#include "ZPSmartPtrVector.h"

typedef K2Vector<PMString>					ZMTitleIDsList;
typedef ZMTitleIDsList::iterator			ZMTitleIDsListIter;
typedef ZMTitleIDsList::const_iterator		ZMTitleIDsListCIter;

typedef std::map<PMString, IStringData *>	ZMTitleIDMap;
typedef ZMTitleIDMap::iterator				ZMTitleIDMapIter;
typedef ZMTitleIDMap::const_iterator		ZMTitleIDMapConstIter;

//TODO: derive from IZMDataObjMgr
class IZMTitleMgr : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZMTITLEMGR };

	//for internal use only
	virtual void		Shutdown() = 0;

	virtual	void		UpdateTitleList(
							const ZMstTitleSList &		inTitleList ) = 0;

	virtual const IStringData *
						GetTitleForID(
							const PMString &			inTitleID ) const = 0;
	virtual const IStringData *	GetTitleForEditionID(
							const PMString &			inEditionID ) const = 0;

	virtual void		GetAllTitleIDs(
							ZMTitleIDsList &			oKeys ) const = 0;

	virtual void		FetchEditionsForTitle(
							const PMString &			inTitleID ) = 0;
};

#endif //_h_IZMTitleMgr_
