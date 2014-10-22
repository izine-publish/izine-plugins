//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/WS/Responses/CZExpGetConfigResponse.cpp $
//	$Revision: 3905 $
//	$Date: 2012-02-10 15:35:39 +0100 (Fri, 10 Feb 2012) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Aman Alam
//	Created: 28-11-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes
#include "K2Vector.tpp"

//OS / ThirdParty includes

#include "IZExpID.h"

//IZP Interfaces
#include "IZExpGlobalData.h"

//IZP General includes
#include "CZExpGetConfigResponse.h"

#pragma mark -

//----------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------
CZExpGetConfigResponse::CZExpGetConfigResponse()
: AZExpWSResponse( eResp_GetConfig )
,mDPIHighRes(GlobalDataDefault_DPIHighRes)
,mDPILowRes(GlobalDataDefault_DPILowRes)
,mExportTiff(GlobalDataDefault_ExportTiff)
,mExportPDF(GlobalDataDefault_ExportPDF)
,mExportLowRes(GlobalDataDefault_ExportLowRes)
,mExportPagePreview(GlobalDataDefault_ExportPagePreview)
,mStyleName(GlobalDataDefault_StyleName)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZExpGetConfigResponse::~CZExpGetConfigResponse()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetDPIHighRes
//----------------------------------------------------------------------------------------
const int
CZExpGetConfigResponse::GetDPIHighRes()const
{
	return mDPIHighRes;
}

//----------------------------------------------------------------------------------------
// SetDPIHighRes
//----------------------------------------------------------------------------------------
void
CZExpGetConfigResponse::SetDPIHighRes(
	const int					inDPIHighRes)
{
	mDPIHighRes = inDPIHighRes;
}

//----------------------------------------------------------------------------------------
// GetDPILowRes
//----------------------------------------------------------------------------------------
const int
CZExpGetConfigResponse::GetDPILowRes()const
{
	return mDPILowRes;
}

//----------------------------------------------------------------------------------------
// SetDPILowRes
//----------------------------------------------------------------------------------------
void
CZExpGetConfigResponse::SetDPILowRes(
	const int					inDPILowRes)
{
	mDPILowRes = inDPILowRes;
}

//----------------------------------------------------------------------------------------
// IsExportTiff
//----------------------------------------------------------------------------------------
const bool
CZExpGetConfigResponse::IsExportTiff()const
{
	return mExportTiff;
}

//----------------------------------------------------------------------------------------
// SetExportTiff
//----------------------------------------------------------------------------------------
void
CZExpGetConfigResponse::SetExportTiff(
	const bool					inExportTiff)
{
	mExportTiff = inExportTiff;
}

//----------------------------------------------------------------------------------------
// IsExportPDF
//----------------------------------------------------------------------------------------
const bool
CZExpGetConfigResponse::IsExportPDF()const
{
	return mExportPDF;
}

//----------------------------------------------------------------------------------------
// SetExportPDF
//----------------------------------------------------------------------------------------
void
CZExpGetConfigResponse::SetExportPDF(
	const bool					inExportPDF)
{
	mExportPDF = inExportPDF;
}

//----------------------------------------------------------------------------------------
// IsExportLowRes
//----------------------------------------------------------------------------------------
const bool
CZExpGetConfigResponse::IsExportLowRes()const
{
	return mExportLowRes;
}

//----------------------------------------------------------------------------------------
// SetExportLowRes
//----------------------------------------------------------------------------------------
void
CZExpGetConfigResponse::SetExportLowRes(
	const bool					inExportLowRes)
{
	mExportLowRes = inExportLowRes;
}

//----------------------------------------------------------------------------------------
// IsExportPagePreview
//----------------------------------------------------------------------------------------
const bool
CZExpGetConfigResponse::IsExportPagePreview()const
{
	return mExportPagePreview;
}

//----------------------------------------------------------------------------------------
// SetExportPagePreview
//----------------------------------------------------------------------------------------
void
CZExpGetConfigResponse::SetExportPagePreview(
	const bool					inExportPagePreview)
{
	mExportPagePreview = inExportPagePreview;
}

//----------------------------------------------------------------------------------------
// GetStyleName
//----------------------------------------------------------------------------------------
const PMString
CZExpGetConfigResponse::GetStyleName()const
{
	return mStyleName;
}

//----------------------------------------------------------------------------------------
// SetStyleName
//----------------------------------------------------------------------------------------
void
CZExpGetConfigResponse::SetStyleName(
	const PMString &			inStyleName)
{
	mStyleName = inStyleName;
}
