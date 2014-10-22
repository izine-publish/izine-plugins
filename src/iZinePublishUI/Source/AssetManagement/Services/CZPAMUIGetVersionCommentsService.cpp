//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/AssetManagement/Services/CZPAMUIGetVersionCommentsService.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 28-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IAMService.h"
#include "IAMServiceData.h"
#include "IAMUIService.h"
#include "IApplication.h"
#include "IBoolData.h"
#include "IDialog.h"
#include "IDialogMgr.h"
#include "IDocument.h"
#include "IIntData.h"
#include "IManageableAsset.h"
#include "IStringData.h"

//SDK General includes
#include "CAlert.h"
#include "CoreResTypes.h"
#include "CPMUnknown.h"
#include "LocaleSetting.h"
#include "RsrcSpec.h"
#include "Utils.h"


//OS / ThirdParty includes

#include "IZPUIID.h"
#include "IZPID.h"

//IZP Interfaces
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

/** This service displays a dialog to get version comments for checkin.
		
	Input IAMServiceData parameters: none.
	Output IAMServiceData parameters:
		IAMUIService::enVersionComment (PMString)
		IAMUIService::enFollowupAction (IAMUIService::FollowupAction):
		
			IAMUIService::enDefaultAction (comment string available as IAMUIService::enVersionComment)
			IAMUIService::enCancel (cancel all followup processing)
*/
#define inheritClass AZPAMUIService
class CZPAMUIGetVersionCommentsService : public inheritClass
{
public:
						CZPAMUIGetVersionCommentsService(IPMUnknown * boss);
	virtual				~CZPAMUIGetVersionCommentsService();

	ErrorCode			Do(
							IManageableAsset *			asset = nil );
protected:
	void				InitDialogValues(
							IControlView *				inDlgPanelView );

	ErrorCode			SetDialogValues(
							IControlView *				inDlgPanelView,
							IManageableAsset *			inDocAsset);
	ErrorCode			SetDialogValues(
							IControlView *				inDlgPanelView,
							const IZPAssetData *		inAssetData);

	void				SaveDialogValues(
							IControlView *				inDlgPanelView,
							IManageableAsset *			inDocAsset);
	void				SaveDialogValues(
							IControlView *				inDlgPanelView,
							IZPAssetCheckInData *		ioAssetCheckInData);

	IZPAssetCheckInData *	QueryAssignmentCheckInData() const;
private:
};


CREATE_PMINTERFACE(CZPAMUIGetVersionCommentsService, kZPAMUIGetVersionCommentsServiceImpl)

CZPAMUIGetVersionCommentsService::CZPAMUIGetVersionCommentsService(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMUIGetVersionCommentsService::~CZPAMUIGetVersionCommentsService()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// Do
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMUIGetVersionCommentsService::Do(
	IManageableAsset * pAsset)
{
	LogFunctionEnterExit;

	ErrorCode result = kFailure;

	InterfacePtr<IAMServiceData> selfAMServiceData(this, UseDefaultIID());
	selfAMServiceData->Set(IAMUIService::enFollowupAction, IAMUIService::enCancel);

#if defined(InDnCS5) || defined(InDnCS5_5)
	if(!pAsset)
	{
		return kSuccess;
	}
#endif
	do
	{
		IDialog* dialog = Utils<IZPUIDialogFacade>()->CreateDialogSaveToRepository();
		ASSERT(dialog);
		
		IControlView* dialogPanel = dialog->GetDialogPanel();
		ASSERT(dialogPanel);
		
		this->InitDialogValues( dialogPanel );
		
		InterfacePtr<IZPAssetCheckInData> assetCheckInData(pAsset, UseDefaultIID());

		const enAMUIServiceType serviceType = this->GetServiceType();
		if( serviceType == eAMUIServiceType_Document || serviceType == eAMUIServiceType_LinkedStory )
		{
			InterfacePtr<const IZPAssetData> assetData(pAsset, UseDefaultIID());
			Utils<IZPAMWSUtils>()->FetchAssetInfo_sync(assetData->GetAssetID());
			result = this->SetDialogValues( dialogPanel, pAsset );
		}
		else
		{	//Assignment
			assetCheckInData.reset( this->QueryAssignmentCheckInData() );
			InterfacePtr<const IZPAssetData> assetData( assetCheckInData, UseDefaultIID() );
			if( !assetData )
			{	//TODO: CS5 porting, In CS3 during create assignment we don't get this call, but in CS5 we get here and crash.
#if defined(InDnCS5) || defined(InDnCS5_5)
				result = kSuccess;//kFailure;
#endif
				break;
			}

			Utils<IZPAMWSUtils>()->FetchAssetInfo_sync(assetData->GetAssetID());
			result = this->SetDialogValues( dialogPanel, assetData );
		}
		if( result != kSuccess )
			break;
		
		result = kFailure;

		//Set the title according to the type of asset.
		Utils<IZPUIDialogFacade>()->SetSaveToRepositoryDialogTitle( dialog );

		// Open the dialog.
		dialog->Open();
		dialog->WaitForDialog();
		
		InterfacePtr<const IBoolData> dlgResultBoolData( dialogPanel, UseDefaultIID() );
		ASSERT( dlgResultBoolData );
		if( dlgResultBoolData->Get() == kFalse )
			break;

		ASSERT( assetCheckInData );

		if( serviceType == eAMUIServiceType_Document || serviceType == eAMUIServiceType_LinkedStory )
		{
			this->SaveDialogValues( dialogPanel, pAsset );
		}
		else
		{	//Assignment
			this->SaveDialogValues( dialogPanel, assetCheckInData );
		}
		
		/*
		enFollowupAction		:the predefined FollowupAction.
		enVersionComment		: user comments.
		*/

		//Set outparams with service data.
		selfAMServiceData->Set(IAMUIService::enVersionComment, assetCheckInData->GetDescription());
		selfAMServiceData->Set(IAMUIService::enFollowupAction, IAMUIService::enSave);
		result = kSuccess;
	} while (false);

	return (result);
}

//----------------------------------------------------------------------------------------
// InitDialogValues
//----------------------------------------------------------------------------------------
void
CZPAMUIGetVersionCommentsService::InitDialogValues(
	IControlView *				inDlgPanelView)
{
	InterfacePtr<IBoolData> dlgResultBoolData( inDlgPanelView, UseDefaultIID() );
	ASSERT( dlgResultBoolData );
	dlgResultBoolData->Set( kFalse );		//Set this even if false init.

	InterfacePtr<IBoolData> dlgIsNewVersion( inDlgPanelView, IID_IZPISNEWVERSION );
	ASSERT( dlgIsNewVersion );
	dlgIsNewVersion->Set( kTrue );	//Set when saving a new version of old asset.
}

//----------------------------------------------------------------------------------------
// SetDialogValues
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMUIGetVersionCommentsService::SetDialogValues(
	IControlView *				inDlgPanelView,
	IManageableAsset *			inDocAsset)
{
	InterfacePtr<const IZPAssetData> assetData(inDocAsset, UseDefaultIID());
	return this->SetDialogValues( inDlgPanelView, assetData );
}

//----------------------------------------------------------------------------------------
// SetDialogValues
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMUIGetVersionCommentsService::SetDialogValues(
	IControlView *				inDlgPanelView,
	const IZPAssetData *		inAssetData)
{
	InterfacePtr<IZPAssetData> dialogAssetData(inDlgPanelView, UseDefaultIID());
	ASSERT( dialogAssetData );

	ASSERT( inAssetData );

	const PMString & assetID = inAssetData->GetAssetID();
	const PMString & titleID = inAssetData->GetTitleID();
	const PMString & editionID = inAssetData->GetEditionID();
	
	dialogAssetData->SetAssetID( assetID );	//Must be valid ID.
	dialogAssetData->SetTitleID( titleID );
	dialogAssetData->SetEditionID( editionID );
	
	InterfacePtr<IStringData> dlgParentAssetID(inDlgPanelView, IID_IZPPARENTASSETID);
	ASSERT( dlgParentAssetID );
	dlgParentAssetID->Set(kNullString);

	//take these from asset cache
	PMString name;
	PMString description;
	enAssetType assetType = eAssetTypeNone;
	PMString statusID;
	const IStringData * theDBAsset = ZPDataHelper::GetAssetByID( assetID );
	ASSERT( theDBAsset );	//New version can be check-in only if present on server.And it must in cache by now.
	//if( !theDBAsset )
	//	return kFailure;
	if( theDBAsset )
	{
		InterfacePtr<const IStringData> dbAssetName( theDBAsset, IID_IZPDATANAME );
		ASSERT( dbAssetName );
		name = dbAssetName->Get();
#if defined(InDnCS5) || defined(InDnCS5_5)
		name.SetTranslatable(kFalse);
#endif

		InterfacePtr<const IZPAssetInfo> dbAssetInfo( theDBAsset, UseDefaultIID() );
		ASSERT( dbAssetInfo );

		description = dbAssetInfo->GetDescription();
		statusID = dbAssetInfo->GetStatusID();
		assetType = dbAssetInfo->GetType();
	}

	//Override some values to show.
	InterfacePtr<IZPAssetCheckInData> assetCheckInData(inAssetData, UseDefaultIID());
	ASSERT( assetCheckInData );
	if( assetCheckInData->GetAssetName().IsEmpty() == kFalse )
		name = assetCheckInData->GetAssetName();
	
	InterfacePtr<IZPAssetCheckInData> dlgAssetCheckInData( inDlgPanelView, UseDefaultIID() );
	ASSERT( dlgAssetCheckInData );

	dlgAssetCheckInData->SetAssetName(name);
	dlgAssetCheckInData->SetDescription(description);
	dlgAssetCheckInData->SetStatusID(statusID);

	//for IsTemplate checkbox
	InterfacePtr<IIntData> dlgAssetType( inDlgPanelView, IID_IZPASSETTYPE );
	ASSERT( dlgAssetType );
	dlgAssetType->Set( assetType );

	return kSuccess;
}

//----------------------------------------------------------------------------------------
// SaveDialogValues
//----------------------------------------------------------------------------------------
void
CZPAMUIGetVersionCommentsService::SaveDialogValues(
	IControlView *				inDlgPanelView,
	IManageableAsset *			inDocAsset)
{
	//InterfacePtr<const IZPAssetData> dialogAssetData(inDlgPanelView, UseDefaultIID());
	//ASSERT( dialogAssetData );
	
	//Saves AssetID, TitleID & EditionID
	//Commented following as these will not change for version.
	//Utils<IZPCommandFacade>()->SaveAssetData( dialogAssetData, inDocAsset );

	//Save non persistent data
	InterfacePtr<IZPAssetCheckInData> assetCheckInData(inDocAsset, UseDefaultIID());
	this->SaveDialogValues( inDlgPanelView, assetCheckInData );
}

//----------------------------------------------------------------------------------------
// SaveDialogValues
//----------------------------------------------------------------------------------------
void
CZPAMUIGetVersionCommentsService::SaveDialogValues(
	IControlView *				inDlgPanelView,
	IZPAssetCheckInData *		ioAssetCheckInData)
{
	ASSERT( ioAssetCheckInData );

	InterfacePtr<IZPAssetCheckInData> dlgAssetCheckInData( inDlgPanelView, UseDefaultIID() );
	ASSERT( dlgAssetCheckInData );

	ioAssetCheckInData->SetAssetName( dlgAssetCheckInData->GetAssetName() );
	ioAssetCheckInData->SetDescription( dlgAssetCheckInData->GetDescription() );
	ioAssetCheckInData->SetStatusID( dlgAssetCheckInData->GetStatusID() );
	//Note: Not setting type as asset Type value does not changes for GetVersionComments.
}

//----------------------------------------------------------------------------------------
// QueryAssignmentCheckInData
//----------------------------------------------------------------------------------------
IZPAssetCheckInData *
CZPAMUIGetVersionCommentsService::QueryAssignmentCheckInData()const
{
	UIDRef assignmentUIDRef;

	InterfacePtr<IAMServiceData> selfAMServiceData(this, UseDefaultIID());
	ASSERT( selfAMServiceData );
	selfAMServiceData->Get( IAMService::enTargetAsset, assignmentUIDRef );

	InterfacePtr<IZPAssetCheckInData> assignmentCheckInData( assignmentUIDRef, UseDefaultIID());
	return assignmentCheckInData.forget();
}

// End, CZPAMUIGetVersionCommentsService.cpp.

