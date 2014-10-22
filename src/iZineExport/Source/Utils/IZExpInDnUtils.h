//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/Utils/IZExpInDnUtils.h $
//	$Revision: 3407 $
//	$Date: 2011-06-29 09:44:58 +0200 (Wed, 29 Jun 2011) $
//	$Author: aman.alam $
//	
//	Creator: Aman Alam
//	Created: 28-6-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZExpInDnUtils_
#define _h_IZExpInDnUtils_
#pragma once

class IDocument;

class IZExpInDnUtils : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZEXPINDNUTILS };

	virtual int			GetPDFStyleIndex(
							const PMString &				inPdfStyleName,
							const IDocument *				inDoc = nil ) const = 0;
	virtual int			GetCurrentPDFExportStyleIndex() const = 0;
	virtual const PMString &	GetCurrentPDFExportStyleName() const = 0;
	virtual PMString	GetPDFExportStyleName(
							int								inStyleIndex ) const = 0;
};


#endif //_h_IZExpInDnUtils_
