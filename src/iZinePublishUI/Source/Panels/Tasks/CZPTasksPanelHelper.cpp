//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Panels/Tasks/CZPTasksPanelHelper.cpp $
//	$Revision: 3296 $
//	$Date: 2011-06-17 14:37:11 +0200 (Fri, 17 Jun 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 25-10-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IControlView.h"
#include "IDropDownListController.h"
#include "IPanelControlData.h"
#include "IStringData.h"
#include "ITreeViewController.h"

//SDK General includes

//OS / ThirdParty includes

#include "IZPUIID.h"

//IZP Interfaces
#include "IZPTasksPanelHelper.h"

//IZP General includes
#include "ZPDataHelper.h"
#include "ZPTreeNodePtr.h"
#include "ZPTasksTVNodeID.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZPTasksPanelHelper>
class CZPTasksPanelHelper : public inheritClass
{
public:
						CZPTasksPanelHelper(IPMUnknown * boss);
	virtual				~CZPTasksPanelHelper();

	const IStringData *	GetSelectedTask() const;
	PMString			GetSelectedTitleID() const;
	PMString			GetSelectedEditionID() const;
	
	PMString			DBObjPopGetSelectedID(
							WidgetID					inWidgetID,
							bool						inEmptyIfNotCurrentlySelected = false ) const;

protected:
	IControlView *		GetWidgetView(
							const WidgetID &			inWidgetID) const;
private:
};


CREATE_PMINTERFACE(CZPTasksPanelHelper, kZPTasksPanelHelperImpl)

CZPTasksPanelHelper::CZPTasksPanelHelper(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPTasksPanelHelper::~CZPTasksPanelHelper()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetSelectedTask
//----------------------------------------------------------------------------------------
const IStringData *
CZPTasksPanelHelper::GetSelectedTask()const
{
	const IStringData *	toReturn = nil;
	do
	{
		IControlView * treeControlView = this->GetWidgetView( kZPUITasksPanelLstTasksWidgetID );
		ASSERT( treeControlView );
		InterfacePtr<const ITreeViewController> tvController( treeControlView, UseDefaultIID());
		ASSERT( tvController );
		NodeIDList selectedNodes;
		tvController->GetSelectedItems( selectedNodes );
		if( selectedNodes.Length() == 1 )
		{
			ZPTreeNodePtr<ZPTasksTVNodeID> nodeID(selectedNodes[0]);
			if (nodeID != nil)
			{
				const PMString & theTaskID = nodeID->GetID();//("55a7be8e-8c82-4a07-a467-f54497b50c10");
				toReturn = ZPDataHelper::GetTaskByID( theTaskID );
			}
		}
	}while(kFalse);

	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetWidgetView
//----------------------------------------------------------------------------------------
IControlView *
CZPTasksPanelHelper::GetWidgetView(
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

//----------------------------------------------------------------------------------------
// GetSelectedTitleID
//----------------------------------------------------------------------------------------
PMString
CZPTasksPanelHelper::GetSelectedTitleID()const
{
	return this->DBObjPopGetSelectedID( kZPUITasksPanelTitleDropDownWidgetID, true );
}

//----------------------------------------------------------------------------------------
// GetSelectedEditionID
//----------------------------------------------------------------------------------------
PMString
CZPTasksPanelHelper::GetSelectedEditionID()const
{
	return this->DBObjPopGetSelectedID( kZPUITasksPanelEditionDropDownWidgetID, true );
}

//----------------------------------------------------------------------------------------
// DBObjPopGetSelectedID
//----------------------------------------------------------------------------------------
PMString
CZPTasksPanelHelper::DBObjPopGetSelectedID(
	WidgetID					inWidgetID,
	bool						inEmptyIfNotCurrentlySelected ) const
{
	InterfacePtr<const IStringData> selectedID( this->GetWidgetView(inWidgetID), IID_IZPUISELECTEDID );
	ASSERT(selectedID);

	InterfacePtr<IDropDownListController> popupListController( selectedID, UseDefaultIID());
	ASSERT(popupListController);
	
	int32 selectedIndex = popupListController->GetSelected();

	if( !inEmptyIfNotCurrentlySelected || selectedIndex >= 0 )
		return selectedID->Get();
	
	return kNullString;

}


