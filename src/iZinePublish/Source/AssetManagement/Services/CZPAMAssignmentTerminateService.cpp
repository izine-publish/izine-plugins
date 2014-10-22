//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/Services/CZPAMAssignmentTerminateService.cpp $
//	$Revision: 3568 $
//	$Date: 2011-08-04 12:29:35 +0200 (Thu, 04 Aug 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 16-8-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IAMUIService.h"
#include "IAssignment.h"
#include "IBoolData.h"
#include "IManageableAsset.h"
#include "IStringData.h"

//SDK General includes
#include "CAlert.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPAMHelper.h"
#include "IZPAMWSUtils.h"
#include "IZPAssetCheckInData.h"
#include "IZPAssetData.h"
#include "IZPAssetInfo.h"
#include "IZPDocUtils.h"
#include "IZPInDnUtils.h"

//IZP General includes
#include "AZPAMService.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AMService
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass AZPAMService
class CZPAMAssignmentTerminateService : public inheritClass
{
public:
						CZPAMAssignmentTerminateService(IPMUnknown * boss);
	virtual				~CZPAMAssignmentTerminateService();

	ErrorCode			Do(
							IManageableAsset * pAsset = nil );
	bool16				VerifyContext(
							IManageableAsset * pAsset = nil );
protected:
	void				CheckAssignmentAndUpdateStatus(
							const IAssignment *			inAssignment );
	void				UpdateAssignment(
							const IZPAssetData *		inAssetData,
							const IZPAssetCheckInData *	inAssetCheckInData,
							const IStringData *			inDBAssetID );

	bool				GetAssetFile(
							const IManageableAsset *	inManageableAsset,
							IDFile &					oAssetFile) const;
private:
};


CREATE_PMINTERFACE(CZPAMAssignmentTerminateService, kZPAMAssignmentTerminateServiceImpl)

CZPAMAssignmentTerminateService::CZPAMAssignmentTerminateService(IPMUnknown * boss)
: inheritClass(boss)
{
	IZPLog_ObjC( thisFileLA );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMAssignmentTerminateService::~CZPAMAssignmentTerminateService()
{
	IZPLog_ObjD( thisFileLA );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// Do
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMAssignmentTerminateService::Do(
	IManageableAsset * pAsset)
{
	LogFunctionEnterExit;

	ErrorCode status = kFailure;

	this->LogManageableAssetInfo( pAsset );

	do {
		//const IStringData *	dbAsset = this->GetDBAsset( pAsset );

		//if( !dbAsset )
		//	break;

		bool16 isHostInCopy = Utils<IZPInDnUtils>()->IsHostAppInCopy();
		//ASSERT( isHostInCopy );
		if( isHostInCopy )
		{
			//Update assignment status if there was any check-in made for a story.
			//Only if no story is check-out.
			//this->CheckAssignmentAndUpdateStatus( this->GetAssignment( pAsset ) );	//#1873: (Update assignment status after story checkin)
		}

		status = kSuccess;
		
	} while (false);
	
	return status;
}

//----------------------------------------------------------------------------------------
// VerifyContext
//----------------------------------------------------------------------------------------
bool16
CZPAMAssignmentTerminateService::VerifyContext(
	IManageableAsset * pAsset)
{
	LogFunctionEnterExit;
	
	this->LogManageableAssetInfo( pAsset );
	
	bool16 toReturn = kFalse;
	//const IStringData *	dbAsset = this->GetDBAsset( pAsset );
	//if( dbAsset )
		toReturn = kTrue;

	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " VerifyContext = %hd", toReturn);
	return toReturn;
}

//----------------------------------------------------------------------------------------
// CheckAssignmentAndUpdateStatus
//----------------------------------------------------------------------------------------
void
CZPAMAssignmentTerminateService::CheckAssignmentAndUpdateStatus(
	const IAssignment *			inAssignment)
{
	LogFunctionEnterExit;
	InterfacePtr<const IBoolData> storyCheckInMade(inAssignment, IID_IZPSTORYCHECKINMADE);
	if( storyCheckInMade && storyCheckInMade->Get() )
	{
		PMString checkInComments;
		UIDRef assignmentUIDRef = ::GetUIDRef( inAssignment );
		const IStringData * dbAssetID = this->GetDBAsset_FromAsgmt( inAssignment );
		IDFile assignmentFile;
		if( Utils<IZPAMHelper>()->GetAssetFilePath( dbAssetID, assignmentFile ) )
		{
			IAMUIService::FollowupAction followupAction = Utils<IZPAMHelper>()->AskVersionComments_Assignment( assignmentFile, assignmentUIDRef, checkInComments );
			if(followupAction != IAMUIService::enCancel && followupAction != IAMUIService::enNone )
			{
				InterfacePtr<const IZPAssetCheckInData> assetCheckInData( inAssignment, UseDefaultIID());
				ASSERT(assetCheckInData);
				InterfacePtr<const IZPAssetData> assetData( inAssignment, UseDefaultIID());
				ASSERT(assetData);
				this->UpdateAssignment( assetData, assetCheckInData, dbAssetID );
			}
		}
	}
}

//----------------------------------------------------------------------------------------
// UpdateAssignment
//----------------------------------------------------------------------------------------
void
CZPAMAssignmentTerminateService::UpdateAssignment(
	const IZPAssetData *		inAssetData,
	const IZPAssetCheckInData *	inAssetCheckInData,
	const IStringData *			inDBAssetID )
{
	Utils<IZPAMHelper>()->UpdateAssetInfoOnServer( inAssetData, inAssetCheckInData, inDBAssetID );
}

//----------------------------------------------------------------------------------------
// GetAssetFile
//----------------------------------------------------------------------------------------
bool
CZPAMAssignmentTerminateService::GetAssetFile(
	const IManageableAsset *	inManageableAsset,
	IDFile &					oAssetFile) const
{
	return inheritClass::GetAssetFile( inManageableAsset, oAssetFile );
}