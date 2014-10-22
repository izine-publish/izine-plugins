//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/Common/Utils/ZPDataHelper.h $
//	$Revision: 3010 $
//	$Date: 2011-05-13 10:50:25 +0200 (Fri, 13 May 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 20-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description: Global functions to ease accessing data objects.
//========================================================================================
#ifndef _h_ZPDataHelper_
#define _h_ZPDataHelper_
#pragma once

#include "ZPTypes.h"

class IStringData;

#ifdef ZPBuild
#define ZPEXPORTS PLUGIN_DECL
#else
#define ZPEXPORTS PUBLIC_DECL
#endif


namespace ZPDataHelper
{
ZPEXPORTS
bool					IsValidGUID(
							const PMString &			inGUID );

ZPEXPORTS
const IStringData *		GetTitleByID(
							const PMString &			inTitleID );

ZPEXPORTS
const IStringData *		GetEditionByID(
							const PMString &			inEditionID );

ZPEXPORTS
const IStringData *		GetTitleStatusByID(
							const PMString &			inStatusID );

ZPEXPORTS
const IStringData *		GetAssetByID(
							const PMString &			inAssetID );

ZPEXPORTS
const IStringData *		GetTaskByID(
							const PMString &			inTaskID );

ZPEXPORTS
enZPUserRole			GetCurrUserRole();

ZPEXPORTS
const PMString &		GetCurrUserID();

ZPEXPORTS
const bool				IsLoggedIn();
}
#endif //_h_ZPDataHelper_