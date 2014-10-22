//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/ZPDataMgr/IZPTitleMgr.h $
//	$Revision: 4036 $
//	$Date: 2012-06-14 09:17:28 +0200 (Thu, 14 Jun 2012) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 29-3-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#ifndef _h_IZPTitleMgr_
#define _h_IZPTitleMgr_
#pragma once

#include <map>

class IStringData;

#include "ZPSmartPtrVector.h"

typedef std::map<PMString, IStringData *>	ZPTitleIDMap;
typedef K2Vector<PMString>					ZPTitleIDsList;
typedef ZPTitleIDMap::iterator				ZPTitleIDMapIter;
typedef ZPTitleIDMap::const_iterator		ZPTitleIDMapConstIter;

//TODO: derive from IZPDBObjMgr
class IZPTitleMgr : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPTITLEMGR };

	//for internal use only
	virtual void		Shutdown() = 0;

	virtual	void		UpdateTitleList(
							const ZPPMStringSList &		inTitleIDList,
							const ZPPMStringSList &		inTitleNames ) = 0;

	virtual const IStringData *
						GetTitleForID(
							const PMString &			inTitleID ) const = 0;
	virtual const IStringData *	GetTitleForEditionID(
							const PMString &			inEditionID ) const = 0;

	virtual void		GetAllTitleIDs(
							ZPTitleIDsList &			oKeys ) const = 0;

	virtual void		FetchEditionsForTitle(
							const PMString &			inTitleID ) = 0;
	virtual void		FetchStatusForTitle(
							const PMString &			inTitleID ) = 0;
	virtual void		FetchUsersForTitle(
							const PMString &			inTitleID ) = 0;
};

struct PLUGIN_DECL CompareTitleIDByName
{
	bool operator()( const PMString & inFirstVal, const PMString & inSecondVal );
};
#endif //_h_IZPTitleMgr_
