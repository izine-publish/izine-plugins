//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/SDK/Public/App/ZMConstants.h $
//	$Revision: 2866 $
//	$Date: 2011-04-27 15:47:40 +0200 (Wed, 27 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 6-5-2010
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_ZPConstants_
#define _h_ZPConstants_
#pragma once

#define MaxSize_AdID		40
#define MaxSize_EditionID	40
#define MaxSize_TitleID		40

#define STR_EmptyGUID "00000000-0000-0000-0000-000000000000"
#ifdef ZPBuild
#define ZPEXPORTS PLUGIN_DECL
#else
#define ZPEXPORTS PUBLIC_DECL
#endif

//Tags - Keep in containership
#define STRXMLTag_ClassWizardImposition		"iZine"			//"ClassWizardImposition"
#define STRXMLTag_Ads							"Ads"
#define STRXMLTag_Ad								"Ad"

//Attributes - Keep Sorted
#define STRXMLAttr_Comment			"comment"
#define STRXMLAttr_Customer			"customer"
#define STRXMLAttr_Height			"height"
#define STRXMLAttr_ID				"a-id"
#define STRXMLAttr_PURL				"p-url"
#define STRXMLAttr_RefKey			"refkey"
#define STRXMLAttr_URL				"url"
#define STRXMLAttr_Width			"widthmm"


extern ZPEXPORTS const PMString kEmptyGUID;
#endif //_h_ZPConstants_
