//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/Controls/AssetsTreeView/CZPAssetsTVActionComponent.cpp $
//	$Revision: 3546 $
//	$Date: 2011-07-26 14:49:31 +0200 (Tue, 26 Jul 2011) $
//	$Author: aman.alam $
//	
//	Creator: Aman Alam
//	Created: 18-7-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IActionStateList.h"
#include "IBoolData.h"
#include "IDialog.h"
#include "IStringData.h"
#include "IWindow.h"

//SDK General includes
#include "CActionComponent.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPUIID.h"
#include "IZPID.h"

//IZP Interfaces
#include "IZPAMWSUtils.h"
#include "IZPAssetLockInfo.h"
#include "IZPUIDialogFacade.h"
#include "IZPUITVNodeData.h"

//IZP General includes

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_ActionComponent
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CActionComponent
class CZPAssetsTVActionComponent : public inheritClass
{
public:
						CZPAssetsTVActionComponent(IPMUnknown * boss);
	virtual				~CZPAssetsTVActionComponent();

	void				DoAction(
							IActiveContext* ac, ActionID actionID, GSysPoint mousePoint, IPMUnknown* widget);
	void				UpdateActionStates(
							IActiveContext* ac, IActionStateList *listToUpdate, GSysPoint mousePoint, IPMUnknown* widget);
protected:
private:
	bool				CanRenameAsset(
							IPMUnknown*						assetNodeWidget);
	void				RenameAsset(
							IPMUnknown*						assetNodeWidget);
};


CREATE_PMINTERFACE(CZPAssetsTVActionComponent, kZPAssetsTVActionComponentImpl)

CZPAssetsTVActionComponent::CZPAssetsTVActionComponent(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAssetsTVActionComponent::~CZPAssetsTVActionComponent()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// UpdateActionStates
//----------------------------------------------------------------------------------------
void
CZPAssetsTVActionComponent::UpdateActionStates(
	IActiveContext* ac, IActionStateList *listToUpdate, GSysPoint mousePoint, IPMUnknown* widget)
{
	do
	{
		int actionIDListLen = listToUpdate->Length();
		for(int i=0; i < actionIDListLen; ++i)
		{
			ActionID actionID = listToUpdate->GetNthAction(i);
			int16 state = listToUpdate->GetNthActionState(i);
			switch (actionID.Get())
			{
				case kIZPRenameAssetActionID:
				{
					if (this->CanRenameAsset(widget))
						state = kEnabledAction;
					else
						state = kDisabled_Unselected;
					break;
				}
				default:
					break;
			}
			listToUpdate->SetNthActionState(i, state);
		}
	}while(kFalse);
}

//----------------------------------------------------------------------------------------
// DoAction
//----------------------------------------------------------------------------------------
void
CZPAssetsTVActionComponent::DoAction(
	IActiveContext* ac, ActionID actionID, GSysPoint mousePoint, IPMUnknown* widget)
{
	LogFunctionEnterExit;
	
	switch (actionID.Get())
	{
		case kIZPRenameAssetActionID:
		{
			this->RenameAsset(widget);
			break;
		}
		default:
			break;
	}
}

//----------------------------------------------------------------------------------------
// CanRenameAsset
//----------------------------------------------------------------------------------------
bool
CZPAssetsTVActionComponent::CanRenameAsset(
	IPMUnknown*						assetNodeWidget)
{
	bool toReturn = false;
	do
	{
		InterfacePtr<const IZPUITVNodeData> nodeData(assetNodeWidget, UseDefaultIID());
		ASSERT(nodeData);
		const IStringData * dbAssetID = (IStringData *)nodeData->GetDataObj();

		InterfacePtr<const IZPAssetLockInfo> assetLockInfo( dbAssetID, UseDefaultIID());
		if (assetLockInfo && (assetLockInfo->GetLockState() == IAMLockable::enAvailable || assetLockInfo->GetLockState() == IAMLockable::enCheckedOut))
			toReturn = true;

	}while(kFalse);

	return toReturn;
}

//----------------------------------------------------------------------------------------
// RenameAsset
//----------------------------------------------------------------------------------------
void
CZPAssetsTVActionComponent::RenameAsset(
	IPMUnknown*						assetNodeWidget)
{
	do
	{
		IDialog* theDialog = Utils<IZPUIDialogFacade>()->CreateDialogRenameAsset(false);
		ASSERT(theDialog);
		
		IControlView* dialogPanel = theDialog->GetDialogPanel();
		ASSERT(dialogPanel);

		InterfacePtr<const IZPUITVNodeData> nodeData(assetNodeWidget, UseDefaultIID());
		ASSERT(nodeData);
		const IStringData * dbAssetID = (IStringData *)nodeData->GetDataObj();

		InterfacePtr<IStringData> dbAssetName( dbAssetID, IID_IZPDATANAME );
		ASSERT( dbAssetName );
		const PMString & oldAssetName = dbAssetName->Get();

		InterfacePtr<IStringData> dlgAssetID( dialogPanel, IID_IZPDATAID );
		ASSERT( dlgAssetID );
		dlgAssetID->Set(dbAssetID->Get());

		InterfacePtr<IStringData> dlgAssetName( dialogPanel, IID_IZPDATANAME );
		ASSERT( dlgAssetName );
		dlgAssetName->Set(oldAssetName);

		InterfacePtr<IBoolData> dlgResultBoolData( dialogPanel, UseDefaultIID() );
		ASSERT( dlgResultBoolData );
		dlgResultBoolData->Set( kFalse );

		// Open the dialog.
		theDialog->Open();
		theDialog->WaitForDialog();

		//Get the dialog result
		InterfacePtr<IBoolData> dialogResult( dialogPanel, UseDefaultIID() );
		ASSERT( dialogResult );
		if (!dialogResult->Get())
			break;

		const PMString & newAssetName = dlgAssetName->Get();
		if (oldAssetName.Compare(kTrue, newAssetName) == 0)	//If both are equal then don't do anything
			break;

		Utils<IZPAMWSUtils>()->UpdateAssetName_sync( dbAssetID->Get(), newAssetName );

		//TODO: if renamed failed, then should we show the dialog again?
	}while(kFalse);
}
