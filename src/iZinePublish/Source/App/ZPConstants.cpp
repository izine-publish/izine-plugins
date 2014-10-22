//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/App/ZPConstants.cpp $
//	$Revision: 4096 $
//	$Date: 2013-04-01 09:20:00 +0200 (Mon, 01 Apr 2013) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 6-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces

//IZP General includes
#include "ZPConstants.h"

#pragma mark -

const PMString kEmptyGUID( STR_EmptyGUID );

#if defined(InDnCS5) || defined(InDnCS5_5)
const PMString kAssetType_InDesignTemplateID(	"2326e21b-f4fb-40eb-acf6-46454d4d9f9f", PMString::kDontTranslateDuringCall);
const PMString kAssetType_InDesignDocumentID(	"a3871d22-7d8b-4d9f-901e-4c284b1a801e", PMString::kDontTranslateDuringCall);
const PMString kAssetType_InCopyTemplateID(		"0db0b0bc-25a8-4b5e-81b1-819336832d68", PMString::kDontTranslateDuringCall);
const PMString kAssetType_InCopyDocumentID(		"bdec0bf8-9591-4533-afea-467d0b340e92", PMString::kDontTranslateDuringCall);
const PMString kAssetType_InDesignStoryID(		"756779a5-bb56-4430-9e95-022226970095", PMString::kDontTranslateDuringCall);
const PMString kAssetType_InCopyAssignmentID(	"b4eee6f9-6061-481e-8a6e-f89dfbebad4e", PMString::kDontTranslateDuringCall);

const PMString kAssetFileExt_InDesignTemplate(	"indt", PMString::kDontTranslateDuringCall);
const PMString kAssetFileExt_InDesignDocument(	"indd", PMString::kDontTranslateDuringCall);
const PMString kAssetFileExt_InCopyTemplate(	"icmt", PMString::kDontTranslateDuringCall);
const PMString kAssetFileExt_InCopyDocument(	"icml", PMString::kDontTranslateDuringCall);
const PMString kAssetFileExt_InCopyAssignment(	"icma", PMString::kDontTranslateDuringCall);

const PMString kPageTypeLeft(	"fdf80540-a121-4456-8d14-67519450f4cf", PMString::kDontTranslateDuringCall);
const PMString kPageTypeRight(	"3514c6ba-f092-47b8-8afa-87d1cf23322e", PMString::kDontTranslateDuringCall);
const PMString kPageTypeUnisex(	"e282278d-fdb0-4c85-91c5-a745232e76c2", PMString::kDontTranslateDuringCall);


const PMString kStatusState_Design(		"DA43364A-0F49-4735-B88B-674B6D20639B", PMString::kDontTranslateDuringCall);
const PMString kStatusState_Editing(	"AB136AE9-728A-421D-873B-EAE32D5E33C7", PMString::kDontTranslateDuringCall);
const PMString kStatusState_Editorial(	"E0501419-3C1B-4833-9FCE-EB8138F9C304", PMString::kDontTranslateDuringCall);
const PMString kStatusState_Final(		"A40327CF-5190-4694-94ED-BABAEDA98C3F", PMString::kDontTranslateDuringCall);

const PMString kTitleStatus_Assigned(		"DD6D1D97-FCC6-4333-AF3E-CF56E4FCDF8D", PMString::kDontTranslateDuringCall);
const PMString kTitleStatus_Unassigned(		"6C120298-A2AD-49E5-9A21-17D0907EBEB4", PMString::kDontTranslateDuringCall);
const PMString kTitleStatus_Template(		"406C6E06-309F-4EB9-8ACC-D4B367328590", PMString::kDontTranslateDuringCall);

const PMString kURIScheme("IZPub", PMString::kDontTranslateDuringCall);
#else
const PMString kAssetType_InDesignTemplateID(	"2326e21b-f4fb-40eb-acf6-46454d4d9f9f", -1, PMString::kNoTranslate);
const PMString kAssetType_InDesignDocumentID(	"a3871d22-7d8b-4d9f-901e-4c284b1a801e", -1, PMString::kNoTranslate);
const PMString kAssetType_InCopyTemplateID(		"0db0b0bc-25a8-4b5e-81b1-819336832d68", -1, PMString::kNoTranslate);
const PMString kAssetType_InCopyDocumentID(		"bdec0bf8-9591-4533-afea-467d0b340e92", -1, PMString::kNoTranslate);
const PMString kAssetType_InDesignStoryID(		"756779a5-bb56-4430-9e95-022226970095", -1, PMString::kNoTranslate);
const PMString kAssetType_InCopyAssignmentID(	"b4eee6f9-6061-481e-8a6e-f89dfbebad4e", -1, PMString::kNoTranslate);

const PMString kAssetFileExt_InDesignTemplate(	"indt", -1, PMString::kNoTranslate);
const PMString kAssetFileExt_InDesignDocument(	"indd", -1, PMString::kNoTranslate);
const PMString kAssetFileExt_InCopyTemplate(	"inct", -1, PMString::kNoTranslate);
const PMString kAssetFileExt_InCopyDocument(	"incx", -1, PMString::kNoTranslate);
const PMString kAssetFileExt_InCopyAssignment(	"inca", -1, PMString::kNoTranslate);

const PMString kPageTypeLeft(	"fdf80540-a121-4456-8d14-67519450f4cf", -1, PMString::kNoTranslate);
const PMString kPageTypeRight(	"3514c6ba-f092-47b8-8afa-87d1cf23322e", -1, PMString::kNoTranslate);
const PMString kPageTypeUnisex(	"e282278d-fdb0-4c85-91c5-a745232e76c2", -1, PMString::kNoTranslate);


//Old Pln values status values.
//#define STRAssetStatus_Final		"22F8ACBD-8E7C-4437-A767-E5773DB76083"
//#define STRAssetStatus_Editing	"0B75FC2D-EDA3-4A56-A6EA-27DC9970E80B"
//#define STRAssetStatus_Editorial	"69C76023-3CF2-4A58-90D7-AD410435F47F"
//#define STRAssetStatus_Design		"A350E418-E5B0-4FE5-92EE-66D9814E29C2"
const PMString kStatusState_Design(		"DA43364A-0F49-4735-B88B-674B6D20639B", -1, PMString::kNoTranslate);
const PMString kStatusState_Editing(	"AB136AE9-728A-421D-873B-EAE32D5E33C7", -1, PMString::kNoTranslate);
const PMString kStatusState_Editorial(	"E0501419-3C1B-4833-9FCE-EB8138F9C304", -1, PMString::kNoTranslate);
const PMString kStatusState_Final(		"A40327CF-5190-4694-94ED-BABAEDA98C3F", -1, PMString::kNoTranslate);

const PMString kTitleStatus_Assigned(		"DD6D1D97-FCC6-4333-AF3E-CF56E4FCDF8D", -1, PMString::kNoTranslate);
const PMString kTitleStatus_Unassigned(		"6C120298-A2AD-49E5-9A21-17D0907EBEB4", -1, PMString::kNoTranslate);
const PMString kTitleStatus_Template(		"406C6E06-309F-4EB9-8ACC-D4B367328590", -1, PMString::kNoTranslate);
#endif
