//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Utils/CZPAssetUtils.cpp $
//	$Revision: 4088 $
//	$Date: 2012-10-26 14:24:55 +0200 (Fri, 26 Oct 2012) $
//	$Author: mindfire $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 2-6-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "IStringData.h"
#if defined(InDnCS5) || defined(InDnCS5_5)
#include "IExecutionContext.h"
#endif

//SDK General includes
#include "Utils.h"

//OS / ThirdParty includes
#ifdef MACINTOSH
#include <uuid/uuid.h>
#endif

#include "IZPID.h"

//IZP Interfaces
#include "IZPApp.h"
#include "IZPAssetData.h"
#include "IZPAssetInfo.h"
#include "IZPAssetMgr.h"
#include "IZPAssetUtils.h"
#include "IZPEditionMgr.h"
#include "IZPFileUtils.h"
#include "IZPInDnUtils.h"
#include "IZPTitleMgr.h"

//IZP General includes
#include "ZPConstants.h"
#include "ZPDataHelper.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_Utils
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

struct ZPAssetTypeIDPair
{
	enAssetType				mType;
	const PMString &		mTypeID;

							ZPAssetTypeIDPair(
								enAssetType				inType,
								const PMString &		inTypeID );
};

ZPAssetTypeIDPair::ZPAssetTypeIDPair( 
	enAssetType				inType,
	const PMString &		inTypeID )
: mType (inType)
, mTypeID ( inTypeID )
{
}

static const ZPAssetTypeIDPair gAssetTypes[] = {
								ZPAssetTypeIDPair( eAssetType_IDDocument, kAssetType_InDesignDocumentID ),
								ZPAssetTypeIDPair( eAssetType_IDTemplate, kAssetType_InDesignTemplateID),
								ZPAssetTypeIDPair( eAssetType_ICDocument, kAssetType_InCopyDocumentID),
								ZPAssetTypeIDPair( eAssetType_ICTemplate, kAssetType_InCopyTemplateID),
								ZPAssetTypeIDPair( eAssetType_ICAssignment, kAssetType_InCopyAssignmentID),
								ZPAssetTypeIDPair( eAssetType_IDStory, kAssetType_InDesignStoryID),
								};
#pragma mark -

#define inheritClass CPMUnknown<IZPAssetUtils>
class CZPAssetUtils : public inheritClass
{
public:
						CZPAssetUtils(IPMUnknown * boss);
	virtual				~CZPAssetUtils();

	bool				IsTemplateTypeID(
							const PMString &			inTypeID ) const;
	bool				IsTemplateType(
							const enAssetType			inType ) const;
	
	enAssetType 		GetAssetTypeFromID(
							const PMString &			inTypeID ) const;
	const PMString &	GetAssetTypeID(
							const enAssetType			inType ) const;

	const PMString &	GetHostAppDocType(
							bool						inForTemplate ) const;
	enAssetType			GetHostAppDocAssetType(
							bool						inForTemplate = false ) const;
	const FileTypeInfoID GetHostAppDocFileTypeInfo(
							bool						inForTemplate ) const;

	const PMString &	GetAssetFileExtenstion(
							const enAssetType			inType ) const;
	const PMString &	GetAssetFileExtenstion(
							const PMString &			inTypeID ) const;

	IDFile				CreateLocalFilePathFromAssetID(
							const PMString &			inAssetID,
							const PMString &			inFileExtension,
							const PMString &			inName) const;
	
	IDFile				CreateAssetCacheFolderPath(
							const PMString &			inAssetID,
							bool						createIfNotExists) const;

	PMString			GetAssetIDFromLocalFile(
							const IDFile&				inFile ) const;
	PMString			GetAssetIDFromLocalFile(
							const PMString &			inFilePath ) const;
	
	PMString			NewGUID() const;

	bool				CanOpenAsset(
							const PMString &			inAssetID ) const;
	bool				CanOpenAssetType(				//Check if we can open inAssetType asset type.
							enAssetType					inAssetType ) const;

	bool				CopyAssetDataFromAsset(			//Copy IZPAssetData values from cached asset object.
							const IStringData *			inAssetID,
							IZPAssetData *				ioCopyDataTo ) const;
	PMString			GetStatusIDOfAsset(
							const PMString &			inAssetID ) const;
	PMString			GetStatusIDOfAsset(
							const IStringData *			inDBAssetID ) const;
#if defined(InDnCS5) || defined(InDnCS5_5)
	bool				FileToURI(
							const IDFile&				inFilePath,
							URI&						oURI ) const;
	bool				URIToIDFile(
							const URI&					inURI,
							IDFile&						oFilePath  ) const;
#endif
protected:
private:
};


CREATE_PMINTERFACE(CZPAssetUtils, kZPAssetUtilsImpl)

CZPAssetUtils::CZPAssetUtils(IPMUnknown * boss)
: inheritClass(boss)
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAssetUtils::~CZPAssetUtils()
{

}

#pragma mark -

//----------------------------------------------------------------------------------------
// IsTemplateTypeID
//----------------------------------------------------------------------------------------
bool
CZPAssetUtils::IsTemplateTypeID(
	const PMString &			inTypeID) const
{
	if((inTypeID.Compare(kFalse, kAssetType_InDesignTemplateID) == 0)
		|| (inTypeID.Compare(kFalse, kAssetType_InCopyTemplateID) == 0) )
	{
		return true;
	}
	return false;
}

//----------------------------------------------------------------------------------------
// IsTemplateType
//----------------------------------------------------------------------------------------
bool
CZPAssetUtils::IsTemplateType(
	const enAssetType			inType) const
{
	return inType == eAssetType_IDTemplate || inType == eAssetType_ICTemplate;
}

//----------------------------------------------------------------------------------------
// GetAssetTypeFromID
//----------------------------------------------------------------------------------------
enAssetType
CZPAssetUtils::GetAssetTypeFromID(
	const PMString &			inTypeID) const
{
	for( int i = 0 ; i < eAssetTypeCount ; ++i )
	{
		if( gAssetTypes[i].mTypeID.Compare(kFalse, inTypeID ) == 0 )
			return gAssetTypes[i].mType;
	}
	return eAssetTypeNone;
}

//----------------------------------------------------------------------------------------
// GetAssetTypeID
//----------------------------------------------------------------------------------------
const PMString &
CZPAssetUtils::GetAssetTypeID(
	const enAssetType			inType) const
{
	for( int i = 0 ; i < eAssetTypeCount ; ++i )
	{
		if( gAssetTypes[i].mType == inType )
			return gAssetTypes[i].mTypeID;
	}
	ASSERT_MSG( false, "Error: Asset type invalid" );
	return kEmptyGUID;	//Must never reach here
}

//----------------------------------------------------------------------------------------
// GetHostAppDocType
//----------------------------------------------------------------------------------------
const PMString &
CZPAssetUtils::GetHostAppDocType(
	bool						inForTemplate) const
{
	return this->GetAssetTypeID( this->GetHostAppDocAssetType( inForTemplate ) );
}

//----------------------------------------------------------------------------------------
// GetHostAppDocAssetType
//----------------------------------------------------------------------------------------
enAssetType
CZPAssetUtils::GetHostAppDocAssetType(
	bool						inForTemplate ) const
{
	if(Utils<IZPInDnUtils>()->IsHostAppInDesign() || Utils<IZPInDnUtils>()->IsHostAppInDesignServer() )
	{
		if( inForTemplate )
			return eAssetType_IDTemplate ;
		else
			return eAssetType_IDDocument;
	}
	else if(Utils<IZPInDnUtils>()->IsHostAppInCopy())
	{
		if( inForTemplate )
			return eAssetType_ICTemplate;
		else
			return eAssetType_ICDocument;
	}
	
	ASSERT(false);	//must never come here.
	return eAssetType_IDDocument;
}

//----------------------------------------------------------------------------------------
// GetHostAppDocFileTypeInfo
//----------------------------------------------------------------------------------------
const FileTypeInfoID
CZPAssetUtils::GetHostAppDocFileTypeInfo(
	bool						inForTemplate) const
{
	if(Utils<IZPInDnUtils>()->IsHostAppInDesign())
	{
		if( inForTemplate )
			return kIDTemplateFileTypeInfoID;
		else
			return kInDesignFileTypeInfoID;
	}
	else if(Utils<IZPInDnUtils>()->IsHostAppInCopy())
	{
		if( inForTemplate )
			return kInCopyTemplateFileTypeInfoID;
		else
#if defined(InDnCS5) || defined(InDnCS5_5)
			return kInCopyXMLMarkupFileTypeInfoID;
#else
			return  kInCopyXMLInterchangeFileTypeInfoID;
#endif
	}
	
	ASSERT(false);	//must never come here.
	return kInDesignFileTypeInfoID;
}

//----------------------------------------------------------------------------------------
// GetAssetFileExtenstion
//----------------------------------------------------------------------------------------
const PMString &
CZPAssetUtils::GetAssetFileExtenstion(
	const enAssetType			inType) const
{
	switch( inType )
	{
		case eAssetType_IDDocument :	return kAssetFileExt_InDesignDocument;
		case eAssetType_IDTemplate :	return kAssetFileExt_InDesignTemplate;
		case eAssetType_ICTemplate :	return kAssetFileExt_InCopyTemplate;
		case eAssetType_ICAssignment :	return kAssetFileExt_InCopyAssignment;
		case eAssetType_ICDocument :
		case eAssetType_IDStory :		return kAssetFileExt_InCopyDocument;
	}
	return kNullString;
}

//----------------------------------------------------------------------------------------
// GetAssetFileExtenstion
//----------------------------------------------------------------------------------------
const PMString &
CZPAssetUtils::GetAssetFileExtenstion(
	const PMString &			inTypeID) const
{
	LogFunctionEnterExit;
	
	if (inTypeID.Compare( kFalse, kAssetType_InDesignDocumentID) == 0)
		return kAssetFileExt_InDesignDocument;
	else if(inTypeID.Compare( kFalse, kAssetType_InCopyDocumentID) == 0)
		return kAssetFileExt_InCopyDocument;
	else if (inTypeID.Compare( kFalse, kAssetType_InDesignTemplateID) == 0)
		return kAssetFileExt_InDesignTemplate;
	else if(inTypeID.Compare( kFalse, kAssetType_InCopyTemplateID) == 0)
		return kAssetFileExt_InCopyTemplate;
	else if(inTypeID.Compare( kFalse, kAssetType_InDesignStoryID) == 0)
		return kAssetFileExt_InCopyDocument;
	else if(inTypeID.Compare( kFalse, kAssetType_InCopyAssignmentID) == 0)
		return kAssetFileExt_InCopyAssignment;

	return kNullString;
}

//----------------------------------------------------------------------------------------
// CanOpenAsset
//----------------------------------------------------------------------------------------
bool
CZPAssetUtils::CanOpenAsset(
	const PMString &			inAssetID) const
{

	const IStringData * assetID = ZPDataHelper::GetAssetByID( inAssetID );
	ASSERT( assetID );
	if(!assetID)
		return false;

	bool bIsInCopy = Utils<IZPInDnUtils>()->IsHostAppInCopy() == kTrue;
	
	InterfacePtr<const IZPAssetInfo> assetInfo( assetID, UseDefaultIID() );
	ASSERT( assetInfo );

	const enAssetType assetType = assetInfo->GetType();

	// why only documents, probably also open just stories
	//InCopy false means InDesign or Server
	if( this->CanOpenAssetType( assetType ) )
	{
		return true;
	}
	else if( ( assetType == eAssetType_ICAssignment && !bIsInCopy )	// Assignment
	||( assetType == eAssetType_IDStory && !bIsInCopy ))			// ID story, i.e story part of assignment
	{
		// if indesign open assigned document
		const PMString & documentID = assetInfo->GetLinkedWithDocumentID();
		if(documentID.IsEmpty() == kFalse && documentID.Compare( kFalse, kEmptyGUID ) != 0 )
		{
			return true;
		}
	}
	return false;
}

//----------------------------------------------------------------------------------------
// CanOpenAssetType
//Check if we can open inAssetType asset type.
//----------------------------------------------------------------------------------------
bool
CZPAssetUtils::CanOpenAssetType(
	enAssetType					inAssetType) const
{
	bool bIsInCopy = Utils<IZPInDnUtils>()->IsHostAppInCopy() == kTrue;

	bool toReturn = ( ( inAssetType == eAssetType_IDDocument && !bIsInCopy )		// InDesign Document
	||( inAssetType == eAssetType_IDTemplate && !bIsInCopy )						// InDesign Template
	||( inAssetType == eAssetType_ICAssignment && bIsInCopy )						// Assignment
	||( inAssetType == eAssetType_IDStory && bIsInCopy )							// ID story can be opened now.
	||( inAssetType == eAssetType_ICDocument && bIsInCopy )							// InCopy Document
	||( inAssetType == eAssetType_ICTemplate && bIsInCopy ) );						// InCopy Template

	return toReturn;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// CreateLocalFilePathFromAssetID
//----------------------------------------------------------------------------------------
IDFile
CZPAssetUtils::CreateLocalFilePathFromAssetID(
	const PMString &			inAssetID,
	const PMString &			inFileExtension,
	const PMString &			inName) const
{
	LogFunctionEnterExit;
	
	IZPLog_Str( thisFileLA, enLT_DebugInfo, "AssetID = %s", inAssetID.GrabCString());
	
	IDFile		toReturn;
	const PMString * fileName = &inAssetID;
	
	// we define a local replica file to have this file name
	//  but be in a special replica folder in the app directory
	if ( Utils<IZPFileUtils>()->GetLocalStoreFolder( toReturn ) )
	{
#ifdef CacheWithFolder
		PMString folderPathStr(inAssetID);
		Utils<IZPFileUtils>()->AppendPath( toReturn, folderPathStr);
#ifdef AssetFileOfAssetName
		//This is the only place where we decide the file name of asset.
		if(!inName.IsEmpty())
			fileName = &inName;
#endif
#endif
		Utils<IZPFileUtils>()->AppendPath( toReturn, *fileName, &inFileExtension );
	}

#ifdef DEBUG
		PMString pathStr;
		Utils<IZPFileUtils>()->ConvertIDFileToStr( toReturn, pathStr );
		IZPLog_Str_( thisFileLA, enLT_DebugInfo, " file path = '%s'", pathStr.GrabCString() );
#endif
	return toReturn;
}

//----------------------------------------------------------------------------------------
// CreateAssetCacheFolderPath
//----------------------------------------------------------------------------------------
IDFile
CZPAssetUtils::CreateAssetCacheFolderPath(
	const PMString &			inAssetID,
	bool						createIfNotExists) const
{
	LogFunctionEnterExit;
	
	IZPLog_Str( thisFileLA, enLT_DebugInfo, "AssetID = %s", inAssetID.GrabCString());
	
	IDFile		toReturn;
	Utils<IZPFileUtils> zpFileUtils;
	if ( zpFileUtils->GetLocalStoreFolder( toReturn ) )
	{
#ifdef CacheWithFolder
		PMString folderPathStr(inAssetID);
		zpFileUtils->AppendPath( toReturn, folderPathStr);
#endif
	}

	if (createIfNotExists)
		zpFileUtils->CreateFolderIfNeeded(toReturn);

#ifdef DEBUG
	PMString pathStr;
	zpFileUtils->ConvertIDFileToStr( toReturn, pathStr );
	IZPLog_Str_( thisFileLA, enLT_DebugInfo, " file path = '%s'", pathStr.GrabCString() );
#endif

	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetAssetIDFromLocalFile
//----------------------------------------------------------------------------------------
PMString
CZPAssetUtils::GetAssetIDFromLocalFile(
	const IDFile&				inFile) const
{
	LogFunctionEnterExit;
	
	PMString	toReturn;
#ifdef CacheWithFolder
	Utils<IZPFileUtils>()->GetParentDirectoryName(inFile, toReturn);
	if (toReturn.CharCount() <= 5) //iZine
		Utils<IZPFileUtils>()->GetFileName( inFile, toReturn );		//Possibly this path is before 3.0.5
#else
	Utils<IZPFileUtils>()->GetFileName( inFile, toReturn );
#endif
	ASSERT(toReturn.CharCount() == 36);	//Asset ID is GUID, and any GUID must be 36 bytes long
	return toReturn;
}

//----------------------------------------------------------------------------------------
// GetAssetIDFromLocalFile
//----------------------------------------------------------------------------------------
PMString
CZPAssetUtils::GetAssetIDFromLocalFile(
	const PMString &			inFilePath) const
{
	LogFunctionEnterExit;
	
	PMString	toReturn;
#ifdef CacheWithFolder
	Utils<IZPFileUtils>()->GetParentDirectoryName(inFilePath, toReturn);
	if (toReturn.CharCount() <= 5) //iZine
		Utils<IZPFileUtils>()->GetFileName( inFilePath, toReturn );		//Possibly this path is before 3.0.5
#else
	Utils<IZPFileUtils>()->GetFileName( inFilePath, toReturn );
#endif
	ASSERT(toReturn.CharCount() == 36);	//Asset ID is GUID, and any GUID must be 36 bytes long
	return toReturn;
}

#pragma mark -

//----------------------------------------------------------------------------------------
// CopyAssetDataFromAsset
// Copy IZPAssetData values from cached asset object.
// Not used yet.
//----------------------------------------------------------------------------------------
bool
CZPAssetUtils::CopyAssetDataFromAsset(
	const IStringData *			inAssetID,
	IZPAssetData *				ioCopyDataTo) const
{
#if defined(InDnCS5) || defined(InDnCS5_5)
		const IExecutionContext* ec = GetExecutionContext();
		ISession* gSession = ec->GetSession();
#endif
	InterfacePtr<IZPAssetInfo> assetInfo( inAssetID, UseDefaultIID() );
	ASSERT( assetInfo );

	if( !assetInfo )
		return false;
	ioCopyDataTo->SetAssetID( inAssetID->Get() );
	//find edition from EditionMgr, for AssetID
	InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<const IZPEditionMgr> theEditionMgr( zpApp->GetEditionMgr(), UseDefaultIID() );
	ASSERT(theEditionMgr);
	const IStringData * editionID = theEditionMgr->GetEditionForAssetID( inAssetID->Get() );
	if( editionID )
		ioCopyDataTo->SetEditionID( editionID->Get() );

	//find this from TitleMgr, for editionID
	InterfacePtr<const IZPTitleMgr> theTitleMgr( zpApp->GetTitleMgr(), UseDefaultIID() );
	ASSERT(theTitleMgr);
	const IStringData * titleID = theTitleMgr->GetTitleForEditionID( editionID->Get() );
	if( titleID )
		ioCopyDataTo->SetTitleID( titleID->Get() );
	
	return true;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// NewGUID
//----------------------------------------------------------------------------------------
PMString
CZPAssetUtils::NewGUID()const
{
	LogFunctionEnterExit;
	
#ifdef MACINTOSH
	uuid_t uu;
	char uuid_s[37];
	uuid_generate(uu);
	uuid_unparse(uu, uuid_s);
	#if defined(InDnCS5) || defined(InDnCS5_5)
		PMString id(uuid_s, PMString::kUnknownEncoding);
	#else
		PMString id(uuid_s, -1, PMString::kNoTranslate);
	#endif
	
	id.ToLower();
	return (id);
#else
	// see http://trac.xapian.org/ticket/303 for similar issue
	// http://trac.xapian.org/attachment/ticket/303/win32-uuid-cleanup.patch
	const size_t UUID_SIZE = 16;

	do {

		UUID uuid;
		if (UuidCreate(&uuid) != RPC_S_OK)
			break;
		
		RPC_WSTR uuidstr;
		if (UuidToString(&uuid, (RPC_WSTR*)(&uuidstr)) != RPC_S_OK)
			break;
		
		WideString id(uuidstr);
		id.ToLower();

		RpcStringFree((RPC_WSTR*)(&uuidstr));

		return (id);
	} while (kFalse);

	return (kNullString);
#endif
}

//----------------------------------------------------------------------------------------
// GetStatusIDOfAsset
//----------------------------------------------------------------------------------------
PMString
CZPAssetUtils::GetStatusIDOfAsset(
	const PMString &			inAssetID) const
{
	const IStringData * dbAssetID = ZPDataHelper::GetAssetByID( inAssetID );
	return this->GetStatusIDOfAsset(dbAssetID);
}

//----------------------------------------------------------------------------------------
// GetStatusIDOfAsset
//----------------------------------------------------------------------------------------
PMString
CZPAssetUtils::GetStatusIDOfAsset(
	const IStringData *			inDBAssetID) const
{
	InterfacePtr<IZPAssetInfo> assetInfo( inDBAssetID, UseDefaultIID() );
	ASSERT( assetInfo );
	if( !assetInfo )
		return kNullString;

	return assetInfo->GetStatusID();
}

#if defined(InDnCS5) || defined(InDnCS5_5)
//----------------------------------------------------------------------------------------
// FileToURI
//----------------------------------------------------------------------------------------
bool
CZPAssetUtils::FileToURI(
	const IDFile&				inFilePath,
	URI&						oURI) const
{
	PMString parentDirectory;
	PMString fileExtension;
	Utils<IZPFileUtils>()->GetExtension(inFilePath, fileExtension);
	if( Utils<IZPFileUtils>()->GetParentDirectoryName(inFilePath, parentDirectory) == false)
		return kFalse;

	if(parentDirectory.IsNull() || parentDirectory.IsEmpty())
		return kFalse;

	//FORMAT:		IZPub://jpg/34686-356-7236-98357
	PMString uri(kURIScheme);
	uri.Append("://");
	uri.Append(fileExtension);
	uri.Append("/");
	uri.Append(parentDirectory);

	oURI.SetURI(uri.GetPlatformString());

	return kTrue;
}

//----------------------------------------------------------------------------------------
// URIToIDFile
//----------------------------------------------------------------------------------------
bool
CZPAssetUtils::URIToIDFile(
	const URI&					inURI,
	IDFile&						oFilePath) const
{
	WideString uriStr = inURI.GetURIW();
	if(uriStr.IsNull() || uriStr.CharCount() <= 8)	//	"IZPub://"
		return kFalse;

	WideString doubleSlash("//");
	WideString singleSlash("/");
	int lengthStr = uriStr.CharCount();

	//Extension first
	int index1 = uriStr.IndexOf( doubleSlash );
	int index2 = uriStr.IndexOf( singleSlash, index1 + 2 );
	index1 = index1 + 2;
	WideString *ext = uriStr.Substring(index1, index2 - index1);
	if(!ext)
		return kFalse;

	++index2;
	//Asset Id	
	WideString *assetIdStr = uriStr.Substring(index2, lengthStr - index2);
	if(!assetIdStr)
		return kFalse;

	const IExecutionContext* ec = GetExecutionContext();
	ISession* gSession = ec->GetSession();
	InterfacePtr<const IZPApp> zpApp( gSession, UseDefaultIID() );
	ASSERT( zpApp );

	InterfacePtr<const IZPAssetMgr> assetMgr( zpApp->GetAssetMgr(), UseDefaultIID() );
	ASSERT(assetMgr);

	PMString assetIdPMStr(*assetIdStr);
	const IStringData * theAssetID = assetMgr->GetAssetByID( assetIdPMStr );
	if( !theAssetID )
		return kFalse;

	InterfacePtr<IZPAssetInfo> assetInfo( theAssetID, UseDefaultIID() );
	ASSERT( assetInfo );
	if( !assetInfo )
		return kFalse;

	const ZPAssetDetailInfo & assetDetailInfo = assetInfo->GetAssetDetail();
	PMString assetName = assetDetailInfo.mDocInfo.mName;

	oFilePath = this->CreateLocalFilePathFromAssetID(assetIdPMStr, PMString(*ext), assetName);

	return kTrue;
}

#endif