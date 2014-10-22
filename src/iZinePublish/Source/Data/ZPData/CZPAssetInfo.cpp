//========================================================================================
//	
//	$HeadURL: svn://localhost/izine/iZinePlns/Codebase/trunk/iZinePublish/Source/Data/ZPData/CZPAssetInfo.cpp $
//	$Revision: 3095 $
//	$Date: 2011-05-27 14:34:03 +0200 (Fri, 27 May 2011) $
//	$Author: rajkumar.sehrawat $
//	
//	Creator: Raj Kumar Sehrawat
//	Created: 24-5-2010
//	Copyright: 2008-2010 iZine Publish. All rights reserved.
//	
//	Description:
//========================================================================================
#include "VCPlugInHeaders.h"

//SDK Interfaces
#include "ISubject.h"

//SDK General includes
#include "Utils.h"

//OS / ThirdParty includes

#include "IZPID.h"

//IZP Interfaces
#include "IZPAssetInfo.h"
#include "IZPAssetUtils.h"

//IZP General includes
#include "ZPConstants.h"

#pragma mark -

#include "IZPLog.h"

#define thisFileLA	enLMA_
#define LogFunctionEnterExit	IZPLog_func( thisFileLA )

#define inheritClass CPMUnknown<IZPAssetInfo>
class CZPAssetInfo : public inheritClass
{
public:
						CZPAssetInfo(IPMUnknown * boss);
	virtual				~CZPAssetInfo();

	void				SetAssetDetail(
							const ZPAssetDetailInfo &			inDetail );

	ZPAssetDetailInfo &	GetAssetDetail();
	const ZPAssetDetailInfo &	GetAssetDetail() const;

	const PMString &	GetStatusID() const;
	const PMString &	GetLinkedWithDocumentID() const;
	const PMString &	GetLinkedWithDocumentName() const;
	const PMString &	GetDescription() const;
	const PMString &	GetVersionID() const;
	const PMString &	GetVersionAuthor() const;
	int					GetVersionNumber() const;
	const time_t &		GetVersionDate() const;
	const PMString &	GetTypeID() const;
	enAssetType			GetType() const;

	bool				IsTypeIDTemplate() const;
	bool				IsTypeIDDocument() const;
	bool				IsTypeIDStory() const;
	bool				IsTypeICDocument() const;
	bool				IsTypeICTemplate() const;
	bool				IsTypeAssignment() const;
protected:
private:
	ZPAssetDetailInfo	mDetail;
	mutable enAssetType	mCachedAssetType;
};


CREATE_PMINTERFACE(CZPAssetInfo, kZPAssetInfoImpl)

CZPAssetInfo::CZPAssetInfo(IPMUnknown * boss)
: inheritClass(boss)
, mCachedAssetType( eAssetTypeNone )
{

}

//----------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------
CZPAssetInfo::~CZPAssetInfo()
{

}

#pragma mark -
//----------------------------------------------------------------------------------------
// SetAssetDetail
//----------------------------------------------------------------------------------------
void
CZPAssetInfo::SetAssetDetail(
	const ZPAssetDetailInfo &			inDetail)
{
	if( mDetail == inDetail )
		return;

	mDetail = inDetail;
	mCachedAssetType = eAssetTypeNone;
	
	InterfacePtr<ISubject> selfSubject( this, UseDefaultIID() );
	ASSERT(selfSubject);

	selfSubject->Change( kZPAssetInfoChangedMsg, IZPAssetInfo::kDefaultIID, nil );
}


//----------------------------------------------------------------------------------------
// GetAssetDetail
//----------------------------------------------------------------------------------------
ZPAssetDetailInfo &
CZPAssetInfo::GetAssetDetail()
{
	return mDetail;
}


//----------------------------------------------------------------------------------------
// GetAssetDetail
//----------------------------------------------------------------------------------------
const ZPAssetDetailInfo &
CZPAssetInfo::GetAssetDetail()const
{
	return mDetail;
}


//----------------------------------------------------------------------------------------
// GetStatusID
//----------------------------------------------------------------------------------------
const PMString &
CZPAssetInfo::GetStatusID()const
{
	return mDetail.mStatusID;
}

//----------------------------------------------------------------------------------------
// GetLinkedWithDocumentID
//----------------------------------------------------------------------------------------
const PMString &
CZPAssetInfo::GetLinkedWithDocumentID()const
{
	return mDetail.mDocInfo.mID;
}

//----------------------------------------------------------------------------------------
// GetLinkedWithDocumentName
//----------------------------------------------------------------------------------------
const PMString &
CZPAssetInfo::GetLinkedWithDocumentName()const
{
	return mDetail.mDocInfo.mName;
}

//----------------------------------------------------------------------------------------
// GetDescription
//----------------------------------------------------------------------------------------
const PMString &
CZPAssetInfo::GetDescription()const
{
#pragma message("//TODO: asset description value is not from server.")
	return kNullString;
}

//----------------------------------------------------------------------------------------
// GetVersionID
//----------------------------------------------------------------------------------------
const PMString &
CZPAssetInfo::GetVersionID()const
{
	return mDetail.mVersionID;
}

//----------------------------------------------------------------------------------------
// GetVersionAuthor
//----------------------------------------------------------------------------------------
const PMString &
CZPAssetInfo::GetVersionAuthor()const
{
	return mDetail.mVersionUser.mName;
}

//----------------------------------------------------------------------------------------
// GetVersionNumber
//----------------------------------------------------------------------------------------
int
CZPAssetInfo::GetVersionNumber()const
{
	return mDetail.mVersionNumber;
}

//----------------------------------------------------------------------------------------
// GetVersionDate
//----------------------------------------------------------------------------------------
const time_t &
CZPAssetInfo::GetVersionDate()const
{
	return mDetail.mVersionDate;
}

//----------------------------------------------------------------------------------------
// GetTypeID
//----------------------------------------------------------------------------------------
const PMString &
CZPAssetInfo::GetTypeID()const
{
	return mDetail.mTypeID;
}

//----------------------------------------------------------------------------------------
// GetType
//----------------------------------------------------------------------------------------
enAssetType
CZPAssetInfo::GetType()const
{
	if( mCachedAssetType == eAssetTypeNone )
	{
		this->mCachedAssetType = Utils<IZPAssetUtils>()->GetAssetTypeFromID( mDetail.mTypeID );
	}
	return mCachedAssetType;
}

#pragma mark -
//----------------------------------------------------------------------------------------
// IsTypeIDTemplate
//----------------------------------------------------------------------------------------
bool
CZPAssetInfo::IsTypeIDTemplate()const
{
	return this->GetType() == eAssetType_IDTemplate;
}

//----------------------------------------------------------------------------------------
// IsTypeIDDocument
//----------------------------------------------------------------------------------------
bool
CZPAssetInfo::IsTypeIDDocument()const
{
	return this->GetType() == eAssetType_IDDocument;
}

//----------------------------------------------------------------------------------------
// IsTypeIDStory
//----------------------------------------------------------------------------------------
bool
CZPAssetInfo::IsTypeIDStory()const
{
	return this->GetType() == eAssetType_IDStory;
}

//----------------------------------------------------------------------------------------
// IsTypeICDocument
//----------------------------------------------------------------------------------------
bool
CZPAssetInfo::IsTypeICDocument()const
{
	return this->GetType() == eAssetType_ICDocument;
}

//----------------------------------------------------------------------------------------
// IsTypeICTemplate
//----------------------------------------------------------------------------------------
bool
CZPAssetInfo::IsTypeICTemplate()const
{
	return this->GetType() == eAssetType_ICTemplate;
}

//----------------------------------------------------------------------------------------
// IsTypeAssignment
//----------------------------------------------------------------------------------------
bool
CZPAssetInfo::IsTypeAssignment()const
{
	return this->GetType() == eAssetType_ICAssignment;
}
