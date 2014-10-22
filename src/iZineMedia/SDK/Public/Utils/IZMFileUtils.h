//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/SDK/Public/Utils/IZMFileUtils.h $
//	$Revision: 2906 $
//	$Date: 2011-04-29 15:21:22 +0200 (Fri, 29 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Aman Alam
//	Created: 4-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZMFileUtils_
#define _h_IZMFileUtils_
#pragma once

#include "ZMTypes.h"

class IZMFileUtils : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZMFILEUTILS };


	virtual bool				FetchEditionsFromPath(
									const PMString&						parentPath,			//Complete path
									ZMstEditionSList &					oEditionList) const = 0;
	virtual void				FetchListFromPath(
									const PMString&						parentPath, 
									ZMstTitleSList &					oTitleList) const = 0;
	virtual bool				FetchAdsFileFromPath(
									const PMString&						parentPath,			//Complete path
									ZMPMStringSList &					oAdsList,
									const PMString&						inFilePrefix = kNullString) const = 0;
	virtual void				GetAllDirectoryNames(
									const PMString&						parentPath, 
									ZMPMStringSList &					oDirectoryNames) const = 0;
#ifdef DEBUG
	virtual void				PrintToLog(
									ZMstTitleSList &					titleList) const = 0;
#endif
	virtual bool16				ValidPath(
									const PMString&						path) const = 0;
									
	virtual PMString			MacPosixToUnixPath(
									const PMString &					inPath ) const = 0;
	virtual PMString			ReplaceSlashWithColon(
									const PMString &					inPath ) const = 0;
};


#endif //_h_IZMFileUtils_
