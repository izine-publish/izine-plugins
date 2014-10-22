//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/Services/CZPLinkResourceHandler.cpp $
//	$Revision: 3923 $
//	$Date: 2012-02-16 05:25:15 +0100 (Thu, 16 Feb 2012) $
//	$Author: aman.alam $
//	
//	Creator: Aman Alam
//	Created: 13-2-2012
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

#include "CZPLinkResourceHandler.h"

//SDK Interfaces

//SDK General includes
#include "IPMStream.h"
#include "IURIUtils.h"
#include "CoreFileUtils.h"
#include "AString.h"
#include "FileTypeInfo.h"
#include "FileUtils.h"
#include "IDFile.h"
#include "StreamUtil.h"
#include "URI.h"
#include "Utils.h"
#include "WideString.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPAssetUtils.h"
#include "IZPFileUtils.h"

//IZP General includes
#include "ZPConstants.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

CREATE_PMINTERFACE(CZPLinkResourceHandler, kZPLinkResourceHandlerImpl)

CZPLinkResourceHandler::CZPLinkResourceHandler(IPMUnknown * boss)
: inheritClass(boss)
{
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPLinkResourceHandler::~CZPLinkResourceHandler()
{
}

#pragma mark -
//----------------------------------------------------------------------------------------
// IsResourceURIValid
//----------------------------------------------------------------------------------------
static const WideString k_fileScheme(kURIScheme);
bool
CZPLinkResourceHandler::IsResourceURIValid(
	const UIDRef& ref, const URI& uri) const
{
	WideString uriScheme(uri.GetComponent(URI::kScheme));
	if (uriScheme != k_fileScheme) {
		return false;
	}

	WideString path(uri.GetComponent(URI::kPath));
	return !path.IsNull();
}

//----------------------------------------------------------------------------------------
// AreResourceIdsEquivalent
//----------------------------------------------------------------------------------------
bool
CZPLinkResourceHandler::AreResourceIdsEquivalent(
	const ILinkResource::ResourceId& id1, const ILinkResource::ResourceId& id2) const
{
	return (id1 == id2);
}

//----------------------------------------------------------------------------------------
// GetResourceDataType
//----------------------------------------------------------------------------------------
FileTypeInfo
CZPLinkResourceHandler::GetResourceDataType(
	const UIDRef& ref, const URI& uri) const
{
	IDFile file;
	if (!Utils<IZPAssetUtils>()->URIToIDFile(uri, file)) {
		return FileTypeInfo();
	}

	AString extension(file.GetName().GetExtension());

#ifdef MACINTOSH
	AOSType creator;
	AOSType type;
	MacFileUtils::GetCreatorAndType(file, creator, type);
	return FileTypeInfo(type, creator, PMString(extension.GrabWString(), extension.UTF16Count()), "", 0);
#else
	return FileTypeInfo(0, 0, PMString(extension.GrabWString(), extension.UTF16Count()), "", 0);
#endif
}

//----------------------------------------------------------------------------------------
// GetShortResourceName
//----------------------------------------------------------------------------------------
WideString
CZPLinkResourceHandler::GetShortResourceName(
	const UIDRef& ref, const URI& uri, bool bUIName) const
{
	IDFile file;
	Utils<IZPAssetUtils>()->URIToIDFile(uri, file);

	AString name(file.GetNameStr());
	return WideString(name.GrabWString(), name.UTF16Count(), name.CharCount());
}

//----------------------------------------------------------------------------------------
// GetLongResourceName
//----------------------------------------------------------------------------------------
WideString
CZPLinkResourceHandler::GetLongResourceName(
	const UIDRef& ref, const URI& uri, bool bUIName) const
{
	IDFile file;
	WideString longName;
	if(Utils<IZPAssetUtils>()->URIToIDFile(uri, file))
	{
		PMString fileName;
		Utils<IZPFileUtils>()->GetFileName(file, fileName, kTrue);
		longName = WideString(fileName);
	}
	
	return longName;
}

//----------------------------------------------------------------------------------------
// AreStampsEquivalent
//----------------------------------------------------------------------------------------
bool
CZPLinkResourceHandler::AreStampsEquivalent(
	const WideString& stamp1, const WideString& stamp2) const
{
	if (stamp1 == stamp2) {
		return true;
	}

	// TODO
	return (stamp1 == stamp2);
}

//----------------------------------------------------------------------------------------
// CanReadResource
//----------------------------------------------------------------------------------------
bool
CZPLinkResourceHandler::CanReadResource(
	const UIDRef& ref, const URI& uri) const
{
	IDFile file;
	return (Utils<IZPAssetUtils>()->URIToIDFile(uri, file) && file.GetAttribute(IDFile::kReadable));
}

//----------------------------------------------------------------------------------------
// CanWriteResource
//----------------------------------------------------------------------------------------
bool
CZPLinkResourceHandler::CanWriteResource(
	const UIDRef& ref, const URI& uri) const
{
	return false;
}

//----------------------------------------------------------------------------------------
// CanReadWriteResource
//----------------------------------------------------------------------------------------
bool
CZPLinkResourceHandler::CanReadWriteResource(
	const UIDRef& ref, const URI& uri) const
{
	return false;
}

//----------------------------------------------------------------------------------------
// CanCreateResourceStream
//----------------------------------------------------------------------------------------
bool
CZPLinkResourceHandler::CanCreateResourceStream(
	const UIDRef& ref, const URI& uri, ILinkResource::AccessMode mode) const
{
	IDFile file;
	if (!Utils<IZPAssetUtils>()->URIToIDFile(uri, file))
		return false;

	switch (mode)
	{
		case ILinkResource::kRead:
			return file.CanOpen(IDFile::kReadAccess, IDFile::kShareReadAccess);
			break;
		case ILinkResource::kWrite:
			break;
		case ILinkResource::kReadWrite:
			break;
		default:
			ASSERT_FAIL("CZPLinkResourceHandler::CanCreateResourceStream() - Unrecognized access mode!");
			break;
	}

	return false;
}

//----------------------------------------------------------------------------------------
// CreateResourceReadStream
//----------------------------------------------------------------------------------------
IPMStream*
CZPLinkResourceHandler::CreateResourceReadStream(
	const UIDRef& ref, const URI& uri) const
{
	IDFile file;
	if (!Utils<IZPAssetUtils>()->URIToIDFile(uri, file) || !file.GetAttribute(IDFile::kReadable)) {
		return nil;
	}

#ifdef MACINTOSH
	AOSType creator;
	AOSType type;
	MacFileUtils::GetCreatorAndType(file, creator, type);
	return InterfacePtr<IPMStream>(StreamUtil::CreateFileStreamReadLazy(file, kOpenIn, type, creator)).forget();
#else
	return InterfacePtr<IPMStream>(StreamUtil::CreateFileStreamReadLazy(file, kOpenIn)).forget();
#endif
}

//----------------------------------------------------------------------------------------
// CreateResourceWriteStream
//----------------------------------------------------------------------------------------
IPMStream*
CZPLinkResourceHandler::CreateResourceWriteStream(
	const UIDRef& ref, const URI& uri) const
{
	return nil;
}

//----------------------------------------------------------------------------------------
// CreateResourceReadWriteStream
//----------------------------------------------------------------------------------------
IPMStream*
CZPLinkResourceHandler::CreateResourceReadWriteStream(
	const UIDRef& ref, const URI& uri) const
{
	return nil;
}

//----------------------------------------------------------------------------------------
// CanCopyToFile
//----------------------------------------------------------------------------------------
bool
CZPLinkResourceHandler::CanCopyToFile(
	const UIDRef& ref, const URI& uri) const
{
	IDFile file;
	return (Utils<IZPAssetUtils>()->URIToIDFile(uri, file) && file.Exists());
}

//----------------------------------------------------------------------------------------
// CopyToFile
//----------------------------------------------------------------------------------------
ErrorCode
CZPLinkResourceHandler::CopyToFile(
	const UIDRef& ref, const URI& uri, IDFile& file) const
{
	IDFile rsrcFile;
	if (!Utils<IZPAssetUtils>()->URIToIDFile(uri, rsrcFile))
		return kFailure;
	
	return CoreFileUtils::CopyFile(rsrcFile, file) ? kSuccess : kFailure;
}

//----------------------------------------------------------------------------------------
// CanEditResource
//----------------------------------------------------------------------------------------
bool
CZPLinkResourceHandler::CanEditResource(
	const UIDRef& ref, const URI& uri) const
{
	IDFile file;
	return (Utils<IZPAssetUtils>()->URIToIDFile(uri, file) && file.GetAttribute(IDFile::kWritable));
}

//----------------------------------------------------------------------------------------
// EditResource
//----------------------------------------------------------------------------------------
ErrorCode
CZPLinkResourceHandler::EditResource(
	const UIDRef& ref, const URI& uri, const AppInfo& appInfo, PMString* errorString) const
{
	IDFile file;
	return (Utils<IZPAssetUtils>()->URIToIDFile(uri, file) && file.GetAttribute(IDFile::kWritable))
			? FileUtils::OpenFileInEditor(file, appInfo, errorString) : kFailure;
}

//----------------------------------------------------------------------------------------
// CanRevealResource
//----------------------------------------------------------------------------------------
bool
CZPLinkResourceHandler::CanRevealResource(
	const UIDRef& ref, const URI& uri) const
{
	return false;
}

//----------------------------------------------------------------------------------------
// RevealResource
//----------------------------------------------------------------------------------------
ErrorCode
CZPLinkResourceHandler::RevealResource(
	const UIDRef& ref, const URI& uri) const
{
	return kFailure;
}

//----------------------------------------------------------------------------------------
// CanRevealResourceInBridge
//----------------------------------------------------------------------------------------
bool
CZPLinkResourceHandler::CanRevealResourceInBridge(
	const UIDRef& ref, const URI& uri) const
{
	return false;
}

//----------------------------------------------------------------------------------------
// RevealResourceInBridge
//----------------------------------------------------------------------------------------
ErrorCode
CZPLinkResourceHandler::RevealResourceInBridge(
	const UIDRef& ref, const URI& uri) const
{
	return kFailure;
}

//----------------------------------------------------------------------------------------
// CanRevealResourceInMiniBridge
//----------------------------------------------------------------------------------------
bool
CZPLinkResourceHandler::CanRevealResourceInMiniBridge(
	const UIDRef& ref, const URI& uri) const
{
	return false;
}

//----------------------------------------------------------------------------------------
// RevealResourceInMiniBridge
//----------------------------------------------------------------------------------------
ErrorCode
CZPLinkResourceHandler::RevealResourceInMiniBridge(
	const UIDRef& ref, const URI& uri) const
{
	return kFailure;
}