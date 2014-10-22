//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/SDK/Public/Utils/ZExpXMLConstants.h $
//	$Revision: 4034 $
//	$Date: 2012-05-30 11:44:13 +0200 (Wed, 30 May 2012) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 18-11-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_ZExpXMLConstants_
#define _h_ZExpXMLConstants_
#pragma once

//Tags - Keep in containership
#define STRXMLTag_Doc				"iZineDoc"
#define STRXMLTag_Spreads				"Spreads"
#define STRXMLTag_Spread					"Spread"
#define STRXMLTag_Layers						"Layers"
#define STRXMLTag_SpreadLayer						"SpreadLayer"
#define STRXMLTag_Pages							"Pages"
#define STRXMLTag_Page								"Page"
#define STRXMLTag_TextFrame								"TextFrame"
#define STRXMLTag_PathInfo									"Shape"
#define STRXMLTag_Path											"Path"
#define STRXMLTag_Point											"Point"
#define STRXMLTag_GraphicStyle								"GStyle"
																//"Attr" //Many child
#define STRXMLTag_TextFrameColumn							"TextFrameColumn"
#define STRXMLTag_TextLine										"TextLine"
#define STRXMLTag_TextRun											"TextRun"
#define STRXMLTag_Glyph													"Glyph"
#define STRXMLTag_Text													"Text"
#define STRXMLTag_StyleOverrideChar										"CharStyleOverrides"
#define STRXMLTag_StyleOverridePara										"ParaStyleOverrides"
#define STRXMLTag_ImageFrame							"ImageFrame"
															//"Shape"
#define STRXMLTag_Stories				"Stories"
#define STRXMLTag_Story						"TextStory"
#define STRXMLTag_ParaStyles			"ParaStyles"
#define STRXMLTag_Style						"Style"
#define STRXMLTag_StyleAttribute				"Attr" //Many child
											//"Style"
#define STRXMLTag_CharStyles			"CharStyles"
											//"Style"
#define STRXMLTag_Colors				"Colors"
#define STRXMLTag_ColorInfo					"ColorInfo"
#define STRXMLTag_Articles				"Articles"
#define STRXMLTag_Article					"Article"
#define STRXMLTag_ArticleItem					"ArticleItem"

//Attributes - Keep Sorted
#define STRXMLAttr_ClassID			"ClassID"
#define STRXMLAttr_EndWithHyphen	"EndsWithHyphen"
#define STRXMLAttr_Height			"Height"
#define STRXMLAttr_ID				"ID"
#define STRXMLAttr_Index			"Index"
#define STRXMLAttr_IsDropCap		"IsDropCap"
#define STRXMLAttr_IsClosed			"IsClosed"
#define STRXMLAttr_IsMasterSpread	"IsMasterSpread"
#define STRXMLAttr_IsVisible		"IsVisible"
#define STRXMLAttr_LayerID			"LayerID"
#define STRXMLAttr_Name				"Name"
#define STRXMLAttr_PageIndex		"Index"
#define STRXMLAttr_PathCount		"PathCount"
#define STRXMLAttr_Points			"Points"
#define STRXMLAttr_PointCount		"PointCount"
#define STRXMLAttr_SegementCount	"SegmentCount"
#define STRXMLAttr_ParentStyleID	"ParentStyleID"
#define STRXMLAttr_RotationAngle	"RotationAngle"
#define STRXMLAttr_ScaleX			"ScaleX"
#define STRXMLAttr_ScaleY			"ScaleY"
#define STRXMLAttr_ScaleFontX		"ScaleFontX"
#define STRXMLAttr_ScaleFontY		"ScaleFontY"
#define STRXMLAttr_SectionIndex		"SectionIndex"
#define STRXMLAttr_SectionName		"SectionName"
#define STRXMLAttr_StoryID			"StoryID"
#define STRXMLAttr_StyleIDChar		"CharStyleID"
#define STRXMLAttr_StyleIDPara		"ParaStyleID"
#define STRXMLAttr_TextIndexEnd		"TextIndexEnd"
#define STRXMLAttr_TextIndexStart	"TextIndexStart"
#define STRXMLAttr_Width			"Width"
#define STRXMLAttr_Value			"Value"
#define STRXMLAttr_X				"X"
#define STRXMLAttr_Y				"Y"
#define STRXMLAttr_Zorder			"Zorder"


#define _TOWSTRING(x) L##x
#define TOWSTRING(x) _TOWSTRING(x)

#endif //_h_ZExpXMLConstants_
