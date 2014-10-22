//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/SDK/Public/Utils/IZMAdFileFacade.h $
//	$Revision: 2805 $
//	$Date: 2011-04-12 15:42:46 +0200 (Tue, 12 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Aman Alam
//	Created: 4-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZMAdFileFacade_
#define _h_IZMAdFileFacade_
#pragma once

#include "ZMTypes.h"

class IZMAdInfo;

class IZMAdFileFacade : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZMADFILEFACADE };

	virtual bool16				CanBeValidXMLFile(
									const PMString&						path) const = 0;
	virtual bool				ParseAdXMLFile(
									const PMString &					path, 
									ZMstAdSList &						oAdsList,
									uint32 *							oLastUpdateDate = nil) const = 0;

								//Replaces %path% with repository path. Return empty if repository path is empty
	virtual PMString			MakeFullAdFilePath(
									const IZMAdInfo *					inAdInfo ) const = 0;
};


#endif //_h_IZMAdFileFacade_
