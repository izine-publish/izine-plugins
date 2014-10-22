//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/Services/CZPAMLinkedStoryCheckOutService.cpp $
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
#include "IManageableAsset.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes

//OS / ThirdParty includes

#include "IZPID.h"
#include "../iZinePublishUI/Source/iZineUIVersion.h"
#include "../iZinePublishUI/Source/Plugin/IZPUIID.h"

//IZP Interfaces

//IZP General includes
#include "AZPAMService.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AMService
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass AZPAMService
class CZPAMLinkedStoryCheckOutService : public inheritClass
{
public:
						CZPAMLinkedStoryCheckOutService(IPMUnknown * boss);
	virtual				~CZPAMLinkedStoryCheckOutService();

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


CREATE_PMINTERFACE(CZPAMLinkedStoryCheckOutService, kZPAMLinkedStoryCheckOutServiceImpl)

CZPAMLinkedStoryCheckOutService::CZPAMLinkedStoryCheckOutService(IPMUnknown * boss)
: inheritClass(boss)
{
	IZPLog_ObjC( thisFileLA );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMLinkedStoryCheckOutService::~CZPAMLinkedStoryCheckOutService()
{
	IZPLog_ObjD( thisFileLA );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// Do
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMLinkedStoryCheckOutService::Do(
	IManageableAsset * pAsset)
{
	LogFunctionEnterExit;
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	this->LogManageableAssetInfo( pAsset );

	ErrorCode toReturn = kFailure;

	const IStringData *	dbAsset = this->GetDBAsset_FromIDStory( pAsset );
#if defined(InDnCS5) || defined(InDnCS5_5)
	if(this->IsAssetLockedByCurrentUser( dbAsset ))
		toReturn = kSuccess;
	else if( dbAsset && !this->IsAssetLockedByCurrentUser( dbAsset ) )
#else
	if( dbAsset && !this->IsAssetLocked( dbAsset ) )
#endif
	{
		toReturn = this->DoAcquireLock( dbAsset );
		if( toReturn == kSuccess && this->IsInCopyTemplate(pAsset))
		{
			//Assumed that InCopy template assets will not open in InDesign.
			InterfacePtr<IEventWatcher> eventWatcher( gSession , IID_IZPUISTORYCHECKOUTEVENTWATCHER );
			if (eventWatcher)	//Patch: Stop watch event to check-out IC template story on typing.
				eventWatcher->StopWatching();
		}
	}

	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " service DO status = %d", toReturn);
	
	return toReturn;
}

//----------------------------------------------------------------------------------------
// VerifyContext
//----------------------------------------------------------------------------------------
bool16
CZPAMLinkedStoryCheckOutService::VerifyContext(
	IManageableAsset * pAsset)
{
	LogFunctionEnterExit;
	this->LogManageableAssetInfo( pAsset );

	bool16 toReturn = kFalse;

	const IStringData *	dbAsset = this->GetDBAsset_FromIDStory( pAsset );
	if( dbAsset && !this->IsAssetLocked( dbAsset ) )
		toReturn = kTrue;

	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " VerifyContext = %hd", toReturn);
	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetAssetFile
//----------------------------------------------------------------------------------------
bool
CZPAMLinkedStoryCheckOutService::GetAssetFile(
	const IManageableAsset *	inManageableAsset,
	IDFile &					oAssetFile) const
{
	//LogFunctionEnterExit;
	return inheritClass::GetIDStoryFile( inManageableAsset, oAssetFile );
}
