//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/AssetManagement/Services/CZPAMUIUpdateVersionStatusService.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 17-6-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IAMServiceData.h"
#include "IBoolData.h"
#include "IDialog.h"
#include "IIntData.h"
#include "IStringData.h"
#include "IWindow.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes
#include "CPMUnknown.h"
#include "ErrorUtils.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPUIID.h"
#include "IZPID.h"

//IZP Interfaces
#include "IZPAssetCheckInData.h"
#include "IZPAssetData.h"
#include "IZPUIDialogFacade.h"
#include "IZPGlobalTempData.h"

//IZP General includes
#include "AZPAMUIService.h"
#include "ZPDataHelper.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AMUIService
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

/** This service displays a change status dialog.

	Input IAMServiceData parameters:
		
		kZPAMServiceDataStatusID
		enVersionComment
		
	Output IAMServiceData parameters:
	
		kZPAMServiceDataStatusID
		enVersionComment

		IAMUIService::enFollowupAction (IAMUIService::FollowupAction):
			IAMUIService::enSave (save to the IAMUIService::enSelectedFile).
			IAMUIService::enCancel (cancel all followup processing).
*/
#define inheritClass AZPAMUIService
class CZPAMUIUpdateVersionStatusService : public inheritClass
{
public:
						CZPAMUIUpdateVersionStatusService(IPMUnknown * boss);
	virtual				~CZPAMUIUpdateVersionStatusService();

	ErrorCode			Do(
							IManageableAsset *			asset = nil );
protected:
	ErrorCode			SetDialogValues(
							IControlView *				inDlgPanelView,
							IManageableAsset *			inDocAsset,
							IAMServiceData *			ioServiceData);

	void				SaveDialogValues(
							IControlView *				inDlgPanelView,
							IAMServiceData *			ioServiceData);

private:
};


CREATE_PMINTERFACE(CZPAMUIUpdateVersionStatusService, kZPAMUIUpdateVersionStatusServiceImpl)

CZPAMUIUpdateVersionStatusService::CZPAMUIUpdateVersionStatusService(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMUIUpdateVersionStatusService::~CZPAMUIUpdateVersionStatusService()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// Do
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMUIUpdateVersionStatusService::Do(
	IManageableAsset * inDocAsset)
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	ErrorCode result = kFailure;

	InterfacePtr<IAMServiceData> selfAMServiceData(this, UseDefaultIID());
	selfAMServiceData->Set(IAMUIService::enFollowupAction, IAMUIService::enCancel);
	
	InterfacePtr<IZPGlobalTempData> gTempData( gSession, UseDefaultIID() );
	ASSERT( gTempData );
	gTempData->SetAssignmentNewStatusID( kNullString );
	do
	{
		IDialog* theDialog = Utils<IZPUIDialogFacade>()->CreateDialogStatusUpdate();
		ASSERT(theDialog);
		
		IControlView* dialogPanel = theDialog->GetDialogPanel();
		ASSERT(dialogPanel);
		
		result = this->SetDialogValues( dialogPanel, inDocAsset, selfAMServiceData);
		if( result != kSuccess )
			break;
		
		result = kFailure;

		//Set the title according to the type of asset.
		InterfacePtr<IWindow> dlgWindow( theDialog, UseDefaultIID() );
		ASSERT( dlgWindow );

		PMString dialogTitle(kIZPUpdateVersionStatusDialogTitleKey);
		dlgWindow->SetTitle( dialogTitle );

		// Open the dialog.
		theDialog->Open();
		theDialog->WaitForDialog();
		
		InterfacePtr<IBoolData> dlgResultBoolData( dialogPanel, UseDefaultIID() );
		ASSERT( dlgResultBoolData );
		if( dlgResultBoolData->Get() == kFalse )
			break;

		InterfacePtr<const IZPAssetCheckInData> dlgAssetCheckInData( dialogPanel, UseDefaultIID() );
		ASSERT( dlgAssetCheckInData );

		//save the values with temp global data, after check-in service will use these values.
		this->SaveDialogValues( dialogPanel, selfAMServiceData );
		
		selfAMServiceData->Set(IAMUIService::enFollowupAction, IAMUIService::enSave);
		
		result = kSuccess;
	} while (false);

	return (result);
}


//----------------------------------------------------------------------------------------
// SetDialogValues
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMUIUpdateVersionStatusService::SetDialogValues(
	IControlView *				inDlgPanelView,
	IManageableAsset *			inDocAsset,
	IAMServiceData *			inServiceData )
{
	InterfacePtr<IBoolData> dlgResultBoolData( inDlgPanelView, UseDefaultIID() );
	ASSERT( dlgResultBoolData );
	dlgResultBoolData->Set( kFalse );		//Set this even if false init.

	InterfacePtr<IStringData> dlgTitleID(inDlgPanelView, IID_IZPTITLEIDSTRINGDATA );
	ASSERT( dlgTitleID );

	InterfacePtr<const IZPAssetData> docAssetData(inDocAsset, UseDefaultIID());
	ASSERT( docAssetData );

	//const PMString & docAssetID = docAssetData->GetAssetID();
	const PMString & docTitleID = docAssetData->GetTitleID();
	ASSERT(docTitleID.IsEmpty() == kFalse );

	dlgTitleID->Set( docTitleID );
	
	PMString description;
	PMString statusID;
	
	inServiceData->Get( kZPAMServiceDataStatusID, statusID );
	inServiceData->Get( enVersionComment, description );

	InterfacePtr<IZPAssetCheckInData> dlgAssetCheckInData( inDlgPanelView, UseDefaultIID() );
	ASSERT( dlgAssetCheckInData );
	
	dlgAssetCheckInData->SetDescription(description);
	dlgAssetCheckInData->SetStatusID(statusID);
	
	return kSuccess;
}


//----------------------------------------------------------------------------------------
// SaveDialogValues
//----------------------------------------------------------------------------------------
void
CZPAMUIUpdateVersionStatusService::SaveDialogValues(
	IControlView *				inDlgPanelView,
	IAMServiceData *			ioServiceData)
{
	//Save non persistent data
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<const IZPAssetCheckInData> dlgAssetCheckInData( inDlgPanelView, UseDefaultIID() );
	ASSERT( dlgAssetCheckInData );
	
	ioServiceData->Set( kZPAMServiceDataStatusID, dlgAssetCheckInData->GetStatusID() );
	ioServiceData->Set( enVersionComment, dlgAssetCheckInData->GetDescription() );

	InterfacePtr<IZPGlobalTempData> gTempData( gSession, UseDefaultIID() );
	ASSERT( gTempData );
	gTempData->SetAssignmentNewStatusID( dlgAssetCheckInData->GetStatusID() );
}

// End, CZPAMUIUpdateVersionStatusService.cpp.

