//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Panels/Tasks/CZPTasksPanelControlView.cpp $
//	$Revision: 2924 $
//	$Date: 2011-05-03 16:04:00 +0200 (Tue, 03 May 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 23-9-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes
#include "PalettePanelView.h"

//OS / ThirdParty includes

#include "IZPUIID.h"

//IZP Interfaces

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass PalettePanelView
class CZPTasksPanelControlView : public inheritClass
{
public:
						CZPTasksPanelControlView(IPMUnknown * boss);
	virtual				~CZPTasksPanelControlView();

	/**
		Before resizing a widget, the client can ask if the size makes
		sense by calling this method.
		@param dimensions The size of the adjusted panel is returned.
		@return  Return size constraints.
	*/
	PMPoint				ConstrainDimensions(
							const PMPoint& dimensions) const;

protected:
private:
};


CREATE_PERSIST_PMINTERFACE(CZPTasksPanelControlView, kZPTasksPanelControlViewImpl)

CZPTasksPanelControlView::CZPTasksPanelControlView(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPTasksPanelControlView::~CZPTasksPanelControlView()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// ConstrainDimensions
//----------------------------------------------------------------------------------------
PMPoint
CZPTasksPanelControlView::ConstrainDimensions(
	const PMPoint& desiredDimen) const
{
	PMPoint constrainedDim = desiredDimen;

	do
	{
		//Disable filtering. Redmine#1764, all filtering controls are made hidden.
		//Width with filtering controls is 740.
		if (constrainedDim.X() < 565)
			constrainedDim.X(565);
		else if (constrainedDim.Y() < 210)
			constrainedDim.Y(210);

	}while(false);

	return constrainedDim;
}
