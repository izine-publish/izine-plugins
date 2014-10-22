//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/AssetManagement/Services/CZPLinkResource.h $
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
#include "ILinkResource.h"

#pragma mark -
#include "IDataLink.h"
#include "URI.h"
#include "IDTime.h"

#define inheritClass CPMUnknown<ILinkResource>
class CZPLinkResource : public inheritClass
{
public:
						CZPLinkResource(IPMUnknown * boss);
	virtual				~CZPLinkResource();

	virtual ResourceId GetURI() const { return mId; }
	virtual ResourceId GetId() const { return mId; }
	virtual bool IsIdEquivalent(const ResourceId& id) const { ASSERT_UNIMPLEMENTED(); return false; }

	virtual LinkClientID GetClientID() const { return mClientId; }

	virtual WideString GetShortName(bool bUIName) const;
	virtual WideString GetLongName(bool bUIName) const;

	virtual ResourceState GetState() const { return mState; }
	virtual void SetState(ResourceState state);

	virtual ResourceStoreState GetStoreState() const { return mStoreState; }
	virtual void SetContainedInObject() { ASSERT_UNIMPLEMENTED(); }

	virtual ResourceStamp GetStamp() const { return mStamp; }
	virtual void SetStamp(const ResourceStamp& stamp);
	virtual bool IsStampEquivalent(const ResourceStamp& stamp) const;

	virtual IDTime GetModTime() const { return mModTime; };
	virtual void SetModTime(const IDTime& time);

	virtual uint64 GetSize() const { return mSize; }
	virtual void SetSize(uint64 size);

	virtual FileTypeInfo GetDataType() const;
	virtual PMString GetFormatType() const { return mFormatType; }
	virtual void SetFormatType(const PMString& formatType);

	virtual bool IsAccessSupported(AccessMode mode) const;
	virtual bool CanQueryStream(AccessMode mode) const;
	virtual IPMStream* QueryStream(AccessMode mode) const;

	virtual bool CanCache() const {ASSERT_UNIMPLEMENTED(); return false;}
	virtual ErrorCode Cache() {ASSERT_UNIMPLEMENTED(); return kFailure;}
	virtual ErrorCode Uncache() {ASSERT_UNIMPLEMENTED(); return kFailure;}

	virtual bool CanEmbed() const {ASSERT_UNIMPLEMENTED(); return false;}
	virtual ErrorCode Embed() {ASSERT_UNIMPLEMENTED(); return kFailure;}

	virtual bool CanUnembed() const {ASSERT_UNIMPLEMENTED(); return false;}
	virtual ErrorCode Unembed() {ASSERT_UNIMPLEMENTED(); return kFailure;}

	virtual bool CanCopyToFile() const { ASSERT_UNIMPLEMENTED(); return false; }
	virtual ErrorCode CopyToFile(IDFile& file) const { ASSERT_UNIMPLEMENTED(); return kFailure; }

	virtual bool CanEdit() const {return false;}
	virtual ErrorCode Edit(const AppInfo& appInfo, PMString* errorString) {ASSERT_UNIMPLEMENTED(); return kFailure;}

	virtual bool CanReveal() const { return false; }
	virtual ErrorCode Reveal() { ASSERT_UNIMPLEMENTED(); return kFailure; }
	virtual bool CanRevealInBridge() const { return false; }
	virtual ErrorCode RevealInBridge() { ASSERT_UNIMPLEMENTED(); return kFailure; }
	virtual bool CanRevealInMiniBridge() const { return false; }
	virtual ErrorCode RevealInMiniBridge() { ASSERT_UNIMPLEMENTED(); return kFailure; }

	virtual void AddChildren(const UIDList& children) { ASSERT_UNIMPLEMENTED(); return; }
	virtual void DeleteChildren(const UIDList& children) { ASSERT_UNIMPLEMENTED(); return; }
	virtual uint32 GetChildren(UIDList& children) const { ASSERT_UNIMPLEMENTED(); return 0; }
	virtual uint32 GetNumChildren() const { ASSERT_UNIMPLEMENTED(); return 0; }

	virtual uint32 GetParents(UIDList& parents) const { ASSERT_UNIMPLEMENTED(); return 0; }

	virtual ErrorCode CopyAttributes(const UIDRef& resourceRef);

	virtual ErrorCode EmbedFromStream(IPMStream* stream) { ASSERT_UNIMPLEMENTED(); return kFailure; }

	//	Specific to this implementation only
//	virtual ErrorCode Init(IDataLink* iDataLink, const URI& uri);

protected:
private:
	// Prevent copy construction and assignment.
	CZPLinkResource(const CZPLinkResource&);
	CZPLinkResource& operator=(const CZPLinkResource&);

	//virtual ErrorCode Init(LinkClientID clientId, const URI& uri,  const ClassID& providerId) { ASSERT_UNIMPLEMENTED(); return kFailure; }
	//virtual ErrorCode SoftInit(const ResourceId& resourceId) { ASSERT_UNIMPLEMENTED(); return kFailure; }
	//virtual void DeleteOwnedObjects() { ASSERT_UNIMPLEMENTED(); }

	//bool GetCurrentTimeAndSizeFromURI(ATime& currentTime, uint64& currentSize);
	//bool ConstructStampFromURI(WideString& newStamp);
	//void ConstructStampFromTimeSize(ATime storedTime, uint64 storedSize, WideString& newStamp);
	//
	//bool TimeStampsCloseEnough(uint64 storedTimeStamp, uint64 currentTimeStamp);

	// Stream query methods
	IPMStream* QueryReadStream() const;

	IDataBase*			mDB;

	ResourceId			mId;
	LinkClientID		mClientId;
	uint64				mSize;
	IDTime				mModTime;
	ResourceState		mState;
	ResourceStamp		mStamp;
	PMString			mFormatType;
	ResourceStoreState	mStoreState;
//	UID					mStoredResourceUID;
};
