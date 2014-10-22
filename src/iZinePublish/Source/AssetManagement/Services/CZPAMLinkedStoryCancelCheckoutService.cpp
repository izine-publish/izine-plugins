//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/Services/CZPAMLinkedStoryCancelCheckoutService.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 2-7-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IEventWatcher.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPID.h"
#include "../iZinePublishUI/Source/iZineUIVersion.h"
#include "../iZinePublishUI/Source/Plugin/IZPUIID.h"

//IZP Interfaces
#include "IZPAMHelper.h"

//IZP General includes
#include "AZPAMService.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AMService
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass AZPAMService
class CZPAMLinkedStoryCancelCheckoutService : public inheritClass
{
public:
						CZPAMLinkedStoryCancelCheckoutService(IPMUnknown * boss);
	virtual				~CZPAMLinkedStoryCancelCheckoutService();

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


CREATE_PMINTERFACE(CZPAMLinkedStoryCancelCheckoutService, kZPAMLinkedStoryCancelCheckoutServiceImpl)

CZPAMLinkedStoryCancelCheckoutService::CZPAMLinkedStoryCancelCheckoutService(IPMUnknown * boss)
: inheritClass(boss)
{
	IZPLog_ObjC( thisFileLA );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMLinkedStoryCancelCheckoutService::~CZPAMLinkedStoryCancelCheckoutService()
{
	IZPLog_ObjD( thisFileLA );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// Do
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMLinkedStoryCancelCheckoutService::Do(
	IManageableAsset * pAsset)
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	this->LogManageableAssetInfo( pAsset );

	ErrorCode toReturn = kFailure;
	do {
		const IStringData *	dbAsset = this->GetDBAsset_FromIDStory( pAsset );
		if( !dbAsset )
			break;
		IDFile assetFile;
		if( !this->GetAssetFile( pAsset, assetFile ) )
			Utils<IZPAMHelper>()->GetAssetFilePath( dbAsset, assetFile );
		//check if this file locally modified or not.
		//Don't release lock if file is locally modified.
		IManagedStatus::VersionState fileState = Utils<IZPAMHelper>()->GetAssetFileVersionState( dbAsset, assetFile );
		if( fileState != IManagedStatus::enLocalIsNewer && fileState != IManagedStatus::enVersionConflict )
		{
			this->DoReleaseLock( dbAsset );
			if (this->IsInCopyTemplate(pAsset))
			{
				//Assumed that InCopy template assets will not open in InDesign.
				InterfacePtr<IEventWatcher> eventWatcher( gSession , IID_IZPUISTORYCHECKOUTEVENTWATCHER );
				if (eventWatcher)	//Patch: Watch event to check-out IC template story on typing.
					eventWatcher->StartWatching();
			}
		}

		toReturn = kSuccess;
		
	} while (false);

	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " service DO status = %d", toReturn);
	
	return toReturn;
}

//----------------------------------------------------------------------------------------
// VerifyContext
//----------------------------------------------------------------------------------------
bool16
CZPAMLinkedStoryCancelCheckoutService::VerifyContext(
	IManageableAsset * pAsset)
{
	LogFunctionEnterExit;
	this->LogManageableAssetInfo( pAsset );

	bool16 toReturn = kFalse;	

	const IStringData *	dbAsset = this->GetDBAsset_FromIDStory( pAsset );

	//check if this lock is acquired by this user & from this app.
	if( dbAsset && this->IsAssetLockedByCurrentUser( dbAsset ) )
	{
		IDFile assetFile;
		if( !this->GetAssetFile( pAsset, assetFile ) )
			Utils<IZPAMHelper>()->GetAssetFilePath( dbAsset, assetFile );
		//check if this file locally modified or not.
		//Don't release lock if file is locally modified.
		IManagedStatus::VersionState fileState = Utils<IZPAMHelper>()->GetAssetFileVersionState( dbAsset, assetFile );
		if( fileState != IManagedStatus::enLocalIsNewer && fileState != IManagedStatus::enVersionConflict )
			toReturn = kTrue;
	}

	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " VerifyContext = %hd", toReturn);
	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetAssetFile
//----------------------------------------------------------------------------------------
bool
CZPAMLinkedStoryCancelCheckoutService::GetAssetFile(
	const IManageableAsset *	inManageableAsset,
	IDFile &					oAssetFile) const
{
	//LogFunctionEnterExit;
	return inheritClass::GetIDStoryFile( inManageableAsset, oAssetFile );
}
