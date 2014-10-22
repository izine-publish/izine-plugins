//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/Data/IZExpGlobalData.h $
//	$Revision: 3930 $
//	$Date: 2012-02-16 08:46:11 +0100 (Thu, 16 Feb 2012) $
//	$Author: aman.alam $
//	
//	Creator: Aman Alam
//	Created: 28-11-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZExpGlobalData_
#define _h_IZExpGlobalData_
#pragma once

#define GlobalDataDefault_DPIHighRes	72
#define GlobalDataDefault_DPILowRes		72
#define GlobalDataDefault_ExportTiff	false
#define GlobalDataDefault_ExportLowRes	true
#define GlobalDataDefault_ExportPagePreview	true
#define GlobalDataDefault_ExportPDF		false
#define GlobalDataDefault_StyleName		""
#define GlobalDataDefault_LastPluginId	""

class IZExpGlobalData : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZEXPGLOBALDATA };

	virtual const int			GetDPIHighRes() const = 0;
	virtual void				SetDPIHighRes(
									const int 					inDPIHighRes) = 0;

	virtual const int			GetDPILowRes() const = 0;
	virtual void				SetDPILowRes(
									const int 					inDPILowRes) = 0;

	virtual const bool			IsExportTiff() const = 0;
	virtual void				SetExportTiff(
									const bool					inExportTiff) = 0;

	virtual const bool			IsExportLowRes() const = 0;
	virtual void				SetExportLowRes(
									const bool					inExportLowRes) = 0;

	virtual const bool			IsExportPagePreview() const = 0;
	virtual void				SetExportPagePreview(
									const bool					inExportPagePreview) = 0;

	virtual const bool			IsExportPDF() const = 0;
	virtual void				SetExportPDF(
									const bool					inExportPDF) = 0;

	virtual const PMString 		GetStyleName() const = 0;
	virtual void				SetStyleName(
									const PMString &			inStyleName) = 0;

	virtual const PMString 		GetLastPluginId() const = 0;
	virtual void				SetLastPluginId(
									const PMString &			inLastPluginId) = 0;

};


#endif //_h_IZExpGlobalData_
