//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/AssetManagement/Services/CZPAMUISaveAsDialogService.cpp $
//	$Revision: 3545 $
//	$Date: 2011-07-26 14:48:04 +0200 (Tue, 26 Jul 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 28-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
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
#include "IZPDocUtils.h"
#include "IZPInDnUtils.h"
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
class CZPAMUISaveAsDialogService : public inheritClass
{
public:
						CZPAMUISaveAsDialogService(IPMUnknown * boss);
	virtual				~CZPAMUISaveAsDialogService();

	ErrorCode			Do(
							IManageableAsset *			asset = nil );
protected:
	ErrorCode			SetDialogValues(
							IControlView *				inDlgPanelView,
							IManageableAsset *			inDocAsset);

	void				SaveDialogValues(
							IControlView *				inDlgPanelView,
							IManageableAsset *			inDocAsset);

	PMString			GetNewTypeID(
							IControlView *				inDlgPanelView);
private:
};


CREATE_PMINTERFACE(CZPAMUISaveAsDialogService, kZPAMUISaveAsDialogServiceImpl)

CZPAMUISaveAsDialogService::CZPAMUISaveAsDialogService(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMUISaveAsDialogService::~CZPAMUISaveAsDialogService()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// Do
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMUISaveAsDialogService::Do(
	IManageableAsset * pAsset)
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
		
		result = this->SetDialogValues( dialogPanel, pAsset );
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
		const PMString & newEditionID = dialogAssetData->GetEditionID();
		const PMString & newAssetName = dlgAssetCheckInData->GetAssetName();
		PMString newAssetID;

		if (kSuccess != Utils<IZPAMHelper>()->CreateNewAsset( newAssetID, newAssetName, newTypeID, newEditionID))
		{
			ErrorUtils::PMSetGlobalErrorCode(kZPWSAssetCreatingError);
			break;
		}

		//Modify the asset ID, with dialog.
		InterfacePtr<IZPAssetData> dialogAssetData2(dialogPanel, UseDefaultIID());
		ASSERT( dialogAssetData2 );
		dialogAssetData2->SetAssetID( newAssetID );	//TODO: test this with save as 


		// lock new document
		if (kSuccess != Utils<IZPAMWSUtils>()->AcquireLock_sync(newAssetID, kNullString, kNullString )
		|| !Utils<IZPAMHelper>()->IsAssetLockedByCurrentUser( newAssetID ) )
		{
			ErrorUtils::PMSetGlobalErrorCode(kZPDocumentLockError);
			break;
		}

		this->SaveDialogValues( dialogPanel, pAsset );

		//Save As or Save A Copy, remove all the links from the document
		InterfacePtr<const IDocument> doc(pAsset, UseDefaultIID());
		if(doc && Utils<IZPInDnUtils>()->IsHostAppInDesign())
		{
			Utils<IZPDocUtils>()->RemoveAllStoriesOnlyFromDoc(doc);
			Utils<IZPDocUtils>()->RemoveAllAssignmentsOnlyFromDoc(doc);
		}

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
//		pAsset->GetWriteFile(file);

		PMString assetFileExtenstion = Utils<IZPAssetUtils>()->GetAssetFileExtenstion( newTypeID );
		IDFile documentFile = Utils<IZPAssetUtils>()->CreateLocalFilePathFromAssetID( newAssetID, assetFileExtenstion, newAssetName );

		//Should we set the write file here?
		//pAsset->SetWriteFile(documentFile);

		//Set outparams with service data.
		if( Utils<IZPAssetUtils>()->IsTemplateTypeID( newTypeID ) )
			selfAMServiceData->SetFlag(enSaveAsStationery, kTrue);
		//else	//RJK: Commented as it returns true even if we are setting false.
		//	selfAMServiceData->SetFlag(enSaveAsStationery, kFalse);
		
		selfAMServiceData->Set(IAMUIService::enSelectedFile, documentFile);
		//TODO: check if it is new document or performing SaveAs
		selfAMServiceData->Set(IAMUIService::enFollowupAction, IAMUIService::enSave);
		//else selfAMServiceData->Set(IAMUIService::enFollowupAction, IAMUIService::enSaveToNewFile);

#pragma message("	//TODO: RJK, check why do we need to release the lock here? It should be done after saving.")
		//Should we wait till this asset is uploaded?
		//////if (!assetid.IsEmpty()) {
		//////	// release lock on old document
		//////	if (!IZPServer::Unlock(assetid))  {
		//////		CAlert::ErrorAlert("Error occured while releasing document.");
		//////		break;
		//////	}
		//////}

		result = kSuccess;
	} while (false);

	return (result);
}


//----------------------------------------------------------------------------------------
// SetDialogValues
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMUISaveAsDialogService::SetDialogValues(
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
	
	InterfacePtr<const IZPAssetData> assetData(inDocAsset, UseDefaultIID());
	ASSERT( assetData );

	InterfacePtr<IStringData> dlgParentAssetID(inDlgPanelView, IID_IZPPARENTASSETID);
	ASSERT( dlgParentAssetID );
	dlgParentAssetID->Set(kNullString);

	const PMString & assetID = assetData->GetAssetID();
	const PMString & titleID = assetData->GetTitleID();
	const PMString & editionID = assetData->GetEditionID();
	
	dialogAssetData->SetAssetID( assetID );	//Will be used to check if it is new asset or old.
	dialogAssetData->SetTitleID( titleID );
	dialogAssetData->SetEditionID( editionID );
	
	//take these from asset cache
	PMString name;
	PMString description;
	enAssetType assetType = eAssetTypeNone;
	PMString statusID;
	const IStringData * theDBAsset = ZPDataHelper::GetAssetByID( assetID );
	if( theDBAsset )
	{
		InterfacePtr<const IStringData> dbAssetName( theDBAsset, IID_IZPDATANAME );
		ASSERT( dbAssetName );
		name = dbAssetName->Get();

		InterfacePtr<const IZPAssetInfo> dbAssetInfo( theDBAsset, UseDefaultIID() );
		ASSERT( dbAssetInfo );

		description = dbAssetInfo->GetDescription();
		statusID = dbAssetInfo->GetStatusID();
		assetType = dbAssetInfo->GetType();
	}
	
	InterfacePtr<IZPAssetCheckInData> dlgAssetCheckInData( inDlgPanelView, UseDefaultIID() );
	ASSERT( dlgAssetCheckInData );
	
	dlgAssetCheckInData->SetAssetName(name);
	dlgAssetCheckInData->SetDescription(description);
	dlgAssetCheckInData->SetStatusID(statusID);
	
	//IsTemplate checkbox
	InterfacePtr<IIntData> dlgAssetType( inDlgPanelView, IID_IZPASSETTYPE );
	ASSERT( dlgAssetType );
	
	dlgAssetType->Set( assetType );
	
	return kSuccess;
}


//----------------------------------------------------------------------------------------
// SaveDialogValues
//----------------------------------------------------------------------------------------
void
CZPAMUISaveAsDialogService::SaveDialogValues(
	IControlView *				inDlgPanelView,
	IManageableAsset *			inDocAsset)
{
	InterfacePtr<const IZPAssetData> dialogAssetData(inDlgPanelView, UseDefaultIID());
	ASSERT( dialogAssetData );
	
	//Saves AssetID, TitleID & EditionID
	Utils<IZPCommandFacade>()->SaveAssetData( dialogAssetData, inDocAsset );

	//Save non persistent data
	InterfacePtr<IZPAssetCheckInData> assetCheckInData(inDocAsset, UseDefaultIID());
	ASSERT( assetCheckInData );
	InterfacePtr<const IZPAssetCheckInData> dlgAssetCheckInData( inDlgPanelView, UseDefaultIID() );
	ASSERT( dlgAssetCheckInData );
	
	assetCheckInData->SetAssetName( dlgAssetCheckInData->GetAssetName() );
	assetCheckInData->SetDescription( dlgAssetCheckInData->GetDescription() );
	assetCheckInData->SetStatusID( dlgAssetCheckInData->GetStatusID() );
}

//----------------------------------------------------------------------------------------
// GetNewTypeID
//----------------------------------------------------------------------------------------
PMString
CZPAMUISaveAsDialogService::GetNewTypeID(
	IControlView *				inDlgPanelView )
{
	PMString toReturn;

	InterfacePtr<const IIntData> dlgAssetType( inDlgPanelView, IID_IZPASSETTYPE );
	ASSERT( dlgAssetType );

	toReturn = Utils<IZPAssetUtils>()->GetAssetTypeID( (enAssetType)dlgAssetType->Get() );
	
	return toReturn;
}

// End, CZPAMUISaveAsDialogService.cpp.

