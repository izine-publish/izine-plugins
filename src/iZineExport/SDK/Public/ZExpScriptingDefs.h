//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/SDK/Public/ZExpScriptingDefs.h $
//	$Revision: 2482 $
//	$Date: 2010-11-09 15:30:38 +0100 (Tue, 09 Nov 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 9-11-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_ZExpScriptingDefs_
#define _h_ZExpScriptingDefs_
#pragma once

#include "GUID.h"

//#define kCPrefs_CLSID			{ 0x8d448fe0, 0x8194, 0x11d3, { 0xa6, 0x53, 0x00, 0xe0, 0x98, 0x71, 0x0a, 0x6f } }
//DECLARE_GUID(CPrefs_CLSID, kCPrefs_CLSID);

// Used guidgen.exe to get these
//#define kIZPDataLink_CLSID	{ 0x9b40e092, 0x2103, 0x4693, { 0xa9, 0x72, 0x3a, 0x60, 0x50, 0xb6, 0x1d, 0xcc } }
//DECLARE_GUID(IZPDataLink_CLSID, kIZPDataLink_CLSID);

//#define kIZPDataLinks_CLSID	{ 0xf912bfe2, 0x130f, 0x49e5, { 0xbf, 0x8b, 0x57, 0x98, 0x58, 0x69, 0x19, 0x4e } }
//DECLARE_GUID(IZPDataLinks_CLSID, kIZPDataLinks_CLSID);

// {202967BF-2A11-4f1e-921C-198B022698F7}
//#define kIZPAssetData_CLSID		{ 0x202967bf, 0x2a11, 0x4f1e, { 0x92, 0x1c, 0x19, 0x8b, 0x02, 0x26, 0x98, 0xf7 } }
//DECLARE_GUID(ZPDataLinks_CLSID, kIZPAssetData_CLSID);

// Note: 4-char IDs below must be unique.
// See ScriptingDefs.h for further information.

// Suite IDs
enum enZExpScripSuits
{
	//s_ZPSuite = 'zpSu',				// iZinePublish suite
};

// Class IDs
enum enZExpScripObjects
{
	//c_ZPAssetData = 'zpAD'			//Asset Data object
};

// Property IDs
enum enZExpScriptProperties
{
	//p_ZPAssetData = 'ZpAD',			//Asset ID
	//p_ZPAssetID = 'ZpAI',			//Asset ID
	//p_ZPTitleID = 'ZpTI',			//Title ID
	//p_ZPEditionID = 'ZpEI'			//Edition ID
	//p_IZPDataLinkInfo = 'IDln',		// Custom datalink information
	//p_IZPDataLinkUniqueKey = 'IDuk',		// Custom datalink unique key
	//p_IZPDataLinkData = 'IDdt'		// Custom datalink name
};


// Event IDs
enum enZExpScriptEvents
{
	
};

#endif //_h_ZExpScriptingDefs_
