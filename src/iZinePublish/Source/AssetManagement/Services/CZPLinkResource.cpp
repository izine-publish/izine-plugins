//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/Services/CZPLinkResource.cpp $
//	$Revision: 3923 $
//	$Date: 2012-02-16 05:25:15 +0100 (Thu, 16 Feb 2012) $
//	$Author: aman.alam $
//	
//	Creator: Aman Alam
//	Created: 14-2-2012
//	Copyright: 2008-2011 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

#include "CZPLinkResource.h"

//SDK Interfaces
#include "ILinkObjectReference.h"
#include "IStoreInternal.h"

//SDK General includes
#include "FileTypeInfo.h"
#include "NameInfo.h"
#include "StreamUtil.h"
#include "StringNumeric.h"
#include "Utils.h"

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

CREATE_PMINTERFACE(CZPLinkResource, kZPLinkResourceImpl)

CZPLinkResource::CZPLinkResource(IPMUnknown * boss)
: inheritClass(boss)
  ,mDB(::GetDataBase(boss))
  ,mClientId(kIDLinkClientID)
  ,mState(kUnknown)
  ,mStoreState(kNormal)
  //,mStoredResourceUID(kInvalidUID)
{
}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPLinkResource::~CZPLinkResource()
{
}

#pragma mark -
//----------------------------------------------------------------------------------------
// GetShortName
//----------------------------------------------------------------------------------------
WideString
CZPLinkResource::GetShortName(
	bool bUIName) const
{
	IDFile file;
	Utils<IZPAssetUtils>()->URIToIDFile(mId, file);

	AString name(file.GetNameStr());
	return WideString(name.GrabWString(), name.UTF16Count(), name.CharCount());
}

//----------------------------------------------------------------------------------------
// GetLongName
//----------------------------------------------------------------------------------------
WideString
CZPLinkResource::GetLongName(
	bool bUIName) const
{
	IDFile file;
	WideString longName;
	if(Utils<IZPAssetUtils>()->URIToIDFile(mId, file))
	{
		PMString fileName;
		Utils<IZPFileUtils>()->GetFileName(file, fileName, kTrue);
		longName = WideString(fileName);
	}
	
	return longName;
}

//----------------------------------------------------------------------------------------
// SetState
//----------------------------------------------------------------------------------------
void
CZPLinkResource::SetState(
	ResourceState state)
{
	if (state == kUnknown)
	{
		ASSERT_FAIL("CZPLinkResource::SetState() - kUnknown is a reserved state, and cannot be set!");
		return;
	}

	if (mState != state) 
		mState = state;
}

//----------------------------------------------------------------------------------------
// SetStamp
//----------------------------------------------------------------------------------------
void
CZPLinkResource::SetStamp(
	const ResourceStamp& stamp)
{
	mStamp = stamp;
}

//----------------------------------------------------------------------------------------
// IsStampEquivalent
//----------------------------------------------------------------------------------------
bool
CZPLinkResource::IsStampEquivalent(
	const ResourceStamp& stamp) const
{
	return (mStamp == stamp);
}

//----------------------------------------------------------------------------------------
// SetModTime
//----------------------------------------------------------------------------------------
void
CZPLinkResource::SetModTime(
	const IDTime& time)
{
	if (mModTime != time)
		mModTime = time;
}

//----------------------------------------------------------------------------------------
// SetSize
//----------------------------------------------------------------------------------------
void
CZPLinkResource::SetSize(
	uint64 size)
{
	if (mSize != size)
		mSize = size;
}

//----------------------------------------------------------------------------------------
// GetDataType
//----------------------------------------------------------------------------------------
FileTypeInfo
CZPLinkResource::GetDataType()const
{
	IDFile file;
	if (!Utils<IZPAssetUtils>()->URIToIDFile(mId, file)) {
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
// SetFormatType
//----------------------------------------------------------------------------------------
void
CZPLinkResource::SetFormatType(
	const PMString& formatType)
{
	if (mFormatType != formatType)
		mFormatType = formatType;
}

//----------------------------------------------------------------------------------------
// IsAccessSupported
//----------------------------------------------------------------------------------------
bool
CZPLinkResource::IsAccessSupported(
	AccessMode mode) const
{
	if (mode == kRead)
		return true;
	else
		return false;
}

//----------------------------------------------------------------------------------------
// CanQueryStream
//----------------------------------------------------------------------------------------
bool
CZPLinkResource::CanQueryStream(
	AccessMode mode) const
{
	if (mode == kRead)
		return true;
	else
		return false;
}

//----------------------------------------------------------------------------------------
// QueryStream
//----------------------------------------------------------------------------------------
IPMStream*
CZPLinkResource::QueryStream(
	AccessMode mode) const
{
	switch (mode)
	{
		case kRead:
			//return QueryReadStream();
			break;
		default:
			ASSERT_FAIL("CZPLinkResource::QueryStream() - Unsupported or unrecognized access mode!");
			break;
	}

	return nil;
}

//----------------------------------------------------------------------------------------
// CopyAttributes
//----------------------------------------------------------------------------------------
ErrorCode
CZPLinkResource::CopyAttributes(
	const UIDRef& resourceRef)
{
	return kFailure;
}
#if 0
//----------------------------------------------------------------------------------------
// Init
//----------------------------------------------------------------------------------------
ErrorCode
CZPLinkResource::Init(
	IDataLink* iDataLink, const URI& uri)
{
	NameInfo	nameInfo;
	PMString	formatName;
	uint32		fileType;
	ErrorCode	errorCode = iDataLink->GetNameInfo (&nameInfo, &formatName, &fileType);

	mId = uri;

	if (errorCode == kSuccess)
	{
		uint64		storedSize = 0;
		uint64		storedTime = 0;
		IDataLink::StateType	calculatedType = IDataLink::kLinkNormal;
		IDataLink::StateType	oldStoredType = iDataLink->GetStoredState (&storedSize, &storedTime);

		uint64		modTime = 0;

		//	Check if times are consistent or close enough.
		ResourceStamp	resourceStamp;
		if (oldStoredType != IDataLink::kEmbedded)
		{
			uint64		currentSize = 0;
			uint64		currentTime = 0;
			bool16		fileExists = GetCurrentTimeAndSizeFromURI (currentTime, currentSize);

			if (fileExists)
			{
#ifdef DEBUG
				bool	hasStamp = 
#endif
					ConstructStampFromURI (resourceStamp);
				ASSERT (hasStamp);

				if (storedSize != currentSize || TimeStampsCloseEnough (storedTime, currentTime) == false)
				{
					//ASSERT (oldStoredType == IDataLink::kLinkOutOfDate);

					ConstructStampFromTimeSize (storedTime, storedSize, resourceStamp);
					modTime = storedTime;

					if (oldStoredType != IDataLink::kLinkOutOfDate)
					{
						calculatedType = IDataLink::kLinkOutOfDate;
						iDataLink->SetStoredState (nil, nil, calculatedType);
					}
				}
				else
				{
					modTime = currentTime;
				}
			}
			else
			{
				//	File does not exist.
				// the persisted type can easily be different from the current type.

				calculatedType = IDataLink::kLinkMissing;
				ConstructStampFromTimeSize (storedTime, storedSize, resourceStamp);
				modTime = storedTime;
			}
		}
		else
		{
			//	Embedded
			calculatedType = IDataLink::kEmbedded;
			ConstructStampFromTimeSize (storedTime, storedSize, resourceStamp);
			modTime = storedTime;
		}

		SetSize(storedSize);

		// the persisted type can easily be different from the current type.
		// commenting out assert. dstephens 06/06/2007
		//ASSERT (calculatedType == oldStoredType);

		ILinkResource::ResourceState	state = (oldStoredType == IDataLink::kLinkMissing) ? kMissing : kAvailable;

		//	Set attributes
		SetFormatType(formatName);
		SetModTime(modTime);
		SetStamp(resourceStamp);
		SetState(state);

		//	Embeded
		if (oldStoredType == IDataLink::kEmbedded)
		{
			InterfacePtr<ILinkObjectReference>	iOldObjectRef (iDataLink, UseDefaultIID ());
			if (iOldObjectRef != nil && iOldObjectRef->GetUID () != kInvalidUID)
			{
				InterfacePtr<IStoreInternal>	iStoreInternal (mDB, iOldObjectRef->GetUID (), IID_ISTOREINTERNAL);
				
				//	Delete old stored data
				UID		storedUID = iStoreInternal->GetStoredUID ();
				if (storedUID != kInvalidUID && mDB->IsValidUID (storedUID))
				{
					//iStoreInternal->SetStoredUID (kInvalidUID);
					mStoreState = kEmbedded;
					SetState(kAvailable);
					mStoredResourceUID = storedUID;
				}
			}
		}
	}

	return errorCode;
}

//----------------------------------------------------------------------------------------
// GetCurrentTimeAndSizeFromURI
//----------------------------------------------------------------------------------------
bool
CZPLinkResource::GetCurrentTimeAndSizeFromURI(
	ATime& currentTime, uint64& currentSize)
{
	IDFile	file;
	Utils<IZPAssetUtils>()->URIToIDFile(mId, file);
	bool bFileExists(file.Exists());

	currentTime = 0;
	currentSize = 0;
	if (bFileExists)
	{
		currentTime = 0;//Utils<ICusDtLnkFacade>()->GetAssetTimestampFromURI(fId);
		currentSize = file.GetSize();
	}

	return bFileExists;
}

//----------------------------------------------------------------------------------------
// ConstructStampFromURI
//----------------------------------------------------------------------------------------
bool
CZPLinkResource::ConstructStampFromURI(
	WideString& newStamp)
{
	ATime	modifiedTime (0);
	uint64	size (0);
	bool bFileExists = GetCurrentTimeAndSizeFromURI(modifiedTime, size);

	if (bFileExists)
	{
		ConstructStampFromTimeSize(modifiedTime, size, newStamp);
	}

	return (bFileExists);
}

//----------------------------------------------------------------------------------------
// ConstructStampFromTimeSize
//----------------------------------------------------------------------------------------
static const WideString k_ConstructStampFromTimeSize_scheme(kURIScheme);
void
CZPLinkResource::ConstructStampFromTimeSize(
	ATime storedTime, uint64 storedSize, WideString& newStamp)
{
	/*
	// update the resource's stamp
	newStamp.clear ();
	newStamp.Append(k_ConstructStampFromTimeSize_scheme);

	WideString modTimeStr;
	if (stringnumeric::ToString(time, modTimeStr) == kSuccess)
	{
		newStamp.Append(modTimeStr);
	}
	else
	{
		ASSERT_FAIL ("CZPLinkResource::ConstructStampFromTimeSize - mod time to WideString failed!");
	
		WideString	dummy ("0000");
		newStamp.Append (dummy);
	}
	
	newStamp.Append(WideString(" "));

	WideString sizeStr;
	if (stringnumeric::ToString(storedSize, sizeStr) == kSuccess)
	{
		newStamp.Append(sizeStr);
	}
	else
	{
		ASSERT_FAIL ("CZPLinkResource::ConstructStampFromTimeSize - File size to WideString failed!");

		WideString	dummy ("0000");
		newStamp.Append (dummy);
	}*/
}

//----------------------------------------------------------------------------------------
// TimeStampsCloseEnough
//----------------------------------------------------------------------------------------
bool
CZPLinkResource::TimeStampsCloseEnough(
	uint64 storedTimeStamp, uint64 currentTimeStamp)
{
	if (storedTimeStamp == currentTimeStamp)
		return true;

	//	If the time stamps are not the same, then we check to see how close the times are.
	//	This hueristic is copied from the old DataLink code.
	//	The reasoning is as follows:
	//		Moving a file from NTFS to FAT makes the links modified because the
	//		time resolution on FAT is 2 second intervals and NTFS is 100 nanoseconds.
	//		There are (normal?) cases where the time stamp on different files are very close together.
	//		For example, QA's test files as they are copied out of Perforce are given the system's
	//		current time.  We don't want to sometimes say the link is up-to-date after you relink
	//		to a different file.


	// Get the time resolution of the file and compare at it's resolution.
	// Compare at 2 second intervals when one of the timestamps doesn't have any fractions of a second.
	if (currentTimeStamp % (uint64)GlobalTime::kOneSecond &&
		storedTimeStamp % (uint64)GlobalTime::kOneSecond)
		return false;  // Both have fractions of a second, we are not lax in this situation, must be exact.

	// Get the seconds, no fractions of a second.
	storedTimeStamp = storedTimeStamp / (uint64)GlobalTime::kOneSecond;
	currentTimeStamp = currentTimeStamp / (uint64)GlobalTime::kOneSecond;

	uint64 diff;
	if (storedTimeStamp > currentTimeStamp)
		diff = storedTimeStamp - currentTimeStamp;
	else
		diff = currentTimeStamp - storedTimeStamp;
	// diff is positive

	if (diff <= 2)
	{
		// OK, one time stamp doesn't have any fractions of a second
		// and the other one is within two seconds, well give it to you.

		return true; // The time stamps are close enough.
	}

	// If the difference is less than a day, check further.
	if (diff < 24 * 3600 + 2)
	{
		// Off by an hour is common when moving between Mac and Windows.  Off by 23 and 24 hours
		// occurs too but much less frequently.

		// Is the difference one hour plus or minus 2 seconds?
		if (diff > 3600 - 2 && diff < 3600 + 2)
			return true;

		// Is the difference 23 hours plus or minus 2 seconds?
		if (diff > 23 * 3600 - 2 && diff < 23 * 3600 + 2)
			return true;

		// Is the difference 24 hours plus or minus 2 seconds?
		if (diff > 24 * 3600 - 2 /*already done above && diff < 24 * 3600 + 2*/ )
			return true;
	}

	return false; // The time stamps are not the same.
}

//----------------------------------------------------------------------------------------
// QueryReadStream
//----------------------------------------------------------------------------------------
IPMStream*
CZPLinkResource::QueryReadStream()const
{
	if (mStoredResourceUID != kInvalidUID)
	{
		return StreamUtil::CreateDBStreamRead(mDB, mStoredResourceUID);;
	}

	return nil;
}
#endif