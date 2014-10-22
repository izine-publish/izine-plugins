//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/SDK/Public/iZMediaUI_ResourceID.h $
//	$Revision: 2754 $
//	$Date: 2011-04-08 12:53:24 +0200 (Fri, 08 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 1-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_iZMediaUI_ResourceID_
#define _h_iZMediaUI_ResourceID_
#pragma once

// Resources
//Note: Resources that are localised must have IDs apart by 50.
//i.e max value defined in pmlocaleids.h for index_xxxx values.
//currently this max value is 0x0024 i.e 36.
	//Panels
#define kZMUIPrefsPanelResourceID		10000
#define kZMUIAdsPanelResourceID			10100
	//Dialogs
#define kZMUIAlertDialogResourceID		11300
	//Widgets
#define kIZMTVNodeWidgetRsrcID					12000
#define kIZMAdsTVAdNodeWidgetRsrcID				12003
#define kIZMAdsTVAdFileNodeWidgetRsrcID			12004
#define kZMUITVHeaderColumnWidgetRsrcID			12450
#define kZMUITVColumnTxtWidgetRsrcID			12500
#define kZMUITVColumnIconWidgetRsrcID			12550

	//Icons
#define kIZMAscIconRsrcID				12000
#define kIZMDescIconRsrcID				12001
#define kIZMBlankIconRsrcID				12002	//Not Used

#define kIZMNodeType_AdIconRsrcID		13000

#endif //_h_iZMediaUI_ResourceID_
