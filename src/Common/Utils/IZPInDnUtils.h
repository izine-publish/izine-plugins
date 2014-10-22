//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/Common/Utils/IZPInDnUtils.h $
//	$Revision: 4000 $
//	$Date: 2012-04-03 11:39:02 +0200 (Tue, 03 Apr 2012) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 1-6-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZPInDnUtils_
#define _h_IZPInDnUtils_
#pragma once

class IDocument;

class IZPInDnUtils : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZPINDNUTILS };

	virtual bool16		IsHostAppInCopy() const = 0;
	virtual bool16		IsHostAppInDesign() const = 0;
	virtual bool16		IsHostAppWithUI() const = 0; //True if incopy or indesign.
	virtual bool16		IsHostAppInDesignServer() const = 0;

	virtual int			GetPDFStyleIndex(
							const PMString &				inPdfStyleName,
							const IDocument *				inDoc = nil ) const = 0;
	virtual UIDRef		GetPDFExportStyle(
							int								inStyleIndex ) const = 0;
	virtual int			GetCurrentPDFExportStyleIndex() const = 0;
	virtual const PMString &	GetCurrentPDFExportStyleName() const = 0;
	virtual PMString	GetPDFExportStyleName(
							int								inStyleIndex ) const = 0;
};


#endif //_h_IZPInDnUtils_
