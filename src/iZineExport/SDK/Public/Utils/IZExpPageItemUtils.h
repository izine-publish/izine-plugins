//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/SDK/Public/Utils/IZExpPageItemUtils.h $
//	$Revision: 2547 $
//	$Date: 2010-11-22 13:10:50 +0100 (Mon, 22 Nov 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 15-11-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#ifndef _h_IZExpPageItemUtils_
#define _h_IZExpPageItemUtils_
#pragma once

class ISpread;
class IGeometry;

class IZExpPageItemUtils : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IZEXPPAGEITEMUTILS };

	virtual PMRect		ConvertPageItemInnerToSpread(
							const ISpread *				inSpread,
							const IGeometry *			inPageItemGeometry,
							const PMRect &				inPageItemRect ) const = 0;

	virtual PMRect		ConvertSpreadInnerToPageInner(
							const IGeometry *			inPageGeometry,
							const PMRect &				inRectOnSpread ) const = 0;

	virtual PMRect		ConvertCoordinatesPageItemToPage(
							const ISpread *				inSpread,
							const IGeometry *			inPageGeometry,
							const IGeometry *			inPageItemGeometry,
							const PMRect &				inPageItemRect ) const = 0;

#pragma mark -
	virtual void		GetPageItemToSpreadMatrix(
							const ISpread *				inSpread,
							const IGeometry *			inPageItemGeometry,
							PMMatrix &					oPageItemToSpreadMatrix ) const = 0;
};


#endif //_h_IZExpPageItemUtils_
