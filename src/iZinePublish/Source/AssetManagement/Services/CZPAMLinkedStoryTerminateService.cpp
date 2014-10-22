//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/Services/CZPAMLinkedStoryTerminateService.cpp $
//	$Revision: 1923 $
//	$Date: 2010-08-10 08:14:00 +0200 (Tue, 10 Aug 2010) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 10-8-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IAMService.h"
#include "ICommandInterceptor.h"
#include "IManageableAsset.h"
#include "IStringData.h"

//SDK General includes
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPAssetInfo.h"

//IZP General includes
#include "AZPAMService.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AMService
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass AZPAMService
class CZPAMLinkedStoryTerminateService : public inheritClass
{
public:
						CZPAMLinkedStoryTerminateService(IPMUnknown * boss);
	virtual				~CZPAMLinkedStoryTerminateService();

	ErrorCode			Do(
							IManageableAsset * pAsset = nil );
	bool16				VerifyContext(
							IManageableAsset * pAsset = nil );
protected:
	bool				GetAssetFile(
							const IManageableAsset *	inManageableAsset,
							IDFile &					oAssetFile) const;
private:
};


CREATE_PMINTERFACE(CZPAMLinkedStoryTerminateService, kZPAMLinkedStoryTerminateServiceImpl)

CZPAMLinkedStoryTerminateService::CZPAMLinkedStoryTerminateService(IPMUnknown * boss)
: inheritClass(boss)
{
	IZPLog_ObjC( thisFileLA );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMLinkedStoryTerminateService::~CZPAMLinkedStoryTerminateService()
{
	IZPLog_ObjD( thisFileLA );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// Do
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMLinkedStoryTerminateService::Do(
	IManageableAsset * pAsset)
{
	LogFunctionEnterExit;

	ErrorCode status = kFailure;

	this->LogManageableAssetInfo( pAsset );

	do {
		const IStringData *	dbAssetID = this->GetDBAsset( pAsset );

		if( !dbAssetID )
			break;

		InterfacePtr<const IZPAssetInfo> dbAssetInfo( dbAssetID, UseDefaultIID() );
		ASSERT( dbAssetInfo );
		if( dbAssetInfo && dbAssetInfo->GetType() == eAssetType_ICTemplate )
		{

			//Document is getting closed. Remove the command interceptor for this document
			IDataBase * db = ::GetDataBase( pAsset );
			UIDRef docRef( db, db->GetRootUID() );
			InterfacePtr<ICommandInterceptor> templateCmdInterceptor( docRef, IID_IZPTEMPLATECMDINTERCEPTOR );
			ASSERT( templateCmdInterceptor );
			templateCmdInterceptor->DeinstallSelf();
		}

		status = kSuccess;
		
	} while (false);
	
	return status;
}

//----------------------------------------------------------------------------------------
// VerifyContext
//----------------------------------------------------------------------------------------
bool16
CZPAMLinkedStoryTerminateService::VerifyContext(
	IManageableAsset * pAsset)
{
	LogFunctionEnterExit;
	
	this->LogManageableAssetInfo( pAsset );
	
	bool16 toReturn = kFalse;
	const IStringData *	dbAsset = this->GetDBAsset( pAsset );
	if( dbAsset )
		toReturn = kTrue;

	//TODO: Check if any async call is pending for this asset.

	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " VerifyContext = %hd", toReturn);
	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetAssetFile
//----------------------------------------------------------------------------------------
bool
CZPAMLinkedStoryTerminateService::GetAssetFile(
	const IManageableAsset *	inManageableAsset,
	IDFile &					oAssetFile) const
{
	return inheritClass::GetAssetFile( inManageableAsset, oAssetFile );
}
