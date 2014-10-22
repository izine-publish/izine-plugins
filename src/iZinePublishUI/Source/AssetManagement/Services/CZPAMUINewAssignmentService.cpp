//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/AssetManagement/Services/CZPAMUINewAssignmentService.cpp $
//	$Revision: 3545 $
//	$Date: 2011-07-26 14:48:04 +0200 (Tue, 26 Jul 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 8-6-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IActiveContext.h"
#include "IAMServiceData.h"
#include "IAMUIService.h"
#include "IBoolData.h"
#include "IDataLink.h"
#include "IDialog.h"
#include "IDocument.h"
#include "IIntData.h"
#include "IManageableAsset.h"
#include "ISaveAssetContent.h"
#include "IStoryList.h"
#include "IStringData.h"

//SDK General includes
#include "CAlert.h"
#include "CPMUnknown.h"
#include "ErrorUtils.h"
#include "FileUtils.h"
#include "UIDList.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPUIID.h"
#include "IZPID.h"

//IZP Interfaces
#include "IZPAMHelper.h"
#include "IZPAMWSUtils.h"
#include "IZPAssetCheckInData.h"
#include "IZPAssetData.h"
#include "IZPAssetInfo.h"
#include "IZPAssetUtils.h"
#include "IZPCommandFacade.h"
#include "IZPUIDialogFacade.h"

//IZP General includes
#include "AZPAMUIService.h"
#include "ZPDataHelper.h"



#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AMUIService
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

/** This service displays a SaveAs dialog.

	Input IAMServiceData parameters:
	
		IAMUIService::enDefaultFile (IDFile)
		IAMUIService::enUseSystemDefaultDirectory (boolean flag)
	
	Output IAMServiceData parameters:
	
		IAMUIService::enSelectedFile (IDFile)
		IAMUIService::enSaveAsStationery (boolean flag)
		IAMUIService::enFollowupAction (IAMUIService::FollowupAction):
		
			IAMUIService::enSave (save to the IAMUIService::enSelectedFile).
			IAMUIService::enCancel (cancel all followup processing).
*/
#define inheritClass AZPAMUIService
class CZPAMUINewAssignmentService : public inheritClass
{
public:
						CZPAMUINewAssignmentService(IPMUnknown * boss);
	virtual				~CZPAMUINewAssignmentService();

	ErrorCode			Do(
							IManageableAsset *			asset = nil );
protected:
	ErrorCode			SetDialogValues(
							IControlView *				inDlgPanelView,
							IManageableAsset *			inDocAsset);

	void				SaveDialogValues(
							IControlView *				inDlgPanelView,
							IAMServiceData *			ioServiceData);

	PMString			GetNewTypeID(
							IControlView *				inDlgPanelView);
private:
};


CREATE_PMINTERFACE(CZPAMUINewAssignmentService, kZPAMUINewAssignmentServiceImpl)

CZPAMUINewAssignmentService::CZPAMUINewAssignmentService(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMUINewAssignmentService::~CZPAMUINewAssignmentService()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// Do
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMUINewAssignmentService::Do(
	IManageableAsset * inDocAsset)
{
	LogFunctionEnterExit;

	ErrorCode result = kFailure;

	InterfacePtr<IAMServiceData> selfAMServiceData(this, UseDefaultIID());
	selfAMServiceData->Set(IAMUIService::enFollowupAction, IAMUIService::enCancel);
	
	do
	{
		IDialog* dialog = Utils<IZPUIDialogFacade>()->CreateDialogSaveToRepository();
		ASSERT(dialog);
		
		IControlView* dialogPanel = dialog->GetDialogPanel();
		ASSERT(dialogPanel);
		
		result = this->SetDialogValues( dialogPanel, inDocAsset);
		if( result != kSuccess )
			break;
		
		result = kFailure;

		//Set the title according to the type of asset.
		Utils<IZPUIDialogFacade>()->SetSaveToRepositoryDialogTitle( dialog );

		// Open the dialog.
		dialog->Open();
		dialog->WaitForDialog();
		
		InterfacePtr<IBoolData> dlgResultBoolData( dialogPanel, UseDefaultIID() );
		ASSERT( dlgResultBoolData );
		if( dlgResultBoolData->Get() == kFalse )
			break;

		InterfacePtr<const IZPAssetData> dialogAssetData(dialogPanel, UseDefaultIID());
		ASSERT( dialogAssetData );
		
		InterfacePtr<const IZPAssetCheckInData> dlgAssetCheckInData( dialogPanel, UseDefaultIID() );
		ASSERT( dlgAssetCheckInData );

		PMString newTypeID = this->GetNewTypeID( dialogPanel );

		InterfacePtr<const IZPAssetData> docAssetData(inDocAsset, UseDefaultIID());
		ASSERT( docAssetData );

		const PMString docAssetID = docAssetData->GetAssetID();
		const PMString & newEditionID = dialogAssetData->GetEditionID();
		const PMString & newAssetName = dlgAssetCheckInData->GetAssetName();
		PMString newAssetID;

		if (kSuccess != Utils<IZPAMHelper>()->CreateNewAsset( newAssetID, newAssetName, newTypeID, newEditionID, docAssetID))
		{
			ErrorUtils::PMSetGlobalErrorCode( kZPWSAssetCreatingError );
			break;
		}

		//Modify the asset ID, with dialog.
		InterfacePtr<IZPAssetData> dialogAssetData2(dialogPanel, UseDefaultIID());
		ASSERT( dialogAssetData2 );
		dialogAssetData2->SetAssetID( newAssetID );	//TODO: test this with save as 

		//Assignment is not created yet.
		//Where to save values of this dialog? Only status ID needs to be saved.
		//Till then save it with service, the caller will fetch & use it.
		this->SaveDialogValues( dialogPanel, selfAMServiceData );
		
		/*
		IAMUIService::enSelectedFile (IDFile)
		IAMUIService::enSaveAsStationery (boolean flag)
		IAMUIService::enFollowupAction (IAMUIService::FollowupAction):
		
		IAMUIService::enSave (save to the IAMUIService::enSelectedFile).
		IAMUIService::enCancel (cancel all followup processing).

		IAMUIService::
		enFollowupAction		:the predefined FollowupAction.
		enSelectedFile			: the user-selected file.
		enTemporaryVersionFile	: temporary version file (for viewing older version).
		enVersionComment		: user comments.
		*/

		PMString assetFileExtenstion = Utils<IZPAssetUtils>()->GetAssetFileExtenstion( newTypeID );
		IDFile documentFile = Utils<IZPAssetUtils>()->CreateLocalFilePathFromAssetID( newAssetID, assetFileExtenstion, newAssetName );

		//Set outparams with service data.
		if( Utils<IZPAssetUtils>()->IsTemplateTypeID( newTypeID ) )
			selfAMServiceData->SetFlag(enSaveAsStationery, kTrue);
		//else	//RJK: Commented as it returns true even if we are setting false.
		//	selfAMServiceData->SetFlag(enSaveAsStationery, kFalse);
		
		selfAMServiceData->Set(IAMUIService::enSelectedFile, documentFile);
		//TODO: check if it is new document or performing SaveAs
		selfAMServiceData->Set(IAMUIService::enFollowupAction, IAMUIService::enSave);
		//else selfAMServiceData->Set(IAMUIService::enFollowupAction, IAMUIService::enSaveToNewFile);

		result = kSuccess;
	} while (false);

	return (result);
}


//----------------------------------------------------------------------------------------
// SetDialogValues
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMUINewAssignmentService::SetDialogValues(
	IControlView *				inDlgPanelView,
	IManageableAsset *			inDocAsset)
{
	InterfacePtr<IBoolData> dlgResultBoolData( inDlgPanelView, UseDefaultIID() );
	ASSERT( dlgResultBoolData );
	dlgResultBoolData->Set( kFalse );		//Set this even if false init.

	InterfacePtr<IBoolData> dlgIsNewVersion( inDlgPanelView, IID_IZPISNEWVERSION );
	ASSERT( dlgIsNewVersion );
	dlgIsNewVersion->Set( kFalse );			//Set this even if false init.

	InterfacePtr<IZPAssetData> dialogAssetData(inDlgPanelView, UseDefaultIID());
	ASSERT( dialogAssetData );
	
	InterfacePtr<const IZPAssetData> docAssetData(inDocAsset, UseDefaultIID());
	ASSERT( docAssetData );

	const PMString docAssetID = docAssetData->GetAssetID();
	const PMString & docTitleID = docAssetData->GetTitleID();
	const PMString & docEditionID = docAssetData->GetEditionID();
	
	InterfacePtr<IStringData> dlgParentAssetID(inDlgPanelView, IID_IZPPARENTASSETID);
	ASSERT( dlgParentAssetID );
	dlgParentAssetID->Set(docAssetID);

	dialogAssetData->SetAssetID( kNullString );	//Will be used to check if it is new asset or old.
	dialogAssetData->SetTitleID( docTitleID );
	dialogAssetData->SetEditionID( docEditionID );
	
	//take these from asset cache
	PMString name;
	PMString description;
	PMString statusID;
	const IStringData * theDBAsset = ZPDataHelper::GetAssetByID( docAssetID );
	if( theDBAsset )
	{
		InterfacePtr<const IStringData> dbAssetName( theDBAsset, IID_IZPDATANAME );
		ASSERT( dbAssetName );
		name = dbAssetName->Get();

		InterfacePtr<const IZPAssetInfo> dbAssetInfo( theDBAsset, UseDefaultIID() );
		ASSERT( dbAssetInfo );

		statusID = dbAssetInfo->GetStatusID();
	}
	
	InterfacePtr<IZPAssetCheckInData> dlgAssetCheckInData( inDlgPanelView, UseDefaultIID() );
	ASSERT( dlgAssetCheckInData );
	
	dlgAssetCheckInData->SetAssetName(name);
	dlgAssetCheckInData->SetDescription(description);
	dlgAssetCheckInData->SetStatusID(statusID);
	
	//IsTemplate checkbox
	InterfacePtr<IIntData> dlgAssetType( inDlgPanelView, IID_IZPASSETTYPE );
	ASSERT( dlgAssetType );
	
	enAssetType assetType = eAssetType_ICAssignment;
	dlgAssetType->Set( assetType );
	
	return kSuccess;
}


//----------------------------------------------------------------------------------------
// SaveDialogValues
//----------------------------------------------------------------------------------------
void
CZPAMUINewAssignmentService::SaveDialogValues(
	IControlView *				inDlgPanelView,
	IAMServiceData *			ioServiceData)
{
	InterfacePtr<const IZPAssetData> dialogAssetData(inDlgPanelView, UseDefaultIID());
	ASSERT( dialogAssetData );
	
	//Saves AssetID, TitleID & EditionID
	ioServiceData->Set( kZPAMServiceDataAssetID, dialogAssetData->GetAssetID() );
	ioServiceData->Set( kZPAMServiceDataTitleID, dialogAssetData->GetTitleID() );
	ioServiceData->Set( kZPAMServiceDataEditionID, dialogAssetData->GetEditionID() );
	
	//Save non persistent data
	InterfacePtr<const IZPAssetCheckInData> dlgAssetCheckInData( inDlgPanelView, UseDefaultIID() );
	ASSERT( dlgAssetCheckInData );
	
	ioServiceData->Set( kZPAMServiceDataStatusID, dlgAssetCheckInData->GetStatusID() );
	ioServiceData->Set( enDocumentName, dlgAssetCheckInData->GetAssetName() );
	ioServiceData->Set( enVersionComment, dlgAssetCheckInData->GetDescription() );
}

//----------------------------------------------------------------------------------------
// GetNewTypeID
//----------------------------------------------------------------------------------------
PMString
CZPAMUINewAssignmentService::GetNewTypeID(
	IControlView *				inDlgPanelView )
{
	PMString toReturn;

	InterfacePtr<const IIntData> dlgAssetType( inDlgPanelView, IID_IZPASSETTYPE );
	ASSERT( dlgAssetType );

	toReturn = Utils<IZPAssetUtils>()->GetAssetTypeID( (enAssetType)dlgAssetType->Get() );
	
	return toReturn;
}

// End, CZPAMUINewAssignmentService.cpp.

