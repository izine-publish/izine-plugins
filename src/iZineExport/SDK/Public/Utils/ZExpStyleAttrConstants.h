//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/SDK/Public/Utils/ZExpStyleAttrConstants.h $
//	$Revision: 3612 $
//	$Date: 2011-08-29 13:38:02 +0200 (Mon, 29 Aug 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 19-11-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_ZExpStyleAttrConstants_
#define _h_ZExpStyleAttrConstants_
#pragma once

class IAttrReport;
typedef void	(*HookStyleAttrValue)( const IAttrReport * inAttrReport, PMString & ioValue );

struct StyleAttrPair
{
	const wchar_t *		mAttrName;
	ClassID				mClassID;
	HookStyleAttrValue	mValueHook;
};

typedef enum enStyleAttr
{
	eStyleAttr_FontName = 0,
	eStyleAttr_FontStyle,
	eStyleAttr_FontSize,
	eStyleAttr_TextColor,
	eStyleAttr_Underline,
	//eStyleAttr_Strikethrough,
	eStyleAttr_HScale,
	eStyleAttr_VScale,
	eStyleAttr_Skew
};

#endif //_h_ZExpStyleAttrConstants_
