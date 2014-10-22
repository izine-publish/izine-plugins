//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/Services/CZPAMLinkedStoryUpdateStoredRefService.cpp $
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
#include "ICoreFilename.h"
#include "IDataLink.h"
#include "IStringData.h"

//SDK General includes
#include "CoreFileUtils.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPAssetInfo.h"
#include "IZPAMHelper.h"
#include "IZPFileUtils.h"
#include "IZPStoryUtils.h"


#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IURIUtils.h"
#endif

//IZP General includes
#include "AZPAMService.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AMService
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

/** Updates stored asset reference data for a managed link. The stored data (if any) depends on
	the requirements of the asset management system. For example, an opaque data byte sequence supplied
	by the Version Cue API is stored with the IDataLink when the link references a Version Cue asset.
	
	Input IAMServiceData parameters: None.
	Output IAMServiceData parameters: None.				
*/
#define inheritClass AZPAMService
class CZPAMLinkedStoryUpdateStoredRefService : public inheritClass
{
public:
						CZPAMLinkedStoryUpdateStoredRefService(IPMUnknown * boss);
	virtual				~CZPAMLinkedStoryUpdateStoredRefService();

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


CREATE_PMINTERFACE(CZPAMLinkedStoryUpdateStoredRefService, kZPAMLinkedStoryUpdateStoredRefServiceImpl)

CZPAMLinkedStoryUpdateStoredRefService::CZPAMLinkedStoryUpdateStoredRefService(IPMUnknown * boss)
: inheritClass(boss)
{
	IZPLog_ObjC( thisFileLA );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMLinkedStoryUpdateStoredRefService::~CZPAMLinkedStoryUpdateStoredRefService()
{
	IZPLog_ObjD( thisFileLA );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// Do
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMLinkedStoryUpdateStoredRefService::Do(
	IManageableAsset * pAsset)
{
	LogFunctionEnterExit;
	this->LogManageableAssetInfo( pAsset );

	ErrorCode toReturn = kFailure;

	do
	{
		if( !pAsset )
			break;
	
		IDFile oldFilePath;
#if defined(InDnCS5) || defined(InDnCS5_5)
		InterfacePtr<ILinkResource> assetLinkResource( pAsset, UseDefaultIID() );
		ASSERT( assetLinkResource );
		if( !Utils<IZPStoryUtils>()->GetLinkResourceFileLocation( assetLinkResource, oldFilePath )) //Don't use this->GetIDStoryFile( pAsset, oldFilePath )
			break;
#else
		InterfacePtr<IDataLink> assetDL( pAsset, UseDefaultIID() );
		ASSERT( assetDL );
		if( !Utils<IZPStoryUtils>()->GetDataLinkFileLocation( assetDL, oldFilePath )) //Don't use this->GetIDStoryFile( pAsset, oldFilePath )
			break;
#endif

		//Find if old path is from current replica folder.
		bool oldFilePathFromOurReplica = Utils<IZPAMHelper>()->IsSubPathOfOurReplicaFolder( oldFilePath );
		const IStringData *	dbAsset = this->GetDBAsset_FromIDStory( pAsset );
		if( !dbAsset )
			break;

		//If we have asset id set, so this is our Asset. Set the proper path.
		IDFile newFilePath;
		if( Utils<IZPAMHelper>()->GetAssetFilePath( dbAsset, newFilePath ) )	//Don't use this->GetAssetFile here
		{
			//Set linked file info if it is changed, previously file name did not changed, but now file name is not asset ID, so it can change.
			if (oldFilePathFromOurReplica && oldFilePath == newFilePath)
				break;	//Old path and new path are same, so nothing to do

#ifdef InDnCS3
			//Change the ID story link path to current replica folder.
			InterfacePtr<IDataLink> assetDataLink( pAsset, UseDefaultIID() );
			ASSERT( assetDataLink );
			
			Utils<IZPStoryUtils>()->SetDataLinkFileLocation( assetDataLink, newFilePath );
#endif
			toReturn = kSuccess;			
			
			//TODO: Move to file utils, also check filename with asset id
			InterfacePtr<ICoreFilename> cfn((ICoreFilename *)::CreateObject(kCoreFilenameBoss, IID_ICOREFILENAME));
			ASSERT(cfn);
			if(cfn)
			{
				cfn->Initialize( &oldFilePath );
				if (cfn->FileExists())
				{
					PMString fileName;
					Utils<IZPFileUtils>()->GetFileName( newFilePath, fileName, true );
					if (fileName.IsEmpty() == false)
					{
						AString aFileName(fileName.GrabCString());
						CoreFileUtils::RenameFile(oldFilePath, aFileName);
					}
				}
			}

			//TODO: 2011Jun22, check if we need it here now?
			//Ensure latest version call is not made in each case by AM,
			//So make one ourself.
			this->CallEnsureLatestVersion( pAsset );

#if defined(InDnCS5) || defined(InDnCS5_5)
			// Update the resource URI
			URI resID;			
			Utils<IURIUtils>()->IDFileToURI(newFilePath, resID);
			Utils<IZPStoryUtils>()->UpdateLinkResourceURI(assetLinkResource, resID);
#endif
		}
	}while(kFalse);

	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " service DO status = %d", toReturn);
	
	return toReturn;
}

//----------------------------------------------------------------------------------------
// VerifyContext
//----------------------------------------------------------------------------------------
bool16
CZPAMLinkedStoryUpdateStoredRefService::VerifyContext(
	IManageableAsset * pAsset)
{
	LogFunctionEnterExit;
	this->LogManageableAssetInfo( pAsset );

	bool16 toReturn = kFalse;
	//Return true if this is versioned asset, i.e version number > 0
	const IStringData *	dbAsset = this->GetDBAsset_FromIDStory( pAsset );
	if( dbAsset )
	{
		InterfacePtr<const IZPAssetInfo> dbAssetInfo( dbAsset, UseDefaultIID() );
		ASSERT( dbAssetInfo );
		if( dbAssetInfo->GetVersionNumber() > 0 )
			toReturn = kTrue;
	}

	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " VerifyContext = %hd", toReturn);
	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetAssetFile
//----------------------------------------------------------------------------------------
bool
CZPAMLinkedStoryUpdateStoredRefService::GetAssetFile(
	const IManageableAsset *	inManageableAsset,
	IDFile &					oAssetFile) const
{
	//LogFunctionEnterExit;
	return inheritClass::GetIDStoryFile( inManageableAsset, oAssetFile );
}
