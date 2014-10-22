//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Panels/Assets/CZPAssetsPanelControlView.cpp $
//	$Revision: 1113 $
//	$Date: 2010-05-05 11:14:21 +0200 (Wed, 05 May 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 28-4-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IPanelControlData.h"
#include "IApplication.h"

//SDK General includes
#include "PalettePanelView.h"

//OS / ThirdParty includes

#include "IZPUIID.h"

//IZP Interfaces

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_PanelAssets
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass PalettePanelView
class ZPAssetsPanelControlView : public inheritClass
{
public:
						ZPAssetsPanelControlView(IPMUnknown * boss);
	virtual				~ZPAssetsPanelControlView();
		/**
		Called after the widget hierarchy is created. Can be
		overriden to do initialization that may need the hierarchy
		to perform.
	*/
	virtual void		DoPostCreate ();

	/**
		Before resizing a widget, the client can ask if the size makes
		sense by calling this method.
		@param dimensions The size of the adjusted panel is returned.
		@return  Return size constraints.
	*/
	virtual PMPoint		ConstrainDimensions(
							const PMPoint& dimensions) const;

	/**
		Resize the widget.
		@param dimensions Specify new dimensions for the panel view.
		@param invalidate Set to kTrue if you want to invalidate the view.
	*/
	virtual void		Resize(
							const PMPoint& dimensions, bool16 invalidate);

protected:
private:
};


CREATE_PERSIST_PMINTERFACE(ZPAssetsPanelControlView, kZPAssetsPanelControlViewImpl)

ZPAssetsPanelControlView::ZPAssetsPanelControlView(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
ZPAssetsPanelControlView::~ZPAssetsPanelControlView()
{

}

//----------------------------------------------------------------------------------------
// DoPostCreate
//----------------------------------------------------------------------------------------
void
ZPAssetsPanelControlView::DoPostCreate()
{
	LogFunctionEnterExit;
	
	inheritClass::DoPostCreate();
}
//----------------------------------------------------------------------------------------
// ConstrainDimensions
//----------------------------------------------------------------------------------------
PMPoint
ZPAssetsPanelControlView::ConstrainDimensions(
	const PMPoint& desiredDimen) const
{
	PMPoint constrainedDim = desiredDimen;

	do
	{
		if (constrainedDim.X() < 640)
			constrainedDim.X(640);
		else if (constrainedDim.Y() < 240)
			constrainedDim.Y(240);

	}while(false);

	return constrainedDim;
}

//----------------------------------------------------------------------------------------
// Resize
//----------------------------------------------------------------------------------------
void
ZPAssetsPanelControlView::Resize(
	const PMPoint& dimensions, bool16 invalidate)
{
	inheritClass::Resize(dimensions, invalidate);
}

