//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/Data/CZExpGlobalData.cpp $
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
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IPMStream.h"

//SDK General includes

//OS / ThirdParty includes

#include "IZExpID.h"

//IZP Interfaces
#include "IZExpGlobalData.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_GlobalData
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZExpGlobalData>
class CZExpGlobalData : public inheritClass
{
public:
						CZExpGlobalData(IPMUnknown * boss);
	virtual				~CZExpGlobalData();

	const int			GetDPIHighRes() const;
	void				SetDPIHighRes(
									const int 					inDPIHighRes);

	const int			GetDPILowRes() const;
	void				SetDPILowRes(
									const int 					inDPILowRes);

	const bool			IsExportTiff() const;
	void				SetExportTiff(
									const bool					inExportTiff);

	const bool			IsExportLowRes() const;
	void				SetExportLowRes(
									const bool					inExportLowRes);

	const bool			IsExportPagePreview() const;
	void				SetExportPagePreview(
									const bool					inExportPagePreview);

	const bool			IsExportPDF() const;
	void				SetExportPDF(
									const bool					inExportPDF);

	const PMString 		GetStyleName() const;
	void				SetStyleName(
									const PMString &			inStyleName);

	const PMString 		GetLastPluginId() const;
	void				SetLastPluginId(
									const PMString &			inLastPluginId);

protected:
private:
	int				mDPIHighRes;
	int				mDPILowRes;
	bool			mExportTiff;
	bool			mExportPDF;
	bool			mExportLowRes;
	bool			mExportPagePreview;
	PMString		mStyleName;
	PMString		mLastPluginId;
};

CREATE_PMINTERFACE(CZExpGlobalData, kZExpGlobalDataImpl)

CZExpGlobalData::CZExpGlobalData(IPMUnknown * boss)
: inheritClass(boss)
,mDPIHighRes(GlobalDataDefault_DPIHighRes)
,mDPILowRes(GlobalDataDefault_DPILowRes)
,mExportTiff(GlobalDataDefault_ExportTiff)
,mExportPDF(GlobalDataDefault_ExportPDF)
,mExportLowRes(GlobalDataDefault_ExportLowRes)
,mExportPagePreview(GlobalDataDefault_ExportPagePreview)
,mStyleName(GlobalDataDefault_StyleName)
,mLastPluginId(GlobalDataDefault_LastPluginId)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZExpGlobalData::~CZExpGlobalData()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetDPIHighRes
//----------------------------------------------------------------------------------------
const int
CZExpGlobalData::GetDPIHighRes()const
{
	return mDPIHighRes;
}

//----------------------------------------------------------------------------------------
// SetDPIHighRes
//----------------------------------------------------------------------------------------
void
CZExpGlobalData::SetDPIHighRes(
	const int					inDPIHighRes)
{
	mDPIHighRes = inDPIHighRes;
}

//----------------------------------------------------------------------------------------
// GetDPILowRes
//----------------------------------------------------------------------------------------
const int
CZExpGlobalData::GetDPILowRes()const
{
	return mDPILowRes;
}

//----------------------------------------------------------------------------------------
// SetDPILowRes
//----------------------------------------------------------------------------------------
void
CZExpGlobalData::SetDPILowRes(
	const int					inDPILowRes)
{
	mDPILowRes = inDPILowRes;
}

//----------------------------------------------------------------------------------------
// IsExportTiff
//----------------------------------------------------------------------------------------
const bool
CZExpGlobalData::IsExportTiff()const
{
	return mExportTiff;
}

//----------------------------------------------------------------------------------------
// SetExportTiff
//----------------------------------------------------------------------------------------
void
CZExpGlobalData::SetExportTiff(
	const bool					inExportTiff)
{
	mExportTiff = inExportTiff;
}

//----------------------------------------------------------------------------------------
// IsExportPDF
//----------------------------------------------------------------------------------------
const bool
CZExpGlobalData::IsExportPDF()const
{
	return mExportPDF;
}

//----------------------------------------------------------------------------------------
// SetExportPDF
//----------------------------------------------------------------------------------------
void
CZExpGlobalData::SetExportPDF(
	const bool					inExportPDF)
{
	mExportPDF = inExportPDF;
}

//----------------------------------------------------------------------------------------
// IsExportLowRes
//----------------------------------------------------------------------------------------
const bool
CZExpGlobalData::IsExportLowRes()const
{
	return mExportLowRes;
}

//----------------------------------------------------------------------------------------
// SetExportLowRes
//----------------------------------------------------------------------------------------
void
CZExpGlobalData::SetExportLowRes(
	const bool					inExportLowRes)
{
	mExportLowRes = inExportLowRes;
}

//----------------------------------------------------------------------------------------
// IsExportPagePreview
//----------------------------------------------------------------------------------------
const bool
CZExpGlobalData::IsExportPagePreview()const
{
	return mExportPagePreview;
}

//----------------------------------------------------------------------------------------
// SetExportPagePreview
//----------------------------------------------------------------------------------------
void
CZExpGlobalData::SetExportPagePreview(
	const bool					inExportPagePreview)
{
	mExportPagePreview = inExportPagePreview;
}

//----------------------------------------------------------------------------------------
// GetStyleName
//----------------------------------------------------------------------------------------
const PMString
CZExpGlobalData::GetStyleName()const
{
	return mStyleName;
}

//----------------------------------------------------------------------------------------
// SetStyleName
//----------------------------------------------------------------------------------------
void
CZExpGlobalData::SetStyleName(
	const PMString &			inStyleName)
{
	mStyleName = inStyleName;
}

//----------------------------------------------------------------------------------------
// GetLastPluginId
//----------------------------------------------------------------------------------------
const PMString
CZExpGlobalData::GetLastPluginId()const
{
	return mLastPluginId;
}

//----------------------------------------------------------------------------------------
// SetLastPluginId
//----------------------------------------------------------------------------------------
void
CZExpGlobalData::SetLastPluginId(
	const PMString &			inLastPluginId)
{
	mLastPluginId = inLastPluginId;
}