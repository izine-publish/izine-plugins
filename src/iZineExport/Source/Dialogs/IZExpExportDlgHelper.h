//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/Dialogs/IZExpExportDlgHelper.h $
//	$Revision: 3682 $
//	$Date: 2011-11-28 12:05:31 +0100 (Mon, 28 Nov 2011) $
//	$Author: aman.alam $
//	
//	Creator: Aman Alam
//	Created: 15-6-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZExpExportDlgHelper_
#define _h_IZExpExportDlgHelper_
#pragma once


class IZExpExportDlgHelper : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZEXPEXPORTDLGHELPER };

	virtual void					FillTitles() const = 0;
	virtual void					FillEditions() const = 0;
	virtual const PMString &		GetTitleId() const = 0;
	virtual const PMString &		GetEditionId() const = 0;

	virtual void					GetConfig() const = 0;
};


#endif //_h_IZExpExportDlgHelper_
