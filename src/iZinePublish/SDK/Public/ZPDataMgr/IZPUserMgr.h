//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/ZPDataMgr/IZPUserMgr.h $
//	$Revision: 2213 $
//	$Date: 2010-10-01 12:44:26 +0200 (Fri, 01 Oct 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 1-10-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPUserMgr_
#define _h_IZPUserMgr_
#pragma once

#include <map>
#include <vector>

#include "ZPWSTypeDefs.h"

class IStringData;

class IZPSoapRequest;
class AZPSoapResponse;

typedef std::map<PMString, IStringData *>	ZPUserIDMap;
typedef K2Vector<PMString>					ZPUserIDsList;
typedef ZPUserIDMap::iterator				ZPUserIDMapIter;
typedef ZPUserIDMap::const_iterator			ZPUserIDMapConstIter;

class IZPUserMgr : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPUSERMGR };

	virtual void		HandleWSResponse(
							const IZPSoapRequest *			inSoapRequest,
							const AZPSoapResponse *			inResponse,
							ZPWSError						inStatus) = 0;

	//for internal use only
	virtual void		Shutdown() = 0;

	virtual void		UpdateUserList(
							const PMString &			inTitleID,
							const ZPIDPairSList &		inUserInfoList ) = 0;

	virtual const IStringData *
						GetUserForID(
							const PMString &			inUserID ) const = 0;

	virtual void		DecrementReferenceForUsers (
							const ZPUserIDsList &		inUserIDs ) = 0;

};


#endif //_h_IZPUserMgr_
