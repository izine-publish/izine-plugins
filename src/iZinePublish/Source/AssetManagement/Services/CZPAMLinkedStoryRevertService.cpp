//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/Services/CZPAMLinkedStoryRevertService.cpp $
//	$Revision: 3568 $
//	$Date: 2011-08-04 12:29:35 +0200 (Thu, 04 Aug 2011) $
//	$Author: aman.alam $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 2-7-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================

#include "VCPlugInHeaders.h"

//SDK Interfaces

//SDK General includes
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPAMHelper.h"

//IZP General includes
#include "AZPAMService.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AMService
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass AZPAMService
class CZPAMLinkedStoryRevertService : public inheritClass
{
public:
						CZPAMLinkedStoryRevertService(IPMUnknown * boss);
	virtual				~CZPAMLinkedStoryRevertService();

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


CREATE_PMINTERFACE(CZPAMLinkedStoryRevertService, kZPAMLinkedStoryRevertServiceImpl)

CZPAMLinkedStoryRevertService::CZPAMLinkedStoryRevertService(IPMUnknown * boss)
: inheritClass(boss)
{
	IZPLog_ObjC( thisFileLA );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMLinkedStoryRevertService::~CZPAMLinkedStoryRevertService()
{
	IZPLog_ObjD( thisFileLA );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// Do
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMLinkedStoryRevertService::Do(
	IManageableAsset * pAsset)
{
	LogFunctionEnterExit;
	this->LogManageableAssetInfo( pAsset );

	ErrorCode toReturn = kFailure;

	const IStringData *	dbAsset = this->GetDBAsset_FromIDStory( pAsset );
	if( dbAsset )
	{
		//Set the local file to out-of-date, so that it can be downloaded again.
		this->UpdateLocalInfo_LocallyModified( dbAsset, false );
		this->UpdateLocalInfo_VersionNumber( dbAsset, 0 );

		//RJK: commented following as the file writing fails if called from here.
		//Download last check-in version over current file.
		//IDFile assetFile;
		//if( !this->GetAssetFile( pAsset, assetFile ) )
		//	Utils<IZPAMHelper>()->GetAssetFilePath( dbAsset, assetFile );
		//toReturn = Utils<IZPAMHelper>()->DoDownloadAsset( dbAsset, assetFile );

		toReturn = kSuccess;
	}

	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " service DO status = %d", toReturn);
	
	return toReturn;
}

//----------------------------------------------------------------------------------------
// VerifyContext
//----------------------------------------------------------------------------------------
bool16
CZPAMLinkedStoryRevertService::VerifyContext(
	IManageableAsset * pAsset)
{
	LogFunctionEnterExit;
	this->LogManageableAssetInfo( pAsset );

	bool16 toReturn = kFalse;
	const IStringData *	dbAsset = this->GetDBAsset_FromIDStory( pAsset );
	if( dbAsset )
	{
		IDFile assetFile;
		if( !this->GetAssetFile( pAsset, assetFile ) )
			Utils<IZPAMHelper>()->GetAssetFilePath( dbAsset, assetFile );
		//check if this file locally modified or not.
		//Don't release lock if file is locally modified.
		IManagedStatus::VersionState fileState = Utils<IZPAMHelper>()->GetAssetFileVersionState( dbAsset, assetFile );
		bool lockedByUser = this->IsAssetLockedByCurrentUser( dbAsset );	//To revert the asset must be locked by current user
		//Check the lock status also, as it also comes for "Cancel Check out" menu visibilty.
		if( fileState == IManagedStatus::enVersionConflict
		|| (( fileState == IManagedStatus::enLocalIsNewer
			|| fileState == IManagedStatus::enUpToDate )
			&& lockedByUser ))
		{
			toReturn = kTrue;
		}
	}

	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " VerifyContext = %hd", toReturn);
	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetAssetFile
//----------------------------------------------------------------------------------------
bool
CZPAMLinkedStoryRevertService::GetAssetFile(
	const IManageableAsset *	inManageableAsset,
	IDFile &					oAssetFile) const
{
	//LogFunctionEnterExit;
	return inheritClass::GetIDStoryFile( inManageableAsset, oAssetFile );
}
