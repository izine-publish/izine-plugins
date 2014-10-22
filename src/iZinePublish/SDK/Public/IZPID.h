//========================================================================================
//  
//  $HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/SDK/Public/IZPID.h $
//  
//  Owner: Rahul Rastogi
//  
//  $Author: mindfire $
//  
//  $Date: 2012-09-18 15:57:56 +0200 (Tue, 18 Sep 2012) $
//  
//  $Revision: 4052 $
//..
//	$Copyright: 2008-2010 iZine Solutions. All rights reserved. $
//========================================================================================


#ifndef __IZPID_h__
#define __IZPID_h__

#include "SDKDef.h"
#ifndef _iZineVersion_h_
#include "iZineVersion.h"
#endif //_iZineVersion_h_

// Company:
#define kIZPCompanyKey			"kIZPPlugInCompanyKey"		// Company name used internally for menu paths and the like. Must be globally unique, only A-Z, 0-9, space and "_".
#define kIZPCompanyValue		"iZine Publish"				// Company name displayed externally.

// Plug-in:
#define kIZPPluginName			"iZine"					// Name of this plug-in.
#define kIZPPluginFileName		"izine"
#define kIZPProductName			"iZine Publish"

#define kIZPPrefixNumber	0x130700	// iZine Publish prefix number
#define kIZPOldPrefix		0x61000		//Un registered ID,used before 2010 Feb 01.
//#define kIZPUIPrefixNumber	0x14d500	// prefix number for iZine Publish UI
//0x61000	//Unregistered ID
//0x130700	//iZine Publish
//0x14d500	//iZine InDesign UI
//0x14d600	//iZine InDesign Server
//0xe9400

#define kIZPVersion kIZPPluginVersionString
#define kIZPAuthor		"iZine Publish"					// Author of this plug-in (for the About Box).
// #define kIZPAuthor		"izine-publish.net"

// Plug-in Prefix: (please change kIZPPrefixNumber above to modify the prefix.)
#define kIZPPrefix		RezLong(kIZPPrefixNumber)			// The unique numeric prefix for all object model IDs for this plug-in.
#define kIZPStringPrefix	IZPMAKESTR(kIZPPrefixNumber)	// The string equivalent of the unique prefix number for  this plug-in.

// Missing plug-in: (see ExtraPluginInfo resource)
#define kIZPMissingPluginURLValue		"http://www.izine-publish.net/" // URL displayed in Missing Plug-in dialog
#define kIZPMissingPluginAlertValue		"One of iZine Publish plugins is missing. Please add it to open the documents." // Message displayed in Missing Plug-in dialog - provide a string that instructs user how to solve their missing plug-in problem

#define kIZPCopyrightShortValue	"Copyright 2008-2010 iZine Solutions."
#define kIZPCopyrightValue	kIZPCopyrightShortValue " All Rights Reserved."
#define kIZPSupportURL		"http://www.izine-publish.com/"


// PluginID:
DECLARE_PMID(kPlugInIDSpace, kIZPPluginID, kIZPPrefix + 0)

// ClassID/Interface/Impl
#include "IZP_ClassDefID.h"


DECLARE_PMID(kServiceIDSpace, kZPLoginUIService,							kIZPPrefix + 0)

//Script Element IDs
	//Objects, Suites
DECLARE_PMID(kScriptInfoIDSpace, kZPSuiteScriptElement,														kIZPPrefix + 0 )
DECLARE_PMID(kScriptInfoIDSpace, kZPAssetDataObjectScriptElement,											kIZPPrefix + 1 )

	//Events
//DECLARE_PMID(kScriptInfoIDSpace, kIZPEventScriptElement,						kIZPPrefix + 30 )
	// Properties
DECLARE_PMID(kScriptInfoIDSpace, kZPAssetDataPropertyScriptElement,											kIZPPrefix + 60 )
DECLARE_PMID(kScriptInfoIDSpace, kZPAssetIDPropertyScriptElement,											kIZPPrefix + 61 )
DECLARE_PMID(kScriptInfoIDSpace, kZPTitleIDPropertyScriptElement,											kIZPPrefix + 62 )
DECLARE_PMID(kScriptInfoIDSpace, kZPEditionIDPropertyScriptElement,											kIZPPrefix + 63 )
DECLARE_PMID(kScriptInfoIDSpace, kAssetDataINXPoliciesMetadataScriptElement,								kIZPPrefix + 64 )

	//AM Data IDs
#define kAMServiceDataIDSpace		kIZPPrefix + 1
DECLARE_PMID(kAMServiceDataIDSpace, kZPAMServiceDataAssetID,												kIZPPrefix + 1 )
DECLARE_PMID(kAMServiceDataIDSpace, kZPAMServiceDataTitleID,												kIZPPrefix + 2 )
DECLARE_PMID(kAMServiceDataIDSpace, kZPAMServiceDataEditionID,												kIZPPrefix + 3 )
DECLARE_PMID(kAMServiceDataIDSpace, kZPAMServiceDataStatusID,												kIZPPrefix + 4 )

// Message IDs:
#include "IZP_MessageID.h"
#include "IZP_ErrorID.h"

#include "IZP_StringKeys.h"

//PNGA resource ID from Assignment UI.apln
#define kZPLinkedStoryState_MissingIconRsrcID				1004
#define kZPLinkedStoryState_TextNormalIconRsrcID			1005
#define kZPLinkedStoryState_TextOutOfDateIconRsrcID			1006
#define kZPLinkedStoryState_ImageNormalIconRsrcID			1007
#define kZPLinkedStoryState_ImageOutOfStateIconRsrcID		1008

//PNGA resource ID from Assignment UI.apln
#define kZPAssignmentState_OutOfDateIconRsrcID				1020
#define kZPAssignmentState_PackagedIconRsrcID				1021
#define kZPAssignmentState_PackageOutOfDateIconRsrcID		1022

//PNGA resource ID from Assignment UI.apln
#define kZPLinkEditState_NotEditingIconRsrcID				1999
#define kZPLinkEditState_EditingHereIconRsrcID				1001
#define kZPLinkEditState_EditingRemoteIconRsrcID			1002
#define kZPLinkEditState_EditingByOtherIconRsrcID			1009

#endif // __IZPID_h__

