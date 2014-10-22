//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineExport/Source/Utils/CZExpPageItemUtils.cpp $
//	$Revision: 2792 $
//	$Date: 2011-04-12 09:39:18 +0200 (Tue, 12 Apr 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 15-11-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IGeometry.h"
#include "ISpread.h"
#include "ITransform.h"

//SDK General includes
#include "PMRect.h"
#include "TransformUtils.h"

//OS / ThirdParty includes

#include "IZExpID.h"

//IZP Interfaces
#include "IZExpPageItemUtils.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZExpPageItemUtils>
class CZExpPageItemUtils : public inheritClass
{
public:
						CZExpPageItemUtils(IPMUnknown * boss);
	virtual				~CZExpPageItemUtils();

	PMRect				ConvertPageItemInnerToSpread(
							const ISpread *				inSpread,
							const IGeometry *			inPageItemGeometry,
							const PMRect &				inPageItemRect ) const;

	PMRect				ConvertSpreadInnerToPageInner(
							const IGeometry *			inPageGeometry,
							const PMRect &				inRectOnSpread ) const;

	PMRect				ConvertCoordinatesPageItemToPage(
							const ISpread *				inSpread,
							const IGeometry *			inPageGeometry,
							const IGeometry *			inPageItemGeometry,
							const PMRect &				inPageItemRect ) const;

#pragma mark -
	void				GetPageItemToSpreadMatrix(
							const ISpread *				inSpread,
							const IGeometry *			inPageItemGeometry,
							PMMatrix &					oPageItemToSpreadMatrix ) const;
protected:
private:
};


CREATE_PMINTERFACE(CZExpPageItemUtils, kZExpPageItemUtilsImpl)

CZExpPageItemUtils::CZExpPageItemUtils(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZExpPageItemUtils::~CZExpPageItemUtils()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// ConvertPageItemInnerToSpread
//----------------------------------------------------------------------------------------
PMRect
CZExpPageItemUtils::ConvertPageItemInnerToSpread(
	const ISpread *				inSpread,
	const IGeometry *			inPageItemGeometry,
	const PMRect &				inPageItemRect) const
{
	PMRect toReturn(inPageItemRect);

#if 1
	InterfacePtr<const ITransform> spreadTransform( inSpread, UseDefaultIID() );
	ASSERT( inSpread );

	InterfacePtr<const ITransform> pageItemTransform( inPageItemGeometry, UseDefaultIID() );
	ASSERT( pageItemTransform );

	PMMatrix pageItemToSpreadMatrix;
	
	pageItemTransform->GetInnerToRootMatrix( &pageItemToSpreadMatrix, spreadTransform );

	pageItemToSpreadMatrix.Transform( &toReturn );
#else
	TransformInnerRectToParent( inPageItemGeometry, &toReturn );	//Page item parent is spread.
#endif

	return toReturn;
}

//----------------------------------------------------------------------------------------
// ConvertSpreadInnerToPageInner
//----------------------------------------------------------------------------------------
PMRect
CZExpPageItemUtils::ConvertSpreadInnerToPageInner(
	const IGeometry *			inPageGeometry,
	const PMRect &				inRectOnSpread) const
{
	PMRect toReturn(inRectOnSpread);

	TransformParentRectToInner( inPageGeometry, &toReturn );

	return toReturn;
}

//----------------------------------------------------------------------------------------
// ConvertCoordinatesPageItemToPage
//----------------------------------------------------------------------------------------
PMRect
CZExpPageItemUtils::ConvertCoordinatesPageItemToPage(
	const ISpread *				inSpread,
	const IGeometry *			inPageGeometry,
	const IGeometry *			inPageItemGeometry,
	const PMRect &				inPageItemRect) const
{
	PMRect toReturn(inPageItemRect);

	//TransformInnerRectToParent( inPageItemGeometry, &toReturn );	//Page item parent is spread.
	PMMatrix toSpreadMatrix;
	GetPageItemToSpreadMatrix(inSpread, inPageItemGeometry, toSpreadMatrix);
	
	toSpreadMatrix.Transform(&toReturn);

	TransformParentRectToInner( inPageGeometry, &toReturn );

	return toReturn;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetPageItemToSpreadMatrix
//----------------------------------------------------------------------------------------
void
CZExpPageItemUtils::GetPageItemToSpreadMatrix(
	const ISpread *				inSpread,
	const IGeometry *			inPageItemGeometry,
	PMMatrix &					oPageItemToSpreadMatrix) const
{
	InterfacePtr<const ITransform> spreadTransform( inSpread, UseDefaultIID() );
	ASSERT( inSpread );

	InterfacePtr<const ITransform> pageItemTransform( inPageItemGeometry, UseDefaultIID() );
	ASSERT( pageItemTransform );

	pageItemTransform->GetInnerToRootMatrix( &oPageItemToSpreadMatrix, spreadTransform );
}
