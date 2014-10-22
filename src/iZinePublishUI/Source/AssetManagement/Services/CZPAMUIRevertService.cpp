//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublishUI/Source/AssetManagement/Services/CZPAMUIRevertService.cpp $
//	$Revision: 1962 $
//	$Date: 2010-08-13 12:52:48 +0200 (Fri, 13 Aug 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 10-7-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IAMServiceData.h"
#include "IStringData.h"

//SDK General includes
#include "CAlert.h"
#include "StringUtils.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPUIID.h"
#include "IZPID.h"

//IZP Interfaces
#include "IZPUIDialogFacade.h"

//IZP General includes
#include "AZPAMUIService.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AMUIService
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

/** This service displays a revert alert dialog.

	Input IAMServiceData parameters: none.
	Output IAMServiceData parameters:
		IAMUIService::enFollowupAction (IAMUIService::FollowupAction):
		
			IAMUIService::enRevertToLastVersion
			IAMUIService::enRevertDocument
			IAMUIService::enCancel				
*/
#define inheritClass AZPAMUIService
class CZPAMUIRevertService : public inheritClass
{
public:
						CZPAMUIRevertService(IPMUnknown * boss);
	virtual				~CZPAMUIRevertService();

	ErrorCode			Do(
							IManageableAsset *	asset = nil );
protected:
private:
};


CREATE_PMINTERFACE(CZPAMUIRevertService, kZPAMUIRevertServiceImpl)

CZPAMUIRevertService::CZPAMUIRevertService(IPMUnknown * boss)
: inheritClass(boss)
{
	IZPLog_ObjC( thisFileLA );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMUIRevertService::~CZPAMUIRevertService()
{
	IZPLog_ObjD( thisFileLA );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// Do
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMUIRevertService::Do(
	IManageableAsset * pAsset)
{
	LogFunctionEnterExit;

	InterfacePtr<IAMServiceData> iamdata(this, UseDefaultIID());
	ASSERT(iamdata);

	const enAMUIServiceType serviceType = this->GetServiceType();

	bool shouldRevert = this->CallVerifyService( pAsset, IAMServiceProvider::enRevert );

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
	
	PMString message( kIZPRevertAlertKey );
	message.Translate();
	
	StringUtils::ReplaceStringParameters(&message, name);

	int16 result = 0;
	if( shouldRevert )
		result = Utils<IZPUIDialogFacade>()->ShowAlertDialog( message, kIZPRevertBtnKey, kIZPRevertToVersionBtnKey, kCancelString, kNullString );
	else
		result = Utils<IZPUIDialogFacade>()->ShowAlertDialog( message, kIZPRevertBtnKey, kNullString, kCancelString, kNullString );
	
	switch (result){
		case 1:
			iamdata->Set(IAMUIService::enFollowupAction, IAMUIService::enRevertDocument);
			break;
		case 2:
			iamdata->Set(IAMUIService::enFollowupAction, IAMUIService::enRevertToLastVersion);
			break;
		case 3:
			iamdata->Set(IAMUIService::enFollowupAction, IAMUIService::enCancel);
			break;
	}
	return (kSuccess);
}
