//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Panels/Assets/CZPAssetsPanelHelper.cpp $
//	$Revision: 2462 $
//	$Date: 2010-11-08 11:47:08 +0100 (Mon, 08 Nov 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 8-11-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IControlView.h"
#include "IPanelControlData.h"
#include "ITreeViewController.h"

//SDK General includes

//OS / ThirdParty includes

#include "IZPUIID.h"

//IZP Interfaces
#include "IZPAssetsPanelHelper.h"

//IZP General includes
#include "ZPTreeNodePtr.h"
#include "ZPDataHelper.h"
#include "ZPAssetsTVNodeID.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZPAssetsPanelHelper>
class CZPAssetsPanelHelper : public inheritClass
{
public:
						CZPAssetsPanelHelper(IPMUnknown * boss);
	virtual				~CZPAssetsPanelHelper();

	const IStringData *	GetSelectedAsset() const;
protected:
	IControlView *		GetWidgetView(
							const WidgetID &			inWidgetID) const;
private:
};


CREATE_PMINTERFACE(CZPAssetsPanelHelper, kZPAssetsPanelHelperImpl)

CZPAssetsPanelHelper::CZPAssetsPanelHelper(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAssetsPanelHelper::~CZPAssetsPanelHelper()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetSelectedAsset
//----------------------------------------------------------------------------------------
const IStringData *
CZPAssetsPanelHelper::GetSelectedAsset()const
{
	const IStringData *	toReturn = nil;
	do
	{
		IControlView * treeControlView = this->GetWidgetView( kZPUIAssetsPanelLstAssetsWidgetID );
		ASSERT( treeControlView );
		InterfacePtr<const ITreeViewController> tvController( treeControlView, UseDefaultIID());
		ASSERT( tvController );
		NodeIDList selectedNodes;
		tvController->GetSelectedItems( selectedNodes );
		if( selectedNodes.Length() == 1 )
		{
			ZPTreeNodePtr<ZPAssetsTVNodeID> nodeID(selectedNodes[0]);
			if (nodeID != nil)
			{
				const PMString & theAssetID = nodeID->GetID();//("55a7be8e-8c82-4a07-a467-f54497b50c10");
				toReturn = ZPDataHelper::GetAssetByID( theAssetID );
			}
		}
	}while(kFalse);

	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetWidgetView
//----------------------------------------------------------------------------------------
IControlView *
CZPAssetsPanelHelper::GetWidgetView(
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
