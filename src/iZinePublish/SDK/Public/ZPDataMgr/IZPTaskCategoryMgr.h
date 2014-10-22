//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/ZPDataMgr/IZPTaskCategoryMgr.h $
//	$Revision: 2302 $
//	$Date: 2010-10-14 14:18:41 +0200 (Thu, 14 Oct 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 14-10-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPTaskCategoryMgr_
#define _h_IZPTaskCategoryMgr_
#pragma once

#include <map>

class IStringData;

class IZPSoapRequest;
class AZPSoapRequest;

#include "ZPSmartPtrVector.h"
#include "ZPWSTypeDefs.h"

typedef std::map<PMString, const IStringData *>	ZPTaskCategoryIDMap;
typedef K2Vector<PMString>						ZPTaskCategoryIDsList;
typedef ZPTaskCategoryIDMap::iterator			ZPTaskCategoryIDMapIter;
typedef ZPTaskCategoryIDMap::const_iterator		ZPTaskCategoryIDMapConstIter;

class IZPTaskCategoryMgr : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPTASKCATEGORYMGR };

	//for internal use only
	virtual void		Shutdown() = 0;

	virtual void		HandleWSResponse(
							const IZPSoapRequest *		inSoapRequest,
							const AZPSoapResponse *		inResponse,
							ZPWSError					inCategory) = 0;

	virtual void		UpdateTaskCategoryList(
							const ZPIDPairSList &		inTaskCategoryList ) = 0;
	virtual const IStringData *
						GetTaskCategoryForID(
							const PMString &			inTaskCategoryID ) const = 0;

	virtual void		GetAllTaskCategoryIDs(
							ZPTaskCategoryIDsList &		oKeys,
							const bool					inFetchIfNotHave = true) const = 0;
};


#endif //_h_IZPTaskCategoryMgr_
