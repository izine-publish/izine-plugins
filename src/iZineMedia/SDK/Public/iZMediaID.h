//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/SDK/Public/iZMediaID.h $
//	$Revision: 2738 $
//	$Date: 2011-04-07 13:08:10 +0200 (Thu, 07 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 9-11-2010
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZMediaID_
#define _h_IZMediaID_
#pragma once

#include "SDKDef.h"
#include "iZMediaVersion.h"

// Company:
#define kIZMediaCompanyKey		"kIZMediaPlugInCompanyKey"	// Company name used internally for menu paths and the like. Must be globally unique, only A-Z, 0-9, space and "_".
#define kIZMediaCompanyValue		"iZine Publish"				// Company name displayed externally.

// Plug-in:
#define kIZMediaPluginName			"iZine Media"			// Name of this plug-in.
#define kIZMediaPluginFileName		"iZine Media"
#define kIZPProductName				"iZine Publish"

#define kIZMediaPrefixNumber	0x16ef00	// iZine Media prefix number
#define kIZMediaUIPrefixNumber	0x16f000	// iZine Media UI prefix number

#define kIZMediaVersion		kIZMediaPluginVersionString
#define kIZMediaAuthor		"iZine Publish"					// Author of this plug-in (for the About Box).

// Plug-in Prefix: (please change kIZMediaPrefixNumber above to modify the prefix.)
#define kIZMediaPrefix			RezLong(kIZMediaPrefixNumber)			// The unique numeric prefix for all object model IDs for this plug-in.
#define kIZMediaStringPrefix	IZPMAKESTR(kIZMediaPrefixNumber)	// The string equivalent of the unique prefix number for  this plug-in.
#define kIZMediaUIPrefix		RezLong(kIZMediaUIPrefixNumber)
#define kIZMediaUIStringPrefix	IZPMAKESTR(kIZMediaUIPrefixNumber)	// The string equivalent of the unique prefix number for  this plug-in.

// Missing plug-in: (see ExtraPluginInfo resource)
#define kIZMediaMissingPluginURLValue		"http://www.izine-publish.net/" // URL displayed in Missing Plug-in dialog
#define kIZMediaMissingPluginAlertValue		"One of iZine Publish plugins is missing. Please add it to open the documents." // Message displayed in Missing Plug-in dialog - provide a string that instructs user how to solve their missing plug-in problem

#define kIZMediaCopyrightShortValue	"Copyright 2010-2011 iZine Solutions."
#define kIZMediaCopyrightValue	kIZMediaCopyrightShortValue " All Rights Reserved."
#define kIZMediaSupportURL		"http://www.izine-publish.com/"


// PluginID:
DECLARE_PMID(kPlugInIDSpace, kIZMediaPluginID, kIZMediaPrefix + 0)

// ClassID/Interface/Impl
#include "IZMedia_ClassDefID.h"
#include "IZMediaUI_ClassDefID.h"
// ActionIDs & Menu positions
#include "IZMedia_ActionID.h"
// WidgetIDs:
#include "IZMediaUI_WidgetID.h"
// String Keys
#include "IZMedia_StringKeys.h"
#include "IZMediaUI_StringKeys.h"

#include "iZMedia_MessageID.h"
#include "iZMediaUI_MessageID.h"
#include "iZMedia_ErrorID.h"

//Services
//DECLARE_PMID(kServiceIDSpace, kZMLoginUIService,							kIZMediaPrefix + 0)

//Script Element IDs
	//Objects, Suites
//DECLARE_PMID(kScriptInfoIDSpace, kZPSuiteScriptElement,														kIZMediaPrefix + 0 )
	//Events
	// Properties


#include "iZMediaUI_ResourceID.h"

#endif //_h_IZMediaID_
