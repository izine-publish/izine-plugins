//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/SDK/Public/Utils/IZExpDocUtils.h $
//	$Revision: 3730 $
//	$Date: 2012-01-04 13:55:40 +0100 (Wed, 04 Jan 2012) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 23-11-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZExpDocUtils_
#define _h_IZExpDocUtils_
#pragma once

class IDocument;

class IZExpDocUtils : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZEXPDOCUTILS };

	virtual ErrorCode		CreatePageItemSnapshot(
								const UIDRef &				inPageItemRef,
								IPMStream *					oStream,
								bool						inLowRes,
								int32						inDrawingFlags,
								int32						inDesiredRes = 72 ) const = 0;
	virtual ErrorCode		CreatePageItemGIFSnapshot(
								const UIDRef &				inPageItemRef,
								IPMStream *					oStream,
								bool						inLowRes,
								int32						inDrawingFlags,
								int32						inDesiredRes = 72 ) const = 0;
	virtual ErrorCode		CreatePageItemTIFFSnapshot(
								const UIDRef &				inPageItemRef,
								IPMStream *					oStream,
								bool						inLowRes,
								int32						inDrawingFlags,
								int32						inDesiredRes = 72 ) const = 0;
#ifdef InDnCS5
	virtual ErrorCode		CreatePageItemPNGSnapshot(
								const UIDRef &				inPageItemRef,
								IPMStream *					oStream,
								bool						inLowRes,
								int32						inDrawingFlags,
								int32						inDesiredRes = 72 ) const = 0;
#endif
	virtual ErrorCode		CreatePDF(
								const IDocument *			inDoc,
								const IDFile &				inPdfFile,
								const PMString &			inPdfStyleName,
								bool						inUseDefaultStyleIfNotFound = true) const = 0;

	virtual ErrorCode		ShowHideDocLayers (
								const IDocument *			inDoc,
								const UIDList &				inDocLayerIDs,
								bool						inShow ) const = 0;
};


#endif //_h_IZExpDocUtils_
