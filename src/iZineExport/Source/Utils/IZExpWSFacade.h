//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/Utils/IZExpWSFacade.h $
//	$Revision: 3943 $
//	$Date: 2012-02-24 08:11:55 +0100 (Fri, 24 Feb 2012) $
//	$Author: aman.alam $
//	
//	Creator: Aman Alam
//	Created: 7-6-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZExpWSFacade_
#define _h_IZExpWSFacade_
#pragma once

#include "KeyValuePair.h"
#include "ZExpWSTypeDefs.h"

class AZExpWSRequest;
class AZExpWSResponse;

class IZExpWSFacade : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZEXPWSFACADE };

	typedef KeyValuePair<PMString, PMString> KeyValue;

	virtual ZExpWSError				GetTitles(
										const AZExpWSRequest *	inReq,
										AZExpWSResponse *		oResp) = 0;
	virtual ZExpWSError				GetEditions(
										const AZExpWSRequest *	inReq,
										AZExpWSResponse *		oResp) = 0;

	virtual ZExpWSError				UploadExport(
										const AZExpWSRequest *	inReq,
										AZExpWSResponse *		oResp) = 0;

	virtual ZExpWSError				ValidateMediaContent(
										const AZExpWSRequest *	inReq,
										AZExpWSResponse *		oResp) = 0;

	virtual ZExpWSError				GetConfig(
										const AZExpWSRequest *	inReq,
										AZExpWSResponse *		oResp) = 0;
};


#endif //_h_IZExpWSFacade_
