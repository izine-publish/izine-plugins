//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/ZPDataMgr/IZPTitleStatusMgr.h $
//	$Revision: 3213 $
//	$Date: 2011-06-13 12:17:48 +0200 (Mon, 13 Jun 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 5-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPTitleStatusMgr_
#define _h_IZPTitleStatusMgr_
#pragma once

#include <map>
#include <vector>

#include "ZPTypes.h"
#include "ZPWSTypeDefs.h"


class IStringData;

class IZPTitleStatusMgr : public IPMUnknown
{
protected:
	
public:
	enum { kDefaultIID = IID_IZPTITLESTATUSMGR };

	//for internal use only
	virtual void		Shutdown() = 0;

	virtual void		UpdateStatusList(
							const ZPTitleStatusInfoSList &	inStatusInfoList ) = 0;

	virtual const IStringData *
						GetStatusForID(
							const PMString &				inStatusID ) const = 0;

	virtual const ZPStatusIDsList &
						GetDefaultStatusIDs() const = 0;

	virtual void		DecrementReferenceForTitleStatus (
							const ZPStatusIDsList &			inStatusIDs ) = 0;

};

struct PLUGIN_DECL CompareStatusIDBySortIndex
{
	bool operator()( const PMString & inFirstVal, const PMString & inSecondVal );
};

#endif //_h_IZPTitleStatusMgr_
