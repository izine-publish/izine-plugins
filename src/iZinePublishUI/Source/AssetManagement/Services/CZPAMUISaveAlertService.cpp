//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/AssetManagement/Services/CZPAMUISaveAlertService.cpp $
//	$Revision: 3473 $
//	$Date: 2011-07-11 12:10:49 +0200 (Mon, 11 Jul 2011) $
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
#include "IApplication.h"
#include "IDataLink.h"
#include "IDialog.h"
#include "IDialogMgr.h"
#include "IDocument.h"
#include "IManageableAsset.h"
#include "ISaveAssetContent.h"
#include "IStandAloneDoc.h"
#include "IStoryList.h"
#include "IStringData.h"


//SDK General includes
#include "CAlert.h"
#include "CoreResTypes.h"
#include "CPMUnknown.h"
#include "FileUtils.h"
#include "LocaleSetting.h"
#include "RsrcSpec.h"
#include "StringUtils.h"
#include "UIDList.h"
#include "Utils.h"


//OS / ThirdParty includes

#include "IZPUIID.h"
#include "IZPID.h"

//IZP Interfaces
#include "IZPUIDialogFacade.h"
#include "IZPInDnUtils.h"

//IZP General includes
#include "AZPAMUIService.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AMUIService
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

/** This service displays a save alert dialog.
	
	Input IAMServiceData parameters:
		IAMUIService::enDocumentName (PMString)
		
	Output IAMServiceData parameters:
		IAMUIService::enFollowupAction (IAMUIService::FollowupAction):
		
			IAMUIService::enSave (save the document)
			IAMUIService::enCancel (cancel the current operation)
			IAMUIService::enSaveVersion (save a version)
			IAMUIService::enDontSave (discard unsaved changes)			
		
*/
#define inheritClass AZPAMUIService
class CZPAMUISaveAlertService : public inheritClass
{
public:
						CZPAMUISaveAlertService(IPMUnknown * boss);
	virtual				~CZPAMUISaveAlertService();

	ErrorCode			Do(
							IManageableAsset *	asset = nil );
protected:
private:
};


CREATE_PMINTERFACE(CZPAMUISaveAlertService, kZPAMUISaveAlertServiceImpl)

CZPAMUISaveAlertService::CZPAMUISaveAlertService(IPMUnknown * boss)
: inheritClass(boss)
{
	IZPLog_ObjC( thisFileLA );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMUISaveAlertService::~CZPAMUISaveAlertService()
{
	IZPLog_ObjD( thisFileLA );
}

#pragma mark -

//----------------------------------------------------------------------------------------
// Do
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMUISaveAlertService::Do(
	IManageableAsset * pAsset)
{
	LogFunctionEnterExit;

	InterfacePtr<IAMServiceData> iamdata(this, UseDefaultIID());
	ASSERT(iamdata);
	
	const enAMUIServiceType serviceType = this->GetServiceType();

	bool canCheckIn = this->CallVerifyService( pAsset, IAMServiceProvider::enCheckIn );

	PMString name;
	iamdata->Get(IAMUIService::enDocumentName, name);
	
	const IStringData *	dbAsset = nil;
	if( serviceType == eAMUIServiceType_LinkedStory )
		dbAsset = this->GetDBAsset_FromIDStory( pAsset );
	else if( serviceType == eAMUIServiceType_Document )
		dbAsset = this->GetDBAsset_FromDoc( pAsset );
	else
		dbAsset = nil;

	if( name.IsEmpty() && dbAsset )
	{
		InterfacePtr<const IStringData> dbAssetName( dbAsset, IID_IZPDATANAME );
		if( dbAssetName )
			name = dbAssetName->Get();
	}

	PMString message(kIZPSaveAlertKey);
	message.Translate();
	
	StringUtils::ReplaceStringParameters(&message, name);
	
	IDataBase * db = ::GetDataBase( pAsset );
	bool isInCopy = Utils<IZPInDnUtils>()->IsHostAppInCopy();

	bool isStandaloneDoc = false;
	if (db)
	{
		InterfacePtr<IDocument> doc(db, db->GetRootUID(), UseDefaultIID());
		InterfacePtr<IStandAloneDoc> standAlone(doc, IID_ISTANDALONEDOC);
		if (standAlone )
			isStandaloneDoc = standAlone->IsStandAloneDoc();
	}

	int16 result = 0;
	//result = CAlert::ModalAlert(message, kSaveButtonString, kDontSaveButtonString, kCancelString, 1, CAlert::eWarningIcon);
	//int16 result = CAlert::ModalAlert(message, kSaveButtonString, kDontSaveButtonString, kIZPSaveVersionBtnKey, 1, CAlert::eWarningIcon);
#if 0
	if( canCheckIn )
		result = Utils<IZPUIDialogFacade>()->ShowAlertDialog( message, kSaveButtonString, kIZPSaveVersionBtnKey, kDontSaveButtonString, kCancelString );
	else if( !isInCopy && db->IsModified() )	//On InCopy if we cannot check-in then don't save it.
		result = Utils<IZPUIDialogFacade>()->ShowAlertDialog( message, kSaveButtonString, kNullString, kDontSaveButtonString, kCancelString );
#else	//#1935, force check-in, don't allow

#if 0
	if( canCheckIn )
		result = Utils<IZPUIDialogFacade>()->ShowAlertDialog( message, kNullString, kIZPSaveVersionBtnKey, kNullString, kCancelString );
	else if( !isInCopy && db->IsModified() )	//On InCopy if we cannot check-in then don't save it.
		result = Utils<IZPUIDialogFacade>()->ShowAlertDialog( message, kSaveButtonString, kNullString, kNullString, kCancelString );
#else
	//Patch: #1948, fix crash when select checkin after selecting No in IC Document
	if (dbAsset == nil && canCheckIn == false)
		result = Utils<IZPUIDialogFacade>()->ShowAlertDialog( message, kSaveButtonString, kNullString, kDontSaveButtonString, kCancelString ); //Save new doc
	else if (isInCopy && canCheckIn == false && db == nil)
		result = Utils<IZPUIDialogFacade>()->ShowAlertDialog( message, kSaveButtonString, kNullString, kNullString, kCancelString ); //Save new IC doc
	else if (isInCopy && canCheckIn && db && db->IsModified() && isStandaloneDoc == false)
		result = 4; // Checkin IC doc
	else if (isInCopy && canCheckIn == false && db && db->IsModified() && isStandaloneDoc == false)
		result = 4; // Cancel Assignment
	else if (isInCopy && canCheckIn && db && db->IsModified() && isStandaloneDoc)
		result = Utils<IZPUIDialogFacade>()->ShowAlertDialog( message, kNullString, kIZPSaveVersionBtnKey, kNullString, kCancelString ); // Checkin template
	else if (isInCopy == false && canCheckIn == false && db && db->IsModified())
		result = Utils<IZPUIDialogFacade>()->ShowAlertDialog( message, kSaveButtonString, kNullString, kNullString, kCancelString ); //Save new ID doc
	else if (isInCopy == false && canCheckIn && db && db->IsModified())
		result = Utils<IZPUIDialogFacade>()->ShowAlertDialog( message, kNullString, kIZPSaveVersionBtnKey, kNullString, kCancelString ); //Checkin
#endif
#endif
	else
		result = 3;
	
	switch (result){
		case 1:
			iamdata->Set(IAMUIService::enFollowupAction, IAMUIService::enSave);
			break;
		case 2:
			iamdata->Set(IAMUIService::enFollowupAction, IAMUIService::enSaveVersion);
			break;
		case 3:
			iamdata->Set(IAMUIService::enFollowupAction, IAMUIService::enDontSave);
			break;
		case 4:
			iamdata->Set(IAMUIService::enFollowupAction, IAMUIService::enCancel);
			break;
	}
	
	return (kSuccess);
}

// End, IZPAMUISaveAlertService.cpp.

