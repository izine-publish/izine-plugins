//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Utils/ZMDataHelper.h $
//	$Revision: 2701 $
//	$Date: 2011-04-06 10:48:06 +0200 (Wed, 06 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 6-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_ZMDataHelper_
#define _h_ZMDataHelper_
#pragma once

//#include "ZPTypes.h"

class IStringData;

#ifdef ZMBuild
#define ZMEXPORTS PLUGIN_DECL
#else
#define ZMEXPORTS PUBLIC_DECL
#endif

namespace ZMDataHelper
{
ZMEXPORTS
bool					IsValidGUID(
							const PMString &			inGUID );

ZMEXPORTS
const IStringData *		GetTitleByID(
							const PMString &			inTitleID );

ZMEXPORTS
const IStringData *		GetEditionByID(
							const PMString &			inEditionID );

ZMEXPORTS
const IStringData *		GetAdFileByID(
							const PMString &			inAdFileID );
}
#endif //_h_ZMDataHelper_
