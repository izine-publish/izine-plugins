//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZineMedia/Source/Panels/Ads/CZMAdsPanelHelper.cpp $
//	$Revision: 2868 $
//	$Date: 2011-04-27 15:50:50 +0200 (Wed, 27 Apr 2011) $
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
#include "IControlView.h"
#include "IPanelControlData.h"
#include "ITreeViewController.h"

//SDK General includes
#include "K2Vector.tpp"

//OS / ThirdParty includes

#include "IZMediaID.h"

//IZP Interfaces
#include "IZMAdsPanelHelper.h"

//IZP General includes
#include "ZMTreeNodePtr.h"
#include "ZMDataHelper.h"
#include "ZMAdsTVNodeID.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZMAdsPanelHelper>
class CZMAdsPanelHelper : public inheritClass
{
public:
						CZMAdsPanelHelper(IPMUnknown * boss);
	virtual				~CZMAdsPanelHelper();

	const IStringData *	GetSelectedAd() const;
protected:
	IControlView *		GetWidgetView(
							const WidgetID &			inWidgetID) const;
private:
};


CREATE_PMINTERFACE(CZMAdsPanelHelper, kZMAdsPanelHelperImpl)

CZMAdsPanelHelper::CZMAdsPanelHelper(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZMAdsPanelHelper::~CZMAdsPanelHelper()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetSelectedAd
//----------------------------------------------------------------------------------------
const IStringData *
CZMAdsPanelHelper::GetSelectedAd()const
{
	const IStringData *	toReturn = nil;
	do
	{
		IControlView * treeControlView = this->GetWidgetView( kZMUIAdsPanelLstAdsWidgetID );
		ASSERT( treeControlView );
		InterfacePtr<const ITreeViewController> tvController( treeControlView, UseDefaultIID());
		ASSERT( tvController );
		NodeIDList selectedNodes;
		tvController->GetSelectedItems( selectedNodes );
		if( selectedNodes.Length() == 1 )
		{
			ZMTreeNodePtr<ZMAdsTVNodeID> nodeID(selectedNodes[0]);
			if (nodeID != nil)
			{
				const PMString & theAdID = nodeID->GetID();//("55a7be8e-8c82-4a07-a467-f54497b50c10");
			//TODO: media porting
			///	toReturn = ZMDataHelper::GetAdByID( theAdID );
			}
		}
	}while(kFalse);

	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetWidgetView
//----------------------------------------------------------------------------------------
IControlView *
CZMAdsPanelHelper::GetWidgetView(
	const WidgetID &			inWidgetID) const
{
	InterfacePtr<const IPanelControlData> selfPanelControlData(this, UseDefaultIID());
	ASSERT(selfPanelControlData);
	if (selfPanelControlData)
	{
		IControlView* widgetView = selfPanelControlData->FindWidget( inWidgetID );
		ASSERT(widgetView);
		return widgetView;
	}
	return NULL;
}
