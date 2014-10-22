//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/ServiceProviders/CZPAMAssignmentServiceProvider.cpp $
//	$Revision: 4096 $
//	$Date: 2013-04-01 09:20:00 +0200 (Mon, 01 Apr 2013) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 28-6-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IAMService.h"
#include "IAMServiceProvider.h"
#include "IAMUIServiceProvider.h"
#include "IAssignment.h"
#include "ILinkObjectReference.h"
#include "IStringData.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IURIUtils.h"
#endif

//SDK General includes
#include "CAlert.h"
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPAssetData.h"
#include "IZPAssetInfo.h"
#include "IZPAssetUtils.h"
#include "IZPAMDebugHelper.h"
#include "IZPDocUtils.h"
#include "IZPFileUtils.h"
#include "IZPInDnUtils.h"

//IZP General includes
#include "AZPAMServiceProvider.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AMSrvP
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass AZPAMServiceProvider
class CZPAMAssignmentServiceProvider : public inheritClass
{
public:
						CZPAMAssignmentServiceProvider(IPMUnknown * boss);
	virtual				~CZPAMAssignmentServiceProvider();

	ErrorCode			Init()
						{	return kSuccess;	}
	bool16				CanService(
							const IDFile & file, const IAMServiceProvider::AssetHints * hints );
	bool16				CanService(
							const IManageableAsset * asset );
	//bool16				CanUpdateScriptedAssetData(
	//						UIDRef assetRef, const PMString & scriptString, int32 scriptID );
	//bool16				GetLockData(
	//						const IDFile & assetFile, IManagedStatus::StatusStringTable & outLockData, UIFlags uiFlags );
	IAMService*			GetService(
							int32 assetServiceID );
	IOpenFileCmdData::OpenFlags OverrideOpenFlags(
							const IDFile & file, IOpenFileCmdData::OpenFlags inOpenFlags );
	
	//bool16				IsTemporaryVersion(
	//						const IDFile & file );
	//bool16				IsVersioned(
	//						const IDFile & file );
	//bool16				IsWriteable(
	//						const IDFile & storyFile );
	//ErrorCode			UpdateScriptedAssetData(
	//						UIDRef assetRef, const PMString & scriptString, int32 scriptID );

#if defined(InDnCS5) || defined(InDnCS5_5)
	bool16				GetLinkResourceId(
							const IDFile & inFile, ILinkResource::ResourceId & outResourceId );
#endif
protected:
	bool16				CanServiceDBAsset(
							const IStringData *			inDbAssetID ) const;
	const char *		GetProviderName() const;
private:
};


CREATE_PMINTERFACE(CZPAMAssignmentServiceProvider, kZPAMAssignmentServiceProviderImpl)

CZPAMAssignmentServiceProvider::CZPAMAssignmentServiceProvider(IPMUnknown * boss)
: inheritClass(boss)
{
	IZPLog_ObjC( thisFileLA );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMAssignmentServiceProvider::~CZPAMAssignmentServiceProvider()
{
	IZPLog_ObjD( thisFileLA );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// CanService
//----------------------------------------------------------------------------------------
bool16
CZPAMAssignmentServiceProvider::CanService(
	const IDFile & file, const IAMServiceProvider::AssetHints * hints)
{
	LogFunctionEnterExit;
	
	bool16 toReturn = kFalse;
	
	if ( hints && (hints->kFileType != 0 || !hints->kFileExtension.IsEmpty()) )
	{
		this->LogFileHints( hints );
		if ( hints->kFileType != 0 
		&& ( hints->kFileType == kAssignmentType) )
		{
				toReturn = kTrue;
		}

		if ( !toReturn 
		&& ( kAssignmentExt.Compare( kFalse, hints->kFileExtension ) == 0) )
		{
			toReturn = kTrue;
		}
	}
	
	if( toReturn ) //Base will check if file is from our replica folder or not.
		toReturn = inheritClass::CanService( file, hints );

	IZPLog_Str_( thisFileLA, enLT_DebugInfo, "canService file : %hd", toReturn );
	

	return toReturn;
}

//----------------------------------------------------------------------------------------
// CanService
//----------------------------------------------------------------------------------------
bool16
CZPAMAssignmentServiceProvider::CanService(
	const IManageableAsset * asset)
{
	LogFunctionEnterExit;
	
	bool16 toReturn = kFalse;

	//TODO: check for IDataLink interface and then check if data link points to assignment
	InterfacePtr<ILinkObjectReference> linkObjectReference( asset, UseDefaultIID() );
	if( linkObjectReference )
	{
		//Assignment must be direct link
		InterfacePtr<IAssignment> assignmentAsset( ::GetDataBase(asset), linkObjectReference->GetUID(), UseDefaultIID() );
		if( assignmentAsset )
		{
#if defined(InDnCS5) || defined(InDnCS5_5)			
//			FileTypeInfoID type = FileTypeRegistry::GetFileTypeInfoIDForFile( IDFile( assignmentAsset->GetFile() ) );
//			if( type == kAssignmentMarkupFileTypeInfoID)			
				toReturn = kTrue;
#else
			toReturn = kTrue;
#endif
			
		}
	}	

	if(!toReturn)
	{	//Patch: for InCopy, it is doc boss.
		if( Utils<IZPInDnUtils>()->IsHostAppInCopy()
			&&	::GetUID( asset ).Get() == ::GetDataBase(asset)->GetRootUID() )
		{
			//Check if this document is an assignment file.
			InterfacePtr<IDocument> assginmentDoc(asset, UseDefaultIID());
			ASSERT(assginmentDoc);

			InterfacePtr<IAssignment> iAssignment( Utils<IZPDocUtils>()->QueryAssignmentFromAssignmentDoc( assginmentDoc ) );
			if(iAssignment)	//Don't handle IC document/templates there. Only Assignment document must be handled.
			{
				InterfacePtr<const IZPAssetData> assetData(iAssignment, UseDefaultIID());
				const IStringData * dbAssetID = this->GetDBAsset( assetData );
				
				toReturn = this->CanServiceDBAsset( dbAssetID );
			}
		}
	}
	
#ifdef DEBUG
	this->LogManageableAssetInfo( asset );
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " CanService asset: %hd", toReturn );
#endif
	return toReturn;
}

//----------------------------------------------------------------------------------------
// CanServiceDBAsset
//----------------------------------------------------------------------------------------
bool16
CZPAMAssignmentServiceProvider::CanServiceDBAsset(
	const IStringData *			inDbAssetID) const
{
	//LogFunctionEnterExit;

	bool16 toReturn = kTrue;
	InterfacePtr<const IZPAssetInfo> dbAssetInfo( inDbAssetID, UseDefaultIID());
	if( dbAssetInfo && (dbAssetInfo->GetType() != eAssetType_ICAssignment ) )
	{
		toReturn = kFalse;	//Must never come here.
	}

#ifdef DEBUG
//	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " CanServiceDBAsset asset: %hd", toReturn );
#endif

	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetService
//----------------------------------------------------------------------------------------
IAMService*
CZPAMAssignmentServiceProvider::GetService(
	int32 assetServiceID)
{
	LogFunctionEnterExit;
	IAMService * toReturn = nil;
	
	ClassID serviceBoss = kInvalidClass;

	bool isInDesign = Utils<IZPInDnUtils>()->IsHostAppInCopy() == kFalse;
	switch ( assetServiceID )
	{
		case IAMServiceProvider::enInitialize: // need this to initialize asset management on the link
			// [RR] We never get call in enInitialize. We don't know the reason. But we get call in enCheckIn.
			serviceBoss =  kZPAMAssignmentInitializeServiceBoss;
			break;
		case IAMServiceProvider::enEnsureLatestVersion :
			serviceBoss =	kZPAMAssignmentEnsureLatestVersionServiceBoss;
			break;
		case IAMServiceProvider::enCheckOut :
			ASSERT( isInDesign );
			serviceBoss =	kZPAMAssignmentCheckOutServiceBoss;
			break;
		case IAMServiceProvider::enCancelCheckOut :
			ASSERT( isInDesign );
			serviceBoss =	kZPAMAssignmentCheckOutServiceBoss;
			break;
		case IAMServiceProvider::enRevert :
			ASSERT( isInDesign );
			serviceBoss =	kZPAMAssignmentRevertServiceBoss;
			break;
		case IAMServiceProvider::enCheckIn :
			ASSERT( isInDesign );
			serviceBoss =	kZPAMAssignmentCheckInServiceBoss;
			break;
		case IAMServiceProvider::enAfterSave:
		case IAMServiceProvider::enAfterSaveAs:
			//ASSERT( isInDesign );	//Also comes here for InCopy, to save check-out stories locally.
			serviceBoss =	kZPAMAssignmentAfterSaveServiceBoss;
			break;
		case IAMServiceProvider::enRelink:
			break;
		case IAMServiceProvider::enUpdateLinkLocation :
			break;
		case IAMServiceProvider::enBeforeCheckOut :
			break;
		case IAMServiceProvider::enAfterCancelCheckOut :
			break;
		case IAMServiceProvider::enAfterCheckIn :
			break;
		case IAMServiceProvider::enUnembed :
			break;
		case IAMServiceProvider::enTerminate :
			serviceBoss =	kZPAMAssignmentTerminateServiceBoss;
			break;
		case IAMServiceProvider::enUpdateProxyVersionNumber :
			serviceBoss =	kZPAMAssignmentUpdateProxyVersionNumberServiceBoss;
			break;
		case IAMServiceProvider::enUpdateStoredAssetReference :
			serviceBoss =	kZPAMAssignmentUpdateStoredAssetReferenceServiceBoss;
			break;
		case IAMServiceProvider::enInvalidateProxyVersionNumber :
			break;
		case IAMServiceProvider::enGetUniqueAssetLocation :
			break;
		case IAMServiceProvider::enBatchCheckIn :
			break;
		case IAMServiceProvider::enInvalidService :
			ASSERT_FAIL( "**IZPAMAssignmentServiceProvider::GetService - called with enInvalidService assetServiceID**" );
			break;
		//TODO: Should we return default service handlers?
	}
	
	if( !toReturn && serviceBoss.Get() != kInvalidClass.Get() )
	{
		toReturn = ::CreateObject2<IAMService>( serviceBoss );
	}
#ifdef DEBUG
	IZPLog_Str_( thisFileLA, enLT_Details, "%s : %08X : service impl instance : %08X", Utils<IZPAMDebugHelper>()->GetAMServiceName((const IAMServiceProvider::AssetService)assetServiceID), assetServiceID, toReturn );
#endif	

	return toReturn;
}

//----------------------------------------------------------------------------------------
// OpenFlags
//----------------------------------------------------------------------------------------
IOpenFileCmdData::OpenFlags
CZPAMAssignmentServiceProvider::OverrideOpenFlags(
	const IDFile & file, IOpenFileCmdData::OpenFlags inOpenFlags)
{
	LogFunctionEnterExit;

	IOpenFileCmdData::OpenFlags openFlags = inOpenFlags;
	//This function must be called only for ID documents, where old version must be opned with IOpenFileCmdData::kOpenCopy flag.

	return openFlags;
}

//----------------------------------------------------------------------------------------
// GetProviderName
//----------------------------------------------------------------------------------------
const char *
CZPAMAssignmentServiceProvider::GetProviderName()const
{
	return "Assignment service provider";
}

#if defined(InDnCS5) || defined(InDnCS5_5)
//----------------------------------------------------------------------------------------
// GetLinkResourceId
//----------------------------------------------------------------------------------------
bool16
CZPAMAssignmentServiceProvider::GetLinkResourceId(
	const IDFile & inFile, ILinkResource::ResourceId & outResourceId )
{
	LogFunctionEnterExit;
	//TODO: CS5: Test functionality
	bool result = Utils<IURIUtils>()->IDFileToURI(inFile, outResourceId);
	return result;
}

#endif
