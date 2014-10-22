//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/Services/CZPAMLinkedStoryAfterCheckInService.cpp $
//	$Revision: 4096 $
//	$Date: 2013-04-01 09:20:00 +0200 (Mon, 01 Apr 2013) $
//	$Author: mindfire $
//	
//	Creator: Aman Alam
//	Created: 16-6-2011
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IAMUIService.h"
#include "IAMUIServiceProvider.h"
#include "IDataLink.h"
#include "IAssignment.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#include "ILinkResource.h"
#include "IWorkGroupFacade.h"
#endif

//SDK General includes
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPAMHelper.h"
#include "IZPGlobalTempData.h"
#include "IZPStoryUtils.h"
#include "IZPAssetData.h"
#include "IZPAssetCheckInData.h"
#include "IZPCommandFacade.h"

//IZP General includes
#include "AZPAMService.h"
#include "ZPTypes.h"
#include "ZPDataHelper.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IZPInDnUtils.h"
#endif

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AMService
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass AZPAMService
class CZPAMLinkedStoryAfterCheckInService : public inheritClass
{
public:
						CZPAMLinkedStoryAfterCheckInService(IPMUnknown * boss);
	virtual				~CZPAMLinkedStoryAfterCheckInService();

	ErrorCode			Do(
							IManageableAsset * pAsset);
	bool16				VerifyContext(
							IManageableAsset * pAsset);
protected:
	bool				GetAssetFile(
							const IManageableAsset *	inManageableAsset,
							IDFile &					oAssetFile) const;
private:
};


CREATE_PMINTERFACE(CZPAMLinkedStoryAfterCheckInService, kZPAMLinkedStoryAfterCheckInServiceImpl)

CZPAMLinkedStoryAfterCheckInService::CZPAMLinkedStoryAfterCheckInService(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMLinkedStoryAfterCheckInService::~CZPAMLinkedStoryAfterCheckInService()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// Do
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMLinkedStoryAfterCheckInService::Do(
	IManageableAsset * pAsset)
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	this->LogManageableAssetInfo( pAsset );

	ErrorCode toReturn = kFailure;

	do
	{
		if( !pAsset )
			break;
	
		InterfacePtr<IZPGlobalTempData> zpGlobalData( gSession, UseDefaultIID() );
		ASSERT( zpGlobalData );
//Returning kFailure leads to failure of creation of link in CS5, so returning kSuccess
#if defined(InDnCS5) || defined(InDnCS5_5)
		if( Utils<IZPInDnUtils>()->IsHostAppInCopy() && zpGlobalData->GetAssignmentNewStatusID().IsEmpty())
		{			
			toReturn = kSuccess;
			break;
		}
#else
		if (zpGlobalData->GetAssignmentNewStatusID().IsEmpty())
			break;
#endif

		IDFile oldFilePath;
#if defined(InDnCS5) || defined(InDnCS5_5)
		InterfacePtr<ILinkResource> assetLinkResource( pAsset, UseDefaultIID() );
		ASSERT( assetLinkResource );
		
		if(!this->GetIDStoryFile( pAsset, oldFilePath ))
			break;
#else
		InterfacePtr<IDataLink> assetDL( pAsset, UseDefaultIID() );
		ASSERT( assetDL );
		//TODO: Try using this->GetIDStoryFile( pAsset, oldFilePath )
		if( !Utils<IZPStoryUtils>()->GetDataLinkFileLocation( assetDL, oldFilePath ))
			break;
#endif

		//Find if old path is from current replica folder.
		if( Utils<IZPAMHelper>()->IsSubPathOfOurReplicaFolder( oldFilePath ) == false)
			break;	//Not our story

		const IStringData *	dbAsset = this->GetDBAsset_FromIDStory( pAsset );
		if( !dbAsset )
			break;

		InterfacePtr<IAssignment> storyAssignment( Utils<IZPStoryUtils>()->QueryAssignmentForStoryAsset( pAsset ) );
		InterfacePtr<const IZPAssetData> assignmentAssetData(storyAssignment, UseDefaultIID());
		ASSERT(assignmentAssetData);
		if (!assignmentAssetData)
			break;

		const PMString & assignmentAssetId = assignmentAssetData->GetAssetID();
		const IStringData * theDBAssetID = ZPDataHelper::GetAssetByID( assignmentAssetId );

#if defined(InDnCS5) || defined(InDnCS5_5) // Patch: for InDesign export selection. Link is not created when returning kFailure, so check if it is unassigned incopy content and return kSuccess		
		if( storyAssignment->GetCompatibility() == IAssignment::kUnassignedCompatibility )
		{
			//toReturn = kSuccess;
			Utils<Facade::IWorkgroupFacade>()->UpdateStoredAssetReference(::GetUIDRef( pAsset ));
		}
#endif
		if( theDBAssetID )
		{
			InterfacePtr<IZPAssetCheckInData> assignmentCheckInData( storyAssignment, UseDefaultIID() );
			ASSERT( assignmentCheckInData );

			assignmentCheckInData->SetStatusID( zpGlobalData->GetAssignmentNewStatusID() );

			const PMString & asgnName = storyAssignment->GetName();
			assignmentCheckInData->SetAssetName( asgnName );

			//Update status on server
			Utils<IZPAMHelper>()->UpdateAssetInfoOnServer( assignmentAssetData, assignmentCheckInData, theDBAssetID );
			
			assignmentCheckInData->SetStatusID( kNullString );
			assignmentCheckInData->SetAssetName( kNullString );
		}

		toReturn = kSuccess;
	}while(kFalse);

	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " service DO status = %d", toReturn);
	
	return toReturn;
}

//----------------------------------------------------------------------------------------
// VerifyContext
//----------------------------------------------------------------------------------------
bool16
CZPAMLinkedStoryAfterCheckInService::VerifyContext(
	IManageableAsset * pAsset)
{
	LogFunctionEnterExit;
	this->LogManageableAssetInfo( pAsset );

	bool16 toReturn = kFalse;

	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " VerifyContext = %hd", toReturn);
	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetAssetFile
//----------------------------------------------------------------------------------------
bool
CZPAMLinkedStoryAfterCheckInService::GetAssetFile(
	const IManageableAsset *	inManageableAsset,
	IDFile &					oAssetFile) const
{
	//LogFunctionEnterExit;
	return inheritClass::GetIDStoryFile( inManageableAsset, oAssetFile );
}
