//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/Common/Utils/ZPStringUtils.h $
//	$Revision: 2590 $
//	$Date: 2011-03-09 11:31:56 +0100 (Wed, 09 Mar 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Aman Alam 
//	Created: 09-03-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_ZPStringUtils_
#define _h_ZPStringUtils_
#pragma once

#pragma mark -
namespace ZPStringUtils
{
	void				ReplaceControlChars(
							WideString &								ioText);

	void				EscapeXMLChars(	//Not being used.
							WideString *								inText,
							WideString *								oText);
}

#endif //_h_ZPStringUtils_
