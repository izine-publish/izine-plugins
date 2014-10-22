//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/ZPDataMgr/IZPTaskStatusMgr.h $
//	$Revision: 2208 $
//	$Date: 2010-09-30 16:51:25 +0200 (Thu, 30 Sep 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 23-9-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPTaskStatusMgr_
#define _h_IZPTaskStatusMgr_
#pragma once

#include <map>

class IStringData;

class IZPSoapRequest;
class AZPSoapRequest;

#include "ZPSmartPtrVector.h"
#include "ZPWSTypeDefs.h"

typedef std::map<PMString, const IStringData *>	ZPTaskStatusIDMap;
typedef K2Vector<PMString>						ZPTaskStatusIDsList;
typedef ZPTaskStatusIDMap::iterator				ZPTaskStatusIDMapIter;
typedef ZPTaskStatusIDMap::const_iterator		ZPTaskStatusIDMapConstIter;

class IZPTaskStatusMgr : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPTASKSTATUSMGR };

	//for internal use only
	virtual void		Shutdown() = 0;

	virtual void		HandleWSResponse(
							const IZPSoapRequest *			inSoapRequest,
							const AZPSoapResponse *			inResponse,
							ZPWSError						inStatus) = 0;

	virtual void		UpdateTaskStatusList(
							const ZPIDPairSList &		inTaskStatusList ) = 0;
	virtual const IStringData *
						GetTaskStatusForID(
							const PMString &			inTaskStatusID ) const = 0;

	virtual void		GetAllTaskStatusIDs(
							ZPTaskStatusIDsList &		oKeys,
							const bool					inFetchIfNotHave = true) const = 0;
};


#endif //_h_IZPTaskStatusMgr_
