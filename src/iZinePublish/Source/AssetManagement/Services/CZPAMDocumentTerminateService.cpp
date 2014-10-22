//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/Services/CZPAMDocumentTerminateService.cpp $
//	$Revision: 3398 $
//	$Date: 2011-06-27 16:16:03 +0200 (Mon, 27 Jun 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 7-7-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IAMService.h"
#include "ICommandInterceptor.h"
#include "IManageableAsset.h"

//SDK General includes
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPInDnUtils.h"

//IZP General includes
#include "AZPAMService.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AMService
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

/** Executes any termination code required before an IManageableAsset instance is destroyed.

	Input IAMServiceData parameters: None.
	Output IAMServiceData parameters: None.		
*/
#define inheritClass AZPAMService
class CZPAMDocumentTerminateService : public inheritClass
{
public:
						CZPAMDocumentTerminateService(IPMUnknown * boss);
	virtual				~CZPAMDocumentTerminateService();

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


CREATE_PMINTERFACE(CZPAMDocumentTerminateService, kZPAMDocumentTerminateServiceImpl)

CZPAMDocumentTerminateService::CZPAMDocumentTerminateService(IPMUnknown * boss)
: inheritClass(boss)
{
	IZPLog_ObjC( thisFileLA );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMDocumentTerminateService::~CZPAMDocumentTerminateService()
{
	IZPLog_ObjD( thisFileLA );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// Do
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMDocumentTerminateService::Do(
	IManageableAsset * pAsset)
{
	LogFunctionEnterExit;

	ErrorCode status = kFailure;

	this->LogManageableAssetInfo( pAsset );

	do {
		//pAsset->SetServiceProviderClass( kInvalidClass );
		pAsset->SetStatus( nil );
		pAsset->SetLockable( nil );
		pAsset->SetWriteFile( IDFile() );
		
		const IStringData *	dbAsset = this->GetDBAsset( pAsset );

		if( !dbAsset )
			break;

		bool16 isHostInCopy = Utils<IZPInDnUtils>()->IsHostAppInCopy();
		if( !isHostInCopy )
		{

			//Document is getting closed. Remove the command interceptor for this document
			InterfacePtr<ICommandInterceptor> docAssignCmdInterceptor( pAsset, IID_IZPASSIGNMENTCMDINTERCEPTOR );
			ASSERT( docAssignCmdInterceptor );
			docAssignCmdInterceptor->DeinstallSelf();
		}

		status = kSuccess;
		
	} while (false);
	
	return status;
}

//----------------------------------------------------------------------------------------
// VerifyContext
//----------------------------------------------------------------------------------------
bool16
CZPAMDocumentTerminateService::VerifyContext(
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
CZPAMDocumentTerminateService::GetAssetFile(
	const IManageableAsset *	inManageableAsset,
	IDFile &					oAssetFile) const
{
	return inheritClass::GetAssetFile( inManageableAsset, oAssetFile );
}
