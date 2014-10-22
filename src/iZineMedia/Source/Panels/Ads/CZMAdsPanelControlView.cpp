//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Panels/Ads/CZMAdsPanelControlView.cpp $
//	$Revision: 2713 $
//	$Date: 2011-04-06 15:37:25 +0200 (Wed, 06 Apr 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 6-4-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
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

#include "IZMediaID.h"

//IZP Interfaces

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_PanelAssets
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass PalettePanelView
class CZMAdsPanelControlView : public inheritClass
{
public:
						CZMAdsPanelControlView(IPMUnknown * boss);
	virtual				~CZMAdsPanelControlView();
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


CREATE_PERSIST_PMINTERFACE(CZMAdsPanelControlView, kZMAdsPanelControlViewImpl)

CZMAdsPanelControlView::CZMAdsPanelControlView(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZMAdsPanelControlView::~CZMAdsPanelControlView()
{

}

//----------------------------------------------------------------------------------------
// DoPostCreate
//----------------------------------------------------------------------------------------
void
CZMAdsPanelControlView::DoPostCreate()
{
	LogFunctionEnterExit;
	
	inheritClass::DoPostCreate();
}
//----------------------------------------------------------------------------------------
// ConstrainDimensions
//----------------------------------------------------------------------------------------
PMPoint
CZMAdsPanelControlView::ConstrainDimensions(
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
CZMAdsPanelControlView::Resize(
	const PMPoint& dimensions, bool16 invalidate)
{
	inheritClass::Resize(dimensions, invalidate);
}

