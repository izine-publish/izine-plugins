//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/SDK/Public/Utils/IZExpStoryUtils.h $
//	$Revision: 2561 $
//	$Date: 2010-11-26 15:11:38 +0100 (Fri, 26 Nov 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 16-11-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZExpStoryUtils_
#define _h_IZExpStoryUtils_
#pragma once

class ITextModel;

class IZExpStoryUtils : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZEXPSTORYUTILS };

	virtual void		GetStoryTextContent(
							const ITextModel *			inTextModel,
							WideString &				oTextContent,
							int							inStartIndex = 0,
							int							inEndIndex = -1 ) const = 0;

	virtual void		FixStoryTextIndexWithInlineItems(
							const ITextModel *			inTextModel,
							WideString &				ioTextContent,
							int							inStartIndex = 0,
							int							inEndIndex = -1 ) const = 0;
};


#endif //_h_IZExpStoryUtils_
