//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/SDK/Public/IZExpID.h $
//	$Revision: 3976 $
//	$Date: 2012-03-06 12:56:58 +0100 (Tue, 06 Mar 2012) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 9-11-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZExpID_
#define _h_IZExpID_
#pragma once

#include "SDKDef.h"
#include "iZineExpVersion.h"

// Company:
#define kIZExpCompanyKey		"kIZExpPlugInCompanyKey"	// Company name used internally for menu paths and the like. Must be globally unique, only A-Z, 0-9, space and "_".
#ifdef RVExport
#define kIZExpCompanyValue		"Redfive Export"				// Company name displayed externally.
#else
#define kIZExpCompanyValue		"iZine Export"				// Company name displayed externally.
#endif
// Plug-in:
#ifdef RVExport
#define kIZExpPluginName			"Redfive Export"			// Name of this plug-in.
#define kIZExpPluginFileName		"Redfive Export"
#else
#define kIZExpPluginName			"iZine Export"			// Name of this plug-in.
#define kIZExpPluginFileName		"iZine Export"
#endif
#define kIZPProductName			"iZine Publish"

#ifdef RVExport
#define kIZExpPrefixNumber		0x173d00	//Redfive Export prefix number
#define kIZExpUIPrefixNumber	0x173e00	//Redfive Export UI
#else
#define kIZExpPrefixNumber	0x165200	// iZine Export prefix number
#define kIZExpUIPrefixNumber	kIZExpPrefixNumber	//iZine Export UI
#endif

#define kIZExpVersion kIZExpPluginVersionString
#define kIZExpAuthor		"iZine Publish"					// Author of this plug-in (for the About Box).

// Plug-in Prefix: (please change kIZExpPrefixNumber above to modify the prefix.)
#define kIZExpPrefix			RezLong(kIZExpPrefixNumber)		// The unique numeric prefix for all object model IDs for this plug-in.
#define kIZExpStringPrefix		IZPMAKESTR(kIZExpPrefixNumber)	// The string equivalent of the unique prefix number for  this plug-in.
#define kIZExpUIPrefix			RezLong(kIZExpUIPrefixNumber)
#define kIZExpUIStringPrefix	IZPMAKESTR(kIZExpPrefixNumber)	// The string equivalent of the unique prefix number for  this plug-in.

// Missing plug-in: (see ExtraPluginInfo resource)
#define kIZExpMissingPluginURLValue		"http://www.izine-publish.net/" // URL displayed in Missing Plug-in dialog
#define kIZExpMissingPluginAlertValue		"One of iZine Publish plugins is missing. Please add it to open the documents." // Message displayed in Missing Plug-in dialog - provide a string that instructs user how to solve their missing plug-in problem

#define kIZExpCopyrightShortValue	"Copyright 2010-2011 iZine Solutions."
#define kIZExpCopyrightValue	kIZExpCopyrightShortValue " All Rights Reserved."
#define kIZExpSupportURL		"http://www.izine-publish.com/"


// PluginID:
DECLARE_PMID(kPlugInIDSpace, kIZExpPluginID, kIZExpPrefix + 0)

#define kZExpPrefsPanelResourceID		10000

//Dialogs
#define kZExpExportDialogResourceID		11000

// ClassID/Interface/Impl
#include "IZExp_ClassDefID.h"
// ActionIDs & Menu positions
#include "IZExp_ActionID.h"
// WidgetIDs:
#include "IZExp_WidgetID.h"
// String Keys
#include "IZExp_StringKeys.h"
#include "IZExp_ErrorID.h"

//Services
//DECLARE_PMID(kServiceIDSpace, kZExpLoginUIService,							kIZExpPrefix + 0)

//Script Element IDs
	//Objects, Suites
//DECLARE_PMID(kScriptInfoIDSpace, kZPSuiteScriptElement,														kIZExpPrefix + 0 )
	//Events
	// Properties

// Message IDs:
//DECLARE_PMID(kMessageIDSpace, kZPLogin_BeforeLoginMsg,														kIZExpPrefix + 0 )

	//Send by -------.
	// Changed by is -------.
//DECLARE_PMID(kMessageIDSpace, kZPWSResponseAvailable,														kIZExpPrefix + 19 )

	//Sent by disclosure triangle, when it is clicked
	//Interface ITriStateControlData::kDefaultIID
DECLARE_PMID(kMessageIDSpace, kZPUIDisclosureBtnStateChangedMsg,											kIZExpPrefix + 30 )


#endif //_h_IZExpID_
