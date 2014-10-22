//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/Services/CZPAMLinkedStoryAfterSaveService.cpp $
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

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces

//IZP General includes
#include "AZPAMService.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_AMService
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass AZPAMService
class CZPAMLinkedStoryAfterSaveService : public inheritClass
{
public:
						CZPAMLinkedStoryAfterSaveService(IPMUnknown * boss);
	virtual				~CZPAMLinkedStoryAfterSaveService();

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


CREATE_PMINTERFACE(CZPAMLinkedStoryAfterSaveService, kZPAMLinkedStoryAfterSaveServiceImpl)

CZPAMLinkedStoryAfterSaveService::CZPAMLinkedStoryAfterSaveService(IPMUnknown * boss)
: inheritClass(boss)
{
	IZPLog_ObjC( thisFileLA );
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAMLinkedStoryAfterSaveService::~CZPAMLinkedStoryAfterSaveService()
{
	IZPLog_ObjD( thisFileLA );
}

#pragma mark -
//----------------------------------------------------------------------------------------
// Do
//----------------------------------------------------------------------------------------
ErrorCode
CZPAMLinkedStoryAfterSaveService::Do(
	IManageableAsset * pAsset)
{
	LogFunctionEnterExit;
	this->LogManageableAssetInfo( pAsset );

	ErrorCode toReturn = kSuccess;

	const IStringData *	dbAsset = this->GetDBAsset_FromIDStory( pAsset );
	if( dbAsset )
		this->UpdateLocalInfo_LocallyModified( dbAsset, true );

	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " service DO status = %d", toReturn);
	
	return toReturn;
}

//----------------------------------------------------------------------------------------
// VerifyContext
//----------------------------------------------------------------------------------------
bool16
CZPAMLinkedStoryAfterSaveService::VerifyContext(
	IManageableAsset * pAsset)
{
	LogFunctionEnterExit;
	this->LogManageableAssetInfo( pAsset );

	bool16 toReturn = kTrue;
	const IStringData *	dbAsset = this->GetDBAsset_FromIDStory( pAsset );
	if( !dbAsset )
		toReturn = kFalse;

	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " VerifyContext = %hd", toReturn);
	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetAssetFile
//----------------------------------------------------------------------------------------
bool
CZPAMLinkedStoryAfterSaveService::GetAssetFile(
	const IManageableAsset *	inManageableAsset,
	IDFile &					oAssetFile) const
{
	//LogFunctionEnterExit;
	return inheritClass::GetIDStoryFile( inManageableAsset, oAssetFile );
}
